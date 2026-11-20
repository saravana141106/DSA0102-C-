// B3: virtual functions vs switch vs std::variant, and exceptions vs return codes.
// Build: g++ -std=c++17 -O2 -Wall -Wextra bench.cpp -o bench
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <variant>
#include <vector>

volatile double sink = 0;                     // prevents the optimiser removing the work

// ---------- Design (a): abstract base class with virtual function ----------
struct VBill {
    virtual ~VBill() = default;
    virtual double total() const = 0;
};
struct VOP : VBill {
    double c, t;
    VOP(double a, double b) : c(a), t(b) {}
    double total() const override { return c + t; }
};
struct VIP : VBill {
    int d;
    double r, s;
    VIP(int dd, double rr, double ss) : d(dd), r(rr), s(ss) {}
    double total() const override { return d * r + s; }
};

// ---------- Design (b): one struct with a type field and a switch ----------
struct SBill {
    std::uint8_t kind;                        // 0 = OP, 1 = IP
    int days;
    double x, y;                              // OP: x=consultation, y=tests; IP: x=rate, y=surgery
};
inline double totalSwitch(const SBill& b)
{
    switch (b.kind) {
        case 0: return b.x + b.y;
        case 1: return b.days * b.x + b.y;
        default: return 0.0;
    }
}

// ---------- Design (c): std::variant with std::visit ----------
struct VarOP { double c, t; double total() const { return c + t; } };
struct VarIP { int d; double r, s; double total() const { return d * r + s; } };
using VarBill = std::variant<VarOP, VarIP>;
inline double totalVisit(const VarBill& b)
{
    return std::visit([](const auto& x) { return x.total(); }, b);
}

// ---------- Timing helper: median of repetitions ----------
template <class F>
double medianMs(F f, int reps)
{
    std::vector<double> ms;
    for (int r = 0; r < reps; ++r) {
        auto t0 = std::chrono::steady_clock::now();
        sink = f();
        auto t1 = std::chrono::steady_clock::now();
        ms.push_back(std::chrono::duration<double, std::milli>(t1 - t0).count());
    }
    std::sort(ms.begin(), ms.end());
    return ms[ms.size() / 2];
}

// ---------- B3.1: correctness on 10 bills ----------
static void correctnessTen()
{
    std::printf("== B3.1 correctness on 10 bills\n");
    std::vector<std::unique_ptr<VBill>> v;
    std::vector<SBill> s;
    std::vector<VarBill> w;
    for (int i = 0; i < 10; ++i) {
        if (i % 2 == 0) {
            v.push_back(std::make_unique<VOP>(500 + i, 1200));
            s.push_back({0, 0, 500.0 + i, 1200});
            w.push_back(VarOP{500.0 + i, 1200});
        } else {
            v.push_back(std::make_unique<VIP>(3, 2500 + i, 40000));
            s.push_back({1, 3, 2500.0 + i, 40000});
            w.push_back(VarIP{3, 2500.0 + i, 40000});
        }
    }
    double sumV = 0, sumS = 0, sumW = 0;
    for (std::size_t i = 0; i < 10; ++i) {
        sumV += v[i]->total();
        sumS += totalSwitch(s[i]);
        sumW += totalVisit(w[i]);
    }
    std::printf("virtual=%.1f switch=%.1f variant=%.1f agree=%s\n", sumV, sumS, sumW,
                (sumV == sumS && sumS == sumW) ? "YES" : "NO");
}

// ---------- B3.2: scaling of total() over n bills ----------
static void scaling()
{
    std::printf("\n== B3.2 total() over n bills (-O2, median of 5 runs)\n");
    std::printf("%-12s %-10s %-12s %-12s\n", "n", "design", "time_ms", "ns_per_bill");
    for (std::size_t n : {std::size_t(1000), std::size_t(1000000), std::size_t(10000000)}) {
        std::vector<std::unique_ptr<VBill>> v;
        std::vector<SBill> s;
        std::vector<VarBill> w;
        v.reserve(n); s.reserve(n); w.reserve(n);
        for (std::size_t i = 0; i < n; ++i) {
            if (i % 2 == 0) {
                v.push_back(std::make_unique<VOP>(500, 1200));
                s.push_back({0, 0, 500.0, 1200.0});
                w.push_back(VarOP{500.0, 1200.0});
            } else {
                v.push_back(std::make_unique<VIP>(3, 2500, 40000));
                s.push_back({1, 3, 2500.0, 40000.0});
                w.push_back(VarIP{3, 2500.0, 40000.0});
            }
        }
        int reps = 5;
        double tV = medianMs([&] { double t = 0; for (auto& p : v) t += p->total(); return t; }, reps);
        double tS = medianMs([&] { double t = 0; for (const auto& b : s) t += totalSwitch(b); return t; }, reps);
        double tW = medianMs([&] { double t = 0; for (const auto& b : w) t += totalVisit(b); return t; }, reps);
        std::printf("%-12zu %-10s %-12.3f %-12.2f\n", n, "virtual", tV, tV * 1e6 / n);
        std::printf("%-12zu %-10s %-12.3f %-12.2f\n", n, "switch", tS, tS * 1e6 / n);
        std::printf("%-12zu %-10s %-12.3f %-12.2f\n", n, "variant", tW, tW * 1e6 / n);
    }
}

// ---------- Space per element ----------
static void space()
{
    std::printf("\n== Space per element (bytes, sizeof)\n");
    std::printf("VOP (virtual, excl. heap header) = %zu, VIP = %zu, unique_ptr = %zu\n",
                sizeof(VOP), sizeof(VIP), sizeof(std::unique_ptr<VBill>));
    std::printf("SBill (switch record)            = %zu\n", sizeof(SBill));
    std::printf("VarBill (variant, tag + largest)  = %zu\n", sizeof(VarBill));
}

// ---------- B3.3: exceptions vs return codes ----------
enum class Status { Ok, Negative, TooLarge };
inline Status validateCode(double a)
{
    if (a < 0) return Status::Negative;
    if (a > 1e6) return Status::TooLarge;
    return Status::Ok;
}
inline void validateThrow(double a)
{
    if (a < 0) throw std::invalid_argument("negative");
    if (a > 1e6) throw std::out_of_range("too large");
}

static void errorCost()
{
    std::printf("\n== B3.3 validate 1,000,000 bills: return codes vs exceptions (-O2, median of 5)\n");
    std::printf("%-8s %-12s %-12s %-12s\n", "invalid", "design", "time_ms", "rejected");
    const std::size_t n = 1000000;
    for (int pct : {0, 1, 50}) {
        std::vector<double> amounts(n);
        for (std::size_t i = 0; i < n; ++i) {
            bool bad = (i % 100) < static_cast<std::size_t>(pct);   // exactly pct % invalid
            amounts[i] = bad ? ((i % 2 == 0) ? -1.0 : 2e6) : 100.0 + static_cast<double>(i % 500);
        }
        std::size_t rejectedCode = 0, rejectedExc = 0;
        double tC = medianMs([&] {
            rejectedCode = 0;
            for (double a : amounts) if (validateCode(a) != Status::Ok) ++rejectedCode;
            return static_cast<double>(rejectedCode);
        }, 5);
        double tE = medianMs([&] {
            rejectedExc = 0;
            for (double a : amounts) {
                try { validateThrow(a); }
                catch (const std::exception&) { ++rejectedExc; }
            }
            return static_cast<double>(rejectedExc);
        }, 5);
        std::printf("%-8d %-12s %-12.3f %-12zu\n", pct, "return-code", tC, rejectedCode);
        std::printf("%-8d %-12s %-12.3f %-12zu\n", pct, "exception", tE, rejectedExc);
    }
}

int main()
{
    correctnessTen();
    scaling();
    space();
    errorCost();
    return 0;
}

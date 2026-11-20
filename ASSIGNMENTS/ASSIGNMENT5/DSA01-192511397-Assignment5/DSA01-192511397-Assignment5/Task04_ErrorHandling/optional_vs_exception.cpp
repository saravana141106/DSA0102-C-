// B2 alternative: std::optional<double> (no value = rejected) vs exceptions, 1,000,000 claims.
// Build: g++ -std=c++17 -O2 -Wall -Wextra optional_vs_exception.cpp -o optional_vs_exception
#include <chrono>
#include <cstdio>
#include <algorithm>
#include <optional>
#include <stdexcept>
#include <vector>
volatile double sink = 0;
std::optional<double> approveOpt(double billed, double cover)
{
    if (billed < 0 || cover < 0) return std::nullopt;
    if (billed > cover) return std::nullopt;
    return billed;
}
double approveThrow(double billed, double cover)
{
    if (billed < 0 || cover < 0) throw std::invalid_argument("negative amount");
    if (billed > cover) throw std::runtime_error("exceeds cover");
    return billed;
}
template <class F> double medianMs(F f)
{
    std::vector<double> ms;
    for (int r = 0; r < 5; ++r) {
        auto t0 = std::chrono::steady_clock::now();
        sink = f();
        auto t1 = std::chrono::steady_clock::now();
        ms.push_back(std::chrono::duration<double, std::milli>(t1 - t0).count());
    }
    std::sort(ms.begin(), ms.end());
    return ms[2];
}
int main()
{
    const std::size_t n = 1000000;
    std::printf("%-8s %-14s %-12s %-10s\n", "invalid", "design", "time_ms", "rejected");
    for (int pct : {0, 1, 50}) {
        std::vector<double> billed(n);
        for (std::size_t i = 0; i < n; ++i) {
            bool bad = (i % 100) < static_cast<std::size_t>(pct);
            billed[i] = bad ? ((i % 2 == 0) ? -1.0 : 60000.0) : 40000.0;
        }
        const double cover = 50000.0;
        std::size_t rOpt = 0, rExc = 0;
        double tO = medianMs([&] { rOpt = 0; double s = 0;
            for (double b : billed) { auto r = approveOpt(b, cover); if (r) s += *r; else ++rOpt; } return s; });
        double tE = medianMs([&] { rExc = 0; double s = 0;
            for (double b : billed) { try { s += approveThrow(b, cover); } catch (const std::exception&) { ++rExc; } } return s; });
        std::printf("%-8d %-14s %-12.3f %-10zu\n", pct, "optional", tO, rOpt);
        std::printf("%-8d %-14s %-12.3f %-10zu\n", pct, "exception", tE, rExc);
    }
    std::printf("sizeof(double)=%zu, sizeof(std::optional<double>)=%zu\n", sizeof(double), sizeof(std::optional<double>));
}

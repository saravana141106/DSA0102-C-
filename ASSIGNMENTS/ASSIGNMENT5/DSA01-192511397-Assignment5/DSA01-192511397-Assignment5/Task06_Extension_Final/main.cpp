// Test driver for Task 06 (B1.1 verification, B4.1 extension, B4.2 resource safety)
#include <cmath>
#include <functional>
#include <cstdlib>
#include "extension.hpp"

static int failures = 0;

static void check(const std::string& name, double actual, double expected)
{
    bool ok = std::fabs(actual - expected) < 1e-9;
    std::cout << (ok ? "PASS " : "FAIL ") << name << ": got " << actual
              << ", expected " << expected << '\n';
    if (!ok) ++failures;
}

static void checkThrows(const std::string& name, const std::function<void()>& f,
                        const std::string& expectedType)
{
    try {
        f();
        std::cout << "FAIL " << name << ": no exception\n";
        ++failures;
    } catch (const BillingError& e) {
        bool ok = expectedType == "BillingError" || expectedType == "rejected";
        std::cout << (ok ? "PASS " : "FAIL ") << name << ": caught " << e.what() << '\n';
        if (!ok) ++failures;
    } catch (const std::exception& e) {
        std::cout << "FAIL " << name << ": non-billing exception " << e.what() << '\n';
        ++failures;
    }
}

// ---------- B1.1: one bill of each kind with each payer; totals verified by hand ----------
static void testCoreMatrix()
{
    std::cout << "== B1.1 core matrix (OP, IP, DC) x (SelfPay, Insurance, Government, Corporate)\n";
    OutPatientBill op("Ravi", 500, 1200);            // by hand: 500 + 1200 = 1700
    InPatientBill ip("Meena", 3, 2500, 40000);       // by hand: 3*2500 + 40000 = 47500
    DayCareBill dc("Asha", 30000);                   // by hand: package price 30000

    SelfPay self;
    Insurance ins(100000, 10);                        // cover 1,00,000, co-pay 10 %
    GovernmentScheme gov({{"OP", 1000}, {"IP", 45000}, {"DC", 25000}});
    CorporatePayer corp(50, 20000);                   // 50 %, capped at 20,000

    check("total OP", op.total(), 1700);
    check("total IP", ip.total(), 47500);
    check("total DC", dc.total(), 30000);

    check("SelfPay covers OP", self.amountCovered(op), 0);
    check("Insurance covers OP  (1700 x 0.9)", ins.amountCovered(op), 1530);
    check("Insurance covers IP  (47500 x 0.9)", ins.amountCovered(ip), 42750);
    check("Insurance covers DC  (30000 x 0.9)", ins.amountCovered(dc), 27000);
    check("Government covers OP (min 1000, 1700)", gov.amountCovered(op), 1000);
    check("Government covers IP (min 45000, 47500)", gov.amountCovered(ip), 45000);
    check("Government covers DC (min 25000, 30000)", gov.amountCovered(dc), 25000);
    check("Corporate covers OP  (850, cap 20000)", corp.amountCovered(op), 850);
    check("Corporate covers IP  (min 23750, cap 20000)", corp.amountCovered(ip), 20000);
    check("Corporate covers DC  (min 15000, cap 20000)", corp.amountCovered(dc), 15000);
}

// ---------- Error paths: rejections, missing records, unknown packages, negatives ----------
static void testErrorPaths()
{
    std::cout << "== Error paths (each must be caught, with a clear reason)\n";
    InPatientBill big("Meena", 3, 2500, 40000);     // total 47500
    Insurance smallCover(40000, 0);
    checkThrows("claim above cover", [&] { smallCover.amountCovered(big); }, "rejected");

    GovernmentScheme noDC({{"OP", 1000}});
    checkThrows("unknown package code", [&] { noDC.amountCovered(DayCareBill("Asha", 30000)); }, "rejected");

    checkThrows("negative consultation charge",
                [] { OutPatientBill("X", -5, 100); }, "rejected");
    checkThrows("negative days", [] { InPatientBill("X", -1, 100, 0); }, "rejected");
    checkThrows("negative corporate cap", [] { CorporatePayer(10, -1); }, "rejected");

    Registry reg;
    reg.add("OP-1", std::make_unique<OutPatientBill>("Ravi", 500, 1200));
    checkThrows("missing patient record", [&] { reg.find("OP-99"); }, "rejected");
    check("registry lookup OP-1 total", reg.find("OP-1").total(), 1700);
}

// ---------- A1.3 / batch processing: a rejected bill is recorded, never lost ----------
static void testBatch()
{
    std::cout << "== Batch settlement and day total\n";
    std::vector<std::unique_ptr<Bill>> bills;
    bills.push_back(std::make_unique<OutPatientBill>("Ravi", 500, 1200));
    bills.push_back(std::make_unique<InPatientBill>("Meena", 3, 2500, 40000));
    bills.push_back(std::make_unique<DayCareBill>("Asha", 30000));
    check("day total over Bill* (1700 + 47500 + 30000)", dayTotal(bills), 79200);

    Insurance ins(40000, 0);
    int accepted = 0, rejected = 0;
    for (const auto& b : bills) {
        Settlement s = settleOne(*b, ins);
        if (s.accepted) ++accepted; else ++rejected;
        std::cout << "  " << s.patient << " via " << s.payer << ": "
                  << (s.accepted ? "paid " + std::to_string(s.paid) : "rejected (" + s.reason + ")")
                  << '\n';
    }
    check("accepted claims", accepted, 2);
    check("rejected claims (bill count preserved)", rejected, 1);
    check("bills still owned after batch", static_cast<double>(bills.size()), 3);
}

// ---------- B4.2: exceptions thrown AND caught inside a run (for the sanitizer) ----------
struct Guard {
    std::string n;
    explicit Guard(std::string s) : n(std::move(s)) {}
    ~Guard() { std::cout << "  ~" << n << '\n'; }
};

static void runAll()
{
    try {
        Guard g("batch-scope");
        std::vector<std::unique_ptr<Bill>> tmp;
        tmp.push_back(std::make_unique<OutPatientBill>("Ravi", 500, -1));  // throws
    } catch (const NegativeChargeError& e) {
        std::cout << "  top-level caught: " << e.what() << '\n';
    }
}

int main()
{
    testCoreMatrix();
    testErrorPaths();
    testBatch();
    std::cout << "== Exception paths under scope guards\n";
    runAll();
    std::cout << (failures == 0 ? "ALL TESTS PASSED" : "TESTS FAILED") << " (" << failures << " failures)\n";
    return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}

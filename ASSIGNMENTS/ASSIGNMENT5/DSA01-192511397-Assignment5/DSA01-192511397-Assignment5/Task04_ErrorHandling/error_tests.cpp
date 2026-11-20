// B2 engineered error conditions: each test prints what the exception design does.
#include <cstdlib>
#include <iostream>
#include <new>
#include <stdexcept>
#include <string>
#include "../Task06_Extension_Final/extension.hpp"

struct Trace {                       // destructor messages prove that objects are destroyed
    std::string n;
    explicit Trace(std::string s) : n(std::move(s)) {}
    ~Trace() { std::cout << "  ~" << n << '\n'; }
};

// Test 1: negative charges and claim above cover (domain errors, typed hierarchy)
void testDomainErrors()
{
    std::cout << "[T1] domain errors\n";
    try { OutPatientBill("Ravi", -5, 100); }
    catch (const NegativeChargeError& e) { std::cout << "  NegativeChargeError: " << e.what() << '\n'; }
    try { Insurance(40000, 0).amountCovered(InPatientBill("Meena", 3, 2500, 40000)); }
    catch (const ClaimError& e) { std::cout << "  ClaimError: " << e.what() << '\n'; }
    catch (const BillingError& e) { std::cout << "  BillingError (unexpected): " << e.what() << '\n'; }
}

// Test 2: unknown package code
void testUnknownPackage()
{
    std::cout << "[T2] unknown package code\n";
    try {
        GovernmentScheme gov({{"OP", 1000}});
        gov.amountCovered(DayCareBill("Asha", 30000));
    } catch (const UnknownPackageError& e) { std::cout << "  UnknownPackageError: " << e.what() << '\n'; }
}

// Test 3: text where a number is expected (std::stod)
void testStod()
{
    std::cout << "[T3] std::stod on bad input\n";
    for (std::string s : {"1500", "12abc", "abc", "1e400"}) {
        try {
            double v = std::stod(s);
            std::cout << "  \"" << s << "\" -> " << v << " (parsed; trailing text ignored)\n";
        } catch (const std::invalid_argument& e) {
            std::cout << "  \"" << s << "\" -> invalid_argument: " << e.what() << '\n';
        } catch (const std::out_of_range& e) {
            std::cout << "  \"" << s << "\" -> out_of_range: " << e.what() << '\n';
        }
    }
}

// Test 4: memory allocation failure (requested size far beyond the address space)
void testBadAlloc()
{
    std::cout << "[T4] allocation failure\n";
    try {
        Trace t("before-alloc");
        // size read at run time so the compiler cannot reject it as a constant
        std::size_t n = std::strtoull("1152921504606846976", nullptr, 10);   // 2^60 doubles
        double* huge = new double[n];
        delete[] huge;
        std::cout << "  allocation unexpectedly succeeded\n";
    } catch (const std::bad_alloc& e) {
        std::cout << "  std::bad_alloc caught: " << e.what() << '\n';
    }
}

// Test 5: top-level catch(...) keeps the program running; all objects are destroyed
void failingBatchItem(int i)
{
    Trace t("item-" + std::to_string(i));
    if (i == 2) throw 7;                              // not derived from std::exception
    std::cout << "  item " << i << " ok\n";
}

void testTopLevel()
{
    std::cout << "[T5] top-level catch(...)\n";
    for (int i = 1; i <= 3; ++i) {
        try {
            failingBatchItem(i);
        } catch (const std::exception& e) {
            std::cout << "  std::exception: " << e.what() << '\n';
        } catch (...) {
            std::cout << "  unknown exception caught at top level; batch continues\n";
        }
    }
}

int main()
{
    testDomainErrors();
    testUnknownPackage();
    testStod();
    testBadAlloc();
    testTopLevel();
    std::cout << "program finished normally\n";
    return EXIT_SUCCESS;
}

// A2 / B1.4 - virtual removed from ~Bill (same program otherwise).
// 'override' is removed from ~OPBill: with a non-virtual base destructor it no longer
// overrides anything (the variant with 'override' fails to compile).
// OPBill owns a heap buffer so that skipping ~OPBill is observable under ASan.
#include <iostream>
#include <vector>
struct Bill {
 ~Bill() { std::cout << "~Bill "; }          // NOTE: not virtual
 virtual double total() const = 0;
 void show() const { std::cout << total() << ' '; }
};
struct OPBill : Bill {
 std::vector<int> buffer{1, 2, 3};            // heap memory owned by OPBill
 ~OPBill() { std::cout << "~OPBill "; }
 double total() const override { return 500; }
};
int main()
{
 Bill* b = new OPBill;
 b->show();
 delete b;                                    // UB: ~OPBill never runs
 std::cout << '\n';
}

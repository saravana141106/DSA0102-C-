// B1.3 - remove virtual from total() (base is concrete so it compiles)
#include <iostream>
struct Bill {
 virtual ~Bill() = default;
 double total() const { return 0; }          // NOTE: not virtual
 void show() const { std::cout << total() << ' '; }
};
struct OPBill : Bill {
 double total() const { return 500; }        // hides, does not override
};
int main()
{
 Bill* b = new OPBill;
 b->show();                                   // prints 0 : static binding
 b->total();
 OPBill* d = new OPBill;
 std::cout << d->total() << ' ';              // prints 500 via derived type
 std::cout << '\n';
 delete b; delete d;
}

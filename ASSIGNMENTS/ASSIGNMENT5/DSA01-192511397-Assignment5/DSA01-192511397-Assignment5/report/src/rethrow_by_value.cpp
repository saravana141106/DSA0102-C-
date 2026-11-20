// A5.1 - 'throw e;' versus 'throw;' : the type of the rethrown object
#include <iostream>
#include <stdexcept>
struct Base : std::exception {
 const char* what() const noexcept override { return "Base"; }
};
struct Derived : Base {
 const char* what() const noexcept override { return "Derived"; }
};
void thrower() { throw Derived{}; }
int main()
{
 try {
  try { thrower(); }
  catch (const Base& e) { std::cout << "caught as Base, what=" << e.what() << '\n'; throw; }
 } catch (const Base& e) {
  std::cout << "outer, with throw;   what=" << e.what() << '\n';
 }
 try {
  try { thrower(); }
  catch (const Base& e) { throw e; }   // slices: static type Base is thrown
 } catch (const Base& e) {
  std::cout << "outer, with throw e; what=" << e.what() << '\n';
 }
}

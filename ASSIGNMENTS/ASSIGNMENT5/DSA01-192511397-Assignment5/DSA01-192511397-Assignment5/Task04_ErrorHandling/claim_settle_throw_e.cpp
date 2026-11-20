// B2 variant: "throw e;" instead of "throw;" (the rethrown object is sliced)
#include <iostream>
#include <stdexcept>
class ClaimError : public std::runtime_error {
public:
 using std::runtime_error::runtime_error;
};
double approve(double billed, double cover)
{
 // TODO 1: Throw std::invalid_argument("negative amount") if billed or cover is
 // negative; throw ClaimError("exceeds cover") if billed > cover; otherwise return
 // billed
 if (billed < 0 || cover < 0) throw std::invalid_argument("negative amount");
 if (billed > cover) throw ClaimError("exceeds cover");
 return billed;
}
double settle(double billed, double cover)
{
 try {
 return approve(billed, cover);
 } catch (const std::runtime_error& e) {
 // TODO 2: Print "audit: " followed by the message, then rethrow the SAME
 // exception
 std::cout << "audit: " << e.what() << '\n';
 throw e;   // rethrows a copy of the STATIC type runtime_error (slicing)
 }
}
int main()
{
 double cases[][2] = {{40000, 50000}, {60000, 50000}, {-1, 50000}};
 for (auto& c : cases) {
 try {
 double paid = settle(c[0], c[1]);
 std::cout << "approved " << paid << '\n';
 } catch (const ClaimError& e) {
 std::cout << "claim rejected: " << e.what() << '\n';
 } catch (const std::exception& e) {
 std::cout << "error: " << e.what() << '\n';
 }
 }
}

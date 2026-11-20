// A5.1 - catch, log, rethrow with throw; (same resources, two levels)
#include <iostream>
#include <stdexcept>
#include <string>
struct Guard {
 std::string n;
 explicit Guard(const std::string& s) : n(s) {}
 ~Guard() { std::cout << "~" << n << ' '; }
};
void lowLevel()
{
 Guard g("lowLevel");
 throw std::runtime_error("disk read failed");
}
void middle()
{
 Guard g("middle");
 try {
 lowLevel();
 } catch (const std::exception& e) {
 std::cout << "[middle logs: " << e.what() << "] ";
 throw;                       // rethrow the SAME exception object
 }
}
int main()
{
 try {
 Guard g("main");
 middle();
 } catch (const std::exception& e) {
 std::cout << "\nreported to user: " << e.what() << '\n';
 }
}

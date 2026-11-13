#include <iostream>
using namespace std;

void applyByValue(long long fee, int percent) {
    fee -= fee * percent / 100;
    cout << "inside value function: " << fee << '\n';
}
void applyByReference(long long& fee, int percent) {
    fee -= fee * percent / 100;
}
void applyByPointer(long long* fee, int percent) {
    if (fee != nullptr) *fee -= *fee * percent / 100;
}
int main() {
    long long a = 45000, b = 45000, c = 45000;
    applyByValue(a, 10);
    cout << "stored after value: " << a << '\n';
    applyByReference(b, 10);
    cout << "stored after reference: " << b << '\n';
    applyByPointer(&c, 10);
    cout << "stored after pointer: " << c << '\n';
}

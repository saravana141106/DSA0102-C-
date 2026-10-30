#include <iostream>
using namespace std;

void byValue(long long fee, int percent) {
    fee -= fee * percent / 100;
    cout << "Inside call by value: " << fee << '\n';
}

void byReference(long long& fee, int percent) {
    fee -= fee * percent / 100;
}

void byPointer(long long* fee, int percent) {
    if (fee != nullptr) {
        *fee -= (*fee) * percent / 100;
    }
}

int& bigger(int& a, int& b) {
    return (a > b) ? a : b;
}

void show(int x, int y = 5, int z = 10) {
    cout << x + y + z << ' ';
}

int main() {
    long long fee = 45000;
    byValue(fee, 10);
    cout << "After call by value: " << fee << '\n';

    byReference(fee, 10);
    cout << "After call by reference: " << fee << '\n';

    byPointer(&fee, 10);
    cout << "After call by pointer: " << fee << '\n';

    int a = 1, b = 2, c = 3;
    a += 0; // Keeps the variables easy to compare with the assignment example.
    b += 0;
    c += 0;
    bigger(a, c) = 100;
    cout << "After bigger(a, c) = 100: " << a << ' ' << c << '\n';

    show(1);
    show(1, 2);
    show(1, 2, 3);
    cout << '\n';
    return 0;
}
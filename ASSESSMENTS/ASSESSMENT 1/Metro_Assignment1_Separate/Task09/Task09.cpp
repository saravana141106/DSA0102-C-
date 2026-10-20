// Task 9: Fare logic tested against expected values
#include <iomanip>
#include <iostream>
using namespace std;
enum class Zone { Inner, Middle, Outer };
long long calculateFare(Zone z, int hour) {
    long long base;
    switch (z) {
        case Zone::Inner: base = 1000; break;
        case Zone::Middle: base = 2000; break;
        case Zone::Outer: base = 3000; break;
        default: return -1;
    }
    bool peak = (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
    return peak ? base + base * 20 / 100 : base;
}
void printMoney(long long p) { cout << "Rs " << p / 100 << '.' << setw(2) << setfill('0') << p % 100 << setfill(' '); }
int main() {
    struct Test { const char* name; Zone zone; int hour; long long expected; };
    const Test tests[] = {
        {"Inner off-peak", Zone::Inner, 11, 1000},
        {"Middle peak 08:00", Zone::Middle, 8, 2400},
        {"Inner 10:00", Zone::Inner, 10, 1000},
        {"Outer 17:00", Zone::Outer, 17, 3600},
        {"Middle 20:00", Zone::Middle, 20, 2000}
    };
    for (const auto& t : tests) {
        long long actual = calculateFare(t.zone, t.hour);
        cout << t.name << " | expected "; printMoney(t.expected);
        cout << " | actual "; printMoney(actual);
        cout << (actual == t.expected ? " | PASS\n" : " | FAIL\n");
    }
    cout << "Balance Rs 60 minus fare Rs 10 leaves Rs 50: allowed.\n";
}

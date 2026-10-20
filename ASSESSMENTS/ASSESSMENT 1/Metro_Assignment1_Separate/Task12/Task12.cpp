// Task 12: Revised fare system with Airport, weekend discount and monthly pass
#include <array>
#include <iostream>
using namespace std;
enum class Zone { Inner = 0, Middle = 1, Outer = 2, Airport = 3 };
constexpr array<long long, 4> BASE_FARE_PAISE{1000, 2000, 3000, 6000};
constexpr int PEAK_PERCENT = 20;
constexpr int WEEKEND_DISCOUNT_PERCENT = 10;

long long revisedFare(Zone zone, bool peak, bool weekend) {
    size_t i = static_cast<size_t>(zone);
    if (i >= BASE_FARE_PAISE.size()) return -1;
    long long fare = BASE_FARE_PAISE[i];
    if (weekend) fare -= fare * WEEKEND_DISCOUNT_PERCENT / 100;
    else if (peak) fare += fare * PEAK_PERCENT / 100;
    return fare;
}
class MonthlyPass {
    array<bool, 4> covered{};
public:
    MonthlyPass(Zone first, Zone second) {
        covered[static_cast<size_t>(first)] = true;
        covered[static_cast<size_t>(second)] = true;
    }
    bool covers(Zone z) const {
        size_t i = static_cast<size_t>(z);
        return i < covered.size() && covered[i];
    }
};
void show(const char* label, long long p) { cout << label << " Rs " << p / 100 << '.' << (p % 100 < 10 ? "0" : "") << p % 100 << '\n'; }
int main() {
    show("Airport peak weekday fare:", revisedFare(Zone::Airport, true, false));
    show("Airport weekend fare (no peak surcharge):", revisedFare(Zone::Airport, true, true));
    MonthlyPass pass(Zone::Inner, Zone::Middle);
    cout << "Pass covers Inner: " << boolalpha << pass.covers(Zone::Inner) << '\n';
    cout << "Pass covers Airport: " << pass.covers(Zone::Airport) << '\n';
}

// Task 2: Tokens, constants, identifiers and enum class
#include <array>
#include <iostream>
using namespace std;

enum class Zone { Inner, Middle, Outer };
constexpr array<long long, 3> FARE_PAISE{1000, 2000, 3000};
constexpr int PEAK_PERCENT = 20;
constexpr long long MIN_BALANCE_PAISE = 5000;

int main() {
    cout << "Inner fare: Rs " << FARE_PAISE[static_cast<int>(Zone::Inner)] / 100 << '\n';
    cout << "Middle fare: Rs " << FARE_PAISE[static_cast<int>(Zone::Middle)] / 100 << '\n';
    cout << "Outer fare: Rs " << FARE_PAISE[static_cast<int>(Zone::Outer)] / 100 << '\n';
    cout << "Peak surcharge: " << PEAK_PERCENT << "%\n";
    cout << "Minimum remaining balance: Rs " << MIN_BALANCE_PAISE / 100 << '\n';
    cout << "Examples: keyword=constexpr, identifier=FARE_PAISE, literal=1000, operator=/, punctuator=;.\n";
}

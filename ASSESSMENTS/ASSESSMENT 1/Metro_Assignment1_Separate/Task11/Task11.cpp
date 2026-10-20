// Task 11: Recommended final design and a simple executable demonstration
#include <array>
#include <iostream>
using namespace std;
enum class Zone { Inner, Middle, Outer };
constexpr array<long long, 3> FARE_PAISE{1000, 2000, 3000};
constexpr long long MIN_BALANCE_PAISE = 5000;
class SmartCard {
    long long balancePaise;
public:
    explicit SmartCard(long long initial) : balancePaise(initial) {}
    bool deduct(long long fare) {
        if (fare < 0 || balancePaise - fare < MIN_BALANCE_PAISE) return false;
        balancePaise -= fare; return true;
    }
    long long balance() const { return balancePaise; }
};
int main() {
    SmartCard card(10000);
    cout << "Recommended: enum class Zone, constexpr fare table, integer paise, long long totals, private card balance.\n";
    cout << "Deduct Rs 20? " << boolalpha << card.deduct(2000) << '\n';
    cout << "Remaining balance in paise: " << card.balance() << '\n';
    cout << "Fare revisions update the central fare table; configuration files avoid recompilation but need validation.\n";
}

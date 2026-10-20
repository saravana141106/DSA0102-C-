// Task 7: Procedural and object-oriented SmartCard versions
#include <iostream>
using namespace std;
constexpr long long MIN_BALANCE_PAISE = 5000;

bool proceduralCanTravel(long long balance, long long fare) {
    return fare >= 0 && balance - fare >= MIN_BALANCE_PAISE;
}
class SmartCard {
private:
    long long balancePaise;
public:
    explicit SmartCard(long long initial) : balancePaise(initial) {}
    long long balance() const { return balancePaise; }
    bool canTravel(long long fare) const { return fare >= 0 && balancePaise - fare >= MIN_BALANCE_PAISE; }
    bool deduct(long long fare) {
        if (!canTravel(fare)) return false;
        balancePaise -= fare;
        return true;
    }
};
int main() {
    long long proceduralBalance = 10000;
    if (proceduralCanTravel(proceduralBalance, 2000)) proceduralBalance -= 2000;
    cout << "Procedural balance: Rs " << proceduralBalance / 100 << '\n';
    SmartCard card(10000);
    cout << "OOP deduction successful: " << boolalpha << card.deduct(2000) << '\n';
    cout << "OOP balance: Rs " << card.balance() / 100 << '\n';
}

// Task 1: Object-oriented analysis of a metro smart-card system
#include <iostream>
#include <string>
using namespace std;

class SmartCard {
private:
    long long balancePaise;
public:
    explicit SmartCard(long long balance) : balancePaise(balance) {}
    long long getBalance() const { return balancePaise; }
    bool canTravel(long long farePaise) const {
        return farePaise >= 0 && balancePaise - farePaise >= 5000;
    }
    bool deduct(long long farePaise) {
        if (!canTravel(farePaise)) return false;
        balancePaise -= farePaise;
        return true;
    }
};

int main() {
    SmartCard card(10000); // Rs 100.00
    cout << "SmartCard balance: Rs " << card.getBalance() / 100 << '\n';
    cout << "Can pay Rs 20 and retain Rs 50? " << boolalpha << card.canTravel(2000) << '\n';
    cout << "Deduction successful: " << card.deduct(2000) << '\n';
    cout << "Balance after journey: Rs " << card.getBalance() / 100 << '\n';
    cout << "Objects: SmartCard, Passenger, Journey, Zone and FareTable.\n";
    cout << "OOP encapsulates balance and centralises rules; a journey must leave at least 5000 paise.\n";
}

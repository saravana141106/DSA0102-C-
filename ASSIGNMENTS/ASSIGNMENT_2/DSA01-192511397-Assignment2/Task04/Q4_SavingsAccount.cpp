#include <iostream>
#include <string>
using namespace std;

class SavingsAccount {
private:
    string accountNumber;
    long long balance;
    long long transactions[10];
    int count;
    int next; // circular insertion position

public:
    SavingsAccount(const string& number, long long opening)
        : accountNumber(number), balance(opening), transactions{}, count(0), next(0) {}
    void deposit(long long amount) {
        if (amount <= 0) return;
        balance += amount;
        record(amount);
    }
    bool withdraw(long long amount) {
        if (amount <= 0 || amount > balance) return false;
        balance -= amount;
        record(-amount);
        return true;
    }
    void miniStatement() const;
private:
    void record(long long amount) {
        transactions[next] = amount;
        next = (next + 1) % 10;
        if (count < 10) ++count;
    }
};

void SavingsAccount::miniStatement() const {
    cout << "Account: " << accountNumber << " Balance: " << balance << '\n';
    cout << "Recent transactions (oldest to newest):\n";
    int start = (count == 10) ? next : 0;
    for (int i = 0; i < count; ++i)
        cout << transactions[(start + i) % 10] << '\n';
}
int main() {
    SavingsAccount account("SB1001", 1000);
    account.deposit(200);
    account.withdraw(100);
    for (int i = 1; i <= 10; ++i) account.deposit(i * 10);
    account.miniStatement();
}

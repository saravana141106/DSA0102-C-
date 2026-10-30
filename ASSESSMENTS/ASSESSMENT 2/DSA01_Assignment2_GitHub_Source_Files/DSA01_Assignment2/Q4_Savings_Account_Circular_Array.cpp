#include <iostream>
using namespace std;

class SavingsAccount {
private:
    int accountNumber;
    double balance;
    double transactions[10];
    int count;
    int nextIndex;

public:
    SavingsAccount(int number, double openingBalance)
        : accountNumber(number), balance(openingBalance), count(0), nextIndex(0) {}

    void deposit(double amount) {
        if (amount <= 0) return;
        balance += amount;
        addTransaction(amount);
    }

    void withdraw(double amount) {
        if (amount <= 0 || amount > balance) return;
        balance -= amount;
        addTransaction(-amount);
    }

    void miniStatement() const {
        cout << "Account: " << accountNumber << "\nBalance: " << balance << '\n';
        cout << "Recent transactions (oldest to newest):\n";
        int start = (count == 10) ? nextIndex : 0;
        for (int i = 0; i < count; i++) {
            cout << transactions[(start + i) % 10] << '\n';
        }
    }

private:
    void addTransaction(double amount) {
        transactions[nextIndex] = amount;
        nextIndex = (nextIndex + 1) % 10;
        if (count < 10) count++;
    }
};

int main() {
    SavingsAccount account(1001, 5000);
    account.deposit(500);
    account.withdraw(200);
    account.deposit(1000);
    account.withdraw(300);
    account.miniStatement();
    return 0;
}
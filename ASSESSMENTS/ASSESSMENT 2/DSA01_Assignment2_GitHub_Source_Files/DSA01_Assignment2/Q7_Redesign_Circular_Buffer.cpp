#include <iostream>
using namespace std;

class TransactionHistory {
private:
    static const int CAPACITY = 10;
    double values[CAPACITY];
    int count;
    int next;

public:
    TransactionHistory() : count(0), next(0) {}

    void add(double value) {
        values[next] = value;
        next = (next + 1) % CAPACITY;
        if (count < CAPACITY) count++;
    }

    void display() const {
        int start = (count == CAPACITY) ? next : 0;
        for (int i = 0; i < count; i++) {
            cout << values[(start + i) % CAPACITY] << '\n';
        }
    }
};

int main() {
    TransactionHistory history;
    for (int i = 1; i <= 12; i++) {
        history.add(i * 100);
    }
    cout << "Last 10 transactions:\n";
    history.display();
    return 0;
}
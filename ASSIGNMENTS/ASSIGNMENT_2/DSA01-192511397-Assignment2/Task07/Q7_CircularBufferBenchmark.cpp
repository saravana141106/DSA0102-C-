#include <iostream>
#include <vector>
#include <chrono>
using namespace std;

long long shiftBenchmark(int capacity, int operations) {
    vector<int> a(capacity, 0);
    auto start = chrono::steady_clock::now();
    for (int op = 0; op < operations; ++op) {
        for (int i = 0; i < capacity - 1; ++i) a[i] = a[i + 1];
        a[capacity - 1] = op;
    }
    auto end = chrono::steady_clock::now();
    return chrono::duration_cast<chrono::microseconds>(end - start).count();
}
long long circularBenchmark(int capacity, int operations) {
    vector<int> a(capacity, 0);
    int next = 0;
    auto start = chrono::steady_clock::now();
    for (int op = 0; op < operations; ++op) {
        a[next] = op;
        next = (next + 1) % capacity;
    }
    auto end = chrono::steady_clock::now();
    return chrono::duration_cast<chrono::microseconds>(end - start).count();
}
int main() {
    const int capacity = 50, operations = 2000000;
    cout << "capacity=" << capacity << " operations=" << operations << '\n';
    cout << "shift_us=" << shiftBenchmark(capacity, operations) << '\n';
    cout << "circular_us=" << circularBenchmark(capacity, operations) << '\n';
}

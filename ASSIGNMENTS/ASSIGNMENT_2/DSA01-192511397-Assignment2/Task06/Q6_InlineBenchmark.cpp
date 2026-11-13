#include <iostream>
#include <chrono>
using namespace std;
volatile long long sinkValue = 0;
inline int fastAdd(int a, int b) { return a + b; }
__attribute__((noinline)) int ordinaryAdd(int a, int b) { return a + b; }
template <typename F>
long long measure(F fn) {
    auto start = chrono::steady_clock::now();
    long long sum = 0;
    for (int i = 0; i < 10000000; ++i) sum += fn(i, 7);
    auto end = chrono::steady_clock::now();
    sinkValue = sum;
    return chrono::duration_cast<chrono::microseconds>(end - start).count();
}
int main() {
    cout << "inline_us=" << measure(fastAdd) << '\n';
    cout << "noinline_us=" << measure(ordinaryAdd) << '\n';
}

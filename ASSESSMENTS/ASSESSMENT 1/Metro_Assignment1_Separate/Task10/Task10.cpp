// Task 10: Compare integer-paise totals with double-rupee totals
#include <iomanip>
#include <iostream>
using namespace std;
int main() {
    const long long counts[] = {20000, 500000, 3000000};
    constexpr long long farePaise = 1000; // Rs 10.00
    cout << fixed << setprecision(2);
    for (long long n : counts) {
        long long exactPaise = n * farePaise;
        double totalRupees = 0.0;
        for (long long i = 0; i < n; ++i) totalRupees += 10.0;
        double exactRupees = static_cast<double>(exactPaise) / 100.0;
        cout << n << " journeys: integer total Rs " << exactRupees
             << ", double total Rs " << totalRupees
             << ", difference Rs " << totalRupees - exactRupees << '\n';
    }
    cout << "Use long long for network totals; int may overflow for totals in paise.\n";
}

// Task 8: Validate numeric input and print formatted output
#include <iomanip>
#include <iostream>
#include <limits>
using namespace std;
bool isPeakHour(int hour) { return (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20); }
int main() {
    int hour;
    cout << "Enter hour (0-23): ";
    while (!(cin >> hour) || hour < 0 || hour > 23) {
        if (cin.eof()) return 0;
        cout << "Invalid input. Enter an integer from 0 to 23: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cout << "Hour " << setw(2) << setfill('0') << hour << setfill(' ')
         << " is " << (isPeakHour(hour) ? "peak" : "off-peak") << ".\n";
}

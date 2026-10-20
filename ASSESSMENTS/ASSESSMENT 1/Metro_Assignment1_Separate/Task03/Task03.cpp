// Task 3: Data types, sizes and money stored in integer paise
#include <climits>
#include <iostream>
using namespace std;

int main() {
    bool peak = true;
    char category = 'S';
    int hour = 8;
    long long balancePaise = 12345;
    unsigned int zoneNumber = 2;
    double illustrativeRupees = 123.45;
    cout << boolalpha << "peak=" << peak << ", category=" << category << ", hour=" << hour << '\n';
    cout << "balancePaise=" << balancePaise << ", zoneNumber=" << zoneNumber << '\n';
    cout << "double example=" << illustrativeRupees << '\n';
    cout << "sizeof(bool)=" << sizeof(bool) << " sizeof(char)=" << sizeof(char)
         << " sizeof(int)=" << sizeof(int) << " sizeof(long long)=" << sizeof(long long)
         << " sizeof(double)=" << sizeof(double) << " bytes\n";
    cout << "INT_MAX=" << INT_MAX << " LLONG_MAX=" << LLONG_MAX << '\n';
    cout << "Money is stored in integer paise to avoid floating-point rounding.\n";
}

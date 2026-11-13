#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

struct Parcel { double kg; bool express; bool insured; };

double charge(int grams, bool express = false, bool insured = false) {
    double base = grams * 0.02;
    return base + (express ? 30 : 0) + (insured ? 15 : 0);
}
double charge(double kg, bool express = false, bool insured = false) {
    double base = kg * 40.0;
    return base + (express ? 30 : 0) + (insured ? 15 : 0);
}
double charge(const Parcel& p) {
    return charge(p.kg, p.express, p.insured);
}
int main() {
    cout << fixed << setprecision(2);
    cout << "charge(250): " << charge(250) << '\n';           // int overload
    cout << "charge(2.0): " << charge(2.0) << '\n';           // double overload
    cout << "charge(2.0f): " << charge(2.0f) << '\n';         // float promotes to double
    cout << "charge(500, true): " << charge(500, true) << '\n';
    cout << "charge(1.5, false, true): " << charge(1.5, false, true) << '\n';
    Parcel p{2.0, true, true};
    cout << "charge(parcel): " << charge(p) << '\n';
}

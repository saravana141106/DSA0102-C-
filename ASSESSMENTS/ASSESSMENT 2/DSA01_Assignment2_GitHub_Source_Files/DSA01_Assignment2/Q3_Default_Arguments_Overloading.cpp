#include <iostream>
#include <string>
using namespace std;

struct Parcel {
    double weightKg;
    bool express;
    bool insured;
};

double charge(int weightGrams, bool express = false, bool insured = false) {
    double amount = weightGrams * 0.05;
    if (express) amount += 30;
    if (insured) amount += 20;
    cout << "Integer grams overload: ";
    return amount;
}

double charge(double weightKg, bool express = false, bool insured = false) {
    double amount = weightKg * 50.0;
    if (express) amount += 30;
    if (insured) amount += 20;
    cout << "Double kg overload: ";
    return amount;
}

double charge(const Parcel& p) {
    double amount = p.weightKg * 50.0;
    if (p.express) amount += 30;
    if (p.insured) amount += 20;
    cout << "Parcel overload: ";
    return amount;
}

int main() {
    cout << charge(200) << '\n';          // int overload
    cout << charge(2) << '\n';            // int overload
    cout << charge(2.0) << '\n';          // double overload
    cout << charge(2.0f) << '\n';         // float promotes to double
    cout << charge(500, true) << '\n';    // int overload, express
    Parcel p{3.0, true, true};
    cout << charge(p) << '\n';            // Parcel overload
    return 0;
}
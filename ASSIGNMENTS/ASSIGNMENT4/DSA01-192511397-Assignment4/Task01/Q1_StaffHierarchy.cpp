#include <iostream>
#include <memory>
#include <string>
#include <vector>
using namespace std;

class Staff {
protected:
    string name;
    int id;
    double basic;
    static constexpr double DA_RATE = 0.42;
public:
    Staff(string n, int i, double b) : name(std::move(n)), id(i), basic(b) {}
    virtual ~Staff() = default;
    virtual double pay() const = 0;
    virtual void print() const { cout << name << " (" << id << "): " << pay() << '\n'; }
};
class Professor : public Staff {
    double research;
public:
    Professor(string n, int i, double b, double r) : Staff(std::move(n), i, b), research(r) {}
    double pay() const override { return basic + basic * DA_RATE + research; }
};
class LabAssistant : public Staff {
public:
    LabAssistant(string n, int i, double b) : Staff(std::move(n), i, b) {}
    double pay() const override { return basic + basic * DA_RATE; }
};
class GuestFaculty : public Staff {
    double hours, rate;
public:
    GuestFaculty(string n, int i, double h, double r) : Staff(std::move(n), i, 0), hours(h), rate(r) {}
    double pay() const override { return hours * rate; }
};
int main() {
    vector<unique_ptr<Staff>> people;
    people.push_back(make_unique<Professor>("Meena", 101, 50000, 8000));
    people.push_back(make_unique<LabAssistant>("Arun", 102, 30000));
    people.push_back(make_unique<GuestFaculty>("Ravi", 103, 20, 900));
    for (const auto& p : people) p->print();
}

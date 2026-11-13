#include <iostream>
#include <string>
using namespace std;

class Address {
    string city; int pin;
public:
    Address(string c, int p) : city(std::move(c)), pin(p) { cout << "Address+ "; }
    ~Address() { cout << "~Address "; }
    string str() const { return city + "-" + to_string(pin); }
};
class Course {
    string title;
public:
    explicit Course(string t) : title(std::move(t)) { cout << "Course+ "; }
    ~Course() { cout << "~Course "; }
};
class Person {
protected:
    string name; int age; Address home;
public:
    Person(string n, int a, string city, int pin)
        : name(std::move(n)), age(a), home(std::move(city), pin) { cout << "Person+ "; }
    virtual ~Person() { cout << "~Person "; }
};
class Student : public Person {
    long long regNo; Course course;
public:
    Student(string n, int a, long long r, string city, int pin, string c)
        : Person(std::move(n), a, std::move(city), pin), regNo(r), course(std::move(c)) {
        cout << "Student+ ";
    }
    ~Student() override { cout << "~Student "; }
    void print() const { cout << regNo << ' ' << name << " (" << age << ") " << home.str() << '\n'; }
};
class Staff : public Person {
    double salary;
public:
    Staff(string n, int a, string city, int pin, double s)
        : Person(std::move(n), a, std::move(city), pin), salary(s) { cout << "Staff+ "; }
    ~Staff() override { cout << "~Staff "; }
};
int main() {
    { Student s("Kavya", 19, 192311001, "Salem", 636001, "OOP"); s.print(); }
    cout << '\n';
    { Staff t("Mohan", 42, "Madurai", 625001, 52000); }
    cout << '\n';
}

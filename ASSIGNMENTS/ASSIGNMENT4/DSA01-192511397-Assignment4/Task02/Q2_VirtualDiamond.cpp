#include <iostream>
#include <string>
using namespace std;

class Person {
protected:
    string name;
    int id;
public:
    Person(string n, int i) : name(std::move(n)), id(i) { cout << "Person "; }
};
class Student : virtual public Person {
protected:
    string programme;
    double cgpa;
public:
    Student(string n, int i, string p, double c)
        : Person(std::move(n), i), programme(std::move(p)), cgpa(c) { cout << "Student "; }
};
class Staff : virtual public Person {
protected:
    string department;
    double salary;
public:
    Staff(string n, int i, string d, double s)
        : Person(std::move(n), i), department(std::move(d)), salary(s) { cout << "Staff "; }
};
class TeachingAssistant : public Student, public Staff {
public:
    TeachingAssistant(string n, int i, string p, double c, string d, double s)
        : Person(n, i), Student(n, i, p, c), Staff(n, i, d, s) { cout << "TA "; }
};
class ResearchScholar : public Student {
    double fellowship;
public:
    ResearchScholar(string n, int i, string p, double c, double f)
        : Person(std::move(n), i), Student("", 0, std::move(p), c), fellowship(f) {}
    double getFellowship() const { return fellowship; }
};
int main() {
    TeachingAssistant ta("Kavya", 11, "CSE", 8.7, "Computing", 28000);
    cout << "\n" << ta.name << ' ' << ta.id << '\n';
    cout << "sizeof(Person)=" << sizeof(Person)
         << " sizeof(TeachingAssistant)=" << sizeof(TeachingAssistant) << '\n';
}

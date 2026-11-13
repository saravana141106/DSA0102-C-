#include <iostream>
#include <memory>
#include <string>
#include <vector>
using namespace std;

class ServiceInfo {
    string dueDate;
public:
    explicit ServiceInfo(string d) : dueDate(std::move(d)) {}
    string due() const { return dueDate; }
};
class Vehicle {
protected:
    string registration;
    double dailyRate;
public:
    Vehicle(string r, double rate) : registration(std::move(r)), dailyRate(rate) {}
    virtual ~Vehicle() = default;
    virtual double rentalCharge(int days) const = 0;
    virtual string type() const = 0;
    virtual void print() const {
        cout << type() << " " << registration << " charge=" << rentalCharge(1) << '\n';
    }
};
class Bicycle : public Vehicle {
public:
    Bicycle(string r, double rate) : Vehicle(std::move(r), rate) {}
    double rentalCharge(int days) const override { return dailyRate * days; }
    string type() const override { return "Bicycle"; }
};
class MotorVehicle : public Vehicle {
protected:
    double fuelOrCharge;
    ServiceInfo service;
public:
    MotorVehicle(string r, double rate, double level, string due)
        : Vehicle(std::move(r), rate), fuelOrCharge(level), service(std::move(due)) {}
};
class TwoWheeler : public MotorVehicle {
public:
    TwoWheeler(string r, double rate, double fuel, string due)
        : MotorVehicle(std::move(r), rate, fuel, std::move(due)) {}
    double rentalCharge(int days) const override { return dailyRate * days + 25; }
    string type() const override { return "TwoWheeler"; }
};
class Car : public MotorVehicle {
protected:
    int seats;
public:
    Car(string r, double rate, double fuel, string due, int s)
        : MotorVehicle(std::move(r), rate, fuel, std::move(due)), seats(s) {}
    double rentalCharge(int days) const override { return dailyRate * days + 100; }
    string type() const override { return "Car"; }
};
class ElectricCar : public Car {
    double batteryKWh;
public:
    ElectricCar(string r, double rate, double charge, string due, int s, double kwh)
        : Car(std::move(r), rate, charge, std::move(due), s), batteryKWh(kwh) {}
    double rentalCharge(int days) const override { return dailyRate * days + batteryKWh * 2.5; }
    string type() const override { return "ElectricCar"; }
};
class ElectricBus : public Vehicle {
    int seats;
    double batteryKWh;
public:
    ElectricBus(string r, double rate, int s, double kwh)
        : Vehicle(std::move(r), rate), seats(s), batteryKWh(kwh) {}
    double rentalCharge(int days) const override { return dailyRate * days + batteryKWh * 2.5 + seats * 5; }
    string type() const override { return "ElectricBus"; }
};
class Fleet {
    vector<unique_ptr<Vehicle>> vehicles;
    vector<bool> rented;
public:
    void add(unique_ptr<Vehicle> v) { vehicles.push_back(std::move(v)); rented.push_back(false); }
    bool rent(size_t i) {
        if (i >= vehicles.size() || rented[i]) return false;
        rented[i] = true; return true;
    }
    double returnVehicle(size_t i, int daysUsed, int daysLate) {
        if (i >= vehicles.size() || !rented[i]) return -1;
        rented[i] = false;
        return vehicles[i]->rentalCharge(daysUsed) + daysLate * 50.0;
    }
    void list() const {
        for (size_t i = 0; i < vehicles.size(); ++i) {
            cout << i << " " << (rented[i] ? "RENTED " : "AVAILABLE ");
            vehicles[i]->print();
        }
    }
};
int main() {
    Fleet f;
    f.add(make_unique<Bicycle>("TN01-B100", 80));
    f.add(make_unique<TwoWheeler>("TN01-T200", 250, 70, "2026-12-01"));
    f.add(make_unique<Car>("TN01-C300", 1500, 40, "2026-12-15", 5));
    f.add(make_unique<ElectricCar>("TN01-E400", 1800, 80, "2026-12-20", 5, 60));
    f.add(make_unique<ElectricBus>("TN01-EB500", 5000, 30, 220)); // extension
    f.list();
    cout << boolalpha << "rent bicycle: " << f.rent(0) << '\n';
    cout << "late return total: " << f.returnVehicle(0, 2, 1) << '\n';
    cout << "rent electric car: " << f.rent(3) << '\n';
    cout << "electric car return: " << f.returnVehicle(3, 2, 0) << '\n';
}

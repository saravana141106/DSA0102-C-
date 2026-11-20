// Hospital billing library: bill hierarchy (A1), payers (A3), exception hierarchy (A4)
#pragma once
#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

// ---------- A4: exception hierarchy derived from std::exception ----------
class BillingError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};
class NegativeChargeError : public BillingError {
public:
    using BillingError::BillingError;
};
class ClaimError : public BillingError {
public:
    using BillingError::BillingError;
};
class UnknownPackageError : public BillingError {
public:
    using BillingError::BillingError;
};
class MissingRecordError : public BillingError {
public:
    using BillingError::BillingError;
};

inline void requireNonNegative(double v, const char* what)
{
    if (v < 0) throw NegativeChargeError(std::string("negative ") + what);
}

// ---------- A1: abstract base class with pure virtual functions ----------
class Bill {
protected:
    std::string patient;
public:
    explicit Bill(std::string p) : patient(std::move(p)) {}
    virtual ~Bill() = default;                       // A2.3: required for delete through Bill*
    virtual double total() const = 0;                // pure virtual
    virtual std::string type() const = 0;            // pure virtual
    const std::string& patientName() const noexcept { return patient; }
    void print(std::ostream& os) const
    {
        os << type() << ' ' << patient << ": " << total() << '\n';
    }
};

class OutPatientBill : public Bill {
    double consultation, tests;
public:
    OutPatientBill(const std::string& p, double c, double t)
        : Bill(p), consultation(c), tests(t)
    {
        requireNonNegative(c, "consultation");
        requireNonNegative(t, "tests");
    }
    double total() const override { return consultation + tests; }
    std::string type() const override { return "OP"; }
};

class InPatientBill : public Bill {
    int days;
    double perDay, surgery;
public:
    InPatientBill(const std::string& p, int d, double rate, double s)
        : Bill(p), days(d), perDay(rate), surgery(s)
    {
        if (d < 0) throw NegativeChargeError("negative days");
        requireNonNegative(rate, "room rate");
        requireNonNegative(s, "surgery");
    }
    double total() const override { return days * perDay + surgery; }
    std::string type() const override { return "IP"; }
};

// ---------- A3: payer interface ----------
class Payer {
public:
    virtual ~Payer() = default;
    virtual std::string name() const = 0;
    // Amount the payer covers. Throws a BillingError subclass to reject a claim.
    virtual double amountCovered(const Bill& b) const = 0;
};

class SelfPay : public Payer {
public:
    std::string name() const override { return "SelfPay"; }
    double amountCovered(const Bill&) const override { return 0.0; }
};

class Insurance : public Payer {
    double coverLimit, copayPercent;
public:
    Insurance(double limit, double copay) : coverLimit(limit), copayPercent(copay)
    {
        requireNonNegative(limit, "cover limit");
        if (copay < 0 || copay > 100) throw BillingError("co-payment out of range");
    }
    std::string name() const override { return "Insurance"; }
    double amountCovered(const Bill& b) const override
    {
        double t = b.total();
        if (t > coverLimit) throw ClaimError("exceeds cover");
        return t * (1.0 - copayPercent / 100.0);
    }
};

class GovernmentScheme : public Payer {
    std::map<std::string, double> packageRates;      // fixed package rate per bill type
public:
    explicit GovernmentScheme(std::map<std::string, double> rates)
        : packageRates(std::move(rates)) {}
    std::string name() const override { return "GovernmentScheme"; }
    double amountCovered(const Bill& b) const override
    {
        auto it = packageRates.find(b.type());
        if (it == packageRates.end()) throw UnknownPackageError("unknown package " + b.type());
        return std::min(it->second, b.total());
    }
};

// ---------- Registry: missing-record detection and ownership of bills ----------
class Registry {
    std::map<std::string, std::unique_ptr<Bill>> bills;
public:
    void add(const std::string& id, std::unique_ptr<Bill> b) { bills[id] = std::move(b); }
    const Bill& find(const std::string& id) const
    {
        auto it = bills.find(id);
        if (it == bills.end()) throw MissingRecordError("missing record " + id);
        return *it->second;
    }
};

// ---------- A1.3: day total over base-class pointers ----------
inline double dayTotal(const std::vector<std::unique_ptr<Bill>>& bills)
{
    double sum = 0;
    for (const auto& b : bills) sum += b->total();   // one virtual call per bill
    return sum;
}

// ---------- Per-claim settlement: a rejected bill is recorded, never lost ----------
struct Settlement {
    std::string patient;
    std::string payer;
    bool accepted;
    double paid;
    std::string reason;
};

inline Settlement settleOne(const Bill& b, const Payer& p)
{
    Settlement s{b.patientName(), p.name(), false, 0.0, ""};
    try {
        s.paid = p.amountCovered(b);
        s.accepted = true;
    } catch (const BillingError& e) {
        s.reason = e.what();
    }
    return s;
}

// B4.1 extension: a NEW bill type and a NEW payer, added in a new file.
// billing.hpp and every existing class are not modified.
#pragma once
#include "billing.hpp"

class DayCareBill : public Bill {                    // new bill type: package price
    double packagePrice;
public:
    DayCareBill(const std::string& p, double price) : Bill(p), packagePrice(price)
    {
        requireNonNegative(price, "package price");
    }
    double total() const override { return packagePrice; }
    std::string type() const override { return "DC"; }
};

class CorporatePayer : public Payer {                // new payer: % cover with per-claim cap
    double percent, cap;
public:
    CorporatePayer(double pct, double capAmount) : percent(pct), cap(capAmount)
    {
        if (pct < 0 || pct > 100) throw BillingError("corporate percent out of range");
        requireNonNegative(capAmount, "corporate cap");
    }
    std::string name() const override { return "Corporate"; }
    double amountCovered(const Bill& b) const override
    {
        return std::min(b.total() * percent / 100.0, cap);
    }
};

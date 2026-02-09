#pragma once

#pragma once
#include "NightBase.h"
#include "NightComp.h"

class NightDerived : public NightBase {
private:
    NightComp locationInfo;   // composition (ok to repeat)
    int objectCount;          // new derived-only data member

public:
    // default ctor
    NightDerived()
        : NightBase(), locationInfo(), objectCount(0) {
    }

    // parameterized ctor
    NightDerived(const std::string& d, int h, NightPhase p,
        const NightComp& loc, int count)
        : NightBase(d, h, p), locationInfo(loc), objectCount(count) {
    }

    // getters/setters (derived fields)
    void setLocationInfo(const NightComp& loc) { locationInfo = loc; }
    NightComp getLocationInfo() const { return locationInfo; }

    void setObjectCount(int c) { objectCount = c; }
    int getObjectCount() const { return objectCount; }

    // override print (required)
    void print(std::ostream& out) const override {
        NightBase::print(out); // MUST call base
        out << "Location empty: "
            << (locationInfo.isEmpty() ? "Yes" : "No") << "\n";
        out << "Object count: " << objectCount << "\n";
    }
};


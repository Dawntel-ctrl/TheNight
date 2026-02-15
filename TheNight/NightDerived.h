#pragma once

#include "NightBase.h"
#include "NightComp.h"

class NightDerived : public NightBase {
private:
    NightComp locationInfo;   // composition
    int objectCount;          // derived-only data

public:
    // default constructor
    NightDerived()
        : NightBase(), locationInfo(), objectCount(0) {
    }

    // parameterized constructor
    NightDerived(const std::string& d, int h, NightPhase p,
        const NightComp& loc, int count)
        : NightBase(d, h, p), locationInfo(loc), objectCount(count) {
    }

    // getters / setters
    void setLocationInfo(const NightComp& loc) { locationInfo = loc; }
    const NightComp& getLocationInfo() const { return locationInfo; }

    void setObjectCount(int c) { objectCount = c; }
    int getObjectCount() const { return objectCount; }

    // override print
    void print(std::ostream& out) const override {
        NightBase::print(out);   // call base version
        out << "Location empty: "
            << (locationInfo.isEmpty() ? "Yes" : "No") << "\n";
        out << "Object count: " << objectCount << "\n";
    }

    std::string getType() const override {   // add to public 
        return "NightDerived";
    }


};

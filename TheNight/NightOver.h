#pragma once

#include "NightBase.h"
#include "NightComp.h"

class NightOver : public NightBase {
private:
    NightComp locationInfo;   // composition
    bool isClearSky;          // derived-only data

public:
    // default constructor
    NightOver()
        : NightBase(), locationInfo(), isClearSky(false) {
    }

    // parameterized constructor
    NightOver(const std::string& d, int h, NightPhase p,
        const NightComp& loc, bool clear)
        : NightBase(d, h, p), locationInfo(loc), isClearSky(clear) {
    }

    // getters / setters
    void setLocationInfo(const NightComp& loc) { locationInfo = loc; }
    NightComp getLocationInfo() const { return locationInfo; }

    void setClearSky(bool c) { isClearSky = c; }
    bool getClearSky() const { return isClearSky; }

    // override print
    void print(std::ostream& out) const override {
        NightBase::print(out);
        out << "Location empty: "
            << (locationInfo.isEmpty() ? "Yes" : "No") << "\n";
        out << "Clear sky: " << (isClearSky ? "Yes" : "No") << "\n";
    }
};
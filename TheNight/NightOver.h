#pragma once

#include "NightBase.h"
#include "NightComp.h"

class NightOver : public NightBase {
private:
    NightComp locationInfo;
    bool isClearSky;

public:
    NightOver()
        : NightBase(), locationInfo(), isClearSky(false) {
    }

    NightOver(const std::string& d, int h, NightPhase p,
        const NightComp& loc, bool clear)
        : NightBase(d, h, p), locationInfo(loc), isClearSky(clear) {
    }

    // 3/22: required override for pure virtual getLocation() in NightBase
    // Without this, NightOver is abstract and cannot be instantiated
    std::string getLocation() const override {
        return locationInfo.getLocation();
    }

    void setLocationInfo(const NightComp& loc) { locationInfo = loc; }
    const NightComp& getLocationInfo() const { return locationInfo; }

    void setClearSky(bool c) { isClearSky = c; }
    bool getClearSky() const { return isClearSky; }

    void print(std::ostream& out) const override {
        NightBase::print(out);

        //  OLD VERSION
        /*//
        out << "Location empty: "
            << (locationInfo.isEmpty() ? "Yes" : "No") << "\n";
        */

        // 3/22print actual location
        out << "Location: " << locationInfo.getLocation() << "\n";

        out << "Clear sky: " << (isClearSky ? "Yes" : "No") << "\n";
    }

    std::string getType() const override {
        return "NightOver";
    }
};
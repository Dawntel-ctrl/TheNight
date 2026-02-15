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

    void setLocationInfo(const NightComp& loc) { locationInfo = loc; }
    const NightComp& getLocationInfo() const { return locationInfo; }

    void setClearSky(bool c) { isClearSky = c; }
    bool getClearSky() const { return isClearSky; }

    void print(std::ostream& out) const override {
        NightBase::print(out);
        out << "Location empty: "
            << (locationInfo.isEmpty() ? "Yes" : "No") << "\n";
        out << "Clear sky: " << (isClearSky ? "Yes" : "No") << "\n";
    }

    std::string getType() const override { // added
        return "NightOver";
    }



};



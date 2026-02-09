#pragma once

#include <string>
#include <iostream>

//  enum (moved out of AstronomyTracker)
enum NightPhase { EARLY_NIGHT = 1, MID_NIGHT = 2, LATE_NIGHT = 3, DAYTIME = 4 };

class NightBase {
protected:
    std::string date;   // protected member 

private:
    int hour24;
    NightPhase phase;

public:
    // default constructor
    NightBase()
        : date(""), hour24(0), phase(DAYTIME) {
    }

    // parameterized constructor
    NightBase(const std::string& d, int h, NightPhase p)
        : date(d), hour24(h), phase(p) {
    }

    // getters/setters 
    void setDate(const std::string& d) { date = d; }
    std::string getDate() const { return date; }

    void setHour24(int h) { hour24 = h; }
    int getHour24() const { return hour24; }

    void setPhase(NightPhase p) { phase = p; }
    NightPhase getPhase() const { return phase; }

    // base print
    virtual void print(std::ostream& out) const {
        out << "Date: " << date << "\n";
        out << "Hour24: " << hour24 << "\n";
        out << "Phase: " << static_cast<int>(phase) << "\n";
    }

    virtual ~NightBase() = default;
};

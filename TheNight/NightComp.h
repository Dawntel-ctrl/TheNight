#pragma once

#include <string>
#include "NightBase.h"

class NightComp {
private:
    std::string location;

public:
    // cons
    NightComp() : location("") {}
    explicit NightComp(const std::string& loc) : location(loc) {}

    // getter/setter
    void setLocation(const std::string& loc) { location = loc; }
    std::string getLocation() const { return location; }

    // helper 
    bool isEmpty() const { return location.empty(); }
};



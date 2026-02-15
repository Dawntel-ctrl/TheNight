#pragma once


#include "NightBase.h"

class NightManager {
private:
    NightBase** items;   // dynamic array of base pointers
    int size;
    int capacity;

    void resize();       // private helper

public:
    NightManager(int cap = 4);
    ~NightManager();

    void add(NightBase* item);
    void remove(int index);

    void printAll(std::ostream& out) const;

    int getSize() const;
};


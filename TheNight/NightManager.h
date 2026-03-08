#pragma once

#include "NightBase.h"
#include "NightDynamic.h"

class NightManager {
private:
    DynamicArray<NightBase*> items;

public:
    // recursive decloration
    int countRecursive(int index = 0) const;
  


   NightManager(int cap = 4);
    ~NightManager();

    void add(NightBase* item);
    void remove(int index);

    void printAll(std::ostream& out) const;

    int getSize() const;

    NightBase* operator[](int index) const;

    NightManager& operator+=(NightBase* item);
    NightManager& operator-=(int index);

    bool isSameSize(const NightManager& other) const;
};
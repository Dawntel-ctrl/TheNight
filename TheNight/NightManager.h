#pragma once

#include "NightBase.h"
//#include "NightDynamic.h"
#include <vector>
#include <string>
#include <iostream>

class NightManager {
private:
   //replacing the dynamicArray
  // DynamicArray<NightBase*> items;
    std::vector<NightBase*> items;





public:
    // 3/22 Insertion sort by hour
    void sortByHour();
    // 3/22 Binary search (requires sorted vector)
    int binarySearchByHour(int hour) const;
   // 3/22 Sequential search (linear search)
    int findByLocation(const std::string& location) const;
    
    // recursive decloration
    int countRecursive(int index = 0) const;
  


   NightManager();
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
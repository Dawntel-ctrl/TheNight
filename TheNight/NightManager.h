#pragma once

#include "NightBase.h"
#include "NightLinkedList.h"
#include "NightStack.h"
#include "NightQueue.h"
#include <string>
#include <iostream>

class NightManager {
private:
    //  NightManager now stores observations in a custom linked-list ADT.
    NightLinkedList items;

public:
    // 3/22 Insertion sort by hour
    void sortByHour();

    // 3/22 Binary search still requires sorted data, even though list access is now sequential.
    int binarySearchByHour(int hour) const;

    // 3/22 Sequential search (linear search)
    int findByLocation(const std::string& location) const;

    // Keep the manager interface stable while the list handles recursive counting.
    int countRecursive(int index = 0) const;

    // Prints observations in reverse order.
    void printNewestFirst(std::ostream& out) const;

    // Prints observations in original review order.
    void printReviewQueue(std::ostream& out) const;

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

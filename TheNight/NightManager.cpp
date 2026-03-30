#include "NightManager.h"
#include <iostream>
#include "NightException.h"

NightManager::NightManager()
{
}

NightManager::~NightManager()
{
    // The linked list ADT owns cleanup for stored observations.
}

void NightManager::add(NightBase* item)
{
    items.insertBack(item);
}

void NightManager::remove(int index)
{
    items.removeAt(index);
}

void NightManager::printAll(std::ostream& out) const
{
    //   Traversal is delegated to the custom linked list iterator.
    items.printAll(out);
}

int NightManager::getSize() const
{
    return items.getSize();
}

NightBase* NightManager::operator[](int index) const
{
    return items.getAt(index);
}

NightManager& NightManager::operator+=(NightBase* item)
{
    add(item);
    return *this;
}

NightManager& NightManager::operator-=(int index)
{
    remove(index);
    return *this;
}

bool NightManager::isSameSize(const NightManager& other) const
{
    return this->getSize() == other.getSize();
}

int NightManager::countRecursive(int index) const
{
    if (index == 0)
        return items.countRecursive();

    if (index >= items.getSize())
        return 0;

    return 1 + countRecursive(index + 1);
}

// 3/22 Sequential search implementation
int NightManager::findByLocation(const std::string& location) const
{
    return items.searchByLocation(location);
}

// 3/22 Insertion sort implementation Algorithm (sort by getHour24)
void NightManager::sortByHour()
{
    // Sort keeps prior manager behavior by swapping linked list data pointers.
    for (int i = 0; i < items.getSize() - 1; i++) {
        for (int j = 0; j < items.getSize() - 1 - i; j++) {
            NightBase* left = items.getAt(j);
            NightBase* right = items.getAt(j + 1);

            if (left->getHour24() > right->getHour24()) {
                items.swapAt(j, j + 1);
            }
        }
    }
}

// 3/22 Binary Search implementation (search by getHour24)
int NightManager::binarySearchByHour(int hour) const
{
    int low = 0;
    int high = items.getSize() - 1;

    while (low <= high)
    {
        int mid = (low + high) / 2;

        int midValue = items.getAt(mid)->getHour24();

        if (midValue == hour)
        {
            return mid;  // found
        }
        else if (midValue < hour)
        {
            low = mid + 1;  // search right
        }
        else
        {
            high = mid - 1; // search left
        }
    }

    return -1;  // not found
}

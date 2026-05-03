#include "NightManager.h"
#include "NightJsonLoader.h"
#include <iostream>
#include "NightException.h"

NightManager::NightManager()
{
}

NightManager::~NightManager()
{
    // The linked list ADT owns cleanup for stored observations.
}

// NightManager exposes this function so the existing program can load JSON data.
int NightManager::loadFromJsonFile(const std::string& fileName)
{
    return NightJsonLoader::loadFromJsonFile(*this, fileName);
}

// STL map insert helper
void NightManager::insertLocationIndex(NightBase* item, int index)
{
    if (item == nullptr)
        return;

    locationMap.insertLocation(item->getLocation(), index);
}

// STL map rebuild helper
void NightManager::rebuildLocationIndexMap()
{
    locationMap.clear();

    for (int i = 0; i < items.getSize(); i++) {
        insertLocationIndex(items.getAt(i), i);
    }
}

void NightManager::add(NightBase* item)
{
    int newIndex = items.getSize();

    items.insertBack(item);

    // STL map insert operation
    locationMap.insertLocation(item->getLocation(), newIndex);
}

void NightManager::remove(int index)
{
    // Get the location first so the map can delete that key before the item is removed.
    NightBase* target = items.getAt(index);

    if (target != nullptr) {
        // STL map delete operation
        locationMap.deleteLocation(target->getLocation());
    }

    items.removeAt(index);

    // Indexes may have shifted, so rebuild the map.
    rebuildLocationIndexMap();
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

// 3/22 Sequential search enhanced by STL map.
int NightManager::findByLocation(const std::string& location) const
{
    return locationMap.lookupLocation(location);
}

// STL map delete use
int NightManager::removeByLocation(const std::string& location)
{
    int index = findByLocation(location);

    if (index == -1)
        return -1;

    remove(index);
    return index;
}

// STL map iteration use
void NightManager::printLocationIndexMap(std::ostream& out) const
{
    locationMap.printAll(out);
}

int NightManager::getLocationMapSize() const
{
    return locationMap.getSize();
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

    // Sorting changes indexes, so the map must be rebuilt.
    rebuildLocationIndexMap();
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

// The stack reverses the linked list order so the newest observation prints first.
void NightManager::printNewestFirst(std::ostream& out) const
{
    if (items.isEmpty())
        return;

    NightStack stack(items.getSize());

    for (int i = 0; i < items.getSize(); i++) {
        stack.push(items.getAt(i));
    }

    while (!stack.isEmpty()) {
        NightBase* current = stack.pop();

        if (current != nullptr) {
            current->print(out);
            out << "-----------------\n";
        }
    }
}

// The queue preserves the linked list order so observations are reviewed first-in, first-out.
void NightManager::printReviewQueue(std::ostream& out) const
{
    if (items.isEmpty())
        return;

    NightQueue queue(items.getSize());

    for (int i = 0; i < items.getSize(); i++) {
        queue.enqueue(items.getAt(i));
    }

    while (!queue.isEmpty()) {
        NightBase* current = queue.dequeue();

        if (current != nullptr) {
            current->print(out);
            out << "-----------------\n";
        }
    }
}
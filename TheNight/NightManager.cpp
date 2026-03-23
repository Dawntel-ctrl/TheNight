#include "NightManager.h"
#include <iostream>
#include "NightException.h"

NightManager::NightManager()
// : items(cap)   // OLD (DynamicArray init) 
{
    // vector does NOT need manual capacity initialization
}

NightManager::~NightManager()
{
    // We still own the NightBase* objects
    // for (int i = 0; i < items.getSize(); i++) {  
    for (int i = 0; i < items.size(); i++) {        // use vector size()
        delete items[i];
        items[i] = nullptr;   // Will this prevent dangling pointer / double delete in debug
    }
}

void NightManager::add(NightBase* item)
{
    // items.add(item);   // OLD (DynamicArray)
    items.push_back(item);   //  vector add
}

void NightManager::remove(int index)
{
    // if (index < 0 || index >= items.getSize()) // OLD 
    if (index < 0 || index >= items.size())       // vec
        throw NightException("NightManager: invalid removal index");

    // delete items[index];     // OLD 
    delete items.at(index);     // safe access (throws if invalid)

    items[index] = nullptr;     // maybe prevent dangling pointer before erase

    // items.remove(index);     // OLD (DynamicArray shift) 
    items.erase(items.begin() + index);   // vector shift + remove
}

void NightManager::printAll(std::ostream& out) const
{
    // for (int i = 0; i < items.getSize(); i++) {   // OLD 
    for (int i = 0; i < items.size(); i++) {        // vec
        items[i]->print(out);
        out << "-----------------\n";
    }
}

int NightManager::getSize() const
{
    // return items.getSize();   
    return static_cast<int>(items.size());
}

NightBase* NightManager::operator[](int index) const
{
    // return items[index];   

    // will this manual bounds check so we throw NightException instead of std::out_of_range
    if (index < 0 || index >= static_cast<int>(items.size()))
        throw NightException("NightManager: invalid index");

    return items[index];  // FIX: removed .at() to avoid std::out_of_range
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
    // if (index >= items.getSize())   
    if (index >= items.size())        // base case uses vec
        return 0;

    // Recursive case
    return 1 + countRecursive(index + 1);
}

// 3/22 Sequential search implementation
int NightManager::findByLocation(const std::string& location) const
{
    for (size_t i = 0; i < items.size(); i++)
    {
        // polymorphism from NightBase
        // OLD (invalid): items[i]->getLocationInfo().getLocation()
        //all derived classes implement getLocation()
        if (items[i]->getLocation() == location)
            return static_cast<int>(i);
    }

    return -1;
}

// 3/22 Insertion sort implementation Algorithm (sort by getHour24)
void NightManager::sortByHour()
{
    // 3/22: use size_t for vector indexing consistency
    for (size_t i = 1; i < items.size(); i++)
    {
        NightBase* key = items[i];

        // 3/22: j must stay int because it goes negative
        int j = static_cast<int>(i) - 1;

        // shift elements greater than key
        while (j >= 0 && items[j]->getHour24() > key->getHour24())
        {
            items[j + 1] = items[j];
            j--;
        }

        items[j + 1] = key;
    }
}

// 3/22 Binary Search implementation (search by getHour24)
int NightManager::binarySearchByHour(int hour) const
{
    int low = 0;
    int high = static_cast<int>(items.size()) - 1;

    while (low <= high)
    {
        int mid = (low + high) / 2;

        int midValue = items[mid]->getHour24();

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
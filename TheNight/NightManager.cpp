#include "NightManager.h"
#include <iostream>
#include "NightException.h"

NightManager::NightManager(int cap)
    : items(cap)   // initialize template container
{
}

NightManager::~NightManager()
{
    // We still own the NightBase* objects
    for (int i = 0; i < items.getSize(); i++) {
        delete items[i];
    }
}

void NightManager::add(NightBase* item)
{
    items.add(item);
}

void NightManager::remove(int index)
{
    if (index < 0 || index >= items.getSize()) // 3/1
        throw NightException("NightManager: invalid removal index");

    delete items[index];     // delete actual object
    items.remove(index);     // shift handled by template
}

void NightManager::printAll(std::ostream& out) const
{
    for (int i = 0; i < items.getSize(); i++) {
        items[i]->print(out);
        out << "-----------------\n";
    }
}

int NightManager::getSize() const
{
    return items.getSize();
}

NightBase* NightManager::operator[](int index) const
{
    return items[index];   // bounds checking handled inside template
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
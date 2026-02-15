#include "NightManager.h"
#include <iostream>

NightManager::NightManager(int cap)
    : size(0), capacity(cap)
{
    items = new NightBase * [capacity];
}

NightManager::~NightManager()
{
    // delete each object
    for (int i = 0; i < size; i++) {
        delete items[i];
    }

    // delete array
    delete[] items;
}

void NightManager::resize()
{
    capacity *= 2;

    NightBase** temp = new NightBase * [capacity];

    for (int i = 0; i < size; i++) {
        temp[i] = items[i];
    }

    delete[] items;
    items = temp;
}

void NightManager::add(NightBase* item)
{
    if (size == capacity) {
        resize();
    }

    items[size++] = item;
}

void NightManager::remove(int index)
{
    if (index < 0 || index >= size)
        return;

    delete items[index];

    for (int i = index; i < size - 1; i++) {
        items[i] = items[i + 1];
    }

    size--;
}

void NightManager::printAll(std::ostream& out) const
{
    for (int i = 0; i < size; i++) {
        items[i]->print(out);
        out << "-----------------\n";
    }
}

int NightManager::getSize() const
{
    return size;
}

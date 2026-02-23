#pragma once
#pragma once
// replaments for NightBase** items;
//int size;
//int capacity;
//void resize();


template<typename T>
class DynamicArray {
private:
    T* items;
    int size;
    int capacity;

    void resize()
    {
        capacity *= 2;

        T* temp = new T[capacity];

        for (int i = 0; i < size; i++) {
            temp[i] = items[i];
        }

        delete[] items;
        items = temp;
    }

public:
    DynamicArray(int cap = 4)
        : size(0), capacity(cap)
    {
        items = new T[capacity];
    }

    ~DynamicArray()
    {
        delete[] items;
    }

    void add(T item)
    {
        if (size == capacity)
            resize();

        items[size++] = item;
    }

    void remove(int index)
    {
        if (index < 0 || index >= size)
            return;

        for (int i = index; i < size - 1; i++) {
            items[i] = items[i + 1];
        }

        size--;
    }

    T operator[](int index) const
    {
        if (index < 0 || index >= size)
            return T{}; // works for pointer type*** changed to safe default value

        return items[index];
    }

    int getSize() const
    {
        return size;
    }
};
#pragma once

#include "NightBase.h"
#include "NightException.h"

class NightStack {
private:
    // Custom array-based stack for NightBase observation pointers.
    NightBase** data;
    int capacity;
    int topIndex;

public:
    NightStack(int maxSize = 10)
        : data(nullptr), capacity(maxSize), topIndex(-1) {
        if (capacity <= 0)
            throw NightException("NightStack: capacity must be greater than zero");

        data = new NightBase * [capacity];
    }

    ~NightStack()
    {
        // Delete only the array storage.
        delete[] data;
    }

    // Push adds an observation pointer to the top of the stack.
    void push(NightBase* item)
    {
        if (isFull())
            throw NightException("NightStack: cannot push onto a full stack");

        topIndex++;
        data[topIndex] = item;
    }

    // Pop removes and returns the observation pointer at the top.
    NightBase* pop()
    {
        if (isEmpty())
            throw NightException("NightStack: cannot pop from an empty stack");

        NightBase* item = data[topIndex];
        topIndex--;
        return item;
    }

    // Peek returns the top observation pointer without removing it.
    NightBase* peek() const
    {
        if (isEmpty())
            throw NightException("NightStack: cannot peek at an empty stack");

        return data[topIndex];
    }

    // Top is another name for peek, matching common stack terminology.
    NightBase* top() const
    {
        return peek();
    }

    bool isEmpty() const
    {
        return topIndex == -1;
    }

    bool isFull() const
    {
        return topIndex == capacity - 1;
    }

    int getSize() const
    {
        return topIndex + 1;
    }

    int getCapacity() const
    {
        return capacity;
    }

    // Assignment-name wrapper for Push.
    void Push(NightBase* item)
    {
        push(item);
    }

    // Assignment-name wrapper for Pop.
    NightBase* Pop()
    {
        return pop();
    }

    // Assignment-name wrapper for Peek.
    NightBase* Peek() const
    {
        return peek();
    }

    // Assignment-name wrapper for Top.
    NightBase* Top() const
    {
        return top();
    }
};

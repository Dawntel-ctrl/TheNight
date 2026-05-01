#pragma once

#include "NightBase.h"
#include "NightException.h"

class NightQueue {
private:
    // Custom circular array-based queue for NightBase observation pointers.
    NightBase** data;
    int capacity;
    int frontIndex;
    int backIndex;
    int count;

public:
    NightQueue(int maxSize = 10)
        : data(nullptr), capacity(maxSize), frontIndex(0), backIndex(-1), count(0) {
        if (capacity <= 0)
            throw NightException("NightQueue: capacity must be greater than zero");

        data = new NightBase * [capacity];
    }

    ~NightQueue()
    {
        // Delete only the array storage.
        delete[] data;
    }

    // Enqueue adds an observation pointer to the back of the queue.
    void enqueue(NightBase* item)
    {
        if (isFull())
            throw NightException("NightQueue: cannot enqueue into a full queue");

        backIndex = (backIndex + 1) % capacity;
        data[backIndex] = item;
        count++;
    }

    // Dequeue removes and returns the observation pointer at the front.
    NightBase* dequeue()
    {
        if (isEmpty())
            throw NightException("NightQueue: cannot dequeue from an empty queue");

        NightBase* item = data[frontIndex];
        frontIndex = (frontIndex + 1) % capacity;
        count--;
        return item;
    }

    // Front returns the front observation pointer without removing it.
    NightBase* front() const
    {
        if (isEmpty())
            throw NightException("NightQueue: cannot view the front of an empty queue");

        return data[frontIndex];
    }

    bool isEmpty() const
    {
        return count == 0;
    }

    bool isFull() const
    {
        return count == capacity;
    }

    int getSize() const
    {
        return count;
    }

    int getCapacity() const
    {
        return capacity;
    }

    // Assignment-name wrapper for Enqueue.
    void Enqueue(NightBase* item)
    {
        enqueue(item);
    }

    // Assignment-name wrapper for Dequeue.
    NightBase* Dequeue()
    {
        return dequeue();
    }

    // Assignment-name wrapper for Front.
    NightBase* Front() const
    {
        return front();
    }
};

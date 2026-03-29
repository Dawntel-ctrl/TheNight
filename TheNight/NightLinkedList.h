#pragma once

#include "NightBase.h"
#include "NightException.h"
#include <iostream>
#include <string>

class NightLinkedList {
private:
    //  Custom unordered singly linked list node for manager storage.
    struct Node {
        NightBase* data;
        Node* next;

        Node(NightBase* value, Node* nextNode = nullptr)
            : data(value), next(nextNode) {
        }
    };

    Node* head;
    Node* tail;
    int size;

    Node* getNodeAt(int index) const
    {
        if (index < 0 || index >= size)
            throw NightException("NightLinkedList: invalid index");

        Node* current = head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }

        return current;
    }

    int countRecursive(Node* current) const
    {
        if (current == nullptr)
            return 0;

        return 1 + countRecursive(current->next);
    }

public:
    //  Iterator class!! 
    class Iterator {
    private:
        Node* current;

    public:
        Iterator(Node* start = nullptr)
            : current(start) {
        }

        bool hasCurrent() const
        {
            return current != nullptr;
        }

        void next()
        {
            if (current != nullptr)
                current = current->next;
        }

        NightBase* getData() const
        {
            if (current == nullptr)
                throw NightException("NightLinkedList::Iterator: invalid current node");

            return current->data;
        }
    };

    NightLinkedList()
        : head(nullptr), tail(nullptr), size(0) {
    }

    ~NightLinkedList()
    {
        Node* current = head;

        while (current != nullptr) {
            Node* nextNode = current->next;
            delete current->data;
            delete current;
            current = nextNode;
        }
    }

    // Front insertion supports one unordered list insertion position.
    void insertFront(NightBase* item)
    {
        Node* newNode = new Node(item, head);
        head = newNode;

        if (tail == nullptr)
            tail = newNode;

        size++;
    }

    // Back insertion preserves the manager's current append behavior.
    void insertBack(NightBase* item)
    {
        Node* newNode = new Node(item);

        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        }
        else {
            tail->next = newNode;
            tail = newNode;
        }

        size++;
    }

    // Delete removes a node and the owned observation pointer.
    void removeAt(int index)
    {
        if (index < 0 || index >= size)
            throw NightException("NightLinkedList: invalid removal index");

        Node* target = nullptr;

        if (index == 0) {
            target = head;
            head = head->next;

            if (head == nullptr)
                tail = nullptr;
        }
        else {
            Node* previous = getNodeAt(index - 1);
            target = previous->next;
            previous->next = target->next;

            if (target == tail)
                tail = previous;
        }

        delete target->data;
        delete target;
        size--;
    }

    // Search walks the list to locate a matching observation.
    int searchByLocation(const std::string& location) const
    {
        Node* current = head;
        int index = 0;

        while (current != nullptr) {
            if (current->data->getLocation() == location)
                return index;

            current = current->next;
            index++;
        }

        return -1;
    }

    NightBase* getAt(int index) const
    {
        return getNodeAt(index)->data;
    }

    // Swap supports manager side sorting without exposing nodes publicly.
    void swapAt(int firstIndex, int secondIndex)
    {
        if (firstIndex == secondIndex)
            return;

        Node* firstNode = getNodeAt(firstIndex);
        Node* secondNode = getNodeAt(secondIndex);

        NightBase* temp = firstNode->data;
        firstNode->data = secondNode->data;
        secondNode->data = temp;
    }

    int getSize() const
    {
        return size;
    }

    bool isEmpty() const
    {
        return size == 0;
    }

    // Traverse prints every node by using the custom iterator.
    void printAll(std::ostream& out) const
    {
        Iterator it = begin();

        while (it.hasCurrent()) {
            it.getData()->print(out);
            out << "-----------------\n";
            it.next();
        }
    }

    int countRecursive() const
    {
        return countRecursive(head);
    }

    Iterator begin() const
    {
        return Iterator(head);
    }
};

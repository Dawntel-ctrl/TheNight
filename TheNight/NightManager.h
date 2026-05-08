#pragma once

#include "NightBase.h"
#include "NightLinkedList.h"
#include "NightStack.h"
#include "NightQueue.h"
#include "NightLocationMap.h"
#include <string>
#include <iostream>

class NightManager {
private:
    //  NightManager now stores observations in a custom linked-list ADT.
    NightLinkedList items;

    //  NightManager enhances location lookup with STL map wrapper to map each location to its manager index.
    NightLocationMap locationMap;

    // Inserts one location/index pair into the STL map wrapper.
    void insertLocationIndex(NightBase* item, int index);

    // Rebuilds the map after removals or sorting because indexes may change.
    void rebuildLocationIndexMap();

public:
    // This function lets NightManager load data from a JSON file.
    int loadFromJSONFile(const std::string& fileName);

    // Loads trivia questions from api.macomb.io into the existing NightManager.
    int loadTriviaFromAPI(
        const std::string& category,
        const std::string& difficulty,
        int count
    );

    // Adds a sample trivia question to the server and displays the assigned ID.
    int POSTSampleTriviaQuestion(std::ostream& out);

    // 3/22 Insertion sort by hour
    void sortByHour();

    // 3/22 Binary search still requires sorted data, even though list access is now sequential.
    int binarySearchByHour(int hour) const;

    // 3/22 Sequential search enhanced with STL map lookup.
    int findByLocation(const std::string& location) const;

    // STL map delete operation
    int removeByLocation(const std::string& location);

    // STL map iteration operation
    void printLocationIndexMap(std::ostream& out) const;

    // STL helper for doctests
    int getLocationMapSize() const;

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
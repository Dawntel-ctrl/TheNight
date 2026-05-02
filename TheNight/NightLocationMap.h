#pragma once

#include "NightException.h"
#include <map>
#include <string>
#include <iostream>

// Custom wrapper class around an STL map.
class NightLocationMap {
private:
    // std::map is useful here because locations are unique lookup keys.
    std::map<std::string, int> locationIndexMap;

public:
    NightLocationMap()
    {
    }

    ~NightLocationMap()
    {
    }

    // Insert adds one location/index key-value pair to the map.
    void insertLocation(const std::string& location, int index)
    {
        if (locationIndexMap.find(location) == locationIndexMap.end()) {
            locationIndexMap.insert(std::pair<std::string, int>(location, index));
        }
    }

    // Lookup retrieves the index for a location.
    int lookupLocation(const std::string& location) const
    {
        std::map<std::string, int>::const_iterator it = locationIndexMap.find(location);

        if (it == locationIndexMap.end())
            return -1;

        return it->second;
    }

    // Delete removes one key-value pair from the map.
    bool deleteLocation(const std::string& location)
    {
        return locationIndexMap.erase(location) > 0;
    }

    // Clear removes all key-value pairs from the map.
    void clear()
    {
        locationIndexMap.clear();
    }

    // Iterate prints every key-value pair in the map.
    void printAll(std::ostream& out) const
    {
        std::map<std::string, int>::const_iterator it;

        for (it = locationIndexMap.begin(); it != locationIndexMap.end(); it++) {
            out << it->first << ": " << it->second << "\n";
        }
    }

    int getSize() const
    {
        return static_cast<int>(locationIndexMap.size());
    }

    bool isEmpty() const
    {
        return locationIndexMap.empty();
    }
};

#pragma once

#include "NightManager.h"
#include "NightDerived.h"
#include "NightComp.h"
#include "NightException.h"
#include "json.hpp"

#include <fstream>
#include <string>

// JSON loader helper
class NightJsonLoader {
public:
    // This class keeps the JSON parsing separate from NightManager.
    static int loadFromJsonFile(NightManager& manager, const std::string& fileName)
    {
        try {
            std::ifstream input(fileName);

            if (!input.is_open())
                throw NightException("NightJsonLoader: JSON file not found");

            nlohmann::json data;
            input >> data;

            if (!data.is_array())
                throw NightException("NightJsonLoader: JSON root must be an array");

            int loadedCount = 0;

            for (const auto& item : data) {
                std::string date = item.at("date").get<std::string>();
                int hour24 = item.at("hour24").get<int>();
                std::string phaseText = item.at("phase").get<std::string>();
                std::string location = item.at("location").get<std::string>();
                int objectCount = item.at("objectCount").get<int>();

                auto phase = MID_NIGHT;

                if (phaseText == "DAYTIME")
                    phase = DAYTIME;
                else if (phaseText == "EARLY_NIGHT")
                    phase = EARLY_NIGHT;
                else if (phaseText == "MID_NIGHT")
                    phase = MID_NIGHT;
                else if (phaseText == "LATE_NIGHT")
                    phase = LATE_NIGHT;

                manager.add(new NightDerived(
                    date,
                    hour24,
                    phase,
                    NightComp(location),
                    objectCount
                ));

                loadedCount++;
            }

            return loadedCount;
        }
        catch (...) {
            // Handles missing file, malformed JSON, missing keys, or wrong value types.
            return -1;
        }
    }
};

#pragma once

#include "HttpClient.h"
#include "NightManager.h"
#include "NightDerived.h"
#include "NightComp.h"
#include "json.hpp"

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <map>
#include <cstddef>

class NightTriviaHttpClient : public HttpClient {
private:
    // This derived class captures the raw HTTP response body returned by HttpClient.
    std::string response;

public:
    // Called when the HTTP response body begins.
    void StartOfData() override
    {
        response.clear();
    }

    // Called when a chunk of response body data arrives.
    void Data(const char* data, const unsigned int size) override
    {
        response.append(data, size);
    }

    // Called when the full HTTP response body has finished.
    void EndOfData() override
    {
    }

    // Gives the rest of the program access to the completed response body.
    std::string GetResponse() const
    {
        return response;
    }
};


class NightTriviaAPIClient {
private:
    // This class keeps REST API and JSON parsing code separate from NightManager.
    static auto difficultyToPhase(const std::string& difficulty)
    {
        if (difficulty == "easy")
            return EARLY_NIGHT;

        if (difficulty == "medium")
            return MID_NIGHT;

        if (difficulty == "hard")
            return LATE_NIGHT;

        return MID_NIGHT;
    }

    static std::map<std::string, std::string> buildTriviaQueryParams(
        const std::string& category,
        const std::string& difficulty,
        int count
    )
    {
        std::map<std::string, std::string> queryParams;

        queryParams["count"] = std::to_string(count);

        if (category != "all")
            queryParams["category"] = category;

        if (difficulty != "all")
            queryParams["difficulty"] = difficulty;

        return queryParams;
    }

public:
    // Parses a trivia GET response and loads each question into the existing NightManager.
    static int loadTriviaFromJSON(NightManager& manager, const std::string& responseBody)
    {
        try {
            nlohmann::json root = nlohmann::json::parse(responseBody);

            const nlohmann::json& questions = root.at("questions");

            int loadedCount = 0;

            for (const auto& questionItem : questions) {
                int id = questionItem.at("id").get<int>();
                std::string category = questionItem.at("category").get<std::string>();
                std::string difficulty = questionItem.at("difficulty").get<std::string>();
                std::string question = questionItem.at("question").get<std::string>();
                std::string answer = questionItem.at("answer").get<std::string>();

                std::vector<std::string> choices =
                    questionItem.at("choices").get<std::vector<std::string>>();

                std::string locationText =
                    "Trivia [" + category + "/" + difficulty + "]: " +
                    question + " Answer: " + answer;

                // NightManager::add stores a loaded trivia question into the linked list and updates the STL map.
                manager.add(new NightDerived(
                    "API_TRIVIA",
                    id % 24,
                    difficultyToPhase(difficulty),
                    NightComp(locationText),
                    static_cast<int>(choices.size())
                ));

                loadedCount++;
            }

            return loadedCount;
        }
        catch (const nlohmann::json::exception&) {
            // Handles malformed JSON, missing fields, or fields with wrong types.
            return -1;
        }
    }

    // Performs a live GET request to api.macomb.io/trivia.
    static int loadTriviaFromAPI(
        NightManager& manager,
        const std::string& category,
        const std::string& difficulty,
        int count
    )
    {
        NightTriviaHttpClient client;

        std::map<std::string, std::string> queryParams =
            buildTriviaQueryParams(category, difficulty, count);

        client.Get("api.macomb.io/trivia", queryParams);

        return loadTriviaFromJSON(manager, client.GetResponse());
    }

    // Parses a POST response and displays the assigned ID returned by the server.
    static int parsePOSTResponse(const std::string& responseBody, std::ostream& out)
    {
        try {
            nlohmann::json root = nlohmann::json::parse(responseBody);

            if (root.contains("question") && root["question"].contains("id")) {
                int id = root["question"]["id"].get<int>();

                out << "Trivia question added successfully. Assigned ID: "
                    << id << "\n";

                return id;
            }

            if (root.contains("error")) {
                out << "Trivia POST error: "
                    << root["error"].get<std::string>() << "\n";
            }

            return -1;
        }
        catch (const nlohmann::json::exception&) {
            // Handles cases where the POST response cannot be parsed.
            out << "Could not parse trivia POST response.\n";
            return -1;
        }
    }

    // Performs a live POST request to add a new trivia question.
    static int POSTSampleQuestion(std::ostream& out)
    {
        nlohmann::json body;

        body["category"] = "science";
        body["difficulty"] = "easy";
        body["question"] = "What is the powerhouse of the cell?";
        body["answer"] = "Mitochondria";
        body["choices"] = {
            "Mitochondria",
            "Nucleus",
            "Ribosome",
            "Golgi apparatus"
        };

        NightTriviaHttpClient client;

        client.Post("api.macomb.io/trivia", body.dump());

        return parsePOSTResponse(client.GetResponse(), out);
    }
};

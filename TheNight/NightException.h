#pragma once

#pragma once
#include <stdexcept>
#include <string>
//custum exception
class NightException : public std::runtime_error {
public:
    explicit NightException(const std::string& message)
        : std::runtime_error(message) {}
};
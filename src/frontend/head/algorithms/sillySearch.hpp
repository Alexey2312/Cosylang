#pragma once

#include <stdexcept>
#include <vector>

class SillySearch
{
public:
    template<typename T>
    unsigned int search(const std::vector<T>& vector, T value);
};

template<typename T>
unsigned int SillySearch::search(const std::vector<T>& vector, T value) {
    for (unsigned int index = 0; index < vector.size(); index++)
    {
        if (vector[index] == value)
        {
            return index;
        }
    }
    throw std::runtime_error("SillySearch: Value not found");
}

#pragma once
#include <vector>

class SillySearch
{
public:
    template<typename T>
    static int search(std::vector<T> vector, T target);
};

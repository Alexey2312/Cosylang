#pragma once
#include <vector>

template<typename T>
unsigned int sillySearch(const std::vector<T>& v, T val) {
    for (unsigned int i = 0; i < v.size(); ++i)
        if (v[i] == val) return i;
    return static_cast<unsigned int>(-1);
}

template unsigned int sillySearch<char>(const std::vector<char>&, char);
template unsigned int sillySearch<int>(const std::vector<int>&, int);
template unsigned int sillySearch<float>(const std::vector<float>&, float);
template unsigned int sillySearch<double>(const std::vector<double>&, double);

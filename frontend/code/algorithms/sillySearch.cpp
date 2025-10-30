#include "../../head/algorithms/sillySearch.hpp"
#include<vector>

template<typename T>
int SillySearch::search(std::vector<T> vector, T target)
{
    for (int i = 0; i < vector.size(); i++)
    {
        if (vector[i] == target)
        {
            target = i;
            return i;
        }
    }
    return -1;
};

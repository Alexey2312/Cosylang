#include "../../head/algorithms/sillySearch.hpp"
#include<vector>

template<typename T>
unsigned int sillySearch(std::vector<T>* inputVector, T target)
{
    for (int i = 0; i < inputVector->size(); i++)
    {
        if (*inputVector[i] == target)
        {
            target = i;
            return i;
        }
    }
    return -1;
};

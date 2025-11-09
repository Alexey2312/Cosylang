#include "../head/convertor.hpp"
#include <vector>

template<typename T>
std::vector<T*> vectorToPtr(std::vector<T>& vec) {
    std::vector<T*> ptrVec;
    for (auto& elem : vec) {
        ptrVec.push_back(&elem);
    }
    return ptrVec;
}

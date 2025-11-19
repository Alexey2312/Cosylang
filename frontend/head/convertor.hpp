#pragma once
#include <vector>
#include <string>

std::string convertCharVectorToString(const std::vector<char>& inputVector);

std::vector<char> convertStringToCharVector(const std::string& inputString);

template<typename T>
std::vector<T*> vectorToPtr(std::vector<T>& vec);

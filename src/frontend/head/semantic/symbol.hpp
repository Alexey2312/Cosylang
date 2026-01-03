#pragma once

#include "typesOfData.hpp"
#include <any>
#include <string>
#include <variant>
#include <optional>
#include <vector>

class VariableInfo
{
    std::string name;
    CosylangType dataType;
    std::any value;
};

class FunctionParameterInfo
{
    std::string name;
    CosylangType dataType;
    std::optional<std::any> defaultValue = std::nullopt;
};

class FunctionInfo
{
    std::string name;
    std::vector<FunctionParameterInfo> parameters;
    CosylangType returnType;
};


class Symbol
{
public:
    Symbol(const std::string& name, std::variant<VariableInfo, FunctionInfo> kindInfo);
};

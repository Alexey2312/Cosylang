#pragma once

#include "typesOfData.hpp"
#include "cosylangValue.hpp"
#include <llvm/IR/DerivedTypes.h>
#include <string>
#include <variant>
#include <optional>
#include <vector>

class VariableInfo
{
    std::string name;
    CosylangType dataType;
    CosylangValue value;

public:
    VariableInfo(std::string thisName, CosylangType thisDataType, CosylangValue thisValue) : name(thisName), dataType(thisDataType), value(thisValue) {}

    std::string getName() { return name; }
    void setName(std::string newName) { name = newName; }

    CosylangType getDataType() { return dataType; }
    void setDataType(CosylangType newDataType) {}

    CosylangValue getValue() { return value; }
    void setValue(CosylangValue newValue) { value = newValue; }

};

class FunctionParameterInfo
{
    std::string name;
    CosylangType dataType;
    std::optional<CosylangValue> defaultValue = std::nullopt;
public:

    FunctionParameterInfo(std::string thisName, CosylangType thisDataType, std::optional<CosylangValue> thisDefaultValue = std::nullopt) : name(thisName), dataType(thisDataType), defaultValue(thisDefaultValue) {}

    std::string getName() { return name; }
    void setName(std::string newName) { name = newName; }

    CosylangType getDataType() { return dataType; }
    void setDataType(CosylangType newDataType) { dataType = newDataType; }

    std::optional<CosylangValue> getDefaultValue() { return defaultValue; }
    void setDefaultValue(std::optional<CosylangValue> newDefaultValue) { defaultValue = newDefaultValue; }
};

class FunctionInfo
{
    std::string name;
    std::vector<FunctionParameterInfo> parameters;
    CosylangType returningType = VoidType();

public:

    FunctionInfo(std::string thisName, std::vector<FunctionParameterInfo> thisParameters, CosylangType thisReturningType = VoidType()) : name(thisName), parameters(thisParameters), returningType(thisReturningType) {}

    std::string getName() { return name; }
    void setName(std::string newName) { name = newName; }

    std::vector<FunctionParameterInfo> getParameters() { return parameters; }
    void setParameters(std::vector<FunctionParameterInfo> newParameters) { parameters = newParameters; }

    CosylangType getReturningType() { return returningType; }
    void setReturningType(CosylangType newReturningType) { returningType = newReturningType; }

};


class Symbol
{
    std::string name;
    std::variant<VariableInfo, FunctionInfo> kindInfo;

public:
    Symbol(const std::string& thisName, std::variant<VariableInfo, FunctionInfo> thisKindInfo) : name(thisName), kindInfo(thisKindInfo) {}

    std::optional<const FunctionInfo*> getFunctionInfo() const
    {
        if (const FunctionInfo* returningFunctionInfo = std::get_if<FunctionInfo>(&kindInfo))
        {
            return returningFunctionInfo;
        }
        return std::nullopt;
    }

    std::optional<const VariableInfo*> getVariableInfo() const
    {
        if (const VariableInfo* returningVariableInfo = std::get_if<VariableInfo>(&kindInfo))
        {
            return returningVariableInfo;
        }
        return std::nullopt;
    }
};

#pragma once

#include "typesOfData.hpp"
#include <optional>
#include <string>
#include <vector>

class TypeDefiner
{
    static inline std::vector<CosylangType> types =
    {
        IntType(8),
        IntType(16),
        IntType(32),
        IntType(64),
        IntType(128),

        Float32Type(),
        Float64Type(),
        Float80Type(),

        BoolType(),
        VoidType(),
    };

    static inline std::vector<CosylangType> customTypes;

public:

    void setCustomTypes(std::vector<CosylangType> newCustomTypes) { customTypes = newCustomTypes; }
    void addCustomType(CosylangType newCustomType) { customTypes.push_back(newCustomType); }

    static std::optional<CosylangType> defineTypeByName(std::string nameOfType)
    {
        for (CosylangType& currentType : types)
        {
            if (nameOfType == currentType.getName())
            {
                return currentType;
            }
        }

        for (CosylangType& currentType : customTypes)
        {
            if (nameOfType == currentType.getName())
            {
                return currentType;
            }
        }

        return std::nullopt;
    }
};

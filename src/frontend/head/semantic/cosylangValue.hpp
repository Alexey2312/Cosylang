#pragma once

#include "typesOfData.hpp"
#include <memory>
#include "../node.hpp"

class CosylangValue
{
    CosylangType type;
    std::shared_ptr<Node> value;
public:
    void setType(CosylangType newType)
    {
        type = newType;
    }
    CosylangType getType()
    {
        return type;
    }
    void setValue(std::shared_ptr<Node> newValue)
    {
        value = newValue;
    }
    std::shared_ptr<Node> getValue()
    {
        return value;
    }

    CosylangValue(std::shared_ptr<Node> newValue, CosylangType typeOfValue) : type(typeOfValue), value(newValue) {}

};

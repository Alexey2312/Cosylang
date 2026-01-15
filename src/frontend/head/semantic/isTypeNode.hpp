#pragma once
#include <functional>
#include <vector>
#include <memory>
#include "../node.hpp"

class IsTypeNodeChecker
{
public:
    static bool isIntType(std::shared_ptr<Node> checkingNode)
    {
        std::vector<std::string> vaildNamesOfThisType = {"int8", "int16", "int32", "int64", "int128"};

        for(std::string currentName : vaildNamesOfThisType)
        {
            if (checkingNode->getValue() == currentName)
            {
                return true;
            }
        }
        return false;
    }

    static bool isFloatType(std::shared_ptr<Node> checkingNode)
    {
        std::vector<std::string> vaildNamesOfThisType = {"float8", "float16", "float32", "float64", "float80"};

        for(std::string& currentName : vaildNamesOfThisType)
        {
            if (checkingNode->getValue() == currentName)
            {
                return true;
            }
        }
        return false;
    }

    static bool isBoolType(std::shared_ptr<Node> checkingNode)
    {
        std::vector<std::string> vaildNamesOfThisType = {"bool"};

        for(std::string currentName : vaildNamesOfThisType)
        {
            if (checkingNode->getValue() == currentName)
            {
                return true;
            }
        }
        return false;
    }

    static bool isStringType(std::shared_ptr<Node> checkingNode)
    {
        std::vector<std::string> vaildNamesOfThisType = {"string"};

        for(std::string currentName : vaildNamesOfThisType)
        {
            if (checkingNode->getValue() == currentName)
            {
                return true;
            }
        }
        return false;
    }



    static inline std::vector<std::function<bool(std::shared_ptr<Node>)>> checkingFunctions =
    {
        isIntType,
        isFloatType,
        isBoolType,
        isStringType,
    };

    static bool isValidNameOfBaseType(std::string name)
    {
        for(std::function<bool(std::shared_ptr<Node>)> currentCheckingFunction : checkingFunctions)
        {
            if (currentCheckingFunction(std::make_shared<Node>(TokenType::ID, name)))
            {
                return true;
            }
        }
        return false;
    }
};

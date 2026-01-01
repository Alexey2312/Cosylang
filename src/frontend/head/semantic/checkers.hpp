#pragma once

#include "../node.hpp"

class ConstructionChecker
{
    TokenType checkingKeyword;
public:
    void check(Node keyword);
    TokenType getCheckingKeyword() const { return checkingKeyword; }
};

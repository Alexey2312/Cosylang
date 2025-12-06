#pragma once

#include "../node.hpp"

class Checker
{
    TokenType checkingKeyword;
public:
    void check(Node node);
    TokenType getCheckingKeyword() const { return checkingKeyword; }
};

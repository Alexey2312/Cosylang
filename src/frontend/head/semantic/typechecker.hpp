#pragma once

#include "../node.hpp"

class TypeChecker
{
    TokenType keyword;
public:
    TypeChecker();
    void startTypeChecking(Node node);
    TokenType getKeyword() { return keyword; };
};

class TypeChecking
{
public:
    void startTypeChecking(Node node); // see implementation in typechecker.cpp
};

#pragma once

#include <string>
#include <vector>
#include "algorithms/sillySearch.hpp"
#include "token.hpp"


enum OperationType
{
    BINARY,
    UNARY,
    NOT_AN_OPERATION
};

class Node;
class Node
{
    TokenType type;
    std::string value;
    int priority;

    OperationType operation;

    Node* parent;
    std::vector<Node*> children;

public:
    Node(TokenType type, std::string value, int priority, std::vector<Node*> children, Node* parent, OperationType operation) : type(type), value(value), priority(priority), children(children), parent(parent), operation(operation) {}

    TokenType getType() const {return type;}

    std::string getValue() const {return value;}

    Node* getParent() const {return parent;}
    void setParent(Node* parent) {parent = parent;}

    std::vector<Node*> getChildren() const {return children;}
    void addChild(Node* child) {children.push_back(child);}

    int getPriority() const {return priority;}

    void removeChild(Node* child)
    {
    };
};

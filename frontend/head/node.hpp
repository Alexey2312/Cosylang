#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "token.hpp"


enum OperationType
{
    BINARY,
    UNARY,
    KEYWORD,
    NOT_AN_OPERATION
};

class Node;
class Node
{
    TokenType type;
    std::string value;
    int priority;
    std::vector<Node> children;
    Node* parent = nullptr;
    OperationType operation;

public:
    Node(TokenType type, std::string value, int priority, std::vector<Node> children, Node* parent, OperationType operation) : type(type), value(value), priority(priority), children(children), parent(parent), operation(operation) {}

    TokenType getType() const {return type;}

    std::string getValue() const {return value;}

    Node* getParent() const {return parent;}
    void setParent(Node* newParent) {parent = newParent;}

    std::vector<Node> getChildren() const {return children;}
    void addChild(Node child) {children.push_back(child);}

    int getPriority() const {return priority;}
    void setPriority(int newPriority) {priority = newPriority;}

    OperationType getOperationType() const {return operation;}
    void setOperationType(OperationType newOperationType) {operation = newOperationType;}

    void printChildren() const
    {
        if (children.empty())
        {
            return;
        }
        for (const auto& child : children)
        {
            std::cout << value + " children: " + child.getValue() << std::endl;
            child.printChildren();
        }
    }
};

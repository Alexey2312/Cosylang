#pragma once

#include <stdexcept>
#include <string>
#include <vector>
#include <iostream>
#include <optional>
#include <memory>
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
    std::vector<std::shared_ptr<Node>> children;
    std::optional<std::weak_ptr<Node>> parent = std::nullopt;
    OperationType operation;

public:
    Node(TokenType type, std::string value, int priority, std::vector<std::shared_ptr<Node>> children, std::optional<std::weak_ptr<Node>> parent = std::nullopt, OperationType operation = NOT_AN_OPERATION) : type(type), value(value), priority(priority), children(children), parent(parent), operation(operation) {}

    Node(const Node& otherNode) : type(otherNode.type), value(otherNode.value), priority(otherNode.priority), children(), parent(otherNode.parent), operation(otherNode.operation)
    {
        if (!otherNode.children.empty())
        {
            for (const auto& child_ptr : otherNode.children)
            {
                if (child_ptr)
                {
                    this->children.push_back(std::make_shared<Node>(*child_ptr));
                }
                else
                {
                    throw std::runtime_error("In constructer of Node: " + value + " children is nullptr!");
                }
            }
        }
        else
        {
            std::cout << "Node: " << value << ", children is empty!" << std::endl;
        }
    }

    TokenType getType() const {return type;}

    std::string getValue() const {return value;}

    std::optional<std::weak_ptr<Node>> getParent() const
    {
        if (!parent.has_value())
        {
            throw std::runtime_error("Node: " + value + ", parent node hasn't value");
        }
        if (!parent.value().expired())
        {
            throw std::runtime_error("Node: " + value +", parent node is nullpointer");
        }
        return parent;
    }
    void setParent(std::weak_ptr<Node> newParent) {parent = newParent;}

    std::vector<std::shared_ptr<Node>> getChildren() const {return children;}
    void addChild(std::shared_ptr<Node> child) {children.push_back(child);}

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
            std::cout << value + " children: " + child->getValue() << std::endl;
            child->printChildren();
        }
    }
};

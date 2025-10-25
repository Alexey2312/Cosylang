#pragma once

#include <string>
#include <vector>
#include "token.hpp"

class Node;
class Node
{
    TokenType type;
    std::string value;
    std::vector<Node*> children;

    TokenType getType() const {return type;}

    std::string getValue() const {return value;}

    std::vector<Node*> getChildren() const {return children;}
    void addChild(Node* child) {children.push_back(child);}

    Node(TokenType type, std::string value, std::vector<Node*> children) : type(type), value(value), children(children) {}

};

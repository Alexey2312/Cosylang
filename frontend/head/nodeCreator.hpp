#pragma once

#include "token.hpp"
#include "node.hpp"

Node createNode(TokenType type, std::string value = "", int priority = 0, OperationType operationType = OperationType::NOT_AN_OPERATION);

#pragma once

#include "token.hpp"
#include "node.hpp"

Node createNode(TokenType type, std::string value, int priority, OperationType operationType);

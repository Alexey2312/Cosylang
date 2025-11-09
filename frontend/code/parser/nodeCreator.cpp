#include "../../head/node.hpp"
#include "../../head/nodeCreator.hpp"
#include "../../head/token.hpp"

Node createNode(TokenType type, std::string value, int priority, OperationType operationType)
{
    return Node(type, value, priority, {nullptr}, nullptr, operationType);
}

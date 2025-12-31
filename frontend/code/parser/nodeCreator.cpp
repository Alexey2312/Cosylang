#include <memory>
#include "../../head/node.hpp"
#include "../../head/nodeCreator.hpp"
#include "../../head/token.hpp"

std::shared_ptr<Node> createNode(TokenType type, std::string value, int priority, OperationType operation)
{
    return std::make_shared<Node>( Node(type, value, priority, {}, std::weak_ptr<Node>(), operation) );
}

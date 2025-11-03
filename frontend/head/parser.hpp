#pragma once

#include <vector>
#include <string>
#include "node.hpp"
#include "token.hpp"

class Parser
{
    public:
        void parseError(const std::string& message);
        std::vector<Node*> parse(std::vector<token> tokens);
};

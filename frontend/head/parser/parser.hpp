#pragma once

#include <vector>
#include <string>
#include "../node.hpp"
#include "../token.hpp"
#include "../reporter.hpp"

class Parser
{
public:
    void parseError(const std::string& message);
    std::shared_ptr<Node> parse(std::vector<token> tokens, Reporter& reporter);
};

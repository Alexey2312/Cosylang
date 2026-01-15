#pragma once

#include "../node.hpp"

class ScopeRange
{
    std::vector<Node>::const_iterator beginOfScope;
    std::vector<Node>::const_iterator endOfScope;
public:
    ScopeRange(std::vector<Node>::const_iterator newBegin, std::vector<Node>::const_iterator newEnd) : beginOfScope(newBegin), endOfScope(newEnd) {}

    std::vector<Node>::const_iterator getBegin()
    {
        return beginOfScope;
    }
    void setBegin(std::vector<Node>::const_iterator newBegin)
    {
        beginOfScope = newBegin;
    }

    std::vector<Node>::const_iterator getEnd()
    {
        return endOfScope;
    }
    void setEnd(std::vector<Node>::const_iterator newEnd)
    {
        endOfScope = newEnd;
    }
};

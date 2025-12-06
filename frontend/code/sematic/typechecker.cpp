#include "../../head/semantic/typechecker.hpp"
#include "../../head/semantic/checkers.hpp"

class CheckersRegistry
{
    static std::vector<Checker> checkers;
public:
    static void addChecker(Checker newChecker)
    {
        checkers.push_back(newChecker);
    }

    static std::vector<Checker> getCheckers()
    {
        return checkers;
    }
};

void TypeChecker::startCheck(Node node)
{
    std::vector<Checker> validCheckers = CheckersRegistry::getCheckers();
    bool found = false;
    for (auto& checker : validCheckers)
    {
        if(checker.getCheckingKeyword() == node.getType())
        {
            checker.check(node);
            found = true;
        }
    }
    if (!found)
    {
        throw std::runtime_error("No checker found for node type");
    }
}

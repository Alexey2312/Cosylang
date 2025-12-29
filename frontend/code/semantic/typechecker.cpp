#include "../../head/semantic/typechecker.hpp"
#include <stdexcept>

class CheckersRegistry
{
    static std::vector<TypeChecker> checkers;
public:
    static void addChecker(TypeChecker newChecker)
    {
        checkers.push_back(newChecker);
    }

    static std::vector<TypeChecker> getCheckers()
    {
        return checkers;
    }
};

inline void TypeChecking::startTypeChecking(Node node)
{
    std::vector<TypeChecker> validCheckers = CheckersRegistry::getCheckers();
    bool found = false;
    for (auto& checker : validCheckers)
    {
        if(checker.getKeyword() == node.getType())
        {
            checker.startTypeChecking(node);
            found = true;
        }
    }
    if (found == false)
    {
        throw std::runtime_error("No checker found for node type");
    }
}

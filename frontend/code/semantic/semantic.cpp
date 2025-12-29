// last change on 19.12.2025

#include "../../head/semantic/semantic.hpp"
#include <map>
#include "../../head/semantic/symbolTable.hpp"
#include "../../head/semantic/declarations.hpp"

SymbolTable symbolTable = SymbolTable();

class OperationTypeBase
{
    std::map<TokenType, std::vector<TokenType>> operationTypes;

public:
    void registerOperationType(TokenType parentType, std::vector<TokenType> childTypes)
    {
        operationTypes.insert(std::make_pair(parentType, childTypes));
    }
    bool isValid(TokenType operationType, TokenType childType)
    {
        try
        {
            return operationTypes.at(operationType).at(childType);
        }
        catch (...)
        {
            return false;
        }
    }
};

class DeclarationsFindersManager
{
    static std::vector<Declaration> declarations;

public:
    void registerDeclaration(Declaration declarationNode)
    {
        declarations.push_back(declarationNode);
    }
    static std::vector<Declaration> getDeclarations()
    {
        return declarations;
    }
};

void findDeclarations(std::vector<Node> lines)
{

}

void SemanticAnalysis::analyze(Node mainNode)
{

}

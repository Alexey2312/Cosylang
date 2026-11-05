#include "declarations-collector.hpp"
#include "module.hpp"
#include "symbol.hpp"
#include "type.hpp"

namespace Cosylang::Semantic
{

void DeclarationsCollector::collectVariable(Parser::Node* variable_node)
{
    if (variable_node->type != Parser::NodeType::VAR)
    {
        return;
    }

    std::string variable_name = "";
    std::string type_name = "";

    if (variable_node->first_child->type == Parser::NodeType::ASSIGN)
    {
        if (variable_node->first_child->first_child->type == Parser::NodeType::COLON)
        {
            variable_name = variable_node->first_child->first_child->first_child->token->name;
            type_name = variable_node->first_child->first_child->first_child->next_sibling->token->name;
        }
    }
    else if (variable_node->first_child->type == Parser::NodeType::COLON)
    {
        type_name = variable_node->first_child->first_child->next_sibling->token->name;
        variable_name = variable_node->first_child->first_child->token->name;
    }
    Symbol* symbol = arena.alloc<Symbol>();
    symbol->name = variable_name;
    symbol->kind = VariableKind{SymbolKind::VARIABLE, Type(type_name, TypeKind::VOID)};
    current_module->scope->loadDeclaration(variable_name, symbol);
}

void DeclarationsCollector::collectDeclarations(const Parser::Node* program_node)
{
    Parser::Node* current_node = program_node->first_child;
    current_module = arena.alloc<Module>("program", current_scope);
    while (current_node)
    {
        switch (current_node->type)
        {
            case Parser::NodeType::VAR:
            {
                collectVariable(current_node);
                current_node = current_node->next_sibling ? current_node->next_sibling : current_node->first_child;
                break;
            }
            default:
            {
                current_node = current_node->next_sibling ? current_node->next_sibling : current_node->first_child;
            }
        }
    }
}

} //namespace Cosylang::Semantic

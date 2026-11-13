#include "declarations-collector.hpp"
#include "module.hpp"
#include "symbol.hpp"
#include "type.hpp"
#include <string>

namespace Cosylang::Semantic
{

void DeclarationsCollector::collectVariable(Parser::Node* variable_node, Scope* parent_scope)
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
    symbol->kind = VariableKind{SymbolKind::VARIABLE, Type(type_name)};
    parent_scope->loadDeclaration(variable_name, symbol);
}

void DeclarationsCollector::collectParam(Parser::Node* param_node, Scope* parent_scope)
{
    if (param_node == nullptr || parent_scope == nullptr)
    {
        return;
    }

    Parser::Node* declaration_node = nullptr;

    Parser::Node* colon_node = nullptr;
    Parser::Node* current_node = param_node;

    while (current_node)
    {
        if (current_node == nullptr)
        {
            return;
        }

        if (current_node->type == Parser::NodeType::COLON)
        {
            colon_node = current_node;
            break;
        }

        current_node = current_node->first_child;
        continue;
    }

    declaration_node = colon_node;

    Parser::Node* name_node = declaration_node->first_child;
    Parser::Node* type_node = name_node->next_sibling;

    std::string param_name = std::string(name_node->token->name);
    std::string type_name = std::string(type_node->token->name);

    Symbol* symbol = arena.alloc<Symbol>();

    symbol->name = param_name;
    symbol->kind = ParameterKind{SymbolKind::PARAM, Type(type_name)};

    parent_scope->loadDeclaration(param_name, symbol);
}

void DeclarationsCollector::collectFunction(Parser::Node* function_node, Scope* parent_scope)
{
    if (function_node == nullptr || function_node->type != Parser::NodeType::FUNC || parent_scope == nullptr)
    {
        return;
    }

    Parser::Node* declaration = function_node->first_child;

    if (declaration == nullptr)
    {
        std::cerr << "Invalid function AST \n";
        return;
    }

    std::string function_name;
    std::string returning_type_name = "void";

    Parser::Node* params_node = nullptr;
    Parser::Node* body_node = nullptr;

    if (declaration->type == Parser::NodeType::COLON)
    {
        Parser::Node* name_node = declaration->first_child;

        if (name_node == nullptr || name_node->token == nullptr)
        {
            std::cerr << "Invalid function AST \n";
            return;
        }

        function_name = name_node->token->name;
        params_node = name_node->next_sibling;

        if (params_node == nullptr)
        {
            std::cerr << "Invalid function AST \n";
            return;
        }

        body_node = params_node->next_sibling;
    }
    else if (declaration->type == Parser::NodeType::ID)
    {
        function_name = declaration->token->name;
        returning_type_name = "void";

        params_node = declaration->next_sibling;

        if (params_node == nullptr)
        {
            std::cerr << "Invalid function AST \n";
            return;
        }

        body_node = params_node->next_sibling;
    }
    else
    {
        std::cerr << "Invalid function AST \n";
        return;
    }

    if (body_node == nullptr || body_node->type != Parser::NodeType::CODE_BLOCK)
    {
        std::cerr << "Invalid function AST \n";
        return;
    }

    Scope* function_scope = visitScope(body_node);

    if (function_scope == nullptr)
    {
        std::cerr << "Failed to create a function scope\n";
        return;
    }

    if (params_node->first_child != nullptr)
    {
        Parser::Node* current_param = params_node->first_child;

        while (current_param != nullptr)
        {
            collectParam(current_param, function_scope);
            current_param = current_param->next_sibling;
        }
    }

    Symbol* symbol = arena.alloc<Symbol>();
    symbol->name = function_name;

    FunctionKind function_kind = {SymbolKind::FUNCTION, function_scope};

    function_kind.returning_type = Type(returning_type_name, TypeKind::VOID);

    symbol->kind = std::move(function_kind);
    parent_scope->loadDeclaration(function_name, symbol);
}

void DeclarationsCollector::collectStructure(Parser::Node* struct_node, Scope* parent_scope)
{
    if (struct_node == nullptr || struct_node->type != Parser::NodeType::STRUCT || parent_scope == nullptr)
    {
        return;
    }

    if (struct_node->first_child->type != Parser::NodeType::ID)
    {
        std::cerr << "Invalid structure AST \n";
    }

    std::string struct_name = std::string(struct_node->first_child->token->name);

    if (struct_node->first_child->next_sibling->type != Parser::NodeType::CODE_BLOCK)
    {
        std::cerr << "Invalid structure AST \n";
    }

    Scope* struct_scope = visitScope(struct_node->first_child->next_sibling);

    Symbol* symbol = arena.alloc<Symbol>();
    symbol->name = struct_name;

    StructKind struct_kind = {SymbolKind::STRUCT, struct_scope};

    symbol->kind = std::move(struct_kind);
    parent_scope->loadDeclaration(struct_name, symbol);
}

Scope* DeclarationsCollector::visitScope(Parser::Node* scope_node)
{
    if (scope_node == nullptr)
    {
        return nullptr;
    }

    Scope* scope = arena.alloc<Scope>();

    Parser::Node* current_node = scope_node->type == Parser::NodeType::CODE_BLOCK ? scope_node->first_child : scope_node;

    while (current_node != nullptr)
    {
        switch (current_node->type)
        {
            case Parser::NodeType::VAR:
            {
                collectVariable(current_node, scope);
                break;
            }

            case Parser::NodeType::FUNC:
            {
                collectFunction(current_node, scope);
                break;
            }

            default:
            {
                break;
            }
        }

        current_node = current_node->next_sibling;
    }

    return scope;
}

void DeclarationsCollector::collectDeclarations(const Parser::Node* program_node)
{
    current_module = arena.alloc<Module>("program", nullptr);
    current_module->scope = visitScope(program_node->first_child);
    manager.loadModule(*current_module);
}

} //namespace Cosylang::Semantic

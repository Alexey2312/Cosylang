#pragma once

#include "scope.hpp"
#include "../../utills/arena-allocator.hpp"
#include "module-manager.hpp"
#include "symbol.hpp"

namespace Cosylang::Semantic
{


struct DeclarationsCollector
{
    Arena& arena;
    ModuleManager& manager;
    Scope* current_scope = nullptr;
    Module* current_module = nullptr;

    void collectVariable(Parser::Node* variable_node);
    void collectDeclarations(const Parser::Node* program_node); // fill modules in manager

    DeclarationsCollector(Arena& this_arena, ModuleManager& this_modules)
        : arena(this_arena), manager(this_modules) {}
};

} //namespace Cosylang::Semantic

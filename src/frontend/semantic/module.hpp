#pragma once

#include "scope.hpp"
#include <vector>

namespace Cosylang::Semantic
{

enum class ModuleState
{
    UNLOADED,
    LOADING,
    LOADED,
    ANALYZED,
    FAILED
};

struct Module
{
    std::string name;
    std::string path = "";

    Scope* scope = nullptr;
    Scope* export_scope = nullptr;

    ModuleState state = ModuleState::UNLOADED;

    Parser::Node* node = nullptr;

    std::vector<std::string> imports_names = {};
    std::vector<Module*> imports = {};

    void addImport(std::string import_name)
    {
        imports_names.push_back(import_name);
    }

    Module(std::string this_name, Semantic::Scope* this_scope, Parser::Node* this_node = nullptr, std::vector<std::string> this_imports_names = {}, Scope* this_export_scope = nullptr)
        : name(this_name), scope(this_scope), node(this_node), imports_names(this_imports_names), export_scope(this_export_scope)  {}
};

}; //namespace Cosylang::Semantic

#pragma once

#include "module-manager.hpp"
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

    Module* parent = nullptr;

    ModuleState state = ModuleState::UNLOADED;

    Parser::Node* node = nullptr;

    std::vector<std::string> imports_names = {};
    std::vector<Module*> imports = {};

    std::vector<Module*> submodules = {};

    void addImport(std::string import_name)
    {
        imports_names.push_back(import_name);
    }

    void addSubmodule(Module* sub)
    {
        submodules.push_back(sub);
    }

    Module(std::string this_name, Semantic::Scope* this_scope, Parser::Node* this_node = nullptr, std::vector<std::string> this_imports_names = {}, Scope* this_export_scope = nullptr, Module* this_parent = nullptr)
        : name(this_name), scope(this_scope), node(this_node), imports_names(this_imports_names), export_scope(this_export_scope), parent(this_parent)  {}
};

}; //namespace Cosylang::Semantic

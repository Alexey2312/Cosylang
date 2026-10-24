#pragma once

#include <unordered_map>
#include "module.hpp"

namespace Cosylang::Semantic
{

class ModuleManager
{
    std::unordered_map<std::string, Module> modules;

    void resolveImport(Module& module); // call after collecting declarations

public:
    Module* findModule(const std::string& name);
    void loadModule(Module module);
    void loadModule(const std::string& name, Scope* scope);
};

} //namespace Cosylang::Semantic

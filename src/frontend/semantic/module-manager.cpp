#include "module-manager.hpp"
#include "module.hpp"
#include <iostream>

namespace Cosylang::Semantic
{

void ModuleManager::loadModule(Module module)
{
    if (modules.find(module.name) == modules.end())
    {
        modules.insert({module.name, module});
    }
}

void ModuleManager::loadModule(const std::string& name, Scope* scope)
{
    if (modules.find(name) == modules.end())
    {
        modules.insert({name, Module(name, scope)});
    }
}

void ModuleManager::resolveImport(Module& module)
{
    for (const auto& current_name : module.imports_names)
    {
        if (modules.find(current_name) != modules.end())
        {
            if (modules.at(current_name).state == ModuleState::LOADED)
            {
                continue;
            }
            if (modules.at(current_name).state != ModuleState::LOADING)
            {
                modules.at(current_name).state = ModuleState::LOADING;
            }
            else
            {
                std::cerr << "Cyclic dependence between '" + module.name + "' and '" + current_name + "' modules \n";
                std::exit(0);
            }

            resolveImport(modules.at(current_name));

            module.imports.push_back(&modules.at(current_name));
            modules.at(current_name).state = ModuleState::LOADED;
        }
        else
        {
            std::cerr << "Unknown module '" + current_name + "'\n";
        }
    }
}

Module* ModuleManager::findModule(const std::string& name)
{
    if (modules.find(name) != modules.end())
    {
        if (modules.at(name).imports.empty())
        {
            resolveImport(modules.at(name));
        }
        return &modules.at(name);
    }
    return nullptr;
}

} //namespace Cosylang::Semantic

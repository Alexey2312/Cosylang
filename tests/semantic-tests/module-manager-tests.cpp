#include "../../external/silteli.hpp"
#include "../../src/frontend/semantic/module-manager.hpp"
#include "../../src/frontend/semantic/module.hpp"
#include "../../src/frontend/semantic/scope.hpp"

#include <sstream>
#include <string>
#include <iostream>

using namespace Cosylang::Semantic;


UNIT_TEST(ModuleManagerLoadModuleTest)
{
    ModuleManager manager;
    Scope scope{};

    Module module("math", &scope);
    manager.loadModule(std::move(module));

    Module* found = manager.findModule("math");

    bool is_valid = found != nullptr && found->name == "math" && found->scope == &scope;
    Silteli::expect(is_valid);
}

UNIT_TEST(ModuleManagerLoadModuleByNameTest)
{
    ModuleManager manager;
    Scope scope{};

    manager.loadModule("math", &scope);

    Module* found = manager.findModule("math");

    bool is_valid = found != nullptr && found->name == "math" && found->scope == &scope;
    Silteli::expect(is_valid);
}

UNIT_TEST(ModuleManagerResolveImportTest)
{
    ModuleManager manager;
    Scope main_scope{};
    Scope library_scope{};

    Module library("library", &library_scope);
    Module main("main", &main_scope);
    main.addImport("library");

    manager.loadModule(std::move(library));
    manager.loadModule(std::move(main));

    Module* found = manager.findModule("main");

    bool is_valid = found != nullptr &&
                    found->imports.size() == 1 &&
                    found->imports.front() != nullptr &&
                    found->imports.front()->name == "library" &&
                    found->imports.front()->state == ModuleState::LOADED;

    Silteli::expect(is_valid);
}

UNIT_TEST(ModuleManagerUnknownImportTest)
{
    ModuleManager manager;
    Scope main_scope{};

    Module main("main", &main_scope);
    main.addImport("missing");

    manager.loadModule(std::move(main));

    std::ostringstream errors;
    auto* old_buffer = std::cerr.rdbuf(errors.rdbuf());

    Module* found = manager.findModule("main");

    std::cerr.rdbuf(old_buffer);

    bool is_valid = found != nullptr &&
                    found->imports.empty() &&
                    errors.str().find("Unknown module 'missing'") != std::string::npos;

    Silteli::expect(is_valid);
}

#include "../../external/silteli.hpp"
#include "../../src/frontend/semantic/symbol.hpp"
#include "../../src/frontend/semantic/scope.hpp"
#include <iostream>
#include <string>

UNIT_TEST(ScopeInsertVariableTest)
{
    Cosylang::Semantic::Scope scope(nullptr);

    Cosylang::Semantic::Symbol var_sym;
    var_sym.name = "x";
    var_sym.kind = Cosylang::Semantic::Kind{Cosylang::Semantic::SymbolKind::VARIABLE};
    var_sym.visibility = Cosylang::Semantic::Visibility::PRIVATE;

    scope.loadDeclaration("x", &var_sym);

    bool is_valid = scope.table.count("x") == 1 &&
                    scope.table["x"]->name == "x" &&
                    std::holds_alternative<Cosylang::Semantic::Kind>(scope.table["x"]->kind);

    Silteli::expect(is_valid);
}

UNIT_TEST(ScopeVariableRedefinitionTest)
{
    Cosylang::Semantic::Scope scope(nullptr);

    Cosylang::Semantic::Symbol var1;
    var1.name = "count";
    var1.kind = Cosylang::Semantic::Kind{Cosylang::Semantic::SymbolKind::VARIABLE};

    Cosylang::Semantic::Symbol var2;
    var2.name = "count";
    var2.kind = Cosylang::Semantic::Kind{Cosylang::Semantic::SymbolKind::VARIABLE};

    auto old_buffer = std::cerr.rdbuf(std::cerr.rdbuf());

    scope.loadDeclaration("count", &var1);
    scope.loadDeclaration("count", &var2);

    std::cerr.rdbuf(old_buffer);

    bool is_valid = scope.table.count("count") == 1 &&
                    scope.table["count"] == &var1;

    Silteli::expect(is_valid);
}

UNIT_TEST(ScopeParentHierarchyTest)
{
    Cosylang::Semantic::Scope global_scope(nullptr);
    Cosylang::Semantic::Scope local_scope(&global_scope);

    Cosylang::Semantic::Symbol global_sym;
    global_sym.name = "global_var";
    global_sym.kind = Cosylang::Semantic::Kind{Cosylang::Semantic::SymbolKind::VARIABLE};

    global_scope.loadDeclaration("global_var", &global_sym);

    bool is_valid = local_scope.parent != nullptr &&
                    local_scope.parent->table.count("global_var") == 1 &&
                    local_scope.parent->table["global_var"] == &global_sym;

    Silteli::expect(is_valid);
}

UNIT_TEST(ScopeLookupTest)
{
    Cosylang::Semantic::Scope scope1(nullptr);
    Cosylang::Semantic::Scope scope2(&scope1);

    Cosylang::Semantic::Symbol symbol;
    symbol.name = "value";
    symbol.kind = Cosylang::Semantic::Kind{Cosylang::Semantic::SymbolKind::VARIABLE};

    scope1.loadDeclaration("value", &symbol);

    Silteli::expect(scope2.lookup("value") == &symbol);
}

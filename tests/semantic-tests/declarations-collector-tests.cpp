#include "../../external/silteli.hpp"
#include "../../src/frontend/semantic/module-manager.hpp"
#include "../../src/frontend/semantic/module.hpp"
#include "../../src/frontend/semantic/scope.hpp"

#include <string>
#include <iostream>
#include "../../src/frontend/lexer/lexer.hpp"
#include "../../src/frontend/parser/parser.hpp"
#include "../../src/diagnostics/source-manager.hpp"
#include "../../src/diagnostics/error-reporter.hpp"
#include "../../src/frontend/semantic/declarations-collector.hpp"


UNIT_TEST(DeclarationsCollectorCollectVariableTest)
{
    const std::string source_code = "var x: int32";

    std::vector<std::unique_ptr<Cosylang::Source::FileBuffer>> files;
    files.push_back(std::make_unique<Cosylang::Source::FileBuffer>("test.cosy", source_code, 0));

    Cosylang::Source::SourceManager source(std::move(files));

    Cosylang::Reporter::DiagnosticConfig config{
        true,
        false,
        true,
        std::cerr
    };
    Cosylang::Reporter::DiagnosticEngine diagnostics(config, source);

    Cosylang::Lexer::Lexer lexer(source_code.c_str());
    auto tokens = lexer.tokenize();

    Arena parser_arena(256);
    Cosylang::Parser::Parser parser(tokens, parser_arena, 0, diagnostics);
    const Cosylang::Parser::Node* program = parser.parse();

    Arena semantic_arena(1024);
    Cosylang::Semantic::Scope scope{};
    Cosylang::Semantic::ModuleManager manager;

    Cosylang::Semantic::DeclarationsCollector collector(semantic_arena, manager);

    collector.collectDeclarations(program);

    bool is_valid =
        manager.findModule("program") &&
        collector.current_module->scope != nullptr &&
        collector.current_module->scope->table.find("x") != collector.current_module->scope->table.end();

    Silteli::expect(is_valid);
}

UNIT_TEST(DeclarationsCollectorFunctionWithParamsTest)
{
    const std::string source_code = "func add(a: int32, b: int32) : void \n { \n  var result: int32 \n}";

    std::vector<std::unique_ptr<Cosylang::Source::FileBuffer>> files;
    files.push_back(std::make_unique<Cosylang::Source::FileBuffer>("test.cosy", source_code, 0));

    Cosylang::Source::SourceManager source(std::move(files));

    Cosylang::Reporter::DiagnosticConfig config{
        true,
        false,
        true,
        std::cerr
    };

    Cosylang::Reporter::DiagnosticEngine diagnostics(config, source);

    Cosylang::Lexer::Lexer lexer(source_code.c_str());
    auto tokens = lexer.tokenize();

    Arena parser_arena(1024);

    Cosylang::Parser::Parser parser(
        tokens,
        parser_arena,
        0,
        diagnostics
    );

    const Cosylang::Parser::Node* program = parser.parse();

    Arena semantic_arena(2048);
    Cosylang::Semantic::ModuleManager manager;

    Cosylang::Semantic::DeclarationsCollector collector(semantic_arena, manager);

    if (program == nullptr)
    {
        std::cerr << "Program node is nullptr \n";
        Silteli::expect(false);
    }

    collector.collectDeclarations(program);

    bool is_valid =
        program != nullptr &&
        collector.current_module != nullptr &&
        collector.current_module->scope != nullptr;

    if (is_valid)
    {
        auto function_it =
            collector.current_module->scope->table.find("add");

        is_valid =
            function_it != collector.current_module->scope->table.end() &&
            function_it->second != nullptr &&
            std::holds_alternative<Cosylang::Semantic::FunctionKind>(function_it->second->kind);
    }

    if (is_valid)
    {
        const auto& function_kind = std::get<Cosylang::Semantic::FunctionKind>(collector.current_module->scope->table.at("add")->kind);

        auto* function_scope = function_kind.scope;

        is_valid = function_scope != nullptr && function_scope->table.count("a") == 1;
    }

    Silteli::expect(is_valid);
}

UNIT_TEST(DeclarationsCollectorStructureTest)
{
    const std::string code = "struct Point { var x: int32 }";

    std::vector<std::unique_ptr<Cosylang::Source::FileBuffer>> files;
    files.push_back(std::make_unique<Cosylang::Source::FileBuffer>(
        "test.cosy",
        code,
        0
    ));

    Cosylang::Source::SourceManager source(std::move(files));

    Cosylang::Reporter::DiagnosticConfig config{
        true,
        false,
        true,
        std::cerr
    };

    Cosylang::Reporter::DiagnosticEngine diagnostics(config, source);

    Cosylang::Lexer::Lexer lexer(code.c_str());
    auto tokens = lexer.tokenize();

    Arena parser_arena(1024);
    Cosylang::Parser::Parser parser(
        tokens,
        parser_arena,
        0,
        diagnostics
    );

    Cosylang::Parser::Node* structure_node = parser.parseStatement();

    Arena semantic_arena(2048);
    Cosylang::Semantic::ModuleManager manager;

    Cosylang::Semantic::DeclarationsCollector collector(semantic_arena, manager);

    Cosylang::Semantic::Scope parent_scope{};

    if (structure_node != nullptr)
    {
        collector.collectStructure(structure_node, &parent_scope);
    }

    bool is_valid =
        structure_node != nullptr &&
        structure_node->type == Cosylang::Parser::NodeType::STRUCT &&
        parent_scope.table.count("Point") == 1;

    if (is_valid)
    {
        Cosylang::Semantic::Symbol* symbol = parent_scope.table.at("Point");

        is_valid =
            symbol != nullptr &&
            symbol->name == "Point" &&
            std::holds_alternative<Cosylang::Semantic::StructKind>(symbol->kind);

        if (is_valid)
        {
            const auto& struct_kind = std::get<Cosylang::Semantic::StructKind>(symbol->kind);

            is_valid =
                struct_kind.scope != nullptr &&
                struct_kind.scope->table.count("x") == 1;
        }
    }

    Silteli::expect(is_valid);
}

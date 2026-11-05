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
    collector.current_scope = &scope;

    collector.collectDeclarations(program);

    bool is_valid =
        manager.findModule("program") &&
        collector.current_module->scope != nullptr &&
        collector.current_module->scope->table.find("x") != collector.current_module->scope->table.end();

    Silteli::expect(is_valid);
}

#include <memory>
#include <sstream>
#include <vector>
#include "../../src/frontend/lexer/lexer.hpp"
#include "../../src/diagnostics/error-reporter.hpp"
#include "../../src/diagnostics/source-manager.hpp"
#include "../../external/silteli.hpp"

UNIT_TEST(ErrorReporter)
{
    std::vector<std::unique_ptr<Cosylang::Source::FileBuffer>> files;
    std::string input = "\treturn 2+2";

    files.push_back(std::make_unique<Cosylang::Source::FileBuffer>("a.cosy", input, 0));

    Cosylang::Source::SourceManager source(std::move(files));

    Cosylang::Lexer::Lexer lex(input.c_str());
    auto tokens = lex.tokenize();

    std::stringstream out;

    Cosylang::Reporter::DiagnosticConfig config
    {
        true,
        false,
        false,
        out,
    };

    Cosylang::Reporter::DiagnosticEngine eng(config, source);
    eng.reportParseError(tokens.at(0), "aaa", 0);

    Silteli::expect(!out.str().empty());
}

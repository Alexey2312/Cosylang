#pragma once

#include <cstddef>
#include <iostream>
#include <ostream>

#include "source-manager.hpp"
#include "../frontend/lexer/token/token.hpp"
#include "../frontend/parser/node/node.hpp"

namespace Cosylang::Reporter
{

struct DiagnosticConfig
{
    const bool enable_colors = true;
    const bool debug = false;
    const bool stop_after_error = true;
    std::ostream& stream = std::cerr;

    const char space_char = ' ';
    const char arrow_char = '^';
    const char operands_arrow = '~';
    const char first_line_border = '|';
    const char second_line_border = '|';
    const int tab_size = 4;
};

class DiagnosticEngine
{
    DiagnosticConfig& config;
    Source::SourceManager& source_manager;

    std::string_view getCodeLine(size_t file_id, size_t token_offset, size_t line);
    std::string createCaretLine(Lexer::Token::Token& token, std::string_view source);
    std::string createCaretLine(const Lexer::Token::Token* token, std::string_view source);
    std::string formatLineWithTabs(std::string_view source);
public:

    DiagnosticEngine(DiagnosticConfig& this_config, Source::SourceManager& this_source_manager) :
        config(this_config), source_manager(this_source_manager) {}

    void reportParseError(Lexer::Token::Token& token, std::string_view error_text, size_t file_id);
    void reportParseError(Lexer::Token::Token* token, std::string_view error_text, size_t file_id);
    void reportParseError(const Lexer::Token::Token* token, std::string_view error_text, size_t file_id);
    void reportSemanticError(Parser::Node& node, std::string_view error_text, size_t file_id);
};

}; // namespace Cosylang::Reporter

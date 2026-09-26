#include "error-reporter.hpp"
#include "source-manager.hpp"
#include <cstddef>
#include <string>
#include <string_view>

namespace Cosylang::Reporter
{

namespace
{
    static constexpr std::string_view RED = "\033[31m";
    static constexpr std::string_view GREEN = "\033[1;32m";
    static constexpr std::string_view BLUE = "\033[1;34m";
    static constexpr std::string_view RESET = "\033[0m";
    static constexpr std::string_view YELLOW = "\033[1;33m";
}

std::string_view DiagnosticEngine::getCodeLine(size_t file_id, size_t token_offset, size_t line)
{
    Source::FileBuffer* file = source_manager.getFileBuffer(file_id);
    if (file == nullptr)
    {
        config.stream << "Compiler crush! DiagnosticEngine::getCodeLine: null file buffer!\n";
        std::exit(1);
    }

    const size_t line_count = file->line_offsets.size();

    if (line_count == 0)
    {
        file->buildFileLineOffsets();
    }


    if (line >= file->line_offsets.size())
    {
        config.stream << "Compiler crush! DiagnosticEngine::getCodeLine: invalid line offset!\n";
        std::exit(1);
    }

    const size_t start = file->line_offsets.at(line);

    const size_t end = (line + 1 < file->line_offsets.size())
        ? file->line_offsets.at(line + 1)
        : file->source_code.size();

    if (start > file->source_code.size() || end > file->source_code.size() || start > end)
    {
        config.stream << "Compiler crush! DiagnosticEngine::getCodeLine: invalid range!\n";
        std::exit(1);
    }

    return std::string_view{file->source_code}.substr(start, end - start);
}


std::string DiagnosticEngine::createCaretLine(Lexer::Token::Token& token, std::string_view source)
{
    std::string caret_line;

    for (size_t i = 1; i < token.column; ++i)
    {
        if (source[i + 1] == '\t')
        {
            for (int tab_char = 0; tab_char < config.tab_size; tab_char++)
            {
                caret_line.push_back(config.space_char);
            }
            continue;
        }
        else
        {
            if (i + 1 != token.column)
            {
                caret_line.push_back(config.space_char);
            }
        }
    }
    for (int i = token.column; i < token.column + token.length; i++)
    {
        caret_line.push_back(config.arrow_char);
    }
    return caret_line;
}

std::string DiagnosticEngine::createCaretLine(const Lexer::Token::Token* token, std::string_view source)
{
    std::string caret_line;

    for (size_t i = 1; i < token->column + 1; ++i)
    {
        if (source[i + 1] == '\t')
        {
            for (int tab_char = 0; tab_char < config.tab_size; tab_char++)
            {
                caret_line.push_back(config.space_char);
            }
            continue;
        }
        else
        {
            if (i != token->column)
            {
                caret_line.push_back(config.space_char);
            }
        }
    }
    for (int i = token->column; i < token->column + token->length; i++)
    {
        caret_line.push_back(config.arrow_char);
    }
    return caret_line;
}

void DiagnosticEngine::reportParseError(Lexer::Token::Token& token, std::string_view error_text, size_t file_id)
{
    const Source::FileBuffer* file = source_manager.getFileBuffer(file_id);
    config.stream <<
        ((config.enable_colors && !config.debug) ? RED : "") <<
        file->file_name <<
        ":" <<
        token.line <<
        ":" <<
        token.column <<
        ": parse error: " <<
        std::string(error_text) <<
        ((config.enable_colors && !config.debug) ? RESET : "") <<
        "\n";

    if (config.debug)
    {
        if (config.stop_after_error)
        {
            std::exit(1);
        }
        return;
    }

    std::string_view source_line = getCodeLine(file_id, token.offset, token.line - 1);
    std::string caret_line = createCaretLine(token, source_line);

    std::string line_number = std::to_string(token.line);

    std::string tab = "";
    for (int i = 0; i < config.tab_size; ++i)
    {
        tab.append(" ");
    }

    std::string number_space = "";
    for (int i = 0; i < line_number.size(); ++i)
    {
        number_space.append(" ");
    }


    config.stream <<
        ((config.enable_colors && !config.debug) ? RED : "") <<
        line_number <<
        config.first_line_border <<
        tab <<
        source_line <<
        '\n' <<
        number_space <<
        config.second_line_border <<
        tab <<
        caret_line <<
        ((config.enable_colors && !config.debug) ? RESET : "") <<
        "\n";

    if (config.stop_after_error)
    {
        std::exit(1);
    }
    return;
}

void DiagnosticEngine::reportParseError(const Lexer::Token::Token* token, std::string_view error_text, size_t file_id)
{
    const Source::FileBuffer* file = source_manager.getFileBuffer(file_id);
    config.stream <<
        ((config.enable_colors && !config.debug) ? RED : "") <<
        file->file_name <<
        ":" <<
        token->line <<
        ":" <<
        token->column <<
        ": parse error: " <<
        std::string(error_text) <<
        ((config.enable_colors && !config.debug) ? RESET : "") <<
        "\n";

    if (config.debug)
    {
        if (config.stop_after_error)
        {
            std::exit(1);
        }
        return;
    }

    std::string_view source_line = getCodeLine(file_id, token->offset, token->line - 1);
    std::string caret_line = createCaretLine(token, source_line);

    std::string line_number = std::to_string(token->line);

    std::string tab = "";
    for (int i = 0; i < config.tab_size; ++i)
    {
        tab.append(" ");
    }

    std::string number_space = "";
    for (int i = 0; i < line_number.size(); ++i)
    {
        number_space.append(" ");
    }


    config.stream <<
        ((config.enable_colors && !config.debug) ? RED : "") <<
        line_number <<
        config.first_line_border <<
        tab <<
        source_line <<
        '\n' <<
        number_space <<
        config.second_line_border <<
        tab <<
        caret_line <<
        ((config.enable_colors && !config.debug) ? RESET : "") <<
        "\n";

    if (config.stop_after_error)
    {
        std::exit(1);
    }
    return;
}

}; // namespace Cosylang::Reporter

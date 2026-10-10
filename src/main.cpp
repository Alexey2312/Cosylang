#include "../external/silteli.hpp"
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <vector>
#include "frontend/lexer/lexer.hpp"
#include "frontend/parser/node/node.hpp"
#include "frontend/parser/parser.hpp"
#include "diagnostics/error-reporter.hpp"
#include "diagnostics/source-manager.hpp"
#include "utills/arena-allocator.hpp"
#include <iostream>
#include <string>
#include <string_view>
#include <thread>
#include <chrono>

namespace
{
    static const std::string RED = "\033[1;31m";
    static const std::string GREEN = "\033[1;32m";
    static const std::string BLUE = "\033[1;34m";
    static const std::string RESET = "\033[0m";
    static const std::string YELLOW = "\033[1;33m";
    static const std::string ORANGE = "\033[1;38;2;255;165;0m";;
}


void typeText(std::string_view text, int delay_ms = 10)
{
    size_t i = 0;
    while (i < text.size())
    {
        unsigned char byte = static_cast<unsigned char>(text[i]);
        size_t char_len = 1;

        if ((byte & 0x80) == 0x00)
        {
            char_len = 1;
        }
        else if ((byte & 0xE0) == 0xC0)
        {
            char_len = 2;
        }
        else if ((byte & 0xF0) == 0xE0)
        {
            char_len = 3;
        }
        else if ((byte & 0xF8) == 0xF0)
        {
            char_len = 4;
        }

        std::cout << text.substr(i, char_len) << std::flush;
        i += char_len;

        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
    }
}

void printHelp(int typing_speed)
{
    typeText(BLUE + "Cosylang Compiler v0.0.0\n"
              + RED + "Usage: cosy <source_files> [options]\n"
              + YELLOW + "Options:\n"
              + "  -o <output>            Specify output file\n"
              + "  -v, --version          Show version\n"
              + "  --typing-speed <value> Set typing speed in ms. 0 by default\n"
              + "  test                   Run compiler tests\n"
              + "  --help                 Show this help\n" + RESET, typing_speed);
}

int main(int argc, char* argv[])
{
    int typing_speed = 0;
    if (argc < 2)
    {
        printHelp(typing_speed);
        return 1;
    }

    std::vector<std::string> inputFilesPaths;
    std::string outputFile = "out.o";

    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "--help")
        {
            printHelp(typing_speed);
            return 0;
        }
        else if (arg == "-v" || arg == "--version")
        {
            typeText(YELLOW + "Cosylang v0.0.0" + RESET + "\n");
            return 0;
        }
        else if (arg == "-o")
        {
            if (i + 1 < argc)
            {
                outputFile = argv[++i];
            }
        }
        else if (arg == "--typing-speed")
        {
            if (i + 1 < argc)
            {
                typing_speed = std::stoi(argv[++i]);
            }
        }
        else if (arg == "test")
        {
            Silteli::TestManager::runTests();
            return 0;
        }
        else if (i + 1 < argc)
        {
            inputFilesPaths.emplace_back(arg);
        }
        else
        {
            printHelp(typing_speed);
            return 0;
        }
    }

    if (inputFilesPaths.empty())
    {
        std::cerr << RED << "Error: No input files specified \n";
        return 1;
    }

    std::vector<std::unique_ptr<Cosylang::Source::FileBuffer>> files;
    Cosylang::Source::SourceManager source(std::move(files));

    Cosylang::Reporter::DiagnosticConfig config
    {
        true,
        false,
        true,
        std::cerr,
    };
    Cosylang::Reporter::DiagnosticEngine error_reporter(config, source);

    for (auto inputFile : inputFilesPaths)
    {
        source.loadFile(inputFile);
        std::string input_source = source.getFileBuffer(source.files.size() - 1)->source_code;
        std::string headline = BLUE + "Compiling " + inputFile + ": " + RESET;
        typeText(headline + "\n", typing_speed);

        Cosylang::Lexer::Lexer lex(input_source.c_str());
        typeText(BLUE + "| " + YELLOW + "Tokenizing..." + BLUE +"     |\n" + RESET, typing_speed);
        auto tokens = lex.tokenize();
        typeText(BLUE + "|" + YELLOW +" | " + GREEN + "Successfully!" +  BLUE + "   |\n" + RESET, typing_speed);

        typeText(BLUE + "| " + ORANGE + "Parsing..." +  BLUE +"        |\n" + RESET, typing_speed);
        Arena nodes_arena = Arena(1024);
        Cosylang::Parser::Parser parser(tokens, nodes_arena, source.files.size() - 1, error_reporter);
        const Cosylang::Parser::Node* main_node = parser.parse();
        typeText(BLUE + "|" + ORANGE +" | " + GREEN + "Successfully!" +  BLUE +"   |\n" + RESET, typing_speed);

        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::cout << "\033[A\033[2K\r";
        std::cout << "\033[A\033[2K\r";
        std::cout << "\033[A\033[2K\r";
        std::cout << "\033[A\033[2K\r";
        std::cout << BLUE << headline << RESET;

        typeText(BLUE + "\033[1A" + GREEN + "✓ \n" + RESET, typing_speed);
    }

    return 0;
}

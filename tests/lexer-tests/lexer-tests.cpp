#include <vector>
#include <string_view>
#include "../../src/frontend/lexer/lexer.hpp"
#include "../../external/silteli.hpp"

using namespace Cosylang::Lexer::Token;

UNIT_TEST(LexerStringTest)
{
    Cosylang::Lexer::Lexer lexer("'Hello, world!'");

    auto tokens = lexer.tokenize();

    bool is_valid = (tokens.size() == 2) &&
                    (tokens[0].type == TokenType::STRING) &&
                    (tokens[0].name == "Hello, world!") &&
                    (tokens[0].line == 1) &&
                    (tokens[0].column == 2) &&
                    (tokens[0].offset == 1) &&
                    (tokens[0].length == 13) &&
                    (tokens[1].type == TokenType::END_OF_FILE);

    Silteli::expect(is_valid);
}

UNIT_TEST(LexerNumbersTest)
{
    Cosylang::Lexer::Lexer lexer("42 3.1415 0");

    auto tokens = lexer.tokenize();

    bool is_valid = (tokens.size() == 4) &&
                    (tokens[0].type == TokenType::INTEGER || tokens[0].type == TokenType::NUMBER) &&
                    (tokens[0].name == "42") &&
                    (tokens[0].offset == 0) &&
                    (tokens[1].type == TokenType::FLOAT || tokens[1].type == TokenType::NUMBER) &&
                    (tokens[1].name == "3.1415") &&
                    (tokens[1].offset == 3) &&
                    (tokens[2].name == "0") &&
                    (tokens[3].type == TokenType::END_OF_FILE);

    Silteli::expect(is_valid);
}

UNIT_TEST(LexerKeywordsAndIdentifiersTest)
{
    Cosylang::Lexer::Lexer lexer("func var const if else true false variable");

    auto tokens = lexer.tokenize();

    bool is_valid = (tokens.size() == 9) &&
                    (tokens[0].type == TokenType::FUNC) &&
                    (tokens[1].type == TokenType::VAR) &&
                    (tokens[2].type == TokenType::CONST) &&
                    (tokens[3].type == TokenType::IF) &&
                    (tokens[4].type == TokenType::ELSE) &&
                    (tokens[5].type == TokenType::TRUE) &&
                    (tokens[6].type == TokenType::FALSE) &&
                    (tokens[7].type == TokenType::ID) &&
                    (tokens[7].name == "variable") &&
                    (tokens[8].type == TokenType::END_OF_FILE);

    Silteli::expect(is_valid);
}

UNIT_TEST(LexerOperatorsTest)
{
    Cosylang::Lexer::Lexer lexer("+ += => = . .. == !=");

    auto tokens = lexer.tokenize();

    bool is_valid = (tokens.size() == 9) &&
                    (tokens[0].type == TokenType::PLUS) &&
                    (tokens[1].type == TokenType::PLUS_ASSIGN) &&
                    (tokens[2].type == TokenType::ARROW) &&
                    (tokens[3].type == TokenType::ASSIGN) &&
                    (tokens[4].type == TokenType::DOT) &&
                    (tokens[5].type == TokenType::RANGE) &&
                    (tokens[6].type == TokenType::EQUALS) &&
                    (tokens[7].type == TokenType::NOT_EQUALS) &&
                    (tokens[8].type == TokenType::END_OF_FILE);

    Silteli::expect(is_valid);
}

UNIT_TEST(LexerLogicalOperatorsTest)
{
    Cosylang::Lexer::Lexer lexer("and && or || not !");

    auto tokens = lexer.tokenize();

    bool is_valid = (tokens.size() == 7) &&
                    (tokens[0].type == TokenType::AND) &&
                    (tokens[1].type == TokenType::AND) &&
                    (tokens[2].type == TokenType::OR) &&
                    (tokens[3].type == TokenType::OR) &&
                    (tokens[4].type == TokenType::NOT) &&
                    (tokens[5].type == TokenType::NOT) &&
                    (tokens[6].type == TokenType::END_OF_FILE);

    Silteli::expect(is_valid);
}

UNIT_TEST(LexerPositionTrackingTest)
{
    const char* code =
        "func name() : int\n"
        "{\n"
        "    function(12, 42)\n"
        "}";

    Cosylang::Lexer::Lexer lexer(code);

    auto tokens = lexer.tokenize();

    bool is_valid = (tokens.size() == 15) &&
                    (tokens[0].type == TokenType::FUNC && tokens[0].name == "func" && tokens[0].line == 1 && tokens[0].column == 1) &&
                    (tokens[1].type == TokenType::ID && tokens[1].name == "name" && tokens[1].line == 1 && tokens[1].column == 6) &&
                    (tokens[2].type == TokenType::LEFT_BRACKET && tokens[2].name == "(" && tokens[2].line == 1 && tokens[2].column == 10) &&
                    (tokens[3].type == TokenType::RIGHT_BRACKET && tokens[3].name == ")" && tokens[3].line == 1 && tokens[3].column == 11) &&
                    (tokens[4].type == TokenType::COLON && tokens[4].name == ":" && tokens[4].line == 1 && tokens[4].column == 13) &&
                    (tokens[5].type == TokenType::ID && tokens[5].name == "int" && tokens[5].line == 1 && tokens[5].column == 15) &&
                    (tokens[6].type == TokenType::LEFT_CURLY_BRACKET && tokens[6].name == "{" && tokens[6].line == 2 && tokens[6].column == 1) &&
                    (tokens[7].type == TokenType::ID && tokens[7].name == "function" && tokens[7].line == 3 && tokens[7].column == 5) &&
                    (tokens[8].type == TokenType::LEFT_BRACKET && tokens[8].name == "(" && tokens[8].line == 3 && tokens[8].column == 13) &&
                    (tokens[9].type == TokenType::INTEGER && tokens[9].name == "12" && tokens[9].line == 3 && tokens[9].column == 14) &&
                    (tokens[10].type == TokenType::COMMA && tokens[10].name == "," && tokens[10].line == 3 && tokens[10].column == 16) &&
                    (tokens[11].type == TokenType::INTEGER && tokens[11].name == "42" && tokens[11].line == 3 && tokens[11].column == 18) &&
                    (tokens[12].type == TokenType::RIGHT_BRACKET && tokens[12].name == ")" && tokens[12].line == 3 && tokens[12].column == 20) &&
                    (tokens[13].type == TokenType::RIGHT_CURLY_BRACKET && tokens[13].name == "}" && tokens[13].line == 4 && tokens[13].column == 1) &&
                    (tokens[14].type == TokenType::END_OF_FILE);

    Silteli::expect(is_valid);
}

UNIT_TEST(LexerEmptyInputTest)
{
    Cosylang::Lexer::Lexer lexer("");

    auto tokens = lexer.tokenize();

    bool is_valid = (tokens.size() == 1) && (tokens[0].type == TokenType::END_OF_FILE);

    Silteli::expect(is_valid);
}

#include "lexer.hpp"
#include "token/token.hpp"
#include <cstdlib>
#include <string_view>
#include <vector>
#include <iostream>

namespace Cosylang::Lexer
{

inline void Lexer::pushBackToken(std::vector<Token::Token>& vector, Token::Token token)
{
    current++;
    vector.push_back(token);
    offset++;
    column++;
}

std::vector<Token::Token> Lexer::tokenize()
{
    std::vector<Token::Token> out_vector;

    while (*current != '\0')
    {
        skipWhitespace();

        if (*current == '\0')
        {
            return out_vector;
        }

        if (table.data[*current] == CharType::IsDigit)
        {
            out_vector.push_back(processNumber());
            continue;
        }
        else if (table.data[*current] == CharType::IsAlpha)
        {
            out_vector.push_back(processIdentifier());
            continue;
        }
        else
        {
            switch (*current)
            {
                case '.':
                {
                    if (current[1] == '.')
                    {
                        pushBackToken(out_vector, Token::Token(Token::TokenType::RANGE, "..", line, column, offset));
                        current++;
                        continue;
                    }

                    pushBackToken(out_vector, Token::Token(Token::TokenType::DOT, ".", line, column, offset));
                    continue;
                }
                case ',':
                {
                    pushBackToken(out_vector, Token::Token(Token::TokenType::COMMA, ",", line, column, offset));
                    continue;
                }
                case ':':
                {
                    pushBackToken(out_vector, Token::Token(Token::TokenType::COLON, ":", line, column, offset));
                    continue;
                }
                case ';':
                {
                    pushBackToken(out_vector, Token::Token(Token::TokenType::SEMICOLON, ";", line, column, offset));
                    continue;
                }
                case '=':
                {
                    if (current[1] == '=')
                    {
                        current++;
                        pushBackToken(out_vector, Token::Token(Token::TokenType::EQUALS, "==", line, column, offset));
                        continue;
                    }
                    if (current[1] == '>')
                    {
                        current++;
                        pushBackToken(out_vector, Token::Token(Token::TokenType::ARROW, "=>", line, column, offset));
                        continue;
                    }
                    pushBackToken(out_vector, Token::Token(Token::TokenType::ASSIGN, "=", line, column, offset));
                    continue;
                }
                case '+':
                {
                    if (current[1] == '=')
                    {
                        current++;
                        pushBackToken(out_vector, Token::Token(Token::TokenType::PLUS_ASSIGN, "+=", line, column, offset));
                        continue;
                    }
                    pushBackToken(out_vector, Token::Token(Token::TokenType::PLUS, "+", line, column, offset));
                    continue;
                }
                case '-':
                {
                    if (current[1] == '=')
                    {
                        current++;
                        pushBackToken(out_vector, Token::Token(Token::TokenType::MINUS_ASSIGN, "-=", line, column, offset));
                        continue;
                    }
                    pushBackToken(out_vector, Token::Token(Token::TokenType::MINUS, "-", line, column, offset));
                    continue;
                }
                case '*':
                {
                    if (current[1] == '=')
                    {
                        current++;
                        pushBackToken(out_vector, Token::Token(Token::TokenType::MUL_ASSIGN, "*=", line, column, offset));
                        continue;
                    }
                    pushBackToken(out_vector, Token::Token(Token::TokenType::MUL, "*", line, column, offset));
                    continue;
                }
                case '/':
                {
                    if (current[1] == '=')
                    {
                        current++;
                        pushBackToken(out_vector, Token::Token(Token::TokenType::DIV_ASSIGN, "/=", line, column, offset));
                        continue;
                    }
                    pushBackToken(out_vector, Token::Token(Token::TokenType::DIV, "/", line, column, offset));
                    continue;
                }
                case '!':
                {
                    if (current[1] == '=')
                    {
                        current++;
                        pushBackToken(out_vector, Token::Token(Token::TokenType::NOT_EQUALS, "!=", line, column, offset));
                        continue;
                    }
                    pushBackToken(out_vector, Token::Token(Token::TokenType::NOT, "!", line, column, offset));
                    continue;
                }
                case '>':
                {
                    if (current[1] == '=')
                    {
                        current++;
                        pushBackToken(out_vector, Token::Token(Token::TokenType::GREATER_OR_EQUAL, ">=", line, column, offset));
                        continue;
                    }
                    pushBackToken(out_vector, Token::Token(Token::TokenType::GREATER, ">", line, column, offset));
                    continue;
                }
                case '<':
                {
                    if (current[1] == '=')
                    {
                        current++;
                        pushBackToken(out_vector, Token::Token(Token::TokenType::LESS_OR_EQUAL, "<=", line, column, offset));
                        continue;
                    }
                    pushBackToken(out_vector, Token::Token(Token::TokenType::LESS, "<", line, column, offset));
                    continue;
                }
                case '(':
                {
                    pushBackToken(out_vector, Token::Token(Token::TokenType::LEFT_BRACKET, "(", line, column, offset));
                    continue;
                }
                case ')':
                {
                    pushBackToken(out_vector, Token::Token(Token::TokenType::RIGHT_BRACKET, ")", line, column, offset));
                    continue;
                }
                case '{':
                {
                    pushBackToken(out_vector, Token::Token(Token::TokenType::LEFT_CURLY_BRACKET, "{", line, column, offset));
                    continue;
                }
                case '}':
                {
                    pushBackToken(out_vector, Token::Token(Token::TokenType::RIGHT_CURLY_BRACKET, "}", line, column, offset));
                    continue;
                }
                case '[':
                {
                    pushBackToken(out_vector, Token::Token(Token::TokenType::LEFT_SQUARE_BRACKET, "[", line, column, offset));
                    continue;
                }
                case ']':
                {
                    pushBackToken(out_vector, Token::Token(Token::TokenType::RIGHT_SQUARE_BRACKET, "]", line, column, offset));
                    continue;
                }
                case '&':
                {
                    if (current[1] == '&')
                    {
                        current++;
                        pushBackToken(out_vector, Token::Token(Token::TokenType::AND, "&&", line, column, offset));
                        continue;
                    }
                    pushBackToken(out_vector, Token::Token(Token::TokenType::BIT_AND, "&", line, column, offset));
                    continue;
                }
                case '|':
                {
                    if (current[1] == '|')
                    {
                        current++;
                        pushBackToken(out_vector, Token::Token(Token::TokenType::OR, "||", line, column, offset));
                        continue;
                    }
                    pushBackToken(out_vector, Token::Token(Token::TokenType::BIT_OR, "|", line, column, offset));
                    continue;
                }
                case '^':
                {
                    pushBackToken(out_vector, Token::Token(Token::TokenType::BIT_XOR, "^", line, column, offset));
                    continue;
                }
                case '~':
                {
                    pushBackToken(out_vector, Token::Token(Token::TokenType::BIT_NOT, "~", line, column, offset));
                    continue;
                }
                case '"':
                case '\'':
                {
                    pushBackToken(out_vector, processString());
                    continue;
                }
                default:
                {
                    std::cerr << "Unknown token! \n";
                }
            }
        }
    }
    pushBackToken(out_vector, Token::Token(Token::TokenType::END_OF_FILE, "EOF", line, column, offset));
    return out_vector;
}

inline void Lexer::skipWhitespace() noexcept
{
    while (*current == ' ' || *current == '\t' || *current == '\r' || *current == '\n')
    {
        if (*current == '\n')
        {
            line++;
            column = 0;
        }
        current++;
        offset++;
        column++;
    }
}

inline Token::Token Lexer::processIdentifier()
{
    const char* begin = current;
    const char* end = current;

    size_t id_offset = offset;
    size_t id_column = column;

    while (table.data[*current] == CharType::IsAlpha || table.data[*current] == CharType::IsDigit || *current == '_')
    {
        column++;
        current++;
        offset++;
        end = current;
    }

    std::string_view token_text(begin, static_cast<size_t>(end - begin));

    switch (token_text.length())
    {
        case 2:
        {
            if (token_text == "or") return Token::Token(Token::TokenType::OR, token_text, line, id_column, id_offset);
            else if (token_text == "if") return Token::Token(Token::TokenType::IF, token_text, line, id_column, id_offset);
            else return Token::Token(Token::TokenType::ID, token_text, line, id_column, id_offset);
        }
        case 3:
        {
            if (token_text == "and") return Token::Token(Token::TokenType::AND, token_text, line, id_column, id_offset);
            else if (token_text == "not") return Token::Token(Token::TokenType::NOT, token_text, line, id_column, id_offset);
            else if (token_text == "var") return Token::Token(Token::TokenType::VAR, token_text, line, id_column, id_offset);
            else if (token_text == "for") return Token::Token(Token::TokenType::FOR, token_text, line, id_column, id_offset);
            else return Token::Token(Token::TokenType::ID, token_text, line, id_column, id_offset);
        }
        case 4:
        {
            if (token_text == "true") return Token::Token(Token::TokenType::TRUE, token_text, line, id_column, id_offset);
            else if (token_text == "else") return Token::Token(Token::TokenType::ELSE, token_text, line, id_column, id_offset);
            else if (token_text == "elif") return Token::Token(Token::TokenType::ELIF, token_text, line, id_column, id_offset);
            else if (token_text == "func") return Token::Token(Token::TokenType::FUNC, token_text, line, id_column, id_offset);
            else if (token_text == "body") return Token::Token(Token::TokenType::BODY, token_text, line, id_column, id_offset);
            else if (token_text == "type") return Token::Token(Token::TokenType::TYPE, token_text, line, id_column, id_offset);
            else return Token::Token(Token::TokenType::ID, token_text, line, id_column, id_offset);
        }
        case 5:
        {
            if (token_text == "false") return Token::Token(Token::TokenType::FALSE, token_text, line, id_column, id_offset);
            else if (token_text == "const") return Token::Token(Token::TokenType::CONST, token_text, line, id_column, id_offset);
            else if (token_text == "match") return Token::Token(Token::TokenType::MATCH, token_text, line, id_column, id_offset);
            else if (token_text == "while") return Token::Token(Token::TokenType::WHILE, token_text, line, id_column, id_offset);
            else if (token_text == "break") return Token::Token(Token::TokenType::BREAK, token_text, line, id_column, id_offset);
            else return Token::Token(Token::TokenType::ID, token_text, line, id_column, id_offset);
        }
        case 6:
        {
            if (token_text == "import") return Token::Token(Token::TokenType::IMPORT, token_text, line, id_column, id_offset);
            else if (token_text == "return") return Token::Token(Token::TokenType::RETURN, token_text, line, id_column, id_offset);
            else return Token::Token(Token::TokenType::ID, token_text, line, id_column, id_offset);
        }
        case 8:
        {
            if (token_text == "continue") return Token::Token(Token::TokenType::CONTINUE, token_text, line, id_column, id_offset);
            else return Token::Token(Token::TokenType::ID, token_text, line, id_column, id_offset);
        }
        case 9:
        {
            if (token_text == "namespace") return Token::Token(Token::TokenType::NAMESPACE, token_text, line, id_column, id_offset);
            else return Token::Token(Token::TokenType::ID, token_text, line, id_column, id_offset);
        }
        default:
        {
            return Token::Token(Token::TokenType::ID, token_text, line, id_column, id_offset);
        }
    }
}

inline Token::Token Lexer::processNumber()
{
    const char* begin = current;
    const char* end = current;
    int dots_count = 0;

    size_t number_offset = offset;
    size_t number_column = column;
    size_t number_line = line;

    while (table.data[*current] == CharType::IsDigit || *current == '.')
    {
        current++;
        offset++;
        column++;

        if (*current == '.')
        {
            dots_count++;
            if (dots_count > 1)
            {
                std::cerr << "\033[31m Invalid suffix " <<  std::string_view(begin, static_cast<size_t>(end - begin)) << " on floating constant \033[0m \n";
            }
        }
        end = current;

    }
    std::string_view token_text(begin, static_cast<size_t>(end - begin));
    if (dots_count > 0)
    {
        return Token::Token(Token::TokenType::FLOAT, token_text, number_line, number_column, number_offset);
    }
    return Token::Token(Token::TokenType::INTEGER, token_text, number_line, number_column, number_offset);
}

inline Token::Token Lexer::processString()
{
    if (*current != '"' && *current != '\'')
    {
        std::exit(1);
    }

    const char* begin = current;
    const char* end = current;

    current++;
    offset++;
    column++;

    size_t string_offset = offset;
    size_t string_column = column;
    size_t string_line = line;

    while (*current != *begin && *current != '\0')
    {
        if (*current == '\n')
        {
            offset++;
            line++;
            column = 1;
            continue;
        }
        current++;
        offset++;
        column++;
        end = current;
    }

    if (*end != *begin)
    {
        std::cerr << "Invalid string!\n";
        std::exit(1);
    }
    else
    {
        std::string_view token_text(begin + 1, static_cast<size_t>(end - (begin + 1)));
        return Token::Token(Token::TokenType::STRING, token_text, string_line, string_column, string_offset);
    }
}

}; // namespace Cosylang::Lexer

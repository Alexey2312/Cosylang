#pragma once

#include <cstdint>
#include <string_view>
#include <vector>

#include "token/token.hpp"

namespace Cosylang::Lexer
{

enum class CharType : uint8_t { IsAlpha, IsDigit, IsSpace, IsOther };

struct LookupTable
{
    CharType data[256]{};

    constexpr LookupTable()
    {
        for (int i = 0; i < 256; ++i) data[i] = CharType::IsOther;
        for (int i = '0'; i <= '9'; ++i) data[i] = CharType::IsDigit;
        for (int i = 'a'; i <= 'z'; ++i) data[i] = CharType::IsAlpha;
        for (int i = 'A'; i <= 'Z'; ++i) data[i] = CharType::IsAlpha;
    }
};

class Lexer
{
    std::string_view source;

    LookupTable table = LookupTable();

    const char* current;

    size_t offset = 0;
    size_t line = 1;
    size_t column = 1;

    inline void skipWhitespace() noexcept;

    inline Token::Token processIdentifier();
    inline Token::Token processNumber();
    inline Token::Token processString();

    inline void pushBackToken(std::vector<Token::Token>& vector, Token::Token token);

public:
    Lexer(const char* this_source) : source(this_source) {current = this_source;}

    std::vector<Token::Token> tokenize();
};

}; // namespace Cosylang::Lexer

#include "../../head/lexer.hpp"
#include "../../head/token.hpp"
#include <stdexcept>
#include <vector>
#include <string>
#include <iostream>



struct checker
{
	std::string correctString;

	TokenType correctToken;
	token outToken = token(correctToken, correctString);

	bool check(std::string checkingString) {return correctString == checkingString; }
	TokenType getToken() {return correctToken; }
	std::string getValue() {return correctString; };
	checker(std::string correct, TokenType selectedToken) : correctString(correct), correctToken(selectedToken) {}

};

enum LineTypes
{
	OTHER,
	STRING_LINE,
	QUOTES,
	NUMBER,
	IDENT,
};

token createToken(std::string value, TokenType type)
{
	return token(type, value);
}


std::vector<checker> checkersVector = {checker(":", TokenType::COLON), checker("=", TokenType::EQUALS), checker("print", TokenType::PRINT)};

std::vector<token> Lexer::tokenize(std::string line)
{
    std::vector<token> outTokens;
    std::string word;
    std::vector<char> buffer;
    unsigned int position = 0;
    TokenType type;

    return outTokens;
}


/* TESTS */

void testLexer()
{
    std::string testingObject = "Lexer";
    Lexer* lexer = new Lexer();
    if (lexer->tokenize(":").at(0).getType() == TokenType::COLON)
    {
        std::cout << testingObject + ": test passed :) " << std::endl;
    }
    else
    {
        std::runtime_error(testingObject + ": test failed :( ");
    }
    delete lexer;

}

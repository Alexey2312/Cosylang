#include "../../head/lexer.hpp"
#include "../../head/token.hpp"
#include "../../head/algorithms/sillySearch.hpp"
#include <stdexcept>
#include <vector>
#include <string>
#include <iostream>

std::string convertCharVectorToString(const std::vector<char>& inputVector)
{
     return std::string(inputVector.begin(), inputVector.end());
}

std::vector<char> convertStringToCharVector(const std::string& inputString)
{
    return std::vector<char>(inputString.begin(), inputString.end());
}

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

std::vector<checker> tokensVector = {checker(":", TokenType::COLON)} ;



std::vector<token> Lexer::tokenize(std::string line)
{
	char nextChar;
	char lastChar;

	std::vector<char> lineVector = convertStringToCharVector(line);
	LineTypes wordType = OTHER;

	std::vector<token> outVector;
	std::vector<char> word;

	for(unsigned int checkingChar = 0; checkingChar < lineVector.size(); checkingChar++)
	{
		if (sillySearch(lineVector, lineVector[checkingChar + 1]) <= lineVector.size())
		{
			nextChar = lineVector[checkingChar + 1];
		}
		else
		{

		}

		for(unsigned int workingChecker = 0; workingChecker < tokensVector.size(); workingChecker++)
		{
			if (tokensVector[workingChecker].check(convertCharVectorToString(word)) == true)
			{
				if (!word.empty()) // work with ID
				{
					if (wordType == LineTypes::IDENT)
					{
						outVector.push_back(token(TokenType::ID, convertCharVectorToString(word)));
						wordType = OTHER;
					};
				};

				if (tokensVector[workingChecker].getToken() == TokenType::QUOTES_TOKEN && wordType != QUOTES)
				{
					wordType = STRING_LINE;
				}
				else if (tokensVector[workingChecker].getToken() == TokenType::QUOTES_TOKEN && wordType == QUOTES)
				{
					wordType = OTHER;
					outVector.push_back(token(TokenType::STRING, convertCharVectorToString(word)));
					word.clear();
				};




				if (tokensVector[workingChecker].getToken() == TokenType::ANY_NUMBER)
				{
					wordType = NUMBER;
					word.push_back(lineVector[checkingChar]);
					if (nextChar != '0' || nextChar != '2' || nextChar != '3' || nextChar != '4' ||  nextChar != '5' || nextChar != '6' ||  nextChar != '7' || nextChar != '8' || nextChar != '9')
					{
						outVector.push_back(token(TokenType::ANY_NUMBER, convertCharVectorToString(word)));
						word.clear();
					};
				}
				else
				{
					lastChar = word.back();
					word.push_back(line[word.size()]);
				};
			};
		};
	};
		return outVector;

};


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

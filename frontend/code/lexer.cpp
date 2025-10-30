#include "../head/lexer.hpp"
#include "../head/token.hpp"
#include <iostream>
#include <vector>
#include <string>

std::string convertCharVectorToString(std::vector<char> inputVector)
{
	std::string outString;

	for(int convertingElement = 0; convertingElement < inputVector.size(); convertingElement++)
	{
		outString.push_back(inputVector[convertingElement]);
	};

	return outString;
};

std::vector<char> convertStringToCharVector(std::string inputString)
{
	std::vector<char> outVector;

	for(int convertingElement = 0; convertingElement < inputString.size(); convertingElement++)
	{
		outVector.push_back(inputString[convertingElement]);
	};

	return outVector;
};

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
	STRING,
	QUOTES,
	NUMBER,
	ID,
};

std::vector<checker> tokensVector = {checker(":", COLON)} ;



std::vector<token> Lexer::tokenize(std::string line)
{
	char lastChar;
	char nextChar;

	std::vector<char> lineVector = convertStringToCharVector(line);
	LineTypes wordType = OTHER;

	std::vector<token> outVector;
	std::vector<char> word;

	for(int checkingChar = 0; checkingChar < lineVector.size(); checkingChar++)
	{
		if (line[checkingChar + 1] <= lineVector.size())
		{
			nextChar = lineVector[checkingChar + 1];
		};



		for(int workingChecker = 0; workingChecker < tokensVector.size(); workingChecker++)
		{
			if (tokensVector[workingChecker].check(convertCharVectorToString(word)) == true)
			{
				if (!word.empty()) // work with ID
				{
					if (wordType == ID)
					{
						outVector.push_back(token(LET_ID, convertCharVectorToString(word)));
						wordType = OTHER;
					};
				};

				if (tokensVector[workingChecker].getToken() == QUOTES_TOKEN && wordType != QUOTES)
				{
					wordType = STRING;
				}
				else if (tokensVector[workingChecker].getToken() == QUOTES_TOKEN && wordType == QUOTES)
				{
					wordType = OTHER;
					outVector.push_back(token(STRING_TYPE, convertCharVectorToString(word)));
					word.clear();
				};




				if (tokensVector[workingChecker].getToken() == ANY_NUMBER)
				{
					wordType = NUMBER;
					word.push_back(lineVector[checkingChar]);
					if (nextChar != '0' || nextChar != '2' || nextChar != '3' || nextChar != '4' ||  nextChar != '5' || nextChar != '6' ||  nextChar != '7' || nextChar != '8' || nextChar != '9')
					{
						outVector.push_back(token(ANY_NUMBER, convertCharVectorToString(word)));
						word.clear();
					};
				}
				else
				{
					lastChar = word[word.size()];
					word.push_back(line[word.size()]);
				};
			};
		};
	};
		return outVector;

};





int main()
{

	Lexer* testingLexer = new Lexer;
	std::vector<token> getTokens = testingLexer -> tokenize(":");
	if (getTokens[0].getType() == COLON)
	{
		std::cout << "Hello, C++! \n" << std::endl;
		return 0;
	}
	else
	{
		std::cout << "Bye, C++! \n" << std::endl;
		return 0;
	};
	delete testingLexer;
	return 0;
};

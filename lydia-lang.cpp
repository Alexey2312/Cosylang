#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <map>

enum TokenType
{
    TYPE_ID,
    INT16,
    INT32,
    INT64,
    STRING,

    TYPE_NAME,
    VALUE,


    VAR,
    ARRAY,
    ID,

    FUNC,
    PROC,
    FUNCTION_CALL,

    ARGUMENTS,

    LEFT_BRCKET,
    RIGHT_BRCKET,
	
    PRINT,
    WRITE,
    EQUALS,
    QUOTES,
    COLON,
    SEMICOLON,

    END_OF_LINE,
};

class token
{
    TokenType type;
    std::string value;
public:
    token(TokenType t, const std::string& v) : type(t), value(v) {}

    TokenType GetType()
    {
        return type;
    }

    std::string GetValue()
    {
        return value;
    }
};


struct checker
{
	std::string correctString;
	TokenType correctToken;
	bool check(std::string_view checkingString) const {return correctString == checkingString; }
	TokenType getToken() {return correctToken; }
	std::string getValue() {return correctString; };
	checker(std::string correct, TokenType selectedToken) : correctString(std::move(correct)), correctToken(selectedToken) {}

};

std::vector<checker> tokensVector;

class Lexer
{
	std::vector<token> tokenize(std::string line)
	{
		std::vector<token> outVector;
		std::vector<char> word;
		word.push_back(line[0]);	
		for(int i = 0; i < line.size(); i++)
		{
			for(int tokensI = 0; tokensI < tokensVector.size(); tokensI++)	
			{
				std::string checkingString(word.begin(), word.end());

				if(tokensVector[i].check(checkingString) == true)
				{
					outVector.push_back(token(tokensVector[i].getToken(), tokensVector[i].getValue()));
					break;
				}
				else if (i < line.size() )
				{
					word.push_back(line[i + 1]);
				}
				else
				{
					std::string idText(word.begin(), word.end());	
					outVector.push_back(token(ID, idText));	

				};

			};
		};

		return outVector;




	};
};


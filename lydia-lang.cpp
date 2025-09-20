#include <iostream>
#include <vector>
#include <string>
#include <map>




enum TokenType
{
    INT16,
    INT32,
    INT64,
    STRING,

    INT16_VAR,
    INT32_VAR,
    INT64_VAR,
    STRING_VAR,

    VAR,
    ARRAY,
    ID,

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

enum nodes
{
	VAR_NODE,
	ID_NODE,
	TYPE_ID_NODE,
	STRING_NODE,

	COLON_NODE,
	SEMICOLON_NODE,
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





class Lexer
{
    short position = 0;

    std::string input;

    std::vector<char> word;
    TokenType type;
    std::string value;

    char character;

public:
    std::vector<token> tokenize(std::string line)
    {
        std::vector<token> outVector;
        std::vector<char> lineVector(line.begin(), line.end());

        for(int i = 0; i < lineVector.size(); i++)
        {
            if(std::string(word.begin(), word.end()) == "")
            {
                outVector.push_back(token(SEMICOLON, ";"));
                word.clear();
                return outVector;
            }
            else if (std::string(word.begin(), word.end()) == ":")
            {
                outVector.push_back(token(COLON, ":"));
                word.clear();
            }
            else if (std::string(word.begin(), word.end()) == "\"")
            {
                outVector.push_back(token(QUOTES, "\""));
                word.clear();
            }
            else if (std::string(word.begin(), word.end()) == std::string("\""))
            {
                outVector.push_back(token(QUOTES, "\""));
                word.clear();
            }
            else if (std::string(word.begin(), word.end()) == "var")
            {
                outVector.push_back(token(VAR, "var"));
                std::cout << "Нашёл токен var";
                word.clear();
            }
            else
            {
                if (outVector[i - 1].GetType() == QUOTES || outVector[i - 1].GetType() == STRING)
                {
                    word.push_back(lineVector[i]);
                }
                else
                {
                    std::cout << "ERROR, unknow token: " << lineVector[i];
                }
            }
        };
        return outVector;
    }
};

int main()
{
    Lexer lexerObject;
    std::string checkingToken = "var";
    std::vector<token> checkingVector;
    checkingVector = lexerObject.tokenize("var");
    std::cout << checkingVector[0].GetValue();
    std::cout << "hi!";
    return 0;
};

class node
{

    static node parent; 
    nodes type;
    nodes leftSubsidiary;
    nodes rightSubsidiary;

    std::string value;
};

class parser
{
	std::map<TokenType, std::vector<TokenType>> correctNextTokens
	{
		{VAR, {ID}}, 
		{ID, {LEFT_BRCKET, COLON}},
		{COLON, {INT16_VAR, INT32_VAR, INT64_VAR, STRING_VAR}},
		{INT16_VAR, {EQUALS, END_OF_LINE}},
		{INT32_VAR, {EQUALS, END_OF_LINE}},
		{INT64_VAR, {EQUALS, END_OF_LINE}},
		{STRING_VAR, {EQUALS, END_OF_LINE}},

	};

	bool isLineCorrect(std::vector<token> checkingTokens)
	{
		for(short i = 0; i <  checkingTokens.size(); i++)
		{
			return false;
		}
	};


};

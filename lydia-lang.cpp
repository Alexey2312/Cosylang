#include <iostream>
#include <vector>
#include <string>

enum TokenType
{
    INT16,
    INT32,
    INT64,
    STRING,
    BOOL,
    BYTE,
    CHAR,

    INT16_VAR,
    INT32_VAR,
    INT64_VAR,
    STRING_VAR,
    BOOL_VAR,
    BYTE_VAR,
    CHAR_VAR,

    VAR,
    ARRAY,
    NAME,

    LEFT_BRCKET,
    RIGHT_BRCKET,

    PRINT,
    WRITE,
    EQUALS,
    QUOTES,
    COLON,
    SEMICOLON,
};

enum nodes
{
	NONE,
        VAR,
        PRINT,
        NAME,
        ARGUMENTS,
	EQUALS,
	SEMICOLON,
        TYPE_ID,
	TYPE,
        VALUE,
        BRECKETS,
        LEFT_BRECKET,
        RIGHT_BRECKET,
        EQUALS,
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

    nodes parent; 
    nodes type;
    nodes leftSubsidiary;
    nodes rightSubsidiary;

    std::string value;
};

class cheker
{
	std::vector<node> ast;

	bool checkLine(std::vector<token> lineVector)
	{
		for(short i = 0; i < lineVector.size(); i++)
		{
			if (lineVector[i].GetValue() == "var")
			{
				ast.push_back(node(nodes.NONE, nodes.VAR, nodes.NAME, nodes.EQUALS, "var")); 
			
			}
		}
	}
};

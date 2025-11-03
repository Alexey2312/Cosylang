#include "../head/parser.hpp"
#include "../head/node.hpp"
#include "../head/token.hpp"
#include "../head/checkOperationsTypes.hpp"
#include <stdexcept>
#include <vector>
#include <iostream>
#include <map>

void Parser::parseError(const std::string& message)
{
    throw std::runtime_error(message);
}

std::map<TokenType, int> tokenPriority = {
    {PLUS, 2},
    {MINUS, 2},
    {MUL, 3},
    {DIV, 3},
    {PRINT, 1},
    {EQUAL, 1},
    {LET_ID, 0},
    {ANY_NUMBER, 0},
};

std::vector<token> tokens;
unsigned int position = 0;

token nextToken()
{
    if (position >= tokens.size())
    {
        return token(TokenType::EOL, "");
    }
    return tokens[position++];
}

token peekToken()
{
    return tokens[position];
}

void advance()
{
    position++;
}

bool isVectorContains(const std::vector<TokenType>& expectedTokens, TokenType currentToken)
{
    for(auto tokenType : expectedTokens)
    {
        if(tokenType == currentToken)
        {
            return true;
        }
    }
    return false;
}

Node* lastOperation = nullptr;
std::vector<Node*> intermediateData;
std::vector<Node*> intermediateExpressions;

bool parseBinary(token currentToken)
{
    if (isBinaryOperator(currentToken.getType()))
    {

        if (intermediateExpressions.empty())
        {
            intermediateExpressions.push_back(new Node(currentToken.getType(), currentToken.getValue(), tokenPriority[currentToken.getType()], {intermediateData[position--]}, nullptr, BINARY));
            lastOperation = intermediateExpressions.back();
            return true;
        }
        else if (intermediateExpressions.back()->getPriority() > tokenPriority[currentToken.getType()])
        {
            intermediateExpressions.back()->addChild(new Node(currentToken.getType(), currentToken.getValue(), tokenPriority[currentToken.getType()], {intermediateData.back()}, nullptr, BINARY));
            lastOperation = intermediateExpressions.back();
            return true;
        }
        else if (intermediateExpressions.back()->getPriority() <= tokenPriority[currentToken.getType()])
        {
            intermediateExpressions.push_back(new Node(currentToken.getType(), currentToken.getValue(), tokenPriority[currentToken.getType()], {intermediateExpressions.back()}, nullptr, BINARY));
            lastOperation = intermediateExpressions.back();
            return true;
        }
    }
    return false;
}

std::vector<Node*> Parser::parse(std::vector<token> inputTokens)
{
    tokens = inputTokens;
    std::vector<Node*> ast;

    for(token parsingToken : tokens)
    {
        std::vector<TokenType> expectedTokens;
        switch(parsingToken.getType())
        {
            case LET_ID:
            {
                if(isVectorContains(expectedTokens, TokenType::LET_ID) || expectedTokens.empty())
                {
                    if (lastOperation != nullptr)
                    {
                            intermediateData.push_back(new Node(parsingToken.getType(), parsingToken.getValue(), tokenPriority[parsingToken.getType()], {nullptr}, nullptr, NOT_AN_OPERATION));
                        if (nextToken().getType() != MUL || nextToken().getType() != DIV || nextToken().getType() != MINUS || nextToken().getType() != PLUS)
                        {
                            lastOperation->addChild(intermediateData.back());
                            intermediateData.clear();
                        }
                        else if (tokenPriority[nextToken().getType()] <= lastOperation->getPriority())
                        {
                            lastOperation->addChild(intermediateData.back());
                            intermediateData.clear();
                            lastOperation = nullptr;
                        }
                    }
                    else
                    {
                        intermediateData.push_back(new Node(parsingToken.getType(), parsingToken.getValue(), tokenPriority[parsingToken.getType()], {nullptr}, nullptr, NOT_AN_OPERATION));
                    }
                }
                break;
            }
            case ANY_NUMBER:
            {
                if(isVectorContains(expectedTokens, TokenType::ANY_NUMBER) || expectedTokens.empty())
                {
                    if (lastOperation != nullptr)
                    {
                        intermediateData.push_back(new Node(parsingToken.getType(), parsingToken.getValue(), tokenPriority[parsingToken.getType()], {nullptr}, nullptr, NOT_AN_OPERATION));
                        if (nextToken().getType() != MUL || nextToken().getType() != DIV || nextToken().getType() != MINUS || nextToken().getType() != PLUS)
                        {
                            lastOperation->addChild(intermediateData.back());
                            intermediateData.clear();
                            expectedTokens = {TokenType::PLUS, TokenType::MINUS, TokenType::MUL, TokenType::DIV, TokenType::RIGHT_PAREN};
                        }
                        else if (tokenPriority[nextToken().getType()] <= lastOperation->getPriority())
                        {
                            lastOperation->addChild(intermediateData.back());
                            intermediateData.clear();
                            lastOperation = nullptr;
                            expectedTokens = {TokenType::PLUS, TokenType::MINUS, TokenType::MUL, TokenType::DIV, TokenType::RIGHT_PAREN};
                        }
                    }
                    else
                    {
                        intermediateData.push_back(new Node(parsingToken.getType(), parsingToken.getValue(), tokenPriority[parsingToken.getType()], {nullptr}, nullptr, NOT_AN_OPERATION));
                        expectedTokens = {TokenType::PLUS, TokenType::MINUS, TokenType::MUL, TokenType::DIV, TokenType::RIGHT_PAREN};
                    }
                }
                break;
            }


            case TokenType::PLUS:
            {
                if(isVectorContains(expectedTokens, TokenType::PLUS)  || expectedTokens.empty())
                {
                    std::cout << "Plus token found: " << parsingToken.getValue() << std::endl;
                    bool binaryParse = parseBinary(parsingToken);
                    if (binaryParse)
                    {
                        advance();
                        expectedTokens = {TokenType::ANY_NUMBER, TokenType::LET_ID};
                    }
                    else
                    {
                        parseError("ERROR, MUST BE BINARY, but got " + parsingToken.getValue());
                    }
                }
                break;
            }

            case TokenType::MINUS:
            {
                if(isVectorContains(expectedTokens, TokenType::MINUS) || expectedTokens.empty())
                {
                    std::cout << "Minus token found: " << parsingToken.getValue() << std::endl;
                    bool binaryParse = parseBinary(parsingToken);
                    if (binaryParse)
                    {
                        advance();
                        expectedTokens.clear();
                        expectedTokens = {TokenType::ANY_NUMBER, TokenType::LET_ID};
                    }
                    else
                    {
                        parseError("ERROR, MUST BE BINARY, but got " + parsingToken.getValue());
                    }
                }
                break;
            }

            default:
            {
                parseError("ERROR, UNKNOWN TOKEN");
            }
            break;
        }
    }
    std::cout << "Parsing complete!" << std::endl;
    if (!intermediateExpressions.empty())
    {
        for (auto exp : intermediateExpressions)
        {
            exp->printChildren();
        }
        ast = intermediateExpressions;
        return ast;
    }
    else
    {
        throw std::runtime_error("No expressions found");
    }
}

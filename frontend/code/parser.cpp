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
};

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
        switch(parsingToken.getType())
        {
            case LET_ID:
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
                break;
            }
            case ANY_NUMBER:
            {
                std::cout << "Number token found: " << parsingToken.getValue() << std::endl;
                if (lastOperation != nullptr)
                {
                    intermediateData.push_back(new Node(parsingToken.getType(), parsingToken.getValue(), tokenPriority[parsingToken.getType()], {nullptr}, nullptr, NOT_AN_OPERATION));
                    if (nextToken().getType() != MUL || nextToken().getType() != DIV || nextToken().getType() != MINUS || nextToken().getType() != PLUS)
                    {
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
                    break;
                }
                else
                {
                    intermediateData.push_back(new Node(parsingToken.getType(), parsingToken.getValue(), tokenPriority[parsingToken.getType()], {nullptr}, nullptr, NOT_AN_OPERATION));
                    break;
                }


            case TokenType::PLUS:
            {
                std::cout << "Plus token found: " << parsingToken.getValue() << std::endl;
                bool binaryParse = parseBinary(parsingToken);
                if (binaryParse)
                {
                    advance();
                }
                else
                {
                    parseError("ERROR, MUST BE BINARY, but got " + parsingToken.getValue());
                }
                break;
            }

            case TokenType::MINUS:
            {
                std::cout << "Minus token found: " << parsingToken.getValue() << std::endl;
                bool binaryParse = parseBinary(parsingToken);
                if (binaryParse)
                {
                    advance();
                }
                else
                {
                    parseError("ERROR, MUST BE BINARY");
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
    }
    std::cout << "Parsing complete!" << std::endl;
    for (auto exp : intermediateExpressions)
    {
        exp->printChildren();
    }
    ast = intermediateExpressions;
    return ast;
}

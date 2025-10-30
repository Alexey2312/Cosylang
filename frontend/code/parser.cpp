#include "../head/parser.hpp"
#include "../head/node.hpp"
#include "../head/token.hpp"
#include <vector>
#include <map>

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
int position = 0;

token nextToken()
{
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


std::vector<Node> parse(std::vector<token> inputTokens)
{
    tokens = inputTokens;
    std::vector<Node> ast;

    Node* lastOperation = nullptr;
    std::vector<Node*> intermediateData;
    std::vector<Node*> intermediateExpressions;

    for(token parsingToken : tokens)
    {
        switch(parsingToken.getType())
        {
            case LET_ID || ANY_NUMBER:
                intermediateData.push_back(new Node(parsingToken.getType(), parsingToken.getValue(), tokenPriority[parsingToken.getType()], {nullptr}, nullptr, NOT_AN_OPERATION));
                if (lastOperation != nullptr)
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
            case PLUS:
                if (intermediateExpressions.empty())
                {
                    intermediateExpressions.push_back(new Node(parsingToken.getType(), parsingToken.getValue(), tokenPriority[parsingToken.getType()], {intermediateData[position--]}, nullptr, BINARY));
                    lastOperation = intermediateExpressions.back();
                }
                else
                {
                    if(intermediateExpressions.back()->getPriority() >= tokenPriority[parsingToken.getType()])
                    {
                        intermediateExpressions.push_back(new Node(parsingToken.getType(), parsingToken.getValue(), tokenPriority[parsingToken.getType()], {intermediateExpressions.back()}, nullptr, BINARY));
                        lastOperation = intermediateExpressions.back();
                    }
                    else if (intermediateExpressions.back()->getPriority() < tokenPriority[parsingToken.getType()])
                    {
                        intermediateExpressions.back()->addChild(new Node(parsingToken.getType(), parsingToken.getValue(), tokenPriority[parsingToken.getType()], {intermediateData.back()}, nullptr, BINARY));
                        lastOperation = intermediateExpressions.back();
                    }
                }



                break;



        };
        position++;
    };

    return ast;
};

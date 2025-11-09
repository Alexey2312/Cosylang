#include "../../head/parser.hpp"
#include "../../head/node.hpp"
#include "../../head/token.hpp"
#include "../../head/nodeCreator.hpp"
#include "../../head/reporter.hpp"
#include "../../head/algorithms/sillySearch.hpp"
#include <cstddef>
#include <stdexcept>
#include <string>
#include <map>
#include <vector>
#include <iostream>


std::map<TokenType, int> nodesPriorities = {
    {TokenType::PLUS, 2},
    {TokenType::MINUS, 2},
    {TokenType::MUL, 3},
    {TokenType::DIV, 3},
    {TokenType::LEFT_PAREN, 1},
    {TokenType::RIGHT_PAREN, 1},
    {TokenType::ANY_NUMBER, 0},
    {TokenType::ID, 0},

    // Keywords
    {TokenType::PRINT, 1},
    {TokenType::IF, 1},
};

std::vector<TokenType> keywordsTypes =
{
    TokenType::PRINT,
    TokenType::IF,
};

Reporter reporter = Reporter();



std::vector<token> tokens;
unsigned int position = 0;

token nextToken()
{
    reporter.report(Reporter::INFO, "nextToken: Next token called");
    if (position + 1 > tokens.size())
    {
        reporter.report(Reporter::WARNING, "nextToken: End of tokens reached, position: " + std::to_string(position));
        return token(TokenType::EOL, "");
    }
    reporter.report(Reporter::INFO, "nextToken: Returning token: " + tokens.at(position + 1).getValue());
    return tokens.at(position + 1);
}

token lastToken()
{
    reporter.report(Reporter::INFO, "lastToken: Last token called, input position: " + std::to_string(position) + ", token: " + tokens.at(position).getValue());
    if (position == 0)
    {
        reporter.report(Reporter::ERROR, "lastToken: Position is 0, cannot return previous token");
        throw std::runtime_error("lastToken: Position is 0");
    }
    return tokens.at(position - 1);
}

token peekToken()
{
    return tokens.at(position);
}

void advance()
{
    reporter.report(Reporter::INFO, "advance: Advance called, new position: " + std::to_string(position + 1) + " token is: " + tokens.at(position + 1).getValue());
    position++;
}

void eat(TokenType expectedToken, std::string message)
{
    reporter.report(Reporter::INFO, "eat: Eat called, message: " + message);
    if (peekToken().getType() == expectedToken || expectedToken == TokenType::EXPRESSION)
    {
        reporter.report(Reporter::INFO, "eat: Token matched, advancing position to " + std::to_string(position + 1) + ". Token is: " + tokens.at(position).getValue());
        advance();
    }
    else
    {
        reporter.printReports();
        throw std::runtime_error(message + ", got: " + tokens.at(position).getValue() + ", position: " + std::to_string(position));
    }
}

bool match(TokenType expectedToken)
{
    if (peekToken().getType() == expectedToken)
    {
        advance();
        return true;
    }
    return false;
}


Node parseFactor(token inputToken)
{
    reporter.report(Reporter::INFO, "parseFactor: Parsing factor, position: " + std::to_string(position) + ", token: " + inputToken.getValue() + ", position: " + std::to_string(position));
    switch (inputToken.getType())
    {
        case TokenType::ANY_NUMBER:
        {
            reporter.report(Reporter::INFO, "parseFactor: Parsing number");
            eat(TokenType::ANY_NUMBER, "Expected a number");
            return createNode(TokenType::ANY_NUMBER, lastToken().getValue(), 2, OperationType::BINARY);
        }

        case TokenType::ID:
        {

            eat(TokenType::ID, "Expected an identifier");
            return createNode(TokenType::ID, lastToken().getValue(), 1, OperationType::NOT_AN_OPERATION);
        }

        case TokenType::LEFT_PAREN:
        {
            reporter.report(Reporter::INFO, "parseFactor: Parsing left parenthesis");

            return createNode(TokenType::LEFT_PAREN, "", 2, OperationType::NOT_AN_OPERATION);
        }

        case TokenType::RIGHT_PAREN:
        {
            reporter.report(Reporter::INFO, "parseFactor: Parsing right parenthesis");
            eat(TokenType::RIGHT_PAREN, "Expected a closing parenthesis");
            return createNode(TokenType::RIGHT_PAREN, ")", 2, OperationType::NOT_AN_OPERATION);
        }

        case TokenType::EOL:
        {
            reporter.report(Reporter::INFO, "parseFactor: Parsing end of line");
            eat(TokenType::EOL, "Expected an end of line");
            return createNode(TokenType::EOL, "", 0, OperationType::NOT_AN_OPERATION);
        }

        default:
        {
            reporter.printReports();
            throw std::runtime_error("Unexpected token: " + inputToken.getValue());
        }

    }
}

Node parseTerm(token inputToken)
{
    reporter.report(Reporter::INFO, "parseTerm: Parsing term, position: " + std::to_string(position));
    int precedence = 3;
    Node left = parseFactor(inputToken);
    reporter.report(Reporter::INFO, "parseTerm: Factor parsed, left: " + left.getValue());
    Node outNode = createNode(TokenType::EOL, "", 0, OperationType::NOT_AN_OPERATION);
    bool isTerm = false;
    while (match(TokenType::MUL) || match(TokenType::DIV))
    {
        isTerm = true;
        token operation = lastToken();
        Node right = parseFactor(peekToken());
        outNode = Node(operation.getType(), operation.getValue(), precedence, {&left, &right}, nullptr, OperationType::BINARY);
    }
    if (!isTerm)
    {
        outNode = left;
    }
    reporter.report(Reporter::INFO, "parseTerm: Term parsed, outNode: " + outNode.getValue());

    return outNode;
}

Node parseExpression(token inputToken)
{
    reporter.report(Reporter::INFO, "parseExpression: Expression parsing started, inputToken: " + inputToken.getValue());
    Node outNode = createNode(TokenType::EOL, "", 0, OperationType::NOT_AN_OPERATION);
    if (inputToken.getType() == TokenType::STRING)
    {
        reporter.report(Reporter::INFO, "parseExpression: String token found");
        return Node(TokenType::STRING, inputToken.getValue(), 0, {nullptr}, nullptr, OperationType::NOT_AN_OPERATION);
    }

    reporter.report(Reporter::INFO, "parseExpression: Parsing term");
    Node left = parseTerm(inputToken);
    while (match(TokenType::PLUS) || match(TokenType::MINUS))
    {
        reporter.report(Reporter::INFO, "parseExpression: Parsing binary operation");
        token operation = lastToken();
        Node right = parseTerm(peekToken());
        outNode = Node(operation.getType(), operation.getValue(), 2, {&left, &right}, nullptr, OperationType::BINARY);
    }

    switch (outNode.getType())
    {
        case TokenType::EOL:
        {
            reporter.report(Reporter::INFO, "parseExpression: returning left node");
            return left;
        }
        default:
        {
            reporter.report(Reporter::INFO, "parseExpression: returning outNode");
            return outNode;
        }
    }
}

std::vector<Node> parseKeywordWithExpression(token keywordToken)
{
    std::vector<Node> nodes;
    size_t keywordIndex;
    SillySearch sillySearch = SillySearch();
    if (sillySearch.search(keywordsTypes, keywordToken.getType()) >= 0)
    {
        nodes.push_back(createNode(keywordToken.getType(), keywordToken.getValue(), 1, OperationType::KEYWORD));
        keywordIndex = nodes.size() - 1;
        reporter.report(Reporter::INFO, "parseKeywordWithExpression: Keyword token was parsed");
    }
    else
    {
        throw std::runtime_error("Invalid keyword token");
    }
    if (match(TokenType::LEFT_PAREN))
    {
        nodes.push_back(createNode(TokenType::LEFT_PAREN, "(", 1, OperationType::NOT_AN_OPERATION));
        nodes[keywordIndex].addChild(&nodes.back());
        nodes.push_back(parseExpression(peekToken()));
        reporter.report(Reporter::INFO, "parseKeywordWithExpression: Expression tokens were parsed, last node: " + nodes.back().getValue());
        nodes.push_back(createNode(TokenType::RIGHT_PAREN, ")", 1, OperationType::NOT_AN_OPERATION));
        nodes[keywordIndex].addChild(&nodes.back());
    }
    else
    {
        eat(TokenType::EXPRESSION, "Expected an expression, but got " + peekToken().getValue());
        nodes.push_back(parseExpression(lastToken()));
    }
    return nodes;
}

std::vector<Node> defineKeyword(token firstToken)
{
    switch (firstToken.getType())
    {
        case TokenType::PRINT:
        {
            reporter.report(Reporter::INFO, "defineKeyword: PRINT keyword found");
            eat(peekToken().getType(), "Expected 'print' keyword");
            return parseKeywordWithExpression(firstToken);
        }
        case TokenType::IF:
        {
            reporter.report(Reporter::INFO, "defineKeyword: IF keyword found");
            eat(peekToken().getType(), "Expected 'if' keyword");
            return parseKeywordWithExpression(peekToken());
        }

        default:
        {
            return parseKeywordWithExpression(firstToken);
        }

    }
}

std::vector<Node> Parser::parse(std::vector<token> inputTokens)
{
    position = 0;
    if (inputTokens.empty())
    {
        throw std::runtime_error("Parse: Empty input tokens");
    }
    reporter.report(Reporter::INFO, "Parser: Parsing started");
    tokens = inputTokens;
    std::vector<Node> ast = defineKeyword(inputTokens.at(0));
    return ast;
}

void testParser()
{
    Parser* parser = new Parser();
    std::cout << "Start first test" << std::endl;
    std::vector<token> tokens = {token(TokenType::PRINT, "print"), token(TokenType::LEFT_PAREN, "("), token(TokenType::STRING, "Hello, world!"), token(TokenType::RIGHT_PAREN, ")")};
    std::vector<Node> nodes = parser->parse(tokens);
    if (nodes.size() == 4)
    {
        for (Node node : nodes)
        {
            std::cout << node.getValue() << std::endl;
        }
        for (Node node : nodes)
        {
            node.printChildren();
            std::cout << " "<< std::endl;
        }
        reporter.printReports();
        std::cout << "test1 passed :), " + std::to_string(nodes[0].getChildren().size()) << std::endl;
    }
    else
    {
        reporter.printReports();
        std::cout << "test1 failed: Unexpected number of nodes, " + std::to_string(nodes.size()) + ", expected 4 :(" << std::endl;

    }


    delete parser;
}

int main()
{
    testParser();
    return 0;
}

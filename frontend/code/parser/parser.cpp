#include "../../head/parser.hpp"
#include "../../head/node.hpp"
#include "../../head/token.hpp"
#include "../../head/nodeCreator.hpp"
#include "../../head/reporter.hpp"
#include "../../head/algorithms/sillySearch.hpp"
#include "../convertor.cpp"
#include <memory>
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
    {TokenType::FUNC, 1}
};

std::vector<TokenType> keywordsTypes =
{
    TokenType::PRINT,
    TokenType::IF,
};

Reporter reporter = Reporter();

Node parseExpression(token inputToken);

std::vector<token> tokens;
unsigned int position = 0;

token nextToken()
{
    try
    {
        reporter.report(Reporter::INFO, "nextToken: Returning token: " + tokens.at(position + 1).getValue());
        return tokens.at(position + 1);
    }
    catch (std::out_of_range& error)
    {
        return token(TokenType::EOL, "eolNext");
    }
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
    try
    {
        reporter.report(Reporter::INFO, "advance: Advance called, new position: " + std::to_string(position + 1) + " token is: " + tokens.at(position + 1).getValue());
        position++;
    }
    catch (const std::exception& error)
    {
        reporter.printReports();
        throw std::runtime_error("advance: Exception caught: " + std::string(error.what()));
    }

}

void eat(TokenType expectedToken, std::string message)
{
    try
    {
        reporter.report(Reporter::INFO, "eat: Eat called, message: " + message);
        if (peekToken().getType() == expectedToken || expectedToken == TokenType::EXPRESSION)
        {
            reporter.report(Reporter::INFO, "eat: Token matched, advancing position to " + std::to_string(position + 1) + ". Token is: " + tokens.at(position + 1).getValue());
            advance();
        }
        else
        {
            reporter.printReports();
            throw std::runtime_error(message + ", got: " + tokens.at(position).getValue() + ", position: " + std::to_string(position));
        }
    }
    catch (const std::exception& error)
    {
        reporter.printReports();

        throw std::runtime_error("eat: Exception caught: " + std::string(error.what()));
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
    reporter.report(Reporter::INFO, "parseFactor: Parsing factor, token: " + inputToken.getValue());
    Node outNode = createNode(TokenType::EOL, "eol1", 0, OperationType::NOT_AN_OPERATION);
    reporter.report(Reporter::INFO, "parseFactor: Parsing factor, position: " + std::to_string(position) + ", token: " + inputToken.getValue() + ", position: " + std::to_string(position));
    switch (inputToken.getType())
    {
        case TokenType::ANY_NUMBER:
        {
            reporter.report(Reporter::INFO, "parseFactor: Parsing number");
            eat(TokenType::ANY_NUMBER, "Expected a number");

            Node numberNode = createNode(TokenType::ANY_NUMBER, lastToken().getValue(), 1, OperationType::NOT_AN_OPERATION);
            return numberNode;
        }

        case TokenType::ID:
        {

            eat(TokenType::ID, "Expected an identifier");
            Node identifierNode = createNode(TokenType::ID, lastToken().getValue(), 1, OperationType::NOT_AN_OPERATION);
            outNode.addChild(identifierNode);
            return outNode;
        }

        case TokenType::LEFT_PAREN:
        {
            reporter.report(Reporter::INFO, "parseFactor: Parsing left parenthesis");
            outNode = createNode(TokenType::EXPRESSION, "expression", 1, OperationType::NOT_AN_OPERATION);
            eat(TokenType::LEFT_PAREN, "Expected a left parenthesis");
            outNode.addChild(createNode(TokenType::LEFT_PAREN, "(", 1, OperationType::NOT_AN_OPERATION));
            outNode.addChild(parseExpression(peekToken()));

            return outNode;
        }

        case TokenType::RIGHT_PAREN:
        {
            reporter.report(Reporter::INFO, "parseFactor: Parsing right parenthesis");
            eat(TokenType::RIGHT_PAREN, "Expected a closing parenthesis");
            outNode = createNode(TokenType::RIGHT_PAREN, ")", 2, OperationType::NOT_AN_OPERATION);
            return outNode;
        }

        case TokenType::EOL:
        {
            reporter.report(Reporter::INFO, "parseFactor: Parsing end of line");
            eat(TokenType::EOL, "Expected an end of line");
            outNode = createNode(TokenType::EOL, "eol2", 0, OperationType::NOT_AN_OPERATION);
            return outNode;
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
    Node outNode = createNode(TokenType::EOL, "eol3", 0, OperationType::NOT_AN_OPERATION);
    reporter.report(Reporter::INFO, "parseTerm: Parsing term, position: " + std::to_string(position));
    int precedence = 3;
    Node left = parseFactor(inputToken);
    reporter.report(Reporter::INFO, "parseTerm: Factor parsed, left: " + left.getValue());
    outNode = createNode(TokenType::EOL, "eol4", 0, OperationType::NOT_AN_OPERATION);
    bool isTerm = false;
    while (match(TokenType::MUL) || match(TokenType::DIV))
    {
        isTerm = true;
        token operation = lastToken();
        Node right = parseFactor(peekToken());
        outNode = Node(operation.getType(), operation.getValue(), precedence, {left, right}, nullptr, OperationType::BINARY);
    }
    if (!isTerm)
    {
        return left;
    }
    reporter.report(Reporter::INFO, "parseTerm: Term parsed, outNode: " + outNode.getValue());

    return outNode;
}

Node parseExpression(token inputToken)
{
    reporter.report(Reporter::INFO, "parseExpression: Expression parsing started, inputToken: " + inputToken.getValue());
    Node outNode = createNode(TokenType::EOL, "eol6", 0, OperationType::NOT_AN_OPERATION);
    if (inputToken.getType() == TokenType::STRING)
    {
        reporter.report(Reporter::INFO, "parseExpression: String token found");
        outNode = createNode(TokenType::STRING, inputToken.getValue(), 0, OperationType::NOT_AN_OPERATION);
        return outNode;
    }

    reporter.report(Reporter::INFO, "parseExpression: Parsing term");
    Node left = parseTerm(inputToken);
    while (match(TokenType::PLUS) || match(TokenType::MINUS))
    {
        reporter.report(Reporter::INFO, "parseExpression: Parsing binary operation");
        token operation = lastToken();
        reporter.report(Reporter::INFO, "parseExpression: operation: " + operation.getValue());
        Node right = parseTerm(peekToken());
        reporter.report(Reporter::INFO, "parseExpression: right: " + right.getValue());

        outNode = createNode(operation.getType(), operation.getValue(), 0, OperationType::BINARY);
        outNode.addChild(left);
        outNode.addChild(right);
    }

    if (outNode.getType() == TokenType::EOL)
    {
        if (nextToken().getType() == TokenType::EOL)
        {
            reporter.report(Reporter::ERROR, "left: " + left.getValue());
            return left;
        }
        reporter.report(Reporter::WARNING, "parseNewToken: " + nextToken().getValue());
        advance();
        left.addChild(parseFactor(peekToken()));
        return left;
    }
    return outNode;
}

Node parseKeywordWithExpression(token keywordToken)
{
    Node outNode = createNode(TokenType::EOL, "EOL8", 0, OperationType::NOT_AN_OPERATION);
    SillySearch sillySearch = SillySearch();
    if (sillySearch.search(keywordsTypes, keywordToken.getType()) >= 0)
    {
        outNode = createNode(keywordToken.getType(), keywordToken.getValue(), 1, OperationType::KEYWORD);
        reporter.report(Reporter::INFO, "parseKeywordWithExpression: Keyword token was parsed");
    }
    else
    {
        throw std::runtime_error("Invalid keyword token");
    }
    Node expressionNode = parseExpression(peekToken());
    outNode.addChild(expressionNode);
    expressionNode.setParent(&outNode);
    return outNode;
}

class IParse
{
public:
    IParse() = default;
    virtual Node parseKeyword() = 0;
    virtual ~IParse() = default;
};

class ParsePrint : public IParse
{
    token parsingToken;
public:
    ParsePrint(token inputToken) : parsingToken(inputToken) {}
    Node parseKeyword() override
    {
        reporter.report(Reporter::INFO, "defineKeyword: PRINT keyword found");
        eat(peekToken().getType(), "Expected 'print' keyword");
        return parseKeywordWithExpression(parsingToken);
    }
};

class ParseIf : public IParse
{
    token parsingToken;
public:
    ParseIf(token inputToken) : parsingToken(inputToken) {}
    Node parseKeyword() override
    {
        reporter.report(Reporter::INFO, "defineKeyword: IF keyword found");
        eat(peekToken().getType(), "Expected 'if' keyword");
        return parseKeywordWithExpression(parsingToken);
    }
};

class ParseNone : public IParse
{
    token parsingToken;
public:
    ParseNone(token inputToken) : parsingToken(inputToken) {}
    Node parseKeyword() override
    {
        reporter.report(Reporter::INFO, "NONE keyword found");
        return createNode(TokenType::EOL, parsingToken.getValue(), 0, OperationType::NOT_AN_OPERATION);
    }
};

class SelectParser
{
    struct Checker
    {
        TokenType tokenType;
        std::shared_ptr<IParse> parseMethod = nullptr;
    public:
        Checker(TokenType type, std::shared_ptr<IParse> parseMethod) : tokenType(type), parseMethod(parseMethod) {}
        bool check(TokenType type)
        {
            return type == tokenType;
        }
        Node parseKeyword()
        {
            Node outNode = parseMethod->parseKeyword();
            return outNode;
        }
    };
public:
    static Node defineKeyword(token firstToken)
    {
        std::vector<Checker> checkers =
        {
            Checker(TokenType::PRINT, std::make_shared<ParsePrint>(token(TokenType::PRINT, "print"))),
            Checker(TokenType::IF, std::make_shared<ParseIf>(token(TokenType::IF, "if"))),
        };

        for(auto checker : checkers)
        {
            if(checker.check(firstToken.getType()))
            {
                return checker.parseKeyword();
            }
        }
        return parseExpression(firstToken);
    }
};

Node Parser::parse(std::vector<token> inputTokens)
{
    position = 0;
    if (inputTokens.empty())
    {
        throw std::runtime_error("Parse: Empty input tokens");
    }
    reporter.report(Reporter::INFO, "Parser: Parsing started");
    tokens = inputTokens;
    Node ast = SelectParser::defineKeyword(inputTokens.at(0));
    return ast;
}

void testParser()
{
    std::unique_ptr<Parser> parser = std::make_unique<Parser>();
    std::cout << "Start first test" << std::endl;
    std::vector<token> tokens = {token(TokenType::PRINT, "print"), token(TokenType::LEFT_PAREN, "("), token(TokenType::STRING, "Hello, world!"), token(TokenType::RIGHT_PAREN, ")"), token(TokenType::EOL, "")};
    //std::vector<token> tokens = {token(TokenType::ANY_NUMBER, "123"), token(TokenType::PLUS, "+"), token(TokenType::ANY_NUMBER, "321"), token(TokenType::ANY_NUMBER, "321"), token(TokenType::EOL, "")};
    Node node = parser->parse(tokens);
    if (node.getChildren().size() == 1)
    {
        reporter.printReports();
        node.printChildren();
        std::cout << "test1 passed :), " + std::to_string(node.getChildren().size()) << std::endl;
    }
    else
    {
        reporter.printReports();
        std::cout << "test1 failed: Unexpected number of nodes, " + std::to_string(node.getChildren().size()) + ", expected 1 :(" << std::endl;
        node.printChildren();
    }
}

int main()
{
    for (auto token : tokens)
    {
        std::cout << token.getValue() << std::endl;
    }
    testParser();
    return 0;
}

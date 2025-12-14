#include "../../head/parser.hpp"
#include "../../head/node.hpp"
#include "../../head/token.hpp"
#include "../../head/nodeCreator.hpp"
#include "../../head/reporter.hpp"
#include "../../head/algorithms/sillySearch.hpp"
#include "../convertor.cpp"
#include <cstdint>
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <iostream>

std::vector<TokenType> keywordsTypes =
{
    TokenType::PRINT,
    TokenType::IF,
};

Reporter reporter = Reporter();


std::vector<token> tokens;
unsigned int position = 0;

Node parseFactor(token inputToken);
Node parseTerm(token inputToken);
Node parseExpression(token inputToken);

class PositionManager
{
    static unsigned int position;

public:

    static void initPositionManager(unsigned int newPosition = 0)
    {
        position = newPosition;
    }

    static void advance(unsigned int& currentPosition = position)
    {
        currentPosition++;
    }

    static void resetPosition()
    {
        position = 0;
    }

    static unsigned int getPosition()
    {
        return position;
    }

    static void setPosition(unsigned int newPosition)
    {
        position = newPosition;
    }

    // TESTS
    static void testAdvance()
    {
        unsigned int testPosition = 0;
        advance(testPosition);
        std::string output = testPosition == 1 ? "passed" : "failed";
        std::cout << "AdvanceTest " << output << ", position: " << testPosition << std::endl;
    }
};

class TokensVectorManager
{
    static std::vector<token> tokens;
    static Reporter& reporter;
public:

    static void init(std::vector<token> tokens, Reporter& newReporter)
    {
        TokensVectorManager::tokens = tokens;
        TokensVectorManager::reporter = newReporter;
    }

    static token nextToken()
    {
        try
        {
            reporter.report(Reporter::INFO, "nextToken: Returning token: " + tokens.at(PositionManager::getPosition() + 1).getValue());
            return tokens.at(PositionManager::getPosition() + 1);
        }
        catch (std::out_of_range& error)
        {
            return token(TokenType::EOL, "eolNext");
        }
    }

    static token lastToken()
    {
        reporter.report(Reporter::INFO, "lastToken: Last token called, input position: " + std::to_string(PositionManager::getPosition()) + ", token: " + tokens.at(PositionManager::getPosition()).getValue());
        if (PositionManager::getPosition() == 0)
        {
            reporter.report(Reporter::ERROR, "lastToken: Position is 0, cannot return previous token");
            throw std::runtime_error("lastToken: Position is 0");
        }
        return tokens.at(PositionManager::getPosition() - 1);
    }

    static token peekToken()
    {
        try
        {
            return TokensVectorManager::peekToken();
        }
        catch (std::out_of_range& error)
        {
            throw std::runtime_error("peekToken: out of range");
        }
    }
};


bool match(token inputToken = TokensVectorManager::peekToken(), TokenType expectedToken = TokenType::EOL)
{
    if (inputToken.getType() == expectedToken)
    {
        PositionManager::advance();
        return true;
    }
    return false;
}

void eat(token inputToken = TokensVectorManager::peekToken(), TokenType expectedToken = TokenType::EOL, std::string message = "")
{
    try
    {
        reporter.report(Reporter::INFO, "eat: Eat called, message: " + message);
        if (inputToken.getType() == expectedToken || expectedToken == TokenType::EXPRESSION)
        {
            reporter.report(Reporter::INFO, "eat: Token matched, advancing position to " + std::to_string(PositionManager::getPosition() + 1) + ". Token is: " + tokens.at(PositionManager::getPosition() + 1).getValue());
            PositionManager::advance();
        }
        else
        {
            reporter.printReports();
            throw std::runtime_error(message + ", got: " + tokens.at(PositionManager::getPosition()).getValue() + ", position: " + std::to_string(PositionManager::getPosition()));
        }
    }
    catch (const std::exception& error)
    {
        reporter.printReports();

        throw std::runtime_error("eat: Exception caught: " + std::string(error.what()));
    }
}



class IParseFactor
{
    TokenType parsingTokenType;
public:
    bool isParsingToken(TokenType tokenType)
    {
        return tokenType == parsingTokenType;
    }

    TokenType getParsingTokenType()
    {
        return parsingTokenType;
    }

    void setParsingTokenType(TokenType tokenType)
    {
        parsingTokenType = tokenType;
    }

    void parseCheckingToken();
    IParseFactor(TokenType tokenType) : parsingTokenType(tokenType) {}
};

Node parseFactorForNumber()
{
    eat(TokensVectorManager::peekToken(), TokenType::ANY_NUMBER, "Expected a number");
    Node numberNode = createNode(TokenType::ANY_NUMBER, TokensVectorManager::lastToken().getValue(), 1, OperationType::NOT_AN_OPERATION);
    return numberNode;
}



Node parseFactorForID()
{
    eat(TokensVectorManager::peekToken(), TokenType::ID, "Expected an identifier");
    Node identifierNode = createNode(TokenType::ID, TokensVectorManager::lastToken().getValue(), 1, OperationType::NOT_AN_OPERATION);
    return identifierNode;
}

Node parseFactorForLeftParen()
{
    Node outNode = createNode(TokenType::EXPRESSION);

    eat(TokensVectorManager::peekToken(), TokenType::LEFT_PAREN, "Expected a left parenthesis");
    outNode.addChild(createNode(TokenType::LEFT_PAREN, "(", 1, OperationType::NOT_AN_OPERATION));

    eat(TokensVectorManager::peekToken(), TokenType::EXPRESSION, "Expected an expression");
    outNode = parseExpression(TokensVectorManager::lastToken());

    eat(TokensVectorManager::peekToken(), TokenType::RIGHT_PAREN, "Expected a right parenthesis");
    outNode.addChild(createNode(TokenType::RIGHT_PAREN, ")", 1, OperationType::NOT_AN_OPERATION));

    return outNode;
}


class IsFactorTokenOfThisClass
{
    TokenType tokenTypeOfThisClass;
    std::function<Node()> parseFunction;
public:

    IsFactorTokenOfThisClass(TokenType tokenTypeOfThisClass, std::function<Node()> parseFunction) : tokenTypeOfThisClass(tokenTypeOfThisClass), parseFunction(parseFunction) {}

    bool isFactorToken(token& inputToken)
    {
        return inputToken.getType() == tokenTypeOfThisClass;
    }

    std::function<Node()> getParseFunction()
    {
        return parseFunction;
    }
};

Node parseFactor(token inputToken)
{
    reporter.report(Reporter::INFO, "parseFactor: Parsing factor, token: " + inputToken.getValue());
    Node outNode = createNode(TokenType::EOL, "eol1", 0, OperationType::NOT_AN_OPERATION);

    std::vector<IsFactorTokenOfThisClass> isFactorTokensCheckers
    {
        IsFactorTokenOfThisClass(TokenType::ANY_NUMBER, parseFactorForNumber),
        IsFactorTokenOfThisClass(TokenType::ID, parseFactorForID),
        IsFactorTokenOfThisClass(TokenType::LEFT_PAREN, parseFactorForLeftParen),
    };
    reporter.report(Reporter::INFO, "parseFactor: Parsing factor, position: " + std::to_string(position) + ", token: " + inputToken.getValue() + ", position: " + std::to_string(position));
    for (auto& checker : isFactorTokensCheckers)
    {
        if (checker.isFactorToken(inputToken))
        {
            return checker.getParseFunction()();
        }
    }
    reporter.report(Reporter::ERROR, "parseFactor: Unexpected token: " + inputToken.getValue());
    throw std::runtime_error("ParseFactor: Unexpected token: " + inputToken.getValue());
}

Node parseTerm(token inputToken)
{

    Node outNode = createNode(TokenType::EOL, "eolParseTerm", 0, OperationType::NOT_AN_OPERATION);

    reporter.report(Reporter::INFO, "parseTerm: Parsing term, position: " + std::to_string(position));

    int precedence = 3;

    Node left = parseFactor(inputToken);

    reporter.report(Reporter::INFO, "parseTerm: Factor parsed, left: " + left.getValue());

    outNode = createNode(TokenType::EOL, "eol4", 0, OperationType::NOT_AN_OPERATION);

    bool isTerm = false;

    while (match(TokensVectorManager::peekToken(), TokenType::MUL) || match(TokensVectorManager::peekToken(), TokenType::DIV))
    {
        left = parseFactor(TokensVectorManager::lastToken());
        isTerm = true;
        token operation = TokensVectorManager::lastToken();
        Node right = parseFactor(TokensVectorManager::peekToken());
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
    while (match(TokensVectorManager::peekToken(), TokenType::PLUS) || match(TokensVectorManager::peekToken(), TokenType::MINUS))
    {
        left = parseTerm(TokensVectorManager::lastToken());
        reporter.report(Reporter::INFO, "parseExpression: Parsing binary operation");
        token operation = TokensVectorManager::lastToken();
        reporter.report(Reporter::INFO, "parseExpression: operation: " + operation.getValue());
        Node right = parseTerm(TokensVectorManager::peekToken());
        reporter.report(Reporter::INFO, "parseExpression: right: " + right.getValue());

        outNode = createNode(operation.getType(), operation.getValue(), 0, OperationType::BINARY);
        outNode.addChild(left);
        outNode.addChild(right);
    }

    if (outNode.getType() == TokenType::EOL)
    {
        if (TokensVectorManager::nextToken().getType() == TokenType::EOL)
        {
            reporter.report(Reporter::ERROR, "left: " + left.getValue());
            return left;
        }
        reporter.report(Reporter::WARNING, "parseNewToken: " + TokensVectorManager::nextToken().getValue());
        PositionManager::advance();
        left.addChild(parseTerm(TokensVectorManager::peekToken()));
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
    Node expressionNode = parseExpression(TokensVectorManager::peekToken());
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
        eat(TokensVectorManager::peekToken(), TokensVectorManager::peekToken().getType(), "Expected 'print' keyword");
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
        eat(TokensVectorManager::peekToken(), TokensVectorManager::peekToken().getType(), "Expected 'if' keyword");
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
    struct IsThisParseMethod
    {
        TokenType tokenType;
        std::shared_ptr<IParse> parseMethod = nullptr;
    public:
        IsThisParseMethod(TokenType type, std::shared_ptr<IParse> parseMethod) : tokenType(type), parseMethod(parseMethod) {}
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
        std::vector<IsThisParseMethod> checkers =
        {
            IsThisParseMethod(TokenType::PRINT, std::make_shared<ParsePrint>(token(TokenType::PRINT, "print"))),
            IsThisParseMethod(TokenType::IF, std::make_shared<ParseIf>(token(TokenType::IF, "if"))),
        };

        for(auto& checker : checkers)
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
    Parser parser = Parser();
    std::cout << "Start first test" << std::endl;
    //std::vector<token> tokens = {token(TokenType::PRINT, "print"), token(TokenType::LEFT_PAREN, "("), token(TokenType::STRING, "Hello, world!"), token(TokenType::RIGHT_PAREN, ")"), token(TokenType::EOL, "")};
    std::vector<token> tokens = {token(TokenType::ANY_NUMBER, "123"), token(TokenType::PLUS, "+"), token(TokenType::ANY_NUMBER, "321"), token(TokenType::ANY_NUMBER, "321"), token(TokenType::EOL, "")};
    Node node = parser.parse(tokens);
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

class TestsManager
{
    static std::vector<std::function<void()>> tests;
public:

    static void initTests()
    {
        registerTest(PositionManager::testAdvance);
        registerTest(testParser);
    }

    static void registerTest(std::function<void()> testFunction)
    {
        tests.push_back(testFunction);
    }

    static void runTests()
    {
        for (auto test : tests)
        {
            test();
        }
    }
};


int main()
{
    PositionManager::initPositionManager();


    const int8_t OK_CODE = 0;

    for (auto token : tokens)
    {
        std::cout << token.getValue() << std::endl;
    }

    TestsManager::initTests();
    TestsManager::runTests();

    return OK_CODE;
}

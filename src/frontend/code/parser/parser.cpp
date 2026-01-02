#include "../../head/parser/parser.hpp"
#include "../../head/node.hpp"
#include "../../head/token.hpp"
#include "../../head/parser/nodeCreator.hpp"
#include "../../head/reporter.hpp"
#include "../../head/parser/reporterHolder.hpp"
#include <cstdint>
#include <exception>
#include <functional>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>
#include <iostream>

std::shared_ptr<Node> parseFactor(token inputToken, Reporter& reporter = ReporterHolderForParser::getReporter());
std::shared_ptr<Node> parseTerm(token inputToken, Reporter& reporter = ReporterHolderForParser::getReporter());
std::shared_ptr<Node> parseExpression(token inputToken, Reporter& reporter = ReporterHolderForParser::getReporter());

class PositionManager
{
    static inline unsigned int position = 0;

public:

    static bool isPositionValid()
    {
        return position;
    }

    static void initializePosition(unsigned int startPosition = 0)
    {
        setPosition(startPosition);
    }

    static void initPositionManager(unsigned int newPosition = 0)
    {
        setPosition(newPosition);
    }

    static void advance(unsigned int& currentPosition = position, Reporter& reporter = ReporterHolderForParser::getReporter())
    {
        try
        {
            currentPosition++;
        }
        catch (const std::exception& error)
        {
            reporter.report(Reporter::ERROR, "Error advancing position: " + std::string(error.what()));
        }
    }

    static void resetPosition()
    {
        setPosition(0);
    }

    static unsigned int getPosition()
    {
        return PositionManager::position;
    }

    static void setPosition(unsigned int newPosition)
    {
        PositionManager::position = newPosition;
    }



    static void testAdvance(Reporter& reporter = ReporterHolderForParser::getReporter())
    {
        unsigned int testPosition = 0;
        advance(testPosition);
        std::string output = (testPosition == 1 ? "passed" : "failed");
        reporter.report(Reporter::INFO, "AdvanceTest " + output + ", position: " + std::to_string(testPosition));
        resetPosition();
        reporter.report(Reporter::INFO, "ResetPositionTest, position: " + std::to_string(getPosition()));
    }
};

class TokensVectorManager
{
    static inline std::vector<token> tokens;
    static inline Reporter& reporter = ReporterHolderForParser::getReporter();
public:

    static void init(std::vector<token> tokens, const Reporter& reporter)
    {
        TokensVectorManager::tokens = tokens;
        TokensVectorManager::reporter = reporter;
    }

    static std::vector<token> getTokens()
    {
        return tokens;
    }

    static void setTokens(std::vector<token> tokens)
    {
        TokensVectorManager::tokens = tokens;
    }

    static void resetTokens()
    {
        TokensVectorManager::tokens.clear();
    }

    static token nextToken() noexcept
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
        try
        {
            reporter.report(Reporter::INFO, "lastToken: Last token called, input position: " + std::to_string(PositionManager::getPosition()) + ", token: " + tokens.at(PositionManager::getPosition()).getValue());
            if (PositionManager::getPosition() == 0)
            {
                reporter.report(Reporter::ERROR, "lastToken: Position is 0, cannot return previous token");
                throw std::runtime_error("lastToken: Position is 0");
            }
            return tokens.at(PositionManager::getPosition() - 1);

        }
        catch (const std::exception&  error)
        {
            reporter.report(Reporter::ERROR, "lastToken: exception caught: " + std::string(error.what()));
            return tokens.at(PositionManager::getPosition() - 1);
        }
    }

    static token peekToken()
    {
        try
        {
            reporter.report(Reporter::INFO, "peekToken: Peek token called, input position: " + std::to_string(PositionManager::getPosition()) + ", token: " + tokens.at(PositionManager::getPosition()).getValue());

            return getTokens().at(PositionManager::getPosition());
        }
        catch (std::out_of_range& error)
        {
            //throw std::runtime_error("peekToken: Failed to retrieve token");
            return token(TokenType::EOL, "peekEOL");
        }
    }
};


bool match(token inputToken = TokensVectorManager::peekToken(), TokenType expectedToken = TokenType::EOL)
{
    try
    {
        if (inputToken.getType() == expectedToken)
        {
            PositionManager::advance();
            return true;
        }
    }
    catch (std::out_of_range& error)
    {
        ReporterHolderForParser::getReporter().printReports();
        throw std::runtime_error("match: out of range, position: " + std::to_string(PositionManager::getPosition()) + ", token: " + TokensVectorManager::getTokens().at(PositionManager::getPosition()).getValue() + ", vector size: " + std::to_string(TokensVectorManager::getTokens().size()));
    }
    return false;
}


void eat(token inputToken = TokensVectorManager::peekToken(), TokenType expectedType = TokenType::EOL, std::string message = "", Reporter& reporter = ReporterHolderForParser::getReporter())
{
    reporter.report(Reporter::INFO, "eat: Eat called, message: " + message + ", expecting: " + inputToken.getValue());

    try
    {
        token inputToken = TokensVectorManager::peekToken();

        reporter.report(Reporter::INFO, "eat: Current token for check: " + inputToken.getValue() + ", at position: " + std::to_string(PositionManager::getPosition()));

        if (inputToken.getType() == expectedType || expectedType == TokenType::EXPRESSION)
        {
            reporter.report(Reporter::INFO, "eat: Token matched: " + inputToken.getValue() + ". Advancing position.");
            PositionManager::advance();
        }
        else
        {
            reporter.report(Reporter::ERROR, "eat: Token mismatch! Expected: " + inputToken.getValue() + ", got: " + inputToken.getValue() + ", at position: " + std::to_string(PositionManager::getPosition()));
            reporter.printReports();
            throw std::runtime_error(message + ", exp type: " + std::to_string(static_cast<int>(expectedType)) + ", got: " + inputToken.getValue() + " type: " + std::to_string(static_cast<int>(inputToken.getType())) + ", position: " + std::to_string(PositionManager::getPosition()));
        }
    }
    catch (const std::exception& error)
    {
        reporter.printReports();
        throw std::runtime_error("eat: Exception caught: " + std::string(error.what()));
    }
}



void eatTest(Reporter& reporter = ReporterHolderForParser::getReporter())
{
    TokensVectorManager::setTokens({token(TokenType::ANY_NUMBER, "12345"), token(TokenType::PLUS, "+"), token(TokenType::ANY_NUMBER, "67890"), token(TokenType::EOL, "TestEat eol")});
    eat(token(TokenType::ANY_NUMBER, "12345"), TokenType::ANY_NUMBER, "Expected a number", reporter);
    TokensVectorManager::resetTokens();
}


std::shared_ptr<Node> parseFactorForNumber(Reporter& reporter = ReporterHolderForParser::getReporter())
{
    try
    {
        reporter.report(Reporter::INFO, "parseFactorForNumber: Parsing factor for number, position: " + std::to_string(PositionManager::getPosition()));
        eat(TokensVectorManager::peekToken(), TokenType::ANY_NUMBER, "Expected a number");
        std::shared_ptr<Node> numberNode = std::make_shared<Node>(Node(TokenType::ANY_NUMBER, TokensVectorManager::lastToken().getValue(), 1, {}, std::weak_ptr<Node>(),OperationType::NOT_AN_OPERATION));
        return numberNode;
    }
    catch (const std::exception& error)
    {
        reporter.printReports();
        reporter.report(Reporter::ERROR, "TokensVectorManager: " + std::to_string(TokensVectorManager::getTokens().size()));
        throw std::runtime_error("parseFactorForNumber: Exception caught: " + std::string(error.what()) + " token: " + TokensVectorManager::peekToken().getValue() + " position: " + std::to_string(PositionManager::getPosition()));
    }
}

std::shared_ptr<Node> parseFactorForString(Reporter& reporter = ReporterHolderForParser::getReporter())
{
    try
    {
        reporter.report(Reporter::INFO, "parseFactorForString: Parsing factor for string, position: " + std::to_string(PositionManager::getPosition()));
        eat(TokensVectorManager::peekToken(), TokenType::STRING, "Expected a string");
        std::shared_ptr<Node> numberNode = std::make_shared<Node>(Node(TokenType::STRING, TokensVectorManager::lastToken().getValue(), 1, {}, std::weak_ptr<Node>(),OperationType::NOT_AN_OPERATION));
        return numberNode;
    }
    catch (const std::exception& error)
    {
        reporter.printReports();
        reporter.report(Reporter::ERROR, "TokensVectorManager: " + std::to_string(TokensVectorManager::getTokens().size()));
        throw std::runtime_error("parseFactorForString: Exception caught: " + std::string(error.what()) + " token: " + TokensVectorManager::peekToken().getValue() + " position: " + std::to_string(PositionManager::getPosition()));
    }
}



std::shared_ptr<Node> parseFactorForID(Reporter& reporter = ReporterHolderForParser::getReporter())
{
    reporter.report(Reporter::INFO, "parseFactorForID: Parsing factor for identifier, position: " + std::to_string(PositionManager::getPosition()));
    eat(TokensVectorManager::peekToken(), TokenType::ID, "Expected an identifier");
    std::shared_ptr<Node> identifierNode = createNode(TokenType::ID, TokensVectorManager::lastToken().getValue(), 1, OperationType::NOT_AN_OPERATION);
    return identifierNode;
}

std::shared_ptr<Node> parseFactorForLeftParen(Reporter& reporter = ReporterHolderForParser::getReporter())
{
    try
    {
        std::shared_ptr<Node> outNode = createNode(TokenType::EXPRESSION);

        eat(TokensVectorManager::peekToken(), TokenType::LEFT_PAREN, "Expected a left parenthesis");
        outNode->addChild(createNode(TokenType::LEFT_PAREN, "(", 1, OperationType::NOT_AN_OPERATION));

        eat(TokensVectorManager::peekToken(), TokenType::EXPRESSION, "Expected an expression");
        outNode = parseExpression(TokensVectorManager::lastToken());

        eat(TokensVectorManager::peekToken(), TokenType::RIGHT_PAREN, "Expected a right parenthesis");
        outNode->addChild(createNode(TokenType::RIGHT_PAREN, ")", 1, OperationType::NOT_AN_OPERATION));

        return outNode;
    }
    catch (const std::exception& error)
    {
        reporter.report(Reporter::ERROR, "Error parsing factor for left parenthesis: " + std::string(error.what()));
        throw std::runtime_error("Error parsing factor for left parenthesis, error: " + std::string(error.what()));
    }
}


class IsFactorTokenOfThisClass
{
    TokenType tokenTypeOfThisClass;
    std::function<std::shared_ptr<Node>(Reporter&)> parseFunction;
public:

    IsFactorTokenOfThisClass(TokenType tokenTypeOfThisClass, std::function<std::shared_ptr<Node>(Reporter&)> parseFunction) : tokenTypeOfThisClass(tokenTypeOfThisClass), parseFunction(parseFunction) {}

    bool isFactorToken(token& inputToken, Reporter& reporter = ReporterHolderForParser::getReporter())
    {
        reporter.report(Reporter::INFO, "parseFactor: Parsing factor, token: " + inputToken.getValue());
        return inputToken.getType() == tokenTypeOfThisClass;
    }

    std::function<std::shared_ptr<Node>(Reporter&)> getParseFunction()
    {
        return parseFunction;
    }
};

std::shared_ptr<Node> parseFactor(token inputToken = TokensVectorManager::peekToken(), Reporter& reporter)
{
    try
    {
       if (inputToken.getType() == TokenType::EOL)
       {
           reporter.report(Reporter::INFO, "parseFactor: Parsing factor, token: EOL");
           return createNode(TokenType::EOL, "EOLfactor");
       }

        reporter.report(Reporter::INFO, "parseFactor: Parsing factor, token: " + inputToken.getValue());
        std::vector<IsFactorTokenOfThisClass> isFactorTokensCheckers =
        {
            IsFactorTokenOfThisClass(TokenType::ANY_NUMBER, parseFactorForNumber),
            IsFactorTokenOfThisClass(TokenType::ID, parseFactorForID),
            IsFactorTokenOfThisClass(TokenType::LEFT_PAREN, parseFactorForLeftParen),
            IsFactorTokenOfThisClass(TokenType::STRING, parseFactorForString),
        };

        reporter.report(Reporter::INFO, "parseFactor: Parsing factor, position: " + std::to_string(PositionManager::getPosition()) + ", token: " + inputToken.getValue() + ", position: " + std::to_string(PositionManager::getPosition()));
        for (auto& checker : isFactorTokensCheckers)
        {
            reporter.report(Reporter::INFO, "parseFactor: Parsing factor, position: " + std::to_string(PositionManager::getPosition()) + ", token: " + inputToken.getValue() + ", position: " + std::to_string(PositionManager::getPosition()));
            if (checker.isFactorToken(inputToken))
            {
                return checker.getParseFunction()(ReporterHolderForParser::getReporter());
            }
        }
        reporter.report(Reporter::ERROR, "parseFactor: Unexpected token, value: " + inputToken.getValue());
        throw std::runtime_error("ParseeeFactor: Unexpected token: " + inputToken.getValue());
    }
    catch (const std::exception& error)
    {
        reporter.report(Reporter::ERROR, "parseFactor: Unexpected token: " + inputToken.getValue());
        throw std::runtime_error("ParssseFactor error: " + std::string(error.what()));
    }
}

std::shared_ptr<Node> parseTerm(token inputToken, Reporter& reporter)
{
    try
    {
        std::shared_ptr<Node> outNode = parseFactor(inputToken);

        reporter.report(Reporter::INFO, "parseTerm: Parsing term, position: " + std::to_string(PositionManager::getPosition()));

        int precedence = 3;
        std::shared_ptr<Node> operation = createNode(TokenType::EOL, "eolParseTerm", 0, OperationType::NOT_AN_OPERATION);

        reporter.report(Reporter::INFO, "parseTerm: Factor parsedd, outNode: " + outNode->getValue());

        while (TokensVectorManager::peekToken().getType() == TokenType::MUL || TokensVectorManager::peekToken().getType() == TokenType::DIV)
        {
            reporter.report(Reporter::INFO, "parseTerm: Parsing term, position: " + std::to_string(PositionManager::getPosition()) + " mul!");
            std::shared_ptr<Node> left = outNode;
            operation = createNode(TokensVectorManager::lastToken().getType(), TokensVectorManager::peekToken().getValue(), precedence, OperationType::BINARY);
            std::shared_ptr<Node> right = parseFactor(TokensVectorManager::peekToken());

            outNode = std::make_shared<Node>(Node(operation->getType(), operation->getValue(), 0, {}, std::weak_ptr<Node>(), OperationType::BINARY));
            outNode->addChild(left);
            outNode->addChild(right);

            left->setParent(outNode);
            right->setParent(outNode);
        }

        reporter.report(Reporter::INFO, "parseTerm: Term parsed, outNode: " + outNode->getValue());

        return outNode;
    }
    catch (const std::exception& error)
    {
        reporter.report(Reporter::ERROR, "parseTerm: Error parsing term, exception: " + std::string(error.what()));
        reporter.printReports();
        throw std::runtime_error("pparseTerm: Error parsing term, position: " + std::to_string(PositionManager::getPosition()) + ", error: " + std::string(error.what()));
    }
}



std::shared_ptr<Node> parseExpression(token inputToken = TokensVectorManager::peekToken(), Reporter& reporter)
{
    try
    {
        reporter.report(Reporter::INFO, "parseExpression: Expression parsing started, inputToken: " + inputToken.getValue());

        std::shared_ptr<Node> outNode = parseTerm(inputToken);

        reporter.report(Reporter::INFO, "1parseExpression: Term parsed, Node: " + outNode->getValue());

        reporter.report(Reporter::INFO, "parseExpression: Parsing term");
        while (TokensVectorManager::peekToken().getType() == TokenType::PLUS || TokensVectorManager::peekToken().getType() == TokenType::MINUS)
        {
            std::shared_ptr<Node> left = outNode;
            reporter.report(Reporter::INFO, "parseExpression: left: " + left->getValue());
            reporter.report(Reporter::INFO, "parseExpression: Parsing binary operation");

            token operation = TokensVectorManager::peekToken();
            PositionManager::advance();
            reporter.report(Reporter::INFO, "parseExpression: operation: " + operation.getValue());

            std::shared_ptr<Node> right = parseTerm(TokensVectorManager::peekToken());
            reporter.report(Reporter::INFO, "parseExpression: right: " + right->getValue());

            outNode = std::make_shared<Node>(Node(operation.getType(), operation.getValue(), 0, {}, std::weak_ptr<Node>(), OperationType::BINARY));
            outNode->addChild(left);
            outNode->addChild(right);

            left->setParent(std::weak_ptr<Node>(outNode));
            right->setParent(std::weak_ptr<Node>(outNode));
        }
        reporter.report(Reporter::INFO, "parseExpression: outNode: " + outNode->getValue());
        return outNode;
    }
    catch (const std::exception& error)
    {
        reporter.report(Reporter::ERROR, "parseExpression: " + std::string(error.what()));
        throw std::runtime_error("ParseExpression error: " + std::string(error.what()));
    }
}

std::shared_ptr<Node> defineKeyword(token firstToken, Reporter& reporter);

class IParse
{
public:
    IParse() = default;
    virtual std::shared_ptr<Node> parseKeyword(Reporter& reporter = ReporterHolderForParser::getReporter()) = 0;
    virtual ~IParse() = default;
};

class ParsePrint : public IParse
{
    token parsingToken = token(TokenType::PRINT, "print");
    Reporter& reporter;
public:
    ParsePrint(Reporter& reporter = ReporterHolderForParser::getReporter()) : reporter((reporter)) {}
    std::shared_ptr<Node> parseKeyword(Reporter& reporter) override
    {
        reporter.report(Reporter::INFO, "defineKeyword: PRINT keyword found");
        eat(parsingToken, TokensVectorManager::peekToken().getType(), "Expected 'print' keyword");
        std::shared_ptr<Node> printKeyword  = std::make_shared<Node>(Node(parsingToken.getType(), parsingToken.getValue(), 1, {}, std::nullopt, OperationType::KEYWORD));
        eat(TokensVectorManager::peekToken(), TokenType::LEFT_PAREN);

        std::shared_ptr<Node> out = createNode(TokenType::EXPRESSION);
        out->addChild(createNode(TokenType::LEFT_PAREN, "("));

        out->addChild(defineKeyword(TokensVectorManager::peekToken(), ReporterHolderForParser::getReporter()));

        eat(TokensVectorManager::peekToken(), TokenType::RIGHT_PAREN);
        out->addChild(createNode(TokenType::RIGHT_PAREN, ")"));
        out->setParent(printKeyword);
        printKeyword->addChild(out);
        return printKeyword;
    }
};

class ParseIf : public IParse
{
    token parsingToken = token(TokenType::IF, "if");
public:
    std::shared_ptr<Node> parseKeyword(Reporter& reporter) override
    {
        reporter.report(Reporter::INFO, "defineKeyword: IF keyword found");
        eat(TokensVectorManager::peekToken(), parsingToken.getType(), "Expected 'if' keyword");
        std::shared_ptr<Node> ifKeyword  = std::make_shared<Node>(Node(parsingToken.getType(), parsingToken.getValue(), 1, {}, std::nullopt, OperationType::KEYWORD));
        std::shared_ptr<Node> out = defineKeyword(TokensVectorManager::peekToken(), ReporterHolderForParser::getReporter());
        out->setParent(ifKeyword);
        ifKeyword->addChild(out);
        return ifKeyword;
    }
};

class ParseVar : public IParse
{
    token parsingToken = token(TokenType::VAR, "var");
public:
    std::shared_ptr<Node> parseKeyword(Reporter& reporter) override
    {
        reporter.report(Reporter::INFO, "defineKeyword: var keyword found");
        eat(TokensVectorManager::peekToken(), parsingToken.getType(), "Expected 'var' keyword");
        std::shared_ptr<Node> varKeyword  = std::make_shared<Node>(Node(parsingToken.getType(), parsingToken.getValue(), 1, {}, std::nullopt, OperationType::KEYWORD));

        eat(TokensVectorManager::peekToken(), TokenType::ID);
        std::shared_ptr<Node> name = createNode(TokensVectorManager::lastToken().getType(), TokensVectorManager::lastToken().getValue());
        std::shared_ptr<Node> colon = nullptr;

        if (match(TokensVectorManager::peekToken(), TokenType::COLON))
        {
            colon = createNode(TokenType::COLON, ":");

            varKeyword->addChild(colon);

            colon->addChild(name);
            colon->addChild(createNode(TokensVectorManager::peekToken().getType(), TokensVectorManager::peekToken().getValue())); //type
            PositionManager::advance();
        }

        if (match(TokensVectorManager::peekToken(), TokenType::EQUALS))
        {
            std::shared_ptr<Node> equals = createNode(TokenType::EQUALS, "=");
            if (colon != nullptr)
            {
                equals->addChild(colon);
            }
            else
            {
                equals->addChild(name);
            }
            equals->addChild(defineKeyword(TokensVectorManager::peekToken(), ReporterHolderForParser::getReporter())); // value
            varKeyword->addChild(equals);
            PositionManager::advance();
        }

        return varKeyword;
    }
};

class ParseFunctionDeclaration : public IParse
{
    token parsingToken = token(TokenType::FUNC, "func");

    std::vector<TokenType> modificatorsTypes =
    {
        TokenType::PUBLIC,
        TokenType::PRIVATE,
        TokenType::OVERRIDE,
    };

    std::vector<std::shared_ptr<Node>> findModificatorsOfFunction(token startToken)
    {
        std::vector<std::shared_ptr<Node>> modificators;
        while
        (
            TokensVectorManager::peekToken().getType() == modificatorsTypes.at(0) ||
            TokensVectorManager::peekToken().getType() == modificatorsTypes.at(1) ||
            TokensVectorManager::peekToken().getType() == modificatorsTypes.at(2)
        )
        {
            for (auto& modificatorType : modificatorsTypes)
            {
                if (TokensVectorManager::peekToken().getType() == modificatorType)
                {
                    modificators.push_back(createNode(modificatorType, TokensVectorManager::peekToken().getValue()));
                    PositionManager::advance();
                }
            }
        }
        return modificators;
    }

    std::shared_ptr<Node> parseArgument()
    {
        eat(TokensVectorManager::peekToken(), TokenType::ID, "Expected name of argument");
        std::shared_ptr<Node> name = createNode(TokenType::ID, TokensVectorManager::lastToken().getValue());
        eat(TokensVectorManager::peekToken(), TokenType::COLON, "Expected ':' after argument name");
        std::shared_ptr<Node> colon = createNode(TokenType::COLON, TokensVectorManager::lastToken().getValue());
        colon->addChild(name);
        colon->addChild(createNode(TokenType::TYPE_NAME, TokensVectorManager::peekToken().getValue())); //Type
        PositionManager::advance();
        return colon;
    }
public:
    std::shared_ptr<Node> parseKeyword(Reporter& reporter = ReporterHolderForParser::getReporter()) override
    {
        std::vector<std::shared_ptr<Node>> modificators = findModificatorsOfFunction(TokensVectorManager::peekToken());
        std::shared_ptr<Node> funcKeyword = createNode(parsingToken.getType(), parsingToken.getValue());
        eat(TokensVectorManager::peekToken(), TokenType::FUNC, "Expected 'func' keyword");


        eat(TokensVectorManager::peekToken(), TokenType::ID, "Expected name of funtion");
        std::shared_ptr<Node> funcName = createNode(TokenType::ID, TokensVectorManager::lastToken().getValue());

        std::shared_ptr<Node> arguments = createNode(TokenType::ARGUMENTS, "");
        funcName->addChild(arguments);

        eat(TokensVectorManager::peekToken(), TokenType::LEFT_PAREN, "Expected '(' after function name");
        arguments->addChild(createNode(TokenType::LEFT_PAREN, TokensVectorManager::lastToken().getValue()));

        while (TokensVectorManager::peekToken().getType() == TokenType::ID)
        {
            std::shared_ptr<Node> argument = parseArgument();
            arguments->addChild(argument);
        }

        eat(TokensVectorManager::peekToken(), TokenType::RIGHT_PAREN, "Expected ')' after function name");
        arguments->addChild(createNode(TokenType::LEFT_PAREN, ")"));

        if (TokensVectorManager::peekToken().getType() == TokenType::RETURNING_TYPE_OPERATOR)
        {
            eat(TokensVectorManager::peekToken(), TokenType::RETURNING_TYPE_OPERATOR, "Expected '->' after function name");
            std::shared_ptr<Node> returningTypeOperator = createNode(TokenType::RETURNING_TYPE_OPERATOR, "->");
            std::shared_ptr<Node> returnType = createNode(TokenType::TYPE_ID, TokensVectorManager::lastToken().getValue());
            PositionManager::advance();

            funcKeyword->addChild(funcName);
            funcName->addChild(returnType);
            funcKeyword->addChild(returningTypeOperator);
        }
        else
        {
            funcKeyword->addChild(funcName);
        }

        return funcKeyword;
    }
};



class IsThisParseMethod
{
    TokenType tokenType;
    std::shared_ptr<IParse> parseMethod = nullptr;
public:
    IsThisParseMethod(TokenType type, std::shared_ptr<IParse> parseMethod) : tokenType(type), parseMethod(parseMethod) {}
    bool check(TokenType type)
    {
        return type == tokenType;
    }
    std::shared_ptr<Node> parseKeyword()
    {
        std::shared_ptr<Node> outNode = parseMethod->parseKeyword();
        return outNode;
    }
};

std::shared_ptr<Node> defineKeyword(token firstToken, Reporter& reporter = ReporterHolderForParser::getReporter())
{
    std::vector<IsThisParseMethod> checkers =
    {
        IsThisParseMethod(TokenType::PRINT, std::make_shared<ParsePrint>()),
        IsThisParseMethod(TokenType::IF, std::make_shared<ParseIf>()),
        IsThisParseMethod(TokenType::VAR, std::make_shared<ParseVar>()),
        IsThisParseMethod(TokenType::FUNC, std::make_shared<ParseFunctionDeclaration>()),
    };

    for(auto& checker : checkers)
    {
        if(checker.check(firstToken.getType()))
        {
            reporter.report(Reporter::INFO, "Parser: Keyword defined");
            return checker.parseKeyword();
        }
    }
    return parseExpression(firstToken);
}

std::shared_ptr<Node> Parser::parse(std::vector<token> inputTokens, Reporter& reporter = ReporterHolderForParser::getReporter())
{
    PositionManager::setPosition(0);
    if (inputTokens.empty())
    {
        throw std::runtime_error("Parse: Empty input tokens");
    }
    reporter.report(Reporter::INFO, "Parser: Parsing started");
    TokensVectorManager::setTokens(inputTokens);
    reporter.report(Reporter::INFO, "Parser: Tokens setted");
    std::shared_ptr<Node> ast = defineKeyword(TokensVectorManager::getTokens().at(0));
    return ast;
}


void testParser(Reporter& reporter)
{
    Parser parser = Parser();
    std::cout << "Start first test" << std::endl;
    //std::vector<token> tokens = {token(TokenType::PRINT, "print"), token(TokenType::LEFT_PAREN, "("), token(TokenType::STRING, "Hello, world!"), token(TokenType::RIGHT_PAREN, ")")};
    //std::vector<token> tokens = {token(TokenType::ANY_NUMBER, "123"), token(TokenType::PLUS, "+"), token(TokenType::ANY_NUMBER, "321")};
    //std::vector<token> tokens = {token(TokenType::IF, "if"), token(TokenType::ANY_NUMBER, "2"), token(TokenType::PLUS, "+"), token(TokenType::ANY_NUMBER, "2")};
    //std::vector<token> tokens = {token(TokenType::VAR, "var"), token(TokenType::ID, "lalala"), token(TokenType::EQUALS, "="), token(TokenType::ANY_NUMBER, "2")};
    std::vector<token> tokens = {token(TokenType::FUNC, "func"), token(TokenType::ID, "some"), token(TokenType::LEFT_PAREN, "("), token(TokenType::RIGHT_PAREN, ")")};
    TokensVectorManager::setTokens(tokens);
    std::shared_ptr<Node> node = parser.parse(tokens, reporter);

    for(auto& tokenValue : TokensVectorManager::getTokens())
    {
        std::cout << "Token: " << tokenValue.getType() << ", Value: " << tokenValue.getValue() << std::endl;
    }

    if (node->getChildren().size() > 0)
    {
        reporter.printReports();
        node->printChildren();
        std::cout << "test1 passed :), " + std::to_string(node->getChildren().size()) << std::endl;
    }
    else
    {
        reporter.printReports();
        //node->printChildren();
        std::cout << "test1 failed: Node: " + node->getValue() + " unexpected number of nodes, " + std::to_string(node->getChildren().size()) + ", expected 1 :(" << std::endl;

        if(node->getParent().has_value())
        {
            std::cout << "Node's parent: " + node->getParent().value().lock()->getValue() << std::endl;
        }
        else
        {
            std::cout << "Node has no parent" << std::endl;
        }
    }
    TokensVectorManager::resetTokens();
}

class TestsManager
{
    static inline std::vector<std::function<void(Reporter&)>> tests = {};
    static inline unsigned int testID = 0;
public:

    static void initTests()
    {
        registerTest(PositionManager::testAdvance);
        registerTest(eatTest);

        registerTest(testParser);
    }

    static void registerTest(std::function<void(Reporter&)> testFunction)
    {
        tests.push_back(testFunction);
    }

    static void runTests(Reporter& reporter)
    {
        try
        {
            for (auto& test : tests)
            {
                test(reporter);
                testID++;
            }
        }
        catch (const std::exception& error)
        {
            throw std::runtime_error("Run tests, test ID: " + std::to_string(testID) + ", error: " + std::string(error.what()));
        }

    }
};


int main()
{
    const int8_t OK_CODE = 0;

    Reporter& reporter = ReporterHolderForParser::getReporter();

    PositionManager::initPositionManager();
    TestsManager::initTests();
    TestsManager::runTests(reporter);

    return OK_CODE;
}

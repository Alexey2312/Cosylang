#include "parser.cpp"

class TermParser
{
public:
    TermParser() {}

    Node parseTerm(token inputToken)
    {

        Node outNode = createNode(TokenType::EOL, "eolParseTerm", 0, OperationType::NOT_AN_OPERATION);

        reporter.report(Reporter::INFO, "parseTerm: Parsing term, position: " + std::to_string(position));

        int precedence = 3;

        Node left = parseFactor(inputToken);

        reporter.report(Reporter::INFO, "parseTerm: Factor parsed, left: " + left.getValue());

        outNode = createNode(TokenType::EOL, "eol4", 0, OperationType::NOT_AN_OPERATION);

        bool isTerm = false;


        while (TokensVectorManager::peekToken().getType() == TokenType::MUL || TokensVectorManager::peekToken().getType() == TokenType::DIV)
        {
            left = parseFactor(TokensVectorManager::peekToken());
            reporter.report(Reporter::INFO, "parseExpression: Parsing binary operation");
            token operation = TokensVectorManager::peekToken();
            reporter.report(Reporter::INFO, "parseExpression: operation: " + operation.getValue());
            Node right = parseFactor(TokensVectorManager::peekToken());
            reporter.report(Reporter::INFO, "parseExpression: right: " + right.getValue());

            outNode = createNode(operation.getType(), operation.getValue(), 0, OperationType::BINARY);
            outNode.addChild(left);
            outNode.addChild(right);
        }

        if (!isTerm)
        {
            return left;
        }

        reporter.report(Reporter::INFO, "parseTerm: Term parsed, outNode: " + outNode.getValue());

        return outNode;
    }

};

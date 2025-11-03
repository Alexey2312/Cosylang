#include <iostream>
#include <stdexcept>
#include <vector>
#include "../head/parser.hpp"
#include "../head/token.hpp"
#include "../head/node.hpp"

int main()
{
    std::string testingObject = "Parser";
    Parser* parser = new Parser();

    std::vector<TokenType> tokens = {ANY_NUMBER, PLUS, ANY_NUMBER, MINUS, ANY_NUMBER, PLUS, ANY_NUMBER};
    std::vector<Node*> nodes = parser->parse({token(ANY_NUMBER, "42"), token(PLUS, "+"), token(ANY_NUMBER, "52"), token(MINUS, "-"), token(ANY_NUMBER, "1488"), token(PLUS, "+"), token(ANY_NUMBER, "44")});

    for (unsigned int checkingNode = 0; checkingNode < nodes.size(); checkingNode++)
    {
        if(nodes.at(checkingNode)->getType() == tokens.at(checkingNode))
        {
            std::cout << testingObject + ": test 1 passed!" << std::endl;
        }
        else
        {
            std::runtime_error(testingObject + ": test 1 failed");
        }
    }

    if (nodes.at(1)->getChildren().size() >= 1)
    {
        std::cout << testingObject + ": test 2 passed!" << std::endl;
    }
    else
    {
        std::runtime_error(testingObject + ": test 2 failed");
    }

    delete parser;
    return 0;
}

#include "token.hpp"
#include <string>
#include <vector>



class Lexer
{
    public: std::vector<token> tokenize(std::string line);
};

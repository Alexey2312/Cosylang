#pragma once

#include <cstddef>
#include <vector>
#include "node/node.hpp"
#include "../../utills/arena-allocator.hpp"
#include "../../diagnostics/error-reporter.hpp"

namespace Cosylang::Parser
{

class Parser
{
private:
    std::vector<Lexer::Token::Token>& tokens;
    size_t cursor = 0;
    Arena& arena;
    Reporter::DiagnosticEngine& err;

    size_t file_id;

    inline int getInfixBindingPower(Lexer::Token::TokenType type);

    inline const Lexer::Token::Token* peek();
    inline const Lexer::Token::Token* previousToken();
    inline const Lexer::Token::Token* consume(Lexer::Token::TokenType type);
    inline const Lexer::Token::Token* consume(Lexer::Token::TokenType type, std::string error);
    inline const Lexer::Token::Token* consume();

    Node* parseVariableDeclaration();
    Node* parseConstantDeclaration();
    Node* parseFunctionDeclaration();
    Node* parseParams();
    Node* parseParam();
    Node* parseCodeBlock();
    Node* parseReturn();

    Node* parseIfStatement();
    Node* parseElifStatement();
    Node* parseElseStatement();

    Node* parseWhileStatement();
    Node* parseForStatement();

    Node* parseMatchStatement();

    Node* parseBodyDeclaration();
    Node* parseTypeDeclaration();
public:
    Node* parseStatement();
    Node* parseExpression(int left_binding_power);
    const Node* parse();

    Parser(std::vector<Lexer::Token::Token>& this_tokens, Arena& this_arena, size_t this_file, Reporter::DiagnosticEngine& this_diagnostic)
        : tokens(this_tokens), arena(this_arena), file_id(this_file), err(this_diagnostic) {}

};

} // namespace Cosylang::Parser

#ifndef PARSER_H
#define PARSER_H

#include "tokens.h"
#include <vector>
#include <memory>

class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual int evaluate() const = 0;
    virtual std::string toString() const = 0;
};

class NumberNode : public ASTNode {
private:
    int value;
public:
    NumberNode(int val);
    int evaluate() const override;
    std::string toString() const override;
};

class AdditionNode : public ASTNode {
private:
    std::shared_ptr<ASTNode> left;
    std::shared_ptr<ASTNode> right;
public:
    AdditionNode(std::shared_ptr<ASTNode> l, std::shared_ptr<ASTNode> r);
    int evaluate() const override;
    std::string toString() const override;
};

class ComparisonNode : public ASTNode {
public:
    enum class Op { LT, GT, EQ, NEQ };

private:
    std::shared_ptr<ASTNode> left;
    std::shared_ptr<ASTNode> right;
    Op operation;

public:
    ComparisonNode(std::shared_ptr<ASTNode> l, std::shared_ptr<ASTNode> r, Op op);
    int evaluate() const override;
    std::string toString() const override;
};

class Parser {
private:
    std::vector<Token> tokens;
    size_t currentPos;

    Token currentToken() const;
    Token peekToken() const;
    void advance();
    void expect(TokenType type, const std::string& errorMsg);

    // Функции разбора для каждого нетерминала
    std::shared_ptr<ASTNode> parseExpression();
    std::shared_ptr<ASTNode> parseAddition();
    ComparisonNode::Op parseOperation();

public:
    Parser(const std::vector<Token>& tokens);
    std::shared_ptr<ASTNode> parse();
};

#endif
#include "parser.h"
#include <sstream>
#include <stdexcept>

NumberNode::NumberNode(int val) : value(val) {}

int NumberNode::evaluate() const {
    return value;
}

std::string NumberNode::toString() const {
    return std::to_string(value);
}

AdditionNode::AdditionNode(std::shared_ptr<ASTNode> l, std::shared_ptr<ASTNode> r)
    : left(l), right(r) {
}

int AdditionNode::evaluate() const {
    return left->evaluate() + right->evaluate();
}

std::string AdditionNode::toString() const {
    return "(" + left->toString() + " + " + right->toString() + ")";
}

ComparisonNode::ComparisonNode(std::shared_ptr<ASTNode> l, std::shared_ptr<ASTNode> r, Op op)
    : left(l), right(r), operation(op) {
}

int ComparisonNode::evaluate() const {
    int leftVal = left->evaluate();
    int rightVal = right->evaluate();

    switch (operation) {
    case Op::LT: return leftVal < rightVal;
    case Op::GT: return leftVal > rightVal;
    case Op::EQ: return leftVal == rightVal;
    case Op::NEQ: return leftVal != rightVal;
    default: throw SemanticError("Неизвестная операция сравнения");
    }
}

std::string ComparisonNode::toString() const {
    static const std::map<Op, std::string> opStrings = {
        {Op::LT, "<"},
        {Op::GT, ">"},
        {Op::EQ, "="},
        {Op::NEQ, "!="}
    };

    return "(" + left->toString() + " " + opStrings.at(operation) +
        " " + right->toString() + ")";
}

Parser::Parser(const std::vector<Token>& tokens)
    : tokens(tokens), currentPos(0) {
}

Token Parser::currentToken() const {
    if (currentPos < tokens.size()) {
        return tokens[currentPos];
    }
    return Token(TokenType::END, "", tokens.empty() ? 0 : tokens.back().position);
}

Token Parser::peekToken() const {
    if (currentPos + 1 < tokens.size()) {
        return tokens[currentPos + 1];
    }
    return Token(TokenType::END, "", tokens.empty() ? 0 : tokens.back().position);
}

void Parser::advance() {
    if (currentPos < tokens.size()) {
        currentPos++;
    }
}

void Parser::expect(TokenType type, const std::string& errorMsg) {
    Token token = currentToken();
    if (token.type != type) {
        throw ParserError(errorMsg, token);
    }
    advance();
}

// Парсинг операции сравнения
ComparisonNode::Op Parser::parseOperation() {
    Token token = currentToken();

    switch (token.type) {
    case TokenType::LT:
        advance();
        return ComparisonNode::Op::LT;
    case TokenType::GT:
        advance();
        return ComparisonNode::Op::GT;
    case TokenType::EQ:
        advance();
        return ComparisonNode::Op::EQ;
    case TokenType::NEQ:
        advance();
        return ComparisonNode::Op::NEQ;
    default:
        throw ParserError("Ожидается операция сравнения (<, >, =, !=)", token);
    }
}

// Парсинг <Сложение>
std::shared_ptr<ASTNode> Parser::parseAddition() {
    // Обрабатываем первое число
    expect(TokenType::INTEGER, "Ожидается число");
    int num = std::stoi(tokens[currentPos - 1].value);
    std::shared_ptr<ASTNode> left = std::make_shared<NumberNode>(num);

    // Рекурсивно обрабатываем дополнительные слагаемые
    while (currentToken().type == TokenType::PLUS) {
        advance(); // Пропускаем '+'

        expect(TokenType::INTEGER, "Ожидается число после '+'");
        int nextNum = std::stoi(tokens[currentPos - 1].value);
        std::shared_ptr<ASTNode> right = std::make_shared<NumberNode>(nextNum);

        // Создаем узел сложения
        left = std::make_shared<AdditionNode>(left, right);
    }

    return left;
}

// Парсинг <Выражение>
std::shared_ptr<ASTNode> Parser::parseExpression() {
    // Левое сложение
    auto left = parseAddition();

    // Операция
    ComparisonNode::Op op = parseOperation();

    // Правое сложение
    auto right = parseAddition();

    // Проверяем, что достигнут конец
    if (currentToken().type != TokenType::END) {
        throw ParserError("Лишние токены в конце выражения", currentToken());
    }

    return std::make_shared<ComparisonNode>(left, right, op);
}

std::shared_ptr<ASTNode> Parser::parse() {
    if (tokens.empty() || (tokens.size() == 1 && tokens[0].type == TokenType::END)) {
        throw ParserError("Пустое выражение", Token(TokenType::END, "", 0));
    }

    return parseExpression();
}
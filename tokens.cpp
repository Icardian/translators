#include "tokens.h"

// Реализация Token
Token::Token(TokenType t, const std::string& v, int pos)
    : type(t), value(v), position(pos) {
}

std::string Token::toString() const {
    static const std::map<TokenType, std::string> typeNames = {
        {TokenType::INTEGER, "INTEGER"},
        {TokenType::PLUS, "PLUS"},
        {TokenType::LT, "LT"},
        {TokenType::GT, "GT"},
        {TokenType::EQ, "EQ"},
        {TokenType::NEQ, "NEQ"},
        {TokenType::END, "END"}
    };

    if (value.empty()) {
        return typeNames.at(type);
    }
    return typeNames.at(type) + "('" + value + "')";
}

// Реализация исключений
LexerError::LexerError(const std::string& msg, int pos)
    : std::runtime_error("Лексическая ошибка в позиции " +
        std::to_string(pos) + ": " + msg),
    position(pos) {
}

ParserError::ParserError(const std::string& msg, const Token& t)
    : std::runtime_error("Синтаксическая ошибка в позиции " +
        std::to_string(t.position) +
        " (токен: " + t.toString() + "): " + msg),
    position(t.position), token(t) {
}

SemanticError::SemanticError(const std::string& msg)
    : std::runtime_error("Семантическая ошибка: " + msg) {}
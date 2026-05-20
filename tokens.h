#ifndef TOKENS_H
#define TOKENS_H

#include <string>
#include <map>
#include <stdexcept>
#include <vector>

//токены
enum class TokenType {
    INTEGER,    // целое число
    PLUS,       // оператор сложения +
    LT,         // оператор сравнения <
    GT,         // оператор сравнения >
    EQ,         // оператор сравнения =
    NEQ,        // оператор сравнения !=
    END         // конец файла/строки
};

struct Token {
    TokenType type;
    std::string value;
    int position; // позиция в исходной строке

    Token(TokenType t, const std::string& v = "", int pos = 0);
    std::string toString() const;
};

// исключения
class LexerError : public std::runtime_error {
public:
    int position;
    LexerError(const std::string& msg, int pos);
};

class ParserError : public std::runtime_error {
public:
    int position;
    Token token;
    ParserError(const std::string& msg, const Token& t);
};

class SemanticError : public std::runtime_error {
public:
    SemanticError(const std::string& msg);
};

#endif
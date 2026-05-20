#include "scanner.h"
#include <cctype>

Scanner::Scanner(const std::string& input)
    : input(input), pos(0) {
    length = input.length();
}

char Scanner::currentChar() const {
    return (pos < length) ? input[pos] : '\0';
}

void Scanner::advance() {
    if (pos < length) pos++;
}

void Scanner::skipWhitespace() {
    while (pos < length && std::isspace(static_cast<unsigned char>(input[pos]))) {
        pos++;
    }
}

Token Scanner::readNumber() {
    size_t startPos = pos;
    while (pos < length && std::isdigit(static_cast<unsigned char>(input[pos]))) {
        advance();
    }

    // Проверка на число, начинающееся с 0 (кроме 0)
    if (input[startPos] == '0' && (pos - startPos) > 1) {
        throw LexerError("Число не может начинаться с 0", startPos);
    }

    std::string number = input.substr(startPos, pos - startPos);
    return Token(TokenType::INTEGER, number, startPos);
}

Token Scanner::readOperator() {
    size_t startPos = pos;
    char c = currentChar();

    if (c == '<' || c == '>' || c == '=') {
        advance();
        return Token(c == '<' ? TokenType::LT :
            c == '>' ? TokenType::GT : TokenType::EQ,
            std::string(1, c), startPos);
    }
    else if (c == '!') {
        advance();
        if (currentChar() == '=') {
            advance();
            return Token(TokenType::NEQ, "!=", startPos);
        }
        else {
            throw LexerError("Ожидается '=' после '!'", pos);
        }
    }
    else if (c == '+') {
        advance();
        return Token(TokenType::PLUS, "+", startPos);
    }

    throw LexerError("Неизвестный оператор: " + std::string(1, c), startPos);
}

std::vector<Token> Scanner::tokenize() {
    std::vector<Token> tokens;
    pos = 0;

    while (pos < length) {
        skipWhitespace();
        if (pos >= length) break;

        char c = currentChar();

        if (std::isdigit(static_cast<unsigned char>(c))) {
            tokens.push_back(readNumber());
        }
        else if (c == '+' || c == '<' || c == '>' || c == '=' || c == '!') {
            tokens.push_back(readOperator());
        }
        else {
            throw LexerError("Неизвестный символ: " + std::string(1, c), pos);
        }
    }

    tokens.push_back(Token(TokenType::END, "", pos));
    return tokens;
}

void Scanner::reset(const std::string& newInput) {
    input = newInput;
    length = newInput.length();
    pos = 0;
}
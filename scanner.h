#ifndef SCANNER_H
#define SCANNER_H

#include "tokens.h"
#include <string>
#include <vector>

class Scanner {
private:
    std::string input;
    size_t pos;
    size_t length;

    char currentChar() const;
    void advance();
    void skipWhitespace();
    Token readNumber();
    Token readOperator();

public:
    Scanner(const std::string& input);
    std::vector<Token> tokenize();
    void reset(const std::string& newInput);
};

#endif
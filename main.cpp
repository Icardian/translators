#include "scanner.h"
#include "parser.h"
#include <iostream>
#include <fstream>
#include <sstream>

void printHelp() {
    std::cout << "Использование:" << std::endl;
    std::cout << "  ./expression_parser \"выражение\"" << std::endl;
    std::cout << "  ./expression_parser -f файл.txt" << std::endl;
    std::cout << "  ./expression_parser -i (интерактивный режим)" << std::endl;
    std::cout << std::endl;
    std::cout << "Грамматика:" << std::endl;
    std::cout << "  <Выражение> ::= <Сложение> <Операция> <Сложение>" << std::endl;
    std::cout << "  <Сложение> ::= i + <Сложение> | i" << std::endl;
    std::cout << "  <Операция> ::= \"<\" | \">\" | \"=\" | \"!=\"" << std::endl;
    std::cout << "  где i - целое число" << std::endl;
    std::cout << std::endl;
    std::cout << "Примеры:" << std::endl;
    std::cout << "  2+3 < 4" << std::endl;
    std::cout << "  10+20+30 > 5+5" << std::endl;
    std::cout << "  100 = 50+50" << std::endl;
    std::cout << "  7+8 != 15" << std::endl;
}

void processExpression(const std::string& input, int lineNum = 0) {
    try {
        // 1. Лексический анализ
        Scanner scanner(input);
        std::vector<Token> tokens = scanner.tokenize();

        // 2. Синтаксический анализ
        Parser parser(tokens);
        std::shared_ptr<ASTNode> ast = parser.parse();

        // 3. Семантический анализ и вычисление
        bool result = ast->evaluate();

        // Вывод результата
        if (lineNum > 0) {
            std::cout << "Строка " << lineNum << ": ";
        }
        std::cout << input << " -> " << (result ? "истина" : "ложь") << std::endl;

    }
    catch (const LexerError& e) {
        std::cerr << "ОШИБКА: " << e.what() << std::endl;
    }
    catch (const ParserError& e) {
        std::cerr << "ОШИБКА: " << e.what() << std::endl;
    }
    catch (const SemanticError& e) {
        std::cerr << "ОШИБКА: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "НЕИЗВЕСТНАЯ ОШИБКА: " << e.what() << std::endl;
    }
}

int main(int argc, char* argv[]) {
    std::cout << "=== Анализатор выражений сравнения с сложением ===" << std::endl;

    // Режим работы программы
    if (argc == 1 || (argc == 2 && std::string(argv[1]) == "-h")) {
        printHelp();
        return 0;
    }

    if (argc == 2 && std::string(argv[1]) == "-i") {
        // Интерактивный режим
        std::cout << "Интерактивный режим. Введите выражения (Ctrl+D для выхода):" << std::endl;
        std::string line;
        int lineNum = 1;

        while (true) {
            std::cout << "> ";
            if (!std::getline(std::cin, line)) {
                break;
            }

            if (line.empty()) continue;
            if (line == "quit" || line == "exit") break;

            processExpression(line, lineNum++);
        }

    }
    else if (argc == 3 && std::string(argv[1]) == "-f") {
        // Чтение из файла
        std::ifstream file(argv[2]);
        if (!file.is_open()) {
            std::cerr << "Ошибка: не могу открыть файл " << argv[2] << std::endl;
            return 1;
        }

        std::string line;
        int lineNum = 1;
        while (std::getline(file, line)) {
            if (!line.empty()) {
                processExpression(line, lineNum);
            }
            lineNum++;
        }

        file.close();

    }
    else if (argc == 2) {
        // Выражение из аргумента командной строки
        processExpression(argv[1]);

    }
    else {
        std::cerr << "Неверные аргументы. Используйте -h для справки." << std::endl;
        return 1;
    }

    return 0;
}
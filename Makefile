CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
TARGET = expression_parser

SRCS = main.cpp tokens.cpp scanner.cpp parser.cpp
OBJS = $(SRCS:.cpp=.o)
HEADERS = tokens.h scanner.h parser.h

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET) "2+3 < 4"

test: $(TARGET)
	@echo "=== Тест 1: 2+3 < 4 ==="
	./$(TARGET) "2+3 < 4"
	@echo ""
	@echo "=== Тест 2: 10+20+30 > 5+5 ==="
	./$(TARGET) "10+20+30 > 5+5"
	@echo ""
	@echo "=== Тест 3: 100 = 50+50 ==="
	./$(TARGET) "100 = 50+50"
	@echo ""
	@echo "=== Тест 4: 7+8 != 15 ==="
	./$(TARGET) "7+8 != 15"

.PHONY: all clean run test 
#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <iostream>
#include <sstream>
#include <string>
#include <stack>
#include <cctype> // isdigit

class Calculator {
    std::string infixNotation, postfixNotation;

    int operation_priority(char operation);

    std::string get_string_number(const std::string& str, int& pos);

    std::string toPostfix(const std::string& infix);

    double execute(char op, double first, double second);

public:
    Calculator(const std::string& infix_notation);

    double calculate();
    void set_input(const std::string& param);
};

#endif // CALCULATOR_H

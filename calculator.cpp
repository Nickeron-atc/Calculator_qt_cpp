#include "calculator.h"

#include <iostream>
#include <sstream>
#include <string>
#include <stack>
#include <cctype> // isdigit

int Calculator::operation_priority(char operation) {
    switch (operation) {
    case '(': return 0;
    case '+': case '-': return 1;
    case '*': case '/': case '÷': case '×': return 2;
    case '~': return 3; // unary minus
    default: return -1;
    }
}

std::string Calculator::get_string_number(const std::string& str, int& pos) {
    std::string strNumber;
    bool has_decimal_point = false;

    while (pos < str.length()) {
        char c = str[pos];
        if (isdigit(c) || (c == '.' && !has_decimal_point)) {
            if (c == '.') has_decimal_point = true;
            strNumber += c;
            pos++;
        } else {
            break;
        }
    }
    pos--;

    return strNumber;
}

std::string Calculator::toPostfix(const std::string& infix) {
    std::string postfix;
    std::stack<char> operator_stack;

    for (int i = 0; i < infix.length(); i++) {
        char c = infix[i];

        if (isdigit(c) || c == '.') {
            int start = i;
            postfix += get_string_number(infix, i) + " ";
        }
        else if (c == '(') {
            operator_stack.push(c);
        }
        else if (c == ')') {
            while (!operator_stack.empty() && operator_stack.top() != '(') {
                postfix += operator_stack.top();
                operator_stack.pop();
            }
            operator_stack.pop();
        }
        else if (operation_priority(c) >= 0) {
            char op = c;
            if (op == '-' && (i == 0 || (!isdigit(infix[i-1]) && infix[i-1] != ')'))) {
                op = '~';
            }

            while (!operator_stack.empty() && (operation_priority(operator_stack.top()) >= operation_priority(op))) {
                postfix += operator_stack.top();
                operator_stack.pop();
            }
            operator_stack.push(op);
        }
    }

    while (!operator_stack.empty()) {
        postfix += operator_stack.top();
        operator_stack.pop();
    }

    return postfix;
}

double Calculator::execute(char op, double first, double second = 0) {
    switch (op) {
    case '+': return first + second;
    case '-': return first - second;
    case '*': case '×': return first * second;
    case '/': case '÷': return first / second;
    case '~': return -first; // unary minus
    default: return 0;
    }
}

Calculator::Calculator(const std::string& infix_notation) : infixNotation(infix_notation) {
    postfixNotation = toPostfix(infix_notation);
}

void Calculator::set_input(const std::string& param) {
    infixNotation = param;
    postfixNotation = toPostfix(infixNotation);
}

double Calculator::calculate() {
    std::stack<double> locals;
    for (int i = 0; i < postfixNotation.length(); i++) {
        char c = postfixNotation[i];

        if (isdigit(c) || c == '.') {
            std::string number;
            while (i < postfixNotation.length() && (isdigit(postfixNotation[i]) || postfixNotation[i] == '.')) {
                number += postfixNotation[i++];
            }
            locals.push(std::stod(number));
            i--;
        }
        else if (c == '~') {
            double last = locals.top();
            locals.pop();
            locals.push(execute('~', last));
        }
        else if (operation_priority(c) > 0) {
            double second = locals.top(); locals.pop();
            double first = locals.top(); locals.pop();
            locals.push(execute(c, first, second));
        }
    }
    return locals.top();
}

#include "token.h"
#include "token_stack.h"
#include <cctype>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

using namespace std;

// Fills in the token stack
TokenStack tokenize(std::string expression) {
    TokenStack stack;
    stringstream stream(expression);

    // While there are still characters in the stream
    while (stream.rdbuf()->in_avail()) {

        // Ignore all whitespaces
        if (isspace(stream.peek())) {
            stream.get();
            continue;
        }

        // If you read a number push it to the stack accordingly
        if (isdigit(stream.peek())) {
            size_t value;
            stream >> value;
            stack.push(Token(TokenKind::NUMBER, value));
            continue;
        }

        // Read in the next char
        char c;
        stream >> c;

        // If it is anything else, perform actions accordingly
        if (c == '(') {

            stack.push(Token(TokenKind::LP));

        } else if (c == ')') {

            // Make sure there is a number before ')'
            if (stack.top().kind == TokenKind::NUMBER) {
                stack.push(Token(TokenKind::RP));
            } else {
                throw invalid_argument("Malformed");
            }

        } else if (c == '+') {

            // Make sure there is a number before '+'
            if (stack.top().kind == TokenKind::NUMBER) {
                stack.push(Token(TokenKind::PLUS));
            } else {
                throw invalid_argument("Malformed");
            }

        } else if (c == '-') {

            stack.push(Token(TokenKind::MINUS));

        } else if (c == '*') {

            // Make sure there is a number before '*'
            if (stack.top().kind == TokenKind::NUMBER) {
                stack.push(Token(TokenKind::TIMES));
            } else {
                throw invalid_argument("Malformed");
            }

        } else if (c == '/') {

            // Make sure there is a number before '/'
            if (stack.top().kind == TokenKind::NUMBER) {
                stack.push(Token(TokenKind::DIVIDE));
            } else {
                throw invalid_argument("Malformed");
            }

        } else {

            // Any other token is invalid
            throw invalid_argument("Malformed");
        }
    }

    // Reversing the stack
    stack.reverse();
    return stack;
}

// Reduces the expression
void reduce(TokenStack& work) {
    TokenStack immediate;

    // Pushing onto immediate until we see a '(' in work
    while (work.top().kind != TokenKind::LP) {
        immediate.push(work.top());
        work.pop();
        // If work is empty and we haven't found a '(' throw an error
        if (work.empty()) {
            throw invalid_argument("Malformed");
        }
    }

    // Pop the '(' from work
    work.pop();

    // Making sure immediate is not empty
    if (immediate.empty()) {
        throw invalid_argument("Malformed");
    }

    // Checking the -p case
    if (immediate.top().kind == TokenKind::MINUS) {
        immediate.pop();
        size_t value = immediate.top().value;
        value = value * -1;
        Token abs(TokenKind::NUMBER, value);
        work.push(abs);
    }

    // Popping the first number and using it for computation
    int res = immediate.top().value;
    immediate.pop();

    // There is no previous operation yet
    TokenKind prevop = TokenKind::INVALID;

    // Computing items in immediate
    while (!immediate.empty()) {
        // Expecting an operation
        if (immediate.top().kind == TokenKind::NUMBER) {
            throw invalid_argument("Malformed");
        }

        TokenKind op;
        op = immediate.top().kind;
        immediate.pop();

        // Checking for correct chaining
        if (prevop != TokenKind::INVALID) {
            if (prevop != op) {
                throw invalid_argument("Malformed");
            }
        }

        // Getting the second number and performing an operation
        int num = immediate.top().value;
        immediate.pop();
        if (op == TokenKind::PLUS) {

            res += num;

        } else if (op == TokenKind::MINUS) {

            res -= num;

        } else if (op == TokenKind::TIMES) {

            res *= num;

        } else if (op == TokenKind::DIVIDE) {

            // Divide by 0
            if (num == 0) {
                throw invalid_argument("Malformed");
            }
            res /= num;

        } else {

            // No operations found
            throw invalid_argument("Malformed");
        }

        // Updating the operation
        prevop = op;
    }

    // Pushing the result into work
    Token new_token(TokenKind::NUMBER, res);
    work.push(new_token);
}

int evaluate(std::string expression) {

    TokenStack tokens = tokenize(expression);
    TokenStack work;

    // If top token is a ')' then reduce, otherwise push to work
    while (!tokens.empty()) {
        if (tokens.top().kind == TokenKind::RP) {
            tokens.pop();
            reduce(work);
        } else {
            work.push(tokens.top());
            tokens.pop();
        }
    }

    // Checking for exactly one number token and return it
    if (work.size() > 1 || work.size() == 0) {
        throw invalid_argument("Malformed");
    } else if (work.top().kind == TokenKind::NUMBER) {
        return work.top().value;
    }

    return -1;
}

// Please DO NOT CHANGE MAIN
int main(int argc, char** argv) {
    if (argc <= 1) {
        cerr << "Expect an expression..." << endl;
        return -1;
    }

    try {
        cout << evaluate(argv[1]) << endl;
    } catch (invalid_argument& e) {
        cerr << "error: " << e.what() << endl;
    }
    return 0;
}
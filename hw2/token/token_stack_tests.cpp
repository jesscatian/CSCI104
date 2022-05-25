#include "token_stack.h"
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

// Posted on Piazza by Aviv Shai
int main() {
    TokenStack stack;
    stringstream ss;

    assert(stack.empty());

    stack.push(Token(TokenKind::LP));
    stack.push(Token(TokenKind::NUMBER, 1));
    stack.push(Token(TokenKind::PLUS));
    stack.push(Token(TokenKind::NUMBER, 2));
    stack.push(Token(TokenKind::RP));

    assert(stack.size() == 5);

    ss << stack;
    assert(ss.str() == "{(, 1, +, 2, )}");

    stack.reverse();
    ss.str(std::string());
    ss << stack;
    assert(ss.str() == "{), 2, +, 1, (}");
    stack.reverse();

    assert(stack.top().kind == TokenKind::RP);
    stack.pop();
    assert(stack.size() == 4);
    assert(stack.top().kind == TokenKind::NUMBER and stack.top().value == 2);
    stack.pop();
    assert(stack.size() == 3);
    assert(stack.top().kind == TokenKind::PLUS);
    stack.pop();
    assert(stack.size() == 2);
    assert(stack.top().kind == TokenKind::NUMBER and stack.top().value == 1);
    stack.pop();
    assert(stack.size() == 1);
    assert(stack.top().kind == TokenKind::LP);
    stack.pop();
    assert(stack.empty() and stack.size() == 0);

    // errors should be thrown when trying to access an empty stack
    bool errorThrown = false;
    try {
        stack.top();
    } catch (std::out_of_range& e) {
        errorThrown = true;
    }
    assert(errorThrown);

    // no error should be thrown for pop
    errorThrown = false;
    try {
        stack.pop();
    } catch (std::out_of_range& e) {
        errorThrown = true;
    }
    assert(!errorThrown);

    cout << "All tests passed successfully!" << endl;
}

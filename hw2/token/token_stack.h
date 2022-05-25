#ifndef TOKEN_STACK_H
#define TOKEN_STACK_H

#include "token_list.h"

class TokenStack {
public:
    size_t size() const;

    bool empty() const;

    void push(const Token& t);

    // The starter code throws an exception for attempting
    // to get the top of an empty stack for you
    const Token& top() const;

    // If the stack is empty, this function should do nothing
    void pop();

    void reverse();

    // Help debugging
    friend std::ostream& operator<<(std::ostream& out, const TokenStack& stack);

private:
    // Our internal container for our data. Using the TokenList this way is
    // called composition; we are building functionality around an internal
    // data structure that offers its own members and methods.
    TokenList list;
};

#endif

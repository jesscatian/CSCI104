#include "token_stack.h"

#include <stdexcept>

// Returns the top value on the stack
const Token& TokenStack::top() const {
    // Checks if the stack is empty
    if (this->empty()) {
        throw std::out_of_range("empty stack");
    }
    // Returns the list at index size - 1
    size_t size = list.size();
    return this->list.at(size - 1);
}

// Returns how many tokens are in the stack
size_t TokenStack::size() const {
    // Returns the size of the list
    return this->list.size();
}

// Checks whether the stack is empty
bool TokenStack::empty() const {
    // Returns true is the size of the list is 0 or false otherwise
    if (this->list.size() == 0) {
        return true;
    }
    return false;
}

// Pushes a new token on top of the stack
void TokenStack::push(const Token& t) {
    // Pushes a token to the back of the list
    this->list.push_back(t);
}

// Removes the top element on the stack
void TokenStack::pop() {
    size_t size = this->list.size();
    // Checks if the list is empty
    if (size == 0) {
        return;
    }
    this->list.remove(size - 1);
}

// Reverses the stack
void TokenStack::reverse() {
    // Reverses the list
    this->list.reverse();
}

// Overloading << operator
std::ostream& operator<<(std::ostream& out, const TokenStack& stack) { return out << stack.list; }
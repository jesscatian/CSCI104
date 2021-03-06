#ifndef TOKEN_LIST_H
#define TOKEN_LIST_H

#include "token.h"
#include <iostream>
#include <memory>

// This defines a circular Token container with random access. Do not modify
// anything unless you are adding private members in the specified location.
class TokenList {
private:
    // This class wraps each node in the linked list. You are in charge of
    // maintaining the links, but don't forget that you can use the reference-
    // counted nature of shared_ptr to your advantage here.
    struct Item {
        Token value;

        // Links to other items in the circular list.
        std::shared_ptr<Item> next;
        std::shared_ptr<Item> previous;

        // Simple constructors
        Item(Token value) : value(value), next(nullptr), previous(nullptr) {}
    };

    // Number of items in the list
    size_t _size = 0;

    // Pointer to head and tail for speed
    std::shared_ptr<Item> head;

public:
    TokenList() : head(nullptr) {}

    TokenList(TokenList const& other);
    TokenList& operator=(TokenList const& other);
    ~TokenList();

    // Public accessor
    size_t size() const;

    // If the list is empty, the starter code throws an exception for you. Note
    // that these two implementations may be identical.
    Token& at(size_t index);
    const Token& at(size_t index) const;

    void push_back(Token value);

    // This function should first increase the size of the list by 1.
    // Then it should insert the token at index%n where n is the new size of the list
    // and assuming that the head of the list is at index 0.
    void insert(size_t index, Token value);

    // If the list is empty, remove should just return without modifying the list.
    // Otherwise, remove should remove the item at index%n where n is the current size of the list
    // assuming that the head of the list is at index 0.
    void remove(size_t index);

    void reverse();

    // Debugging
    friend std::ostream& operator<<(std::ostream& out, const TokenList& list);

private:
};

#endif

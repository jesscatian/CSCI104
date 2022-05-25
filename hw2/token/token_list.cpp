#include "token_list.h"

#include <algorithm>
#include <stdexcept>

// Copies other list into this list
TokenList::TokenList(TokenList const& other) {
    // Checking if the other circular doubly linked list is empty
    if (other.head == nullptr) {
        this->head = nullptr;
    } else {
        // Create a new Item pointer and copy element by element
        std::shared_ptr<TokenList::Item> cursor = other.head;
        for (size_t i = 0; i < other._size; i++) {
            this->push_back(cursor->value);
            cursor = cursor->next;
        }
    }
}

// Assigns other list into this list
TokenList& TokenList::operator=(TokenList const& other) {
    // Checking for self-assignment
    if (this == &other) {
        return *this;
    } else {
        // Empty this list
        while (this->size() != 0) {
            remove(0);
        }
        // Create a new Item pointer and copy element by element
        std::shared_ptr<TokenList::Item> cursor = other.head;
        for (size_t i = 0; i < other.size(); i++) {
            this->push_back(cursor->value);
            cursor = cursor->next;
        }
        return *this;
    }
}

// Destroys the list
TokenList::~TokenList() {
    // Empty this list
    while (this->size() != 0) {
        remove(0);
    }
}

// Returns the size of list
size_t TokenList::size() const { return this->_size; }

// Gets the Token at an arbitrary index that may change
Token& TokenList::at(size_t index) {
    // Check if the list is empty
    if (this->size() == 0) {
        throw std::out_of_range("no items");
    }

    // Create a new Item pointer and iterate to the index
    std::shared_ptr<TokenList::Item> cursor = this->head;
    for (size_t i = 0; i < index; i++) {
        cursor = cursor->next;
    }
    return cursor->value;
}

// Get the Token at an arbitrary index that you may not change
const Token& TokenList::at(size_t index) const {
    // Check if the list is empty
    if (this->size() == 0) {
        throw std::out_of_range("no items");
    }

    // Create a new Item pointer and iterate to the index
    std::shared_ptr<TokenList::Item> cursor = this->head;
    for (size_t i = 0; i < index; i++) {
        cursor = cursor->next;
    }
    return cursor->value;
}

// Inserts (a copy of) the given Token at the end of list
void TokenList::push_back(Token value) {
    // Insert at index 0 if the list is empty
    if (this->size() == 0) {
        insert(0, value);
    } else {
        // Create a new Item, push to the end of the list, and increment the size
        std::shared_ptr<TokenList::Item> new_item = std::make_shared<TokenList::Item>(value);
        this->head->previous->next = new_item;
        new_item->previous = this->head->previous;
        this->head->previous = new_item;
        new_item->next = this->head;
        _size++;
    }
}

// Inserts (a copy of) the given Token at the index given
void TokenList::insert(size_t index, Token value) {
    // Creates a new Item
    std::shared_ptr<TokenList::Item> new_item = std::make_shared<TokenList::Item>(value);

    // Increment the size and adjust the index
    this->_size++;
    index = index % this->size();

    // Insertion when the list is initially empty
    if (this->size() == 1 && index == 0) {
        this->head = new_item;
        new_item->next = new_item;
        new_item->previous = new_item;
        return;
    }

    // Create a new Item and iterate through to index
    std::shared_ptr<TokenList::Item> cursor = this->head;
    for (size_t i = 0; i < index; i++) {
        cursor = cursor->next;
    }

    // Insertion anywhere else
    cursor->previous->next = new_item;
    new_item->previous = cursor->previous;
    new_item->next = cursor;
    cursor->previous = new_item;

    // Special for insertion in beginning
    if (index == 0) {
        this->head = new_item;
    }
}

// Removes the item at the given index from the list
void TokenList::remove(size_t index) {
    // Check is the list is empty
    if (this->size() == 0) {
        return;
    }

    // If there is only one item in the list
    if (this->size() == 1) {
        this->head->next = nullptr;
        this->head->previous = nullptr;
        this->head = nullptr;
        _size--;
        return;
    }

    // Adjust the index
    index = index % this->size();

    // If removing from the beginning of the list
    if (index == 0) {
        this->head->previous->next = this->head->next;
        this->head->next->previous = this->head->previous;
        this->head = this->head->next;
    } else {
        // If removing anywhere else
        std::shared_ptr<TokenList::Item> cursor = this->head;
        for (size_t i = 0; i < index; i++) {
            cursor = cursor->next;
        }
        cursor->previous->next = cursor->next;
        cursor->next->previous = cursor->previous;
    }
    // Decrement the size
    _size--;
}

// Reverses the list
void TokenList::reverse() {
    // Check if the list is empty
    if (this->size() == 0) {
        return;
    }

    // Traverses the list and flips the next and previous pointers
    std::shared_ptr<TokenList::Item> cursor = this->head;
    for (size_t i = 0; i < this->size(); i++) {
        swap(cursor->next, cursor->previous);
        cursor = cursor->next;
    }

    // Adjusting the head pointer
    this->head = this->head->next;
}

// Overloading the << operator
std::ostream& operator<<(std::ostream& out, const TokenList& list) {
    out << "{";
    std::shared_ptr<TokenList::Item> cursor = list.head;
    if (cursor != nullptr) {
        for (size_t i = 0; i < list.size() - 1; i++) {
            out << cursor->value << ", ";
            cursor = cursor->next;
        }
        out << cursor->value;
    }
    return out << "}";
}

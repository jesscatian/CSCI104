#ifndef RBBST_H
#define RBBST_H

#include "bst.h"
#include <algorithm>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <memory>

struct KeyError {};

/**
 * A special kind of node for an AVL tree, which adds the balance as a data member, plus
 * other additional helper functions. You do NOT need to implement any functionality or
 * add additional data members or helper functions.
 */
template<typename Key, typename Value>
class AVLNode : public Node<Key, Value> {
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, std::shared_ptr<AVLNode<Key, Value>> parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    char getBalance() const;
    void setBalance(char balance);
    void updateBalance(char diff);

    std::shared_ptr<AVLNode<Key, Value>> getParent_AVL() const;
    std::shared_ptr<AVLNode<Key, Value>> getLeft_AVL() const;
    std::shared_ptr<AVLNode<Key, Value>> getRight_AVL() const;

protected:
    // to store the balance of a given node
    char balance_;  //-1, 0, 1
};

// -------------------------------------------------
// Begin implementations for the AVLNode class.
// -------------------------------------------------

// An explicit constructor to initialize the elements by calling the base class constructor and setting
// the color to red since every new node will be red when it is first inserted.

template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, std::shared_ptr<AVLNode<Key, Value>> parent)
        : Node<Key, Value>(key, value, parent), balance_(0) {}

// A destructor which does nothing.
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode() {}

// A getter for the balance of a AVLNode.
template<class Key, class Value>
char AVLNode<Key, Value>::getBalance() const {
    return balance_;
}

// A setter for the balance of a AVLNode.
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(char balance) {
    balance_ = balance;
}

// Adds diff to the balance of a AVLNode.
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(char diff) {
    balance_ += diff;
}

// A separate getParent_AVL function other than the base class function due to covariant return types
template<class Key, class Value>
std::shared_ptr<AVLNode<Key, Value>> AVLNode<Key, Value>::getParent_AVL() const {
    return std::static_pointer_cast<AVLNode<Key, Value>>(this->parent_);
}

// Similar getLeft_AVL function
template<class Key, class Value>
std::shared_ptr<AVLNode<Key, Value>> AVLNode<Key, Value>::getLeft_AVL() const {
    return std::static_pointer_cast<AVLNode<Key, Value>>(this->left_);
}

// Similar getRight_AVL function
template<class Key, class Value>
std::shared_ptr<AVLNode<Key, Value>> AVLNode<Key, Value>::getRight_AVL() const {
    return std::static_pointer_cast<AVLNode<Key, Value>>(this->right_);
}

// -----------------------------------------------
// End implementations for the AVLNode class.
// -----------------------------------------------

template<class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value> {
public:
    void rotateLeft(std::shared_ptr<AVLNode<Key, Value>> p, std::shared_ptr<AVLNode<Key, Value>> n);
    void rotateRight(std::shared_ptr<AVLNode<Key, Value>> p, std::shared_ptr<AVLNode<Key, Value>> n);

    // Remember, AVL is a self-balancing BST
    // Resultant tree after the insert and remove function should be a balanced tree
    // Make appropriate calls to rotateLeft(...) and rotateRight(...)
    // in insert and remove for balancing the height of the AVLTree
    virtual void insert(const std::pair<const Key, Value>& new_item);
    virtual void remove(const Key& key);

protected:
    // Helper function already provided to you.
    virtual void nodeSwap(std::shared_ptr<AVLNode<Key, Value>> n1, std::shared_ptr<AVLNode<Key, Value>> n2);

    // Add helper functions here
    // Consider adding functions like getBalance(...) given a key in the Tree
    // setBalance(...) given a key to a node and balance value, etc
    void insertFix(std::shared_ptr<AVLNode<Key, Value>> p, std::shared_ptr<AVLNode<Key, Value>> n);
    void removeFix(std::shared_ptr<AVLNode<Key, Value>> n, int diff);
    // You may implement a printRootAVL(...)
    // using the printRoot() function from the BST implementation
};

// Pre condition: p is the parent of n
// Post condition: p is the left child of n
template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(std::shared_ptr<AVLNode<Key, Value>> p, std::shared_ptr<AVLNode<Key, Value>> n) {
    auto g = p->getParent_AVL();
    auto nLeft = n->getLeft_AVL();

    if (g == nullptr) {
        this->root_ = n;
    }

    // Move n up
    if (g != nullptr) {
        if (p == g->getLeft_AVL()) {
            g->setLeft(n);
        } else {
            g->setRight(n);
        }
    }
    n->setParent(g);

    // Move p down
    p->setParent(n);
    n->setLeft(p);

    // Re-parent n's left child
    if (nLeft != nullptr) {
        nLeft->setParent(p);
    }
    p->setRight(nLeft);
}

// Pre condition: p is the parent of n
// Post condition: p is the right child of n
template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(std::shared_ptr<AVLNode<Key, Value>> p, std::shared_ptr<AVLNode<Key, Value>> n) {
    auto g = p->getParent_AVL();
    auto nRight = n->getRight_AVL();

    if (g == nullptr) {
        this->root_ = n;
    }
    // Move n up
    if (g != nullptr) {
        if (p == g->getLeft_AVL()) {
            g->setLeft(n);
        } else {
            g->setRight(n);
        }
    }
    n->setParent(g);

    // Move p down
    p->setParent(n);
    n->setRight(p);

    // Re-parent n's left child
    if (nRight != nullptr) {
        nRight->setParent(p);
    }
    p->setLeft(nRight);
}

template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value>& new_item) {
    std::shared_ptr<AVLNode<Key, Value>> newNode(nullptr);

    // Empty AVL
    if (this->root_ == nullptr) {
        this->root_ = std::make_shared<AVLNode<Key, Value>>(new_item.first, new_item.second, nullptr);

        return;
    }

    // Insert node
    auto traverse = std::static_pointer_cast<AVLNode<Key, Value>>(this->root_);
    while (true) {
        // Same key
        if (new_item.first == traverse->getKey()) {
            traverse->setValue(new_item.second);
            newNode.reset();
            return;
        }
        // Smaller key, go left
        else if (new_item.first < traverse->getKey()) {
            if (traverse->getLeft_AVL() == nullptr) {
                newNode = std::make_shared<AVLNode<Key, Value>>(new_item.first, new_item.second, traverse);
                newNode->setBalance(0);
                traverse->setLeft(newNode);
                break;
            }
            traverse = traverse->getLeft_AVL();
        }
        // Bigger key, go right
        else {
            if (traverse->getRight_AVL() == nullptr) {
                newNode = std::make_shared<AVLNode<Key, Value>>(new_item.first, new_item.second, traverse);
                newNode->setBalance(0);
                traverse->setRight(newNode);
                break;
            }
            traverse = traverse->getRight_AVL();
        }
    }

    // Update parent balance
    if (traverse->getBalance() == -1 || traverse->getBalance() == 1) {
        traverse->setBalance(0);
    } else {
        if (newNode == traverse->getLeft_AVL()) {
            traverse->setBalance(-1);
        } else {
            traverse->setBalance(1);
        }
        insertFix(traverse, newNode);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(std::shared_ptr<AVLNode<Key, Value>> p, std::shared_ptr<AVLNode<Key, Value>> n) {
    auto g = p->getParent_AVL();
    if (p == nullptr || g == nullptr) {
        return;
    }

    // Assume p is the left child of g
    if (p == g->getLeft_AVL()) {
        g->updateBalance(-1);
        // Case 1: b(g) == 0
        if (g->getBalance() == 0) {
            return;
        }
        // Case 2: b(g) == -1
        else if (g->getBalance() == -1) {
            insertFix(g, p);
        }
        // Case 3: b(g) == -2
        else {
            // zig-zig
            if (n == p->getLeft_AVL()) {
                rotateRight(g, p);
                p->setBalance(0);
                g->setBalance(0);
            }
            // zig-zag
            else {
                rotateLeft(p, n);
                rotateRight(g, n);
                // Case 3a : b(n) == -1 then b(p) = 0; b(g) = +1; b(n) = 0;
                if (n->getBalance() == -1) {
                    p->setBalance(0);
                    g->setBalance(1);
                }
                // Case 3b: b(n) == 0 then b(p) = 0; b(g) = 0; b(n) = 0;
                else if (n->getBalance() == 0) {
                    p->setBalance(0);
                    g->setBalance(0);
                }
                // Case 3c: b(n) == +1 then b(p) = -1; b(g) = 0; b(n) = 0;
                else {
                    p->setBalance(-1);
                    g->setBalance(0);
                }
                n->setBalance(0);
            }
        }
    } else {
        g->updateBalance(1);
        // Case 1: b(g) == 0
        if (g->getBalance() == 0) {
            return;
        }
        // Case 2: b(g) == 1
        else if (g->getBalance() == 1) {
            insertFix(g, p);
        }
        // Case 3: b(g) == 2
        else {
            // zig-zig
            if (n == p->getRight_AVL()) {
                rotateLeft(g, p);
                p->setBalance(0);
                g->setBalance(0);
            }
            // zig-zag
            else {
                rotateRight(p, n);
                rotateLeft(g, n);
                // Case 3a : b(n) == -1 then b(p) = 0; b(g) = +1; b(n) = 0;
                if (n->getBalance() == 1) {
                    p->setBalance(0);
                    g->setBalance(-1);
                }
                // Case 3b: b(n) == 0 then b(p) = 0; b(g) = 0; b(n) = 0;
                else if (n->getBalance() == 0) {
                    p->setBalance(0);
                    g->setBalance(0);
                }
                // Case 3c: b(n) == +1 then b(p) = -1; b(g) = 0; b(n) = 0;
                else {
                    p->setBalance(1);
                    g->setBalance(0);
                }
                n->setBalance(0);
            }
        }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key) {
    // Find node
    auto removeNode = std::static_pointer_cast<AVLNode<Key, Value>>(this->internalFind(key));
    // If node not found, return
    if (removeNode == nullptr) {
        return;
    }

    if (removeNode->getLeft_AVL() != nullptr && removeNode->getRight_AVL() != nullptr) {
        auto pred = std::static_pointer_cast<AVLNode<Key, Value>>(this->predecessor(removeNode));
        AVLTree<Key, Value>::nodeSwap(removeNode, pred);
    }

    auto p = removeNode->getParent_AVL();
    int diff = 0;
    if (p != nullptr) {
        if (removeNode == removeNode->getParent_AVL()->getLeft_AVL()) {
            diff = 1;
        } else {
            diff = -1;
        }
    }

    if (removeNode->getLeft_AVL() == nullptr && removeNode->getRight_AVL() == nullptr) {
        if (removeNode->getParent_AVL() == nullptr) {
            this->root_ = nullptr;
        } else if (removeNode == removeNode->getParent_AVL()->getLeft_AVL()) {
            removeNode->getParent_AVL()->setLeft(nullptr);
        } else {
            removeNode->getParent_AVL()->setRight(nullptr);
        }
    } else {
        if (removeNode->getLeft_AVL() != nullptr) {
            if (removeNode->getParent_AVL() == nullptr) {
                std::cerr << "IN GET LEFT AVL" << removeNode->getKey() << std::endl;
                removeNode->getLeft_AVL()->setParent(nullptr);
                this->root_ = removeNode->getLeft_AVL();
            } else {
                if (removeNode == removeNode->getParent_AVL()->getLeft_AVL()) {
                    removeNode->getParent_AVL()->setLeft(removeNode->getLeft_AVL());
                } else {
                    removeNode->getParent_AVL()->setRight(removeNode->getRight_AVL());
                }
                removeNode->getLeft_AVL()->setParent(removeNode->getParent_AVL());
            }
        }
        if (removeNode->getRight_AVL() != nullptr) {
            if (removeNode->getParent_AVL() == nullptr) {
                std::cerr << "IN GET RIGHT AVL" << removeNode->getKey() << std::endl;
                removeNode->getRight_AVL()->setParent(nullptr);
                this->root_ = removeNode->getRight_AVL();
            } else {
                if (removeNode == removeNode->getParent_AVL()->getRight_AVL()) {
                    removeNode->getParent_AVL()->setLeft(removeNode->getRight_AVL());
                } else {
                    removeNode->getParent_AVL()->setRight(removeNode->getRight_AVL());
                }
                removeNode->getRight_AVL()->setParent(removeNode->getParent_AVL());
            }
        }
    }

    removeNode.reset();
    removeFix(p, diff);
    return;
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(std::shared_ptr<AVLNode<Key, Value>> n, int diff) {  // 3, -1
    // If n is null, return
    if (n == nullptr) {
        return;
    }

    int ndiff = 0;
    auto p = n->getParent_AVL();
    std::shared_ptr<AVLNode<Key, Value>> c = nullptr;
    std::shared_ptr<AVLNode<Key, Value>> g = nullptr;

    // Compute next recursive call's arguments now before altering tree
    if (p != nullptr) {
        if (BinarySearchTree<Key, Value>::isLeftChild(n)) {
            ndiff = 1;
        } else {
            ndiff = -1;
        }
    }

    // Assume diff = -1, node from right subtree removed
    if (diff == -1) {
        // Case 1: b(n) + diff == -2
        if (n->getBalance() + diff == -2) {
            c = n->getLeft_AVL();
            // Case 1a: b(c) == -1 (zig-zig case)
            if (c->getBalance() == -1) {
                rotateRight(n, c);
                n->setBalance(0);
                c->setBalance(0);
                removeFix(p, ndiff);
            }
            // Case 1b: b(c) == 0 (zig-zig case)
            else if (c->getBalance() == 0) {
                rotateRight(n, c);
                n->setBalance(-1);
                c->setBalance(1);
                return;
            }
            // Case 1c: b(c) == +1 (zig zag case)
            else {
                auto g = c->getRight_AVL();
                rotateLeft(c, g);
                rotateRight(n, c);
                if (g->getBalance() == 1) {
                    n->setBalance(0);
                    c->setBalance(-1);
                    g->setBalance(0);
                } else if (g->getBalance() == 0) {
                    n->setBalance(0);
                    c->setBalance(0);
                    g->setBalance(0);
                } else {
                    n->setBalance(1);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                removeFix(p, ndiff);
            }
        }
        // Case 2: b(n) + diff == -1
        else if (n->getBalance() + diff == -1) {
            n->setBalance(-1);
            return;
        }
        // Case 3: b(n) + diff == 0
        else {
            n->setBalance(0);
            removeFix(p, ndiff);
        }
    }

    // Assume diff = 1, node from left subtree removed
    if (diff == 1) {
        // Case 1: b(n) + diff == 2
        if (n->getBalance() + diff == 2) {
            c = n->getRight_AVL();
            // Case 1a: b(c) == +1 (zig-zig case)
            if (c->getBalance() == 1) {
                rotateLeft(n, c);
                n->setBalance(0);
                c->setBalance(0);
                removeFix(p, ndiff);
            }
            // Case 1b: b(c) == 0 (zig-zig case)
            else if (c->getBalance() == 0) {
                rotateLeft(n, c);
                n->setBalance(1);
                c->setBalance(-1);
                return;
            }
            // Case 1c: b(c) == -1 (zig zag case)
            else {
                auto g = c->getLeft_AVL();
                rotateRight(c, g);
                rotateLeft(n, c);
                if (g->getBalance() == -1) {
                    n->setBalance(0);
                    c->setBalance(1);
                    g->setBalance(0);
                } else if (g->getBalance() == 0) {
                    n->setBalance(0);
                    c->setBalance(0);
                    g->setBalance(0);
                } else {
                    n->setBalance(-1);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                removeFix(p, ndiff);
            }
        }
        // Case 2: b(n) + diff == 1
        else if (n->getBalance() + diff == 1) {
            n->setBalance(1);
            return;
        }
        // Case 3: b(n) + diff == 0
        else {
            n->setBalance(0);
            removeFix(p, ndiff);
        }
    }
}

// Function already completed for you
template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap(std::shared_ptr<AVLNode<Key, Value>> n1, std::shared_ptr<AVLNode<Key, Value>> n2) {
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    char tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

#endif
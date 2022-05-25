#ifndef BST_H
#define BST_H

#include <algorithm>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <memory>
#include <utility>

// A templated class for a Node in a search tree.
// The getters for parent/left/right are virtual so
// that they can be overridden for future kinds of
// search trees, such as Red Black trees, Splay trees,
// and AVL trees.

// Think carefully when implementing ths BST class functionalities
// as you would be using them for the next part of the assignment.
template<typename Key, typename Value>
class Node {
public:
    Node(const Key& key, const Value& value, std::shared_ptr<Node<Key, Value>> parent);
    virtual ~Node();

    const std::pair<const Key, Value>& getItem() const;
    std::pair<const Key, Value>& getItem();
    const Key& getKey() const;
    const Value& getValue() const;
    Value& getValue();

    virtual std::shared_ptr<Node<Key, Value>> getParent() const;
    virtual std::shared_ptr<Node<Key, Value>> getLeft() const;
    virtual std::shared_ptr<Node<Key, Value>> getRight() const;

    void setParent(std::shared_ptr<Node<Key, Value>> parent);
    void setLeft(std::shared_ptr<Node<Key, Value>> left);
    void setRight(std::shared_ptr<Node<Key, Value>> right);
    void setValue(const Value& value);

protected:
    std::pair<const Key, Value> item_;
    std::shared_ptr<Node<Key, Value>> parent_;
    std::shared_ptr<Node<Key, Value>> left_;
    std::shared_ptr<Node<Key, Value>> right_;
};

/*
  -----------------------------------------
  Begin implementations for the Node class.
  -----------------------------------------
*/

// Explicit constructor for a node.
template<typename Key, typename Value>
Node<Key, Value>::Node(const Key& key, const Value& value, std::shared_ptr<Node<Key, Value>> parent)
        : item_(key, value), parent_(parent), left_(NULL), right_(NULL) {}

/*
 * Destructor, which does not need to do anything since the pointers inside of a node
 * are only used as references to existing nodes. The nodes pointed to by parent/left/right
 * are freed within the deleteAll() helper method in the BinarySearchTree.
 */
template<typename Key, typename Value>
Node<Key, Value>::~Node() {}

/**
 * A const getter for the item.
 */
template<typename Key, typename Value>
const std::pair<const Key, Value>& Node<Key, Value>::getItem() const {
    return item_;
}

/**
 * A non-const getter for the item.
 */
template<typename Key, typename Value>
std::pair<const Key, Value>& Node<Key, Value>::getItem() {
    return item_;
}

/**
 * A const getter for the key.
 */
template<typename Key, typename Value>
const Key& Node<Key, Value>::getKey() const {
    return item_.first;
}

/**
 * A const getter for the value.
 */
template<typename Key, typename Value>
const Value& Node<Key, Value>::getValue() const {
    return item_.second;
}

/**
 * A non-const getter for the value.
 */
template<typename Key, typename Value>
Value& Node<Key, Value>::getValue() {
    return item_.second;
}

/**
 * An implementation of the virtual function for retrieving the parent.
 */
template<typename Key, typename Value>
std::shared_ptr<Node<Key, Value>> Node<Key, Value>::getParent() const {
    return parent_;
}

/**
 * An implementation of the virtual function for retrieving the left child.
 */
template<typename Key, typename Value>
std::shared_ptr<Node<Key, Value>> Node<Key, Value>::getLeft() const {
    return left_;
}

/**
 * An implementation of the virtual function for retrieving the right child.
 */
template<typename Key, typename Value>
std::shared_ptr<Node<Key, Value>> Node<Key, Value>::getRight() const {
    return right_;
}

/**
 * A setter for setting the parent of a node.
 */
template<typename Key, typename Value>
void Node<Key, Value>::setParent(std::shared_ptr<Node<Key, Value>> parent) {
    parent_ = parent;
}

/**
 * A setter for setting the left child of a node.
 */
template<typename Key, typename Value>
void Node<Key, Value>::setLeft(std::shared_ptr<Node<Key, Value>> left) {
    left_ = left;
}

/**
 * A setter for setting the right child of a node.
 */
template<typename Key, typename Value>
void Node<Key, Value>::setRight(std::shared_ptr<Node<Key, Value>> right) {
    right_ = right;
}

/**
 * A setter for the value of a node.
 */
template<typename Key, typename Value>
void Node<Key, Value>::setValue(const Value& value) {
    item_.second = value;
}

/*
  ---------------------------------------
  End implementations for the Node class.
  ---------------------------------------
*/

/**
 * A templated unbalanced binary search tree.
 */
template<typename Key, typename Value>
class BinarySearchTree {
public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    virtual void insert(const std::pair<const Key, Value>& keyValuePair);
    virtual void remove(const Key& key);
    void clear();
    bool isBalanced() const;
    void print() const;
    bool empty() const;

public:
    // An internal iterator class for traversing the contents of the BST.
    // Complete the required implementations
    // Additional functions are not necessary for this class.
    // Although, you may add them if you need any.
    class iterator {
    public:
        iterator();

        std::pair<const Key, Value>& operator*() const;
        std::shared_ptr<std::pair<const Key, Value>> operator->() const;

        bool operator==(const iterator& rhs) const;
        bool operator!=(const iterator& rhs) const;

        iterator& operator++();

    protected:
        friend class BinarySearchTree<Key, Value>;
        iterator(std::shared_ptr<Node<Key, Value>> ptr);
        std::shared_ptr<Node<Key, Value>> current_;
    };

public:
    // Functions already completed for you
    iterator begin() const;
    iterator end() const;
    iterator find(const Key& key) const;

protected:
    // Mandatory helper functions you need to complete
    std::shared_ptr<Node<Key, Value>> internalFind(const Key& k) const;
    std::shared_ptr<Node<Key, Value>> getSmallestNode() const;
    static std::shared_ptr<Node<Key, Value>> predecessor(std::shared_ptr<Node<Key, Value>> current);
    static std::shared_ptr<Node<Key, Value>> successor(std::shared_ptr<Node<Key, Value>> current);
    // Note:  static means these functions don't have a "this" pointer
    //        and instead just use the input argument.

    // Helper functions completed for you
    virtual void printRoot(std::shared_ptr<Node<Key, Value>> r) const;
    virtual void nodeSwap(std::shared_ptr<Node<Key, Value>> n1, std::shared_ptr<Node<Key, Value>> n2);

    // Add helper functions here
    // Consider adding simple helper functions like hasParent(...), isLeftChild(...), isRightChild(...)
    // Or functions like getHeight(...), removeInternal(...), etc
    bool isLeftChild(std::shared_ptr<Node<Key, Value>> node);
    bool isRightChild(std::shared_ptr<Node<Key, Value>> node);
    bool hasParent(std::shared_ptr<Node<Key, Value>> node);
    void clearHelper(std::shared_ptr<Node<Key, Value>> root);
    bool isLeafNode(std::shared_ptr<Node<Key, Value>> node);
    void promoteLeft(std::shared_ptr<Node<Key, Value>> remove);
    void promoteRight(std::shared_ptr<Node<Key, Value>> remove);
    bool hasOneChild(std::shared_ptr<Node<Key, Value>> node);
    int getHeight(std::shared_ptr<Node<Key, Value>> root) const;
    bool isBalancedHelper(std::shared_ptr<Node<Key, Value>> root) const;
    std::shared_ptr<Node<Key, Value>> getRoot();
    std::shared_ptr<Node<Key, Value>> internalFindHelper(std::shared_ptr<Node<Key, Value>> node, const Key& key) const;

protected:
    std::shared_ptr<Node<Key, Value>> root_;
    // You should not add other data members
};

/*
--------------------------------------------------------------
Begin implementations for the BinarySearchTree::iterator class.
---------------------------------------------------------------
*/

// Explicit constructor that initializes an iterator with a given node pointer.
template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator(std::shared_ptr<Node<Key, Value>> ptr) : current_(ptr) {}

// A default constructor that initializes the iterator to NULL.
template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator() : current_(NULL) {}

// Provides access to the item.
template<class Key, class Value>
std::pair<const Key, Value>& BinarySearchTree<Key, Value>::iterator::operator*() const {
    return current_->getItem();
}

// Provides access to the address of the item.
template<class Key, class Value>
std::shared_ptr<std::pair<const Key, Value>> BinarySearchTree<Key, Value>::iterator::operator->() const {
    return std::make_shared<std::pair<const Key, Value>>(current_->getItem());
}

// Checks if 'this' iterator's internals have the same value as 'rhs'
template<class Key, class Value>
bool BinarySearchTree<Key, Value>::iterator::operator==(const BinarySearchTree<Key, Value>::iterator& rhs) const {
    return this->current_ == rhs.current_;
}

// Checks if 'this' iterator's internals have a different value as 'rhs'
template<class Key, class Value>
bool BinarySearchTree<Key, Value>::iterator::operator!=(const BinarySearchTree<Key, Value>::iterator& rhs) const {
    return this->current_ != rhs.current_;
}

// Advances the iterator's location using an in-order sequencing
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator& BinarySearchTree<Key, Value>::iterator::operator++() {
    this->current_ = successor(this->current_);
    return *this;
}

// -------------------------------------------------------------
// End implementations for the BinarySearchTree::iterator class.
// -------------------------------------------------------------

// -----------------------------------------------------
// Begin implementations for the BinarySearchTree class.
// -----------------------------------------------------

// Helper functions
template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::isLeftChild(std::shared_ptr<Node<Key, Value>> node) {

    // return node->getParent()->getKey > node->getKey();
    if (node == nullptr || node->getParent() == nullptr) {
        return false;
    }
    // return node->getKey() < node->getParent()->getKey();
    return node == node->getParent()->getLeft();
}

template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::isRightChild(std::shared_ptr<Node<Key, Value>> node) {
    if (node == nullptr || node->getParent() == nullptr) {
        return false;
    }
    return node == node->getParent()->getRight();
    // return node->getKey() > node->getParent()->getKey();
}

template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::hasParent(std::shared_ptr<Node<Key, Value>> node) {
    return node->getParent() != nullptr;
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::clearHelper(std::shared_ptr<Node<Key, Value>> root) {
    // Recurse both ways then call reset() on current node
    if (root == nullptr) {
        return;
    }
    clearHelper(root->getLeft());
    clearHelper(root->getRight());
    root->setLeft(nullptr);
    root->setRight(nullptr);
    root->setParent(nullptr);
    root.reset();
}
template<class Key, class Value>
bool BinarySearchTree<Key, Value>::isLeafNode(std::shared_ptr<Node<Key, Value>> node) {
    return node->getLeft() == nullptr && node->getRight() == nullptr;
}
template<class Key, class Value>
void BinarySearchTree<Key, Value>::promoteLeft(std::shared_ptr<Node<Key, Value>> remove) {
    if (isLeftChild(remove)) {
        remove->getParent()->setLeft(remove->getLeft());
    } else {
        remove->getParent()->setRight(remove->getLeft());
    }
    remove->getLeft()->setParent(remove->getParent());
}
template<class Key, class Value>
void BinarySearchTree<Key, Value>::promoteRight(std::shared_ptr<Node<Key, Value>> remove) {
    if (isLeftChild(remove)) {
        remove->getParent()->setLeft(remove->getRight());
    } else {
        remove->getParent()->setRight(remove->getRight());
    }
    remove->getRight()->setParent(remove->getParent());
}
template<class Key, class Value>
bool BinarySearchTree<Key, Value>::hasOneChild(std::shared_ptr<Node<Key, Value>> node) {
    return node->getLeft() != nullptr && node->getRight() == nullptr
           || node->getRight() != nullptr && node->getLeft() == nullptr;
}

template<class Key, class Value>
int BinarySearchTree<Key, Value>::getHeight(std::shared_ptr<Node<Key, Value>> root) const {
    if (root == nullptr) {
        return 0;
    }

    return 1 + std::max(getHeight(root->getLeft()), getHeight(root->getRight()));
}

template<class Key, class Value>
bool BinarySearchTree<Key, Value>::isBalancedHelper(std::shared_ptr<Node<Key, Value>> root) const {
    int left = 0;
    int right = 0;

    if (root == nullptr) {
        return true;
    }

    left = getHeight(root->getLeft());
    right = getHeight(root->getRight());

    if (abs(left - right) < 2 && isBalancedHelper(root->getLeft()) && isBalancedHelper(root->getRight())) {
        return true;
    }
    return false;
}

template<class Key, class Value>
std::shared_ptr<Node<Key, Value>> BinarySearchTree<Key, Value>::getRoot() {
    return root_;
}

template<class Key, class Value>
std::shared_ptr<Node<Key, Value>>
BinarySearchTree<Key, Value>::internalFindHelper(std::shared_ptr<Node<Key, Value>> node, const Key& key) const {
    // Base case
    if (node->getKey() != key && node->getLeft() == nullptr && node->getRight() == nullptr) {
        return nullptr;
    }
    // If found, otherwise traverse
    if (node->getKey() == key) {
        return node;
    } else if (key < node->getKey()) {
        if (node->getLeft() == nullptr) {
            return nullptr;
        } else {
            return internalFindHelper(node->getLeft(), key);
        }
    } else {
        if (node->getRight() == nullptr) {
            return nullptr;
        } else {
            return internalFindHelper(node->getRight(), key);
        }
    }
}

// Default constructor for a BinarySearchTree, which sets the root to NULL.
template<class Key, class Value>
BinarySearchTree<Key, Value>::BinarySearchTree() {
    root_ = nullptr;
}

template<typename Key, typename Value>
BinarySearchTree<Key, Value>::~BinarySearchTree() {
    clear();
}

// Returns true if tree is empty
template<class Key, class Value>
bool BinarySearchTree<Key, Value>::empty() const {
    return root_ == NULL;
}

// print the tree using the provided printRoot function
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::print() const {
    printRoot(root_);
    std::cout << "\n";
}

// Returns an iterator to the "smallest" item in the tree
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator BinarySearchTree<Key, Value>::begin() const {
    BinarySearchTree<Key, Value>::iterator begin(getSmallestNode());
    return begin;
}

// Returns an iterator whose value means INVALID
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator BinarySearchTree<Key, Value>::end() const {
    BinarySearchTree<Key, Value>::iterator end(NULL);
    return end;
}

// Returns an iterator to the item with the given key, k
// or the end iterator if k does not exist in the tree
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator BinarySearchTree<Key, Value>::find(const Key& k) const {
    std::shared_ptr<Node<Key, Value>> curr = internalFind(k);
    BinarySearchTree<Key, Value>::iterator it(curr);
    return it;
}

// An insert method to insert into a Binary Search Tree.
// If the key is already present in the tree,
// update the current value with the new value.
// The tree may not remain balanced when inserting.
template<class Key, class Value>
void BinarySearchTree<Key, Value>::insert(const std::pair<const Key, Value>& keyValuePair) {
    // Empty BST
    if (root_ == nullptr) {
        root_ = std::make_shared<Node<Key, Value>>(keyValuePair.first, keyValuePair.second, nullptr);
        return;
    }

    auto traverse = root_;
    while (true) {
        // Same key
        if (keyValuePair.first == traverse->getKey()) {
            traverse->setValue(keyValuePair.second);
            break;
        }
        // Smaller key, go left
        else if (keyValuePair.first < traverse->getKey()) {
            if (traverse->getLeft() == nullptr) {
                traverse->setLeft(
                        std::make_shared<Node<Key, Value>>(keyValuePair.first, keyValuePair.second, traverse));
                break;
            }
            traverse = traverse->getLeft();
        }
        // Bigger key, go right
        else {
            if (traverse->getRight() == nullptr) {
                traverse->setRight(
                        std::make_shared<Node<Key, Value>>(keyValuePair.first, keyValuePair.second, traverse));
                break;
            }
            traverse = traverse->getRight();
        }
    }
}

// A remove method to remove a specific key from a Binary Search Tree.
// Does nothing if key not found.
// The tree may not remain balanced after removal.
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::remove(const Key& key) {
    // Find the node
    auto removeNode = internalFind(key);
    // If node not found, return
    if (removeNode == nullptr) {
        return;
    }

    if (removeNode->getLeft() != nullptr && removeNode->getRight() != nullptr) {
        auto pred = predecessor(removeNode);
        nodeSwap(removeNode, pred);
    }

    // Case 1: If node is a leaf node, simply remove
    if (isLeafNode(removeNode)) {
        // Node is root
        if (removeNode->getParent() == nullptr) {
            root_ = nullptr;
        }
        // Node is left child
        else if (isLeftChild(removeNode)) {
            removeNode->getParent()->setLeft(nullptr);
        }
        // Node is right child
        else {
            removeNode->getParent()->setRight(nullptr);
        }
    } else {
        // Left subtree exists
        if (removeNode->getLeft() != nullptr) {
            // Node is root
            if (removeNode == root_) {
                removeNode->getLeft()->setParent(nullptr);
                root_ = removeNode->getLeft();
            }
            // Promote left
            else {
                promoteLeft(removeNode);
            }
        }
        // Right subtree exists
        if (removeNode->getRight() != nullptr) {
            // Node is root
            if (removeNode == root_) {
                removeNode->getRight()->setParent(nullptr);
                root_ = removeNode->getRight();
            }
            // Promote right
            else {
                promoteRight(removeNode);
            }
        }
    }

    removeNode.reset();
    return;
}

// Returns the next largest node or nullptr if it doesn't exist
template<class Key, class Value>
std::shared_ptr<Node<Key, Value>> BinarySearchTree<Key, Value>::successor(std::shared_ptr<Node<Key, Value>> current) {
    if (current == nullptr) {
        return nullptr;
    }

    auto traverse = current;
    // Case 1: If the node has a right child: go right then all the way left
    if (traverse->getRight() != nullptr) {
        traverse = traverse->getRight();
        while (traverse->getLeft() != nullptr) {
            traverse = traverse->getLeft();
        }
        return traverse;
    }
    // Case 2: If not, we have to go up the tree until we find a left child
    else {
        while (true) {
            if (traverse->getParent() != nullptr && traverse == traverse->getParent()->getLeft()) {
                return traverse->getParent();
            }
            if (traverse->getParent() == nullptr) {
                return nullptr;
            }
            traverse = traverse->getParent();
        }
    }
}

// Returns the next smallest node or nullptr if it doesn't exist
template<class Key, class Value>
std::shared_ptr<Node<Key, Value>> BinarySearchTree<Key, Value>::predecessor(std::shared_ptr<Node<Key, Value>> current) {
    if (current == nullptr) {
        return nullptr;
    }

    auto traverse = current;
    // Case 1: If the node has a left child: go left then all the way right
    if (traverse->getLeft() != nullptr) {
        traverse = traverse->getLeft();
        while (traverse->getRight() != nullptr) {
            traverse = traverse->getRight();
        }
        return traverse;
    }
    // Case 2: If not, we have to go up the tree until we find a right child
    else {
        while (true) {
            if (traverse->getParent() != nullptr && traverse == traverse->getParent()->getRight()) {
                return traverse->getParent();
            }
            if (traverse->getParent() == nullptr) {
                return nullptr;
            }
            traverse = traverse->getParent();
        }
    }
}

// A method to remove all contents of the tree and
// reset the values in the tree for use again.
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::clear() {
    clearHelper(root_);
    // root_ = nullptr;
}

// A helper function to find the smallest node in the tree.
template<typename Key, typename Value>
std::shared_ptr<Node<Key, Value>> BinarySearchTree<Key, Value>::getSmallestNode() const {
    if (root_ == nullptr) {
        return nullptr;
    }

    auto traverse = root_;
    while (traverse->getLeft() != nullptr) {
        traverse = traverse->getLeft();
    }
    return traverse;
}

// Helper function to find a node with given key, k and
// return a pointer to it or nullptr if no item with that key exists
template<typename Key, typename Value>
std::shared_ptr<Node<Key, Value>> BinarySearchTree<Key, Value>::internalFind(const Key& key) const {
    // Empty BST
    if (root_ == nullptr) {
        return nullptr;
    }

    auto in = internalFindHelper(root_, key);
    return in;
}

// Return true iff the BST is balanced.
// You may use additional helper functions
template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::isBalanced() const {
    auto traverse = root_;
    return isBalancedHelper(traverse);
}

// Function already implemented for you
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::nodeSwap(
        std::shared_ptr<Node<Key, Value>> n1, std::shared_ptr<Node<Key, Value>> n2) {
    if ((n1 == n2) || (n1 == NULL) || (n2 == NULL)) {
        return;
    }
    std::shared_ptr<Node<Key, Value>> n1p = n1->getParent();
    std::shared_ptr<Node<Key, Value>> n1r = n1->getRight();
    std::shared_ptr<Node<Key, Value>> n1lt = n1->getLeft();
    bool n1isLeft = false;
    if (n1p != NULL && (n1 == n1p->getLeft()))
        n1isLeft = true;
    std::shared_ptr<Node<Key, Value>> n2p = n2->getParent();
    std::shared_ptr<Node<Key, Value>> n2r = n2->getRight();
    std::shared_ptr<Node<Key, Value>> n2lt = n2->getLeft();
    bool n2isLeft = false;
    if (n2p != NULL && (n2 == n2p->getLeft()))
        n2isLeft = true;

    std::shared_ptr<Node<Key, Value>> temp;
    temp = n1->getParent();
    n1->setParent(n2->getParent());
    n2->setParent(temp);

    temp = n1->getLeft();
    n1->setLeft(n2->getLeft());
    n2->setLeft(temp);

    temp = n1->getRight();
    n1->setRight(n2->getRight());
    n2->setRight(temp);

    if ((n1r != NULL && n1r == n2)) {
        n2->setRight(n1);
        n1->setParent(n2);
    } else if (n2r != NULL && n2r == n1) {
        n1->setRight(n2);
        n2->setParent(n1);

    } else if (n1lt != NULL && n1lt == n2) {
        n2->setLeft(n1);
        n1->setParent(n2);

    } else if (n2lt != NULL && n2lt == n1) {
        n1->setLeft(n2);
        n2->setParent(n1);
    }

    if (n1p != NULL && n1p != n2) {
        if (n1isLeft)
            n1p->setLeft(n2);
        else
            n1p->setRight(n2);
    }
    if (n1r != NULL && n1r != n2) {
        n1r->setParent(n2);
    }
    if (n1lt != NULL && n1lt != n2) {
        n1lt->setParent(n2);
    }

    if (n2p != NULL && n2p != n1) {
        if (n2isLeft)
            n2p->setLeft(n1);
        else
            n2p->setRight(n1);
    }
    if (n2r != NULL && n2r != n1) {
        n2r->setParent(n1);
    }
    if (n2lt != NULL && n2lt != n1) {
        n2lt->setParent(n1);
    }

    if (this->root_ == n1) {
        this->root_ = n2;
    } else if (this->root_ == n2) {
        this->root_ = n1;
    }
}

/**
 * Lastly, we are providing you with a print function,
   BinarySearchTree::printRoot().
   Just call it with a node to start printing at, e.g:
   this->printRoot(this->root_) // or any other node pointer

   It will print up to 5 levels of the tree rooted at the passed node,
   in ASCII graphics format.
   We hope it will make debugging easier!
  */

// include print function (in its own file because it's fairly long)
#include "print_bst.h"

// ---------------------------------------------------
// End implementations for the BinarySearchTree class.
// ---------------------------------------------------

#endif

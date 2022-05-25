#ifndef HEAP_H
#define HEAP_H
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <vector>

template<typename T, typename TComparator = std::less<T>>
class Heap {
public:
    // Constructs an m-ary heap for any m >= 2
    Heap(size_t m, TComparator comparator = TComparator()) : m(m), comparator(comparator) {}

    // Destructor as needed
    ~Heap() = default;

    // Adds an item
    void push(const T& item);

    // returns the element at the top of the heap
    //  max (if max-heap) or min (if min-heap)
    const T& top() const;

    // Removes the top element
    void pop();

    // returns true if the heap is empty
    bool empty() const;

private:
    // Add whatever helper functions and data members you need below
    void trickleUp(int index);
    // heapify() helper function
    void heapify(int index);

    // Data
    std::vector<T> store;
    size_t m;
    TComparator comparator;
};

template<class T, class TComparator>
void Heap<T, TComparator>::trickleUp(int index) {
    // Find the parent
    int parent = (index - 1) / (int)m;
    while (parent >= 0 && comparator(store[index], store[parent])) {
        std::swap(store[parent], store[index]);
        index = parent;
        parent = (index - 1) / (int)m;
    }
}

template<class T, class TComparator>
void Heap<T, TComparator>::heapify(int index) {
    // Check if leaf node
    if (index * m >= store.size()) {
        return;
    }
    // Start with left child
    int goodChild = m * index + 1;
    // Find the goodChild
    for (int i = m * index + 1; i < (m * index + 1) + m; i++) {
        if (i >= store.size()) {
            break;
        }
        if (comparator(store[i], store[goodChild])) {
            goodChild = i;
        }
    }
    // Swap with goodChild
    if (comparator(store[goodChild], store[index])) {
        std::swap(store[index], store[goodChild]);
        heapify(goodChild);
    }
}

template<class T, class TComparator>
void Heap<T, TComparator>::push(const T& item) {
    store.push_back(item);
    trickleUp(store.size() - 1);
}

template<class T, class TComparator>
const T& Heap<T, TComparator>::top() const {
    if (empty()) {
        throw std::logic_error("can't top an empty heap");
    }
    return store[0];
}

template<class T, class TComparator>
void Heap<T, TComparator>::pop() {
    if (empty()) {
        throw std::logic_error("can't pop an empty heap");
    }
    store[0] = store.back();
    store.pop_back();
    heapify(0);
}

template<class T, class TComparator>
bool Heap<T, TComparator>::empty() const {
    return store.size() == 0;
}

#endif

#ifndef INT_QUEUE_H
#define INT_QUEUE_H

#include <cstddef>

// IntQueue is an abstract class because it has at least one pure virtual function.
// A queue is an abstract data type (an idea).
// IntQueue abstract base class which is an implementation of the queue abstract data type.
// An abstract base class is sort of like an interface.
// This is our own version of STL queue.
class IntQueue {
public:
    virtual int front() = 0;
    virtual bool empty() = 0;
    virtual size_t size() = 0;
    virtual void push_back(int v) = 0;
    virtual void pop_front() = 0;
};

#endif

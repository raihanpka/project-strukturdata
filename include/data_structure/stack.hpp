#ifndef STACK_HPP
#define STACK_HPP

#include <stdexcept>

template <typename T>
class Stack {
private:
    struct Node {
        T data;
        Node* next;
        Node(T val) : data(val), next(nullptr) {}
    };
    Node* top;
    size_t count;  // Added to track size
    
public:
    Stack() : top(nullptr), count(0) {}
    ~Stack();
    void push(const T& val);
    void pop();
    const T& peek() const;
    bool isEmpty() const;
    size_t size() const;
};

#endif
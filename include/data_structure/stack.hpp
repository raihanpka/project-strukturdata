// include/data_structure/stack.hpp
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
    
public:
    Stack() : top(nullptr) {}
    ~Stack();
    void push(T val);
    void pop();
    T peek() const;
    bool isEmpty() const;
};

#endif
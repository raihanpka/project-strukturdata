#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <cstddef>
#include <stdexcept>


template <typename T>
class Queue {
private:
    struct Node {
        T data;
        Node* next;
        Node(T val) : data(val), next(nullptr) {}
    };
    Node* front;
    Node* rear;
    size_t count;  // Added to track size
    
public:
    Queue();
    ~Queue();
    void enqueue(const T& val);
    void dequeue();
    const T& peek() const;
    bool isEmpty() const;
    size_t size() const;
};

#endif
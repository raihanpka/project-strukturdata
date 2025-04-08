// include/data_structure/queue.hpp
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
    
public:
    Queue();
    ~Queue();
    void enqueue(T val);
    void dequeue();
    T peek() const;
    bool isEmpty() const;
    size_t size() const;
};

#endif
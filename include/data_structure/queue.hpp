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
    size_t count;
    
public:
    Queue();
    ~Queue();
    Queue(const Queue& other);
    Node* getFront() const { return front; }
    Queue& operator=(const Queue& other);
    void enqueue(const T& val);
    void dequeue();
    const T& peek() const;
    bool isEmpty() const;
    size_t size() const;
};

#endif
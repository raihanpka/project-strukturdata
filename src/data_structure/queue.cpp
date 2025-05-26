#include "queue.hpp"
#include "data.hpp"
#include <string>
#include <stdexcept>

template <typename T>
Queue<T>::Queue() : front(nullptr), rear(nullptr), count(0) {}

template <typename T>
Queue<T>::~Queue() {
    while(!isEmpty()) dequeue();
}

template <typename T>
void Queue<T>::enqueue(const T& val) {
    Node* newNode = new Node(val);
    if(rear) rear->next = newNode;
    rear = newNode;
    if(!front) front = rear;
    ++count;
}

template <typename T>
void Queue<T>::dequeue() {
    if(!front) return;
    Node* temp = front;
    front = front->next;
    delete temp;
    if(!front) rear = nullptr;
    --count;
}

template <typename T>
const T& Queue<T>::peek() const {
    if(!front) throw std::runtime_error("Queue is empty");
    return front->data;
}

template <typename T>
bool Queue<T>::isEmpty() const {
    return front == nullptr;
}

template <typename T>
size_t Queue<T>::size() const {
    return count;
}

template class Queue<std::string>;
template class Queue<Pemesanan>;
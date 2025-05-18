#include "stack.hpp"
#include "data.hpp"
#include <string>
#include <stdexcept>

template <typename T>
Stack<T>::~Stack() {
    while (!isEmpty()) pop();
}

template <typename T>
void Stack<T>::push(const T& val) {
    Node* newNode = new Node(val);
    newNode->next = top;
    top = newNode;
    ++count;
}

template <typename T>
void Stack<T>::pop() {
    if (!top) return;
    Node* temp = top;
    top = top->next;
    delete temp;
    --count;
}

template <typename T>
const T& Stack<T>::peek() const {
    if (!top) throw std::runtime_error("Stack is empty");
    return top->data;
}

template <typename T>
bool Stack<T>::isEmpty() const {
    return top == nullptr;
}

template <typename T>
size_t Stack<T>::size() const {
    return count;
}

// Explicit instantiation for needed types
template class Stack<std::string>;
template class Stack<int>;
template class Stack<Pemesanan>;
template class Stack<Jadwal>;

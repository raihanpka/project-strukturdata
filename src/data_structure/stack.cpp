#include "stack.hpp"
#include "data.hpp"
#include <string>
#include <stdexcept>

// Destruktor, menghapus seluruh node pada stack
// Akan dipanggil otomatis saat objek Stack dihancurkan
template <typename T>
Stack<T>::~Stack() {
    while (!isEmpty()) pop(); // Hapus node satu per satu dari atas
}

// Fungsi untuk menambah elemen ke atas stack
// val: data yang akan dimasukkan
template <typename T>
void Stack<T>::push(const T& val) {
    Node* newNode = new Node(val); // Buat node baru dengan data val
    newNode->next = top;           // Node baru menunjuk ke node paling atas sebelumnya
    top = newNode;                 // Update top ke node baru
    ++count;                       // Tambah jumlah elemen
}

// Fungsi untuk menghapus elemen paling atas dari stack
template <typename T>
void Stack<T>::pop() {
    if (!top) return;              // Jika stack kosong, tidak lakukan apa-apa
    Node* temp = top;              // Simpan pointer ke node paling atas
    top = top->next;               // Geser top ke node berikutnya
    delete temp;                   // Hapus node lama
    --count;                       // Kurangi jumlah elemen
}

// Mengambil data paling atas tanpa menghapusnya
// Jika stack kosong, lempar exception
template <typename T>
const T& Stack<T>::peek() const {
    if (!top) throw std::runtime_error("Stack is empty"); // Stack kosong, error
    return top->data;                // Kembalikan data node paling atas
}

// Mengecek apakah stack kosong
template <typename T>
bool Stack<T>::isEmpty() const {
    return top == nullptr;           // True jika top null
}

// Mengambil jumlah elemen dalam stack
template <typename T>
size_t Stack<T>::size() const {
    return count;
}

// Instansiasi template Stack untuk tipe yang dibutuhkan
template class Stack<std::string>;
template class Stack<int>;
template class Stack<Pemesanan>;
template class Stack<Jadwal>;

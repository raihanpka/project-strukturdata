#include "queue.hpp"
#include "data.hpp"
#include <string>
#include <stdexcept>

// Konstruktor default, inisialisasi queue kosong
template <typename T>
Queue<T>::Queue() : front(nullptr), rear(nullptr), count(0) {}

// Destruktor, menghapus seluruh node pada queue
template <typename T>
Queue<T>::~Queue() {
    while(!isEmpty()) dequeue(); // Hapus node satu per satu
}

// Menambahkan elemen ke belakang queue
template <typename T>
void Queue<T>::enqueue(const T& val) {
    Node* newNode = new Node(val); // Buat node baru
    if(rear) rear->next = newNode; // Jika queue tidak kosong, sambungkan node baru ke belakang
    rear = newNode;                // Update rear ke node baru
    if(!front) front = rear;       // Jika queue kosong, front juga menunjuk ke node baru
    ++count;                       // Tambah jumlah elemen
}

// Menghapus elemen paling depan dari queue
template <typename T>
void Queue<T>::dequeue() {
    if(!front) return;             // Jika queue kosong, tidak lakukan apa-apa
    Node* temp = front;            // Simpan pointer ke node depan
    front = front->next;           // Geser front ke node berikutnya
    delete temp;                   // Hapus node lama
    if(!front) rear = nullptr;     // Jika queue jadi kosong, rear juga di-null-kan
    --count;                       // Kurangi jumlah elemen
}

// Mengambil data paling depan tanpa menghapusnya
template <typename T>
const T& Queue<T>::peek() const {
    if(!front) throw std::runtime_error("Queue is empty"); // Jika kosong, lempar exception
    return front->data;            // Kembalikan data node depan
}

// Mengecek apakah queue kosong
template <typename T>
bool Queue<T>::isEmpty() const {
    return front == nullptr;
}

// Mengambil jumlah elemen dalam queue
template <typename T>
size_t Queue<T>::size() const {
    return count;
}

// Copy constructor (deep copy), menyalin seluruh isi queue dari queue lain
template <typename T>
Queue<T>::Queue(const Queue<T>& other) : front(nullptr), rear(nullptr), count(0) {
    Node* curr = other.front;      // Mulai dari node depan queue lain
    while (curr) {
        enqueue(curr->data);       // Salin data satu per satu ke queue baru
        curr = curr->next;
    }
}

// Assignment operator (deep copy), menyalin seluruh isi queue dari queue lain
template <typename T>
Queue<T>& Queue<T>::operator=(const Queue<T>& other) {
    if (this != &other) {          // Cegah self-assignment
        while (!isEmpty()) dequeue(); // Kosongkan queue lama
        Node* curr = other.front;
        while (curr) {
            enqueue(curr->data);   // Salin data satu per satu ke queue ini
            curr = curr->next;
        }
    }
    return *this;
}

// Spesialisasi template untuk tipe string dan Pemesanan
template class Queue<std::string>;
template class Queue<Pemesanan>;
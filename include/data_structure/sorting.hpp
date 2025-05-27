#pragma once

#include <vector>
#include <functional>
#include <algorithm>

template<typename T>
void quicksort(std::vector<T>& arr, int left, int right, std::function<bool(const T&, const T&)> comp) {
    // Basis: jika indeks kiri >= kanan, tidak perlu diurutkan lagi
    if (left >= right) return;
    // Pilih elemen paling kanan sebagai pivot
    T pivot = arr[right];
    int i = left - 1;
    // Iterasi dari left sampai right-1
    for (int j = left; j < right; ++j) {
        // Jika elemen arr[j] lebih kecil dari pivot (berdasarkan comparator)
        if (comp(arr[j], pivot)) {
            ++i;
            std::swap(arr[i], arr[j]); // Tukar elemen ke posisi yang benar
        }
    }
    // Tempatkan pivot di posisi yang tepat
    std::swap(arr[i+1], arr[right]);
    int p = i+1; // Posisi pivot setelah penempatan
    // Rekursif untuk bagian kiri pivot
    quicksort(arr, left, p-1, comp);
    // Rekursif untuk bagian kanan pivot
    quicksort(arr, p+1, right, comp);
}
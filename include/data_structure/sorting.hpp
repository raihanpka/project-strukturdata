#pragma once

#include <vector>
#include <functional>
#include <algorithm>

template<typename T>
void quicksort(std::vector<T>& arr, int left, int right, std::function<bool(const T&, const T&)> comp) {
    if (left >= right) return;
    T pivot = arr[right];
    int i = left - 1;
    for (int j = left; j < right; ++j) {
        if (comp(arr[j], pivot)) {
            ++i;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i+1], arr[right]);
    int p = i+1;
    quicksort(arr, left, p-1, comp);
    quicksort(arr, p+1, right, comp);
}
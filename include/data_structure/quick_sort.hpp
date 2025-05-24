#pragma once

#include <vector>
#include <functional>
#include <algorithm>

template <typename T, typename Compare>
void quickSort(std::vector<T>& arr, int left, int right, Compare comp) {
    if (left >= right) return;
    int i = left, j = right;
    T pivot = arr[left + (right - left) / 2];
    while (i <= j) {
        while (comp(arr[i], pivot)) i++;
        while (comp(pivot, arr[j])) j--;
        if (i <= j) {
            std::swap(arr[i], arr[j]);
            i++; j--;
        }
    }
    if (left < j) quickSort(arr, left, j, comp);
    if (i < right) quickSort(arr, i, right, comp);
}
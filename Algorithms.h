#pragma once
#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include <stack>
template <typename T> class Sorter {
public:
    void quickSort(T* A, int low, int high) {
        std::stack<std::pair<int, int>> stack;
        stack.push({ low, high });

        while (!stack.empty()) {
            std::pair<int, int> current = stack.top();
            stack.pop();
            int current_low = current.first;
            int current_high = current.second;

            if (current_low < current_high) {
                int pivotIndex = partition(A, current_low, current_high);
                stack.push({ current_low, pivotIndex - 1 });
                stack.push({ pivotIndex + 1, current_high });
            }
        }
    }

private:
    int partition(T* A, int low, int high) {
        T pivot = A[high];
        int i = (low - 1);

        for (int j = low; j < high; j++) {
            if (A[j] < pivot) {
                i++;
                std::swap(A[i], A[j]);
            }
        }
        std::swap(A[i + 1], A[high]);
        return (i + 1);
    }

public:
    void heapSort(T* A, int n) {
        for (int i = n / 2 - 1; i >= 0; i--) {
            heapify(A, n, i);
        }
        for (int i = n - 1; i > 0; i--) {
            std::swap(A[0], A[i]);
            heapify(A, i, 0);
        }
    }

private:
    void heapify(T* A, int n, int i) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < n && A[left] > A[largest]) {
            largest = left;
        }

        if (right < n && A[right] > A[largest]) {
            largest = right;
        }

        if (largest != i) {
            std::swap(A[i], A[largest]);
            heapify(A, n, largest);
        }
    }

public:
    void insertSort(T* A, int n) {
        for (int i = 1; i < n; i++) {
            T key = A[i];
            int j = i - 1;
            while (j >= 0 && A[j] > key) {
                A[j + 1] = A[j];
                j--;
            }
            A[j + 1] = key;
        }
    }

public:
    void binaryInsertSort(T* A, int n) {
        for (int i = 1; i < n; i++) {
            T key = A[i];
            int left = 0;
            int right = i - 1;
            while (left <= right) {
                int mid = left + (right - left) / 2;
                if (A[mid] < key) {
                    left = mid + 1;
                }
                else {
                    right = mid - 1;
                }
            }
            for (int j = i - 1; j >= left; j--) {
                A[j + 1] = A[j];
            }
            A[left] = key;
        }
    }
};
#endif // ALGORITHMS_H
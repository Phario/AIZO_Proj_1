#pragma once
#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include <stack>
// Sorter class to implement various sorting algorithms
template <typename T> class Sorter {
public:
	// Sorts the array using the quicksort algorithm
    void quickSort(T* A, int low, int high) {
		// Use a stack to avoid recursion
        std::stack<std::pair<int, int>> stack;
        stack.push({ low, high });
		// Iterate while stack is not empty
        while (!stack.empty()) {
			// Get the top element from the stack
            std::pair<int, int> current = stack.top();
            stack.pop();
			// Get the low and high indices
            int current_low = current.first;
            int current_high = current.second;
			// Check if the current range is valid
            if (current_low < current_high) {
				// Partition the array
                int pivotIndex = partition(A, current_low, current_high);
				// Push the left and right subarrays onto the stack
                stack.push({ current_low, pivotIndex - 1 });
                // Push the right subarray onto the stack
                stack.push({ pivotIndex + 1, current_high });
            }
        }
    }

private:
	// Partition function for quicksort
    int partition(T* A, int low, int high) {
		// Choose the rightmost element as pivot
        T pivot = A[high];
		// Initialize index of smaller element
        int i = (low - 1);
		// Iterate through the array
        for (int j = low; j < high; j++) {
			// If current element is smaller than or equal to pivot
            if (A[j] < pivot) {
                i++;
                std::swap(A[i], A[j]);
            }
        }
		// Swap the pivot element with the element at index i + 1
        std::swap(A[i + 1], A[high]);
        return (i + 1);
    }

public:
	// Sorts the array using the heapsort algorithm
    void heapSort(T* A, int n) {
		// Build heap (rearrange array)
        for (int i = n / 2 - 1; i >= 0; i--) {
			// Call heapify on the root element
            heapify(A, n, i);
        }
		// One by one extract elements from heap
        for (int i = n - 1; i > 0; i--) {
			// Move current root to end
            std::swap(A[0], A[i]);
			// Call heapify on the reduced heap
            heapify(A, i, 0);
        }
    }

private:
	// Heapify function for heapsort
    void heapify(T* A, int n, int i) {
		// Find largest among root, left child and right child
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;
		// Check if left child is larger than root
        if (left < n && A[left] > A[largest]) {
			// Update largest if left child is larger
            largest = left;
        }
		// Check if right child is larger than largest so far
        if (right < n && A[right] > A[largest]) {
			// Update largest if right child is larger
            largest = right;
        }
		// If largest is not root
        if (largest != i) {
			// Swap root with largest
            std::swap(A[i], A[largest]);
            heapify(A, n, largest);
        }
    }

public:
	// Sorts the array using the insertion sort algorithm
    void insertSort(T* A, int n) {
		// Iterate through the array
        for (int i = 1; i < n; i++) {
			// Store the current element (key) to be inserted
            T key = A[i];
			// Initialize j to the index of the last sorted element
            int j = i - 1;
            while (j >= 0 && A[j] > key) {
				// Shift the element to the right
                A[j + 1] = A[j];
                j--;
            }
			// Insert the key at the correct position
            A[j + 1] = key;
        }
    }

public:
	// Sorts the array using the binary insertion sort algorithm
    void binaryInsertSort(T* A, int n) {
		// Iterate through the array
        for (int i = 1; i < n; i++) {
			// Store the current element (key) to be inserted
            T key = A[i];
			// Initialize left and right pointers for binary search
            int left = 0;
            int right = i - 1;
			// Binary search for the correct position of the key
            while (left <= right) {
                int mid = left + (right - left) / 2;
				// Compare the key with the middle element
                if (A[mid] < key) {
					// key is greater, search in the right half
                    left = mid + 1;
                }
                else {
					// key is smaller, search in the left half
                    right = mid - 1;
                }
            }
			// Shift elements to the right to make space for the key
            for (int j = i - 1; j >= left; j--) {
				// Shift the element to the right
                A[j + 1] = A[j];
            }
			// Insert the key at the correct position
            A[left] = key;
        }
    }
};
#endif // ALGORITHMS_H
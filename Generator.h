#pragma once
#ifndef GENERATOR_H
#define GENERATOR_H

#include <cstdlib>
#include <climits>

template <typename T>
class Generator {
public:
    Generator() {}
    int* generateRandomIntegerArray(int size, int amountSorted) {
        int sortedAmount = size * amountSorted / 100;
        int* A = new int[size];
        for (int i = 0; i < size; ++i) {
            if (i < sortedAmount) {
                A[i] = INT_MIN + 1;
            }
            else {
                A[i] = rand();
            }
        }
        return A;
    }
    float* generateRandomFloatArray(int size, int amountSorted) {
        int sortedAmount = size * amountSorted / 100;
        float* A = new float[size];
        for (int i = 0; i < size; ++i) {
            if (i < sortedAmount) {
                A[i] = -1.0f;
            }
            else {
                A[i] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
            }
        }
        return A;
    }
    T* generateAscDescTArray(int size, bool direction) {
        T* A = new T[size];
        for (int i = 0; i < size; ++i) {
            if (direction) {
                A[i] = static_cast<T>(i);
            }
            else {
                A[i] = static_cast<T>(size - i);
            }
        }
        return A;
    }
	void printArray(T* A, int size) {
		for (int i = 0; i < size; ++i) {
			std::cout << A[i] << " ";
		}
		std::cout << std::endl;
	}
};
#endif // GENERATOR_H

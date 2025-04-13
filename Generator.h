#pragma once
#ifndef GENERATOR_H
#define GENERATOR_H

#include <cstdlib>
#include <climits>

template <typename T>
// Generator class to generate random arrays
class Generator {
public:
	// Constructor to seed the random number generator
    Generator() { srand(time(NULL)); }
	// generate random integer array with a certain amount sorted in % (0 for unsorted)
    int* generateRandomIntegerArray(int size, int amountSorted) {
        int sortedAmount = size * amountSorted / 100;
        int* A = new int[size];
        for (int i = 0; i < size; ++i) {
            if (i < sortedAmount) {
                A[i] = 0;
            }
            else {
                A[i] = rand();
            }
        }
        return A;
    }
	// generate random float array with a certain amount sorted in % (0 for unsorted)
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
	// generate random integer array descending or ascending
    T* generateAscDescTArray(int size, bool direction) {
        T* A = new T[size];
        int randomNum = rand();
        for (int i = 0; i < size; ++i) {
            if (direction) {
                A[i] = randomNum + static_cast<T>(i);
            }
            else {
                A[i] = randomNum + static_cast<T>(size - i);
            }
        }
        return A;
    }
	// print the array
	void printArray(T* A, int size) {
		for (int i = 0; i < size; ++i) {
			std::cout << A[i] << " ";
		}
		std::cout << std::endl;
	}
};
#endif // GENERATOR_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <chrono>
#include <cmath>
#include <vector>
#include <algorithm>
#include "Algorithms.h"
#include "Generator.h"
int algorithmAmount = 4;
struct FileData {
	int mode;
	int size;
	int algorithm;
	int amountSorted;
	int dataType;
	int instanceAmount;
};
struct resultsInfo {
	double avgTime;
	double minTime;
	double maxTime;
	double medianTime;
	double stdDevTime;
	std::vector<double> instanceTime;
};
resultsInfo resultCalculator(const std::vector<double>& inputTimes) {
	resultsInfo result;
	if (inputTimes.empty()) {
		throw std::invalid_argument("Input vector is empty.");
	}
	result.instanceTime = inputTimes; // Copy original data
	int size = inputTimes.size();
	double sum = 0.0;
	result.minTime = inputTimes[0];
	result.maxTime = inputTimes[0];
	for (double time : inputTimes) {
		sum += time;
		if (time < result.minTime) result.minTime = time;
		if (time > result.maxTime) result.maxTime = time;
	}
	result.avgTime = sum / size;
	// Median
	std::vector<double> sortedTimes = inputTimes;
	std::sort(sortedTimes.begin(), sortedTimes.end());
	if (size % 2 == 0) {
		result.medianTime = (sortedTimes[size / 2 - 1] + sortedTimes[size / 2]) / 2;
	}
	else {
		result.medianTime = sortedTimes[size / 2];
	}
	// Standard deviation
	double varianceSum = 0.0;
	for (double time : inputTimes) {
		varianceSum += (time - result.avgTime) * (time - result.avgTime);
	}
	result.stdDevTime = std::sqrt(varianceSum / size);
	return result;
}
/*
Structure of config file:
0	#mode:
10000	#size
0	#algorithm: 0-heapsort, 1-insertion sort, 2-quicksort, 3-binary insert sort
33	#amount: sorted in %
0	#data type: 0-int, 1-float
100 #instance amount
*/
FileData loadFileData() {
	FileData fileData;
	std::string filePath;
	std::fstream fileStream;
	std::cout << "Enter the path to the config file: ";
	std::cin >> filePath;
	fileStream.open(filePath);
	// TODO: Parsing file data
	if (!fileStream.is_open()) {
		std::cerr << "Error opening file: " << filePath << std::endl;
		fileData.mode = -1; //Indicate an error
		fileData.size = -1;
		fileData.algorithm = -1;
		fileData.amountSorted = -1;
		fileData.dataType = -1;
		fileStream.close();
		return fileData;
	}
    std::string line;
    int lineIndex = 0;
    while (std::getline(fileStream, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }
        std::istringstream iss(line);
        int value;
        if (iss >> value) {
            switch (lineIndex) {
            case 0:
                fileData.mode = value;
                break;
            case 1:
                fileData.size = value;
                break;
            case 2:
                fileData.algorithm = value;
                break;
            case 3:
                fileData.amountSorted = value;
                break;
            case 4:
                fileData.dataType = value;
                break;
			case 5:
				fileData.instanceAmount = value;
            default:
                break;
            }
            lineIndex++;
        }
    }
    fileStream.close();
	return fileData;
}
void performTest(FileData fileData) {
	if (fileData.dataType == 0) {
		Sorter<int> sorter;
		Generator<int> generator;
		int* A = nullptr;
		A = generator.generateRandomIntegerArray(fileData.size, fileData.amountSorted);
		generator.printArray(A, fileData.size);
		switch (fileData.algorithm) {
		case 0:
			std::cout << "Heapsort:" << std::endl;
			sorter.heapSort(A, fileData.size);
			generator.printArray(A, fileData.size);
			break;
		case 1:
			std::cout << "Insertionsort:" << std::endl;
			sorter.insertSort(A, fileData.size);
			generator.printArray(A, fileData.size);
			break;
		case 2:
			std::cout << "Quicksort:" << std::endl;
			sorter.quickSort(A, 0, fileData.size-1);
			generator.printArray(A, fileData.size);
			break;
		case 3:
			std::cout << "Binary Insertionsort:" << std::endl;
			sorter.binaryInsertSort(A, fileData.size);
			generator.printArray(A, fileData.size);
			break;
		default:
			std::cerr << "Invalid algorithm" << std::endl;
			break;
		}
	}
	else if (fileData.dataType == 1) {
		Generator<float> generator;
		Sorter<float> sorter;
		float* A = nullptr;
		A = generator.generateRandomFloatArray(fileData.size, fileData.amountSorted);
		generator.printArray(A, fileData.size);
		std::cout << "Quicksort:" << std::endl;
		sorter.quickSort(A, 0, fileData.size - 1);
	}
	else {
		std::cerr << "Invalid data type" << std::endl;
		return;
	}
}
//TODO: finish this
double* performSimulation(FileData fileData) {
	Generator<int> generator;
	Sorter<int> sorter;
	int* A = nullptr;
	resultsInfo quickSortResults;
	resultsInfo insertionSortResults;
	resultsInfo heapSortResults;
	resultsInfo binaryInsertionSortResults;
	// generate a 3d array with arrays to test for each algorithm
	int*** testingArray = new int** [algorithmAmount]; //1d
	for (int i = 0; i < algorithmAmount; ++i) {
		testingArray[i] = new int* [fileData.instanceAmount]; //2d
		for (int j = 0; j < fileData.instanceAmount; ++j) {
			testingArray[i][j] = new int[fileData.size]; //3d
		}
	}
	// fill the first dimension with generated arrays according to fileData, then copy the arrays to other columns
	for (int i = 0; i < fileData.instanceAmount; ++i) {
		A = generator.generateRandomIntegerArray(fileData.size, fileData.amountSorted);
		for (int j = 0; j < algorithmAmount; ++j) {
			for (int k = 0; k < fileData.size; ++k) {
				testingArray[j][i][k] = A[k];
			}
		}
	}
	// sort each array with the corresponding algorithm, measure each array sort time time in the instanceTime array
	heapSortResults.instanceTime.resize(fileData.instanceAmount);
	insertionSortResults.instanceTime.resize(fileData.instanceAmount);
	quickSortResults.instanceTime.resize(fileData.instanceAmount);
	binaryInsertionSortResults.instanceTime.resize(fileData.instanceAmount);
	for (int j = 0; j < algorithmAmount; j++) {
		for (int i = 0; i < fileData.instanceAmount; ++i) {
			switch (j) {
			case 0: {
				auto t1 = std::chrono::high_resolution_clock::now();
				sorter.heapSort(testingArray[j][i], fileData.size);
				auto t2 = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> elapsedTime = t2 - t1;
				heapSortResults.instanceTime[i] = elapsedTime.count();
				break;
			}
			case 1: {
				auto t3 = std::chrono::high_resolution_clock::now();
				sorter.insertSort(testingArray[j][i], fileData.size);
				auto t4 = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> elapsedTime2 = t4 - t3;
				insertionSortResults.instanceTime[i] = elapsedTime2.count();
				break;
			}
			case 2: {
				auto t5 = std::chrono::high_resolution_clock::now();
				sorter.quickSort(testingArray[j][i], 0, fileData.size - 1);
				auto t6 = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> elapsedTime3 = t6 - t5;
				quickSortResults.instanceTime[i] = elapsedTime3.count();
				break;
			}
			case 3: {
				auto t7 = std::chrono::high_resolution_clock::now();
				sorter.binaryInsertSort(testingArray[j][i], fileData.size);
				auto t8 = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> elapsedTime4 = t8 - t7;
				binaryInsertionSortResults.instanceTime[i] = elapsedTime4.count();
				break;
			}
			default:
				break;
			}
		}
	}
	// calculate results for each algorithm
	quickSortResults = resultCalculator(quickSortResults.instanceTime);
	insertionSortResults = resultCalculator(insertionSortResults.instanceTime);
	heapSortResults = resultCalculator(heapSortResults.instanceTime);
	binaryInsertionSortResults = resultCalculator(binaryInsertionSortResults.instanceTime);
	// print out the data
	std::cout << "Algorithm: Heapsort" << std::endl;
	std::cout << "Average time: " << heapSortResults.avgTime << std::endl;
	std::cout << "Minimum time: " << heapSortResults.minTime << std::endl;
	std::cout << "Maximum time: " << heapSortResults.maxTime << std::endl;
	std::cout << "Median time: " << heapSortResults.medianTime << std::endl;
	std::cout << "Standard deviation: " << heapSortResults.stdDevTime << std::endl;
	std::cout << "Algorithm: Insertionsort" << std::endl;
	std::cout << "Average time: " << insertionSortResults.avgTime << std::endl;
	std::cout << "Minimum time: " << insertionSortResults.minTime << std::endl;
	std::cout << "Maximum time: " << insertionSortResults.maxTime << std::endl;
	std::cout << "Median time: " << insertionSortResults.medianTime << std::endl;
	std::cout << "Standard deviation: " << insertionSortResults.stdDevTime << std::endl;
	std::cout << "Algorithm: Quicksort" << std::endl;
	std::cout << "Average time: " << quickSortResults.avgTime << std::endl;
	std::cout << "Minimum time: " << quickSortResults.minTime << std::endl;
	std::cout << "Maximum time: " << quickSortResults.maxTime << std::endl;
	std::cout << "Median time: " << quickSortResults.medianTime << std::endl;
	std::cout << "Standard deviation: " << quickSortResults.stdDevTime << std::endl;
	std::cout << "Algorithm: Binary Insertionsort" << std::endl;
	std::cout << "Average time: " << binaryInsertionSortResults.avgTime << std::endl;
	std::cout << "Minimum time: " << binaryInsertionSortResults.minTime << std::endl;
	std::cout << "Maximum time: " << binaryInsertionSortResults.maxTime << std::endl;
	std::cout << "Median time: " << binaryInsertionSortResults.medianTime << std::endl;
	std::cout << "Standard deviation: " << binaryInsertionSortResults.stdDevTime << std::endl;
	// free the memory
	for (int i = 0; i < algorithmAmount; ++i) {
		for (int j = 0; j < fileData.instanceAmount; ++j) {
			delete[] testingArray[i][j]; // Free innermost array
		}
		delete[] testingArray[i]; // Free second dimension
	}
	delete[] testingArray;
	return nullptr;
}
int main() {
	FileData fileData = loadFileData();
    if (fileData.mode == 0) performTest(fileData);
	else if (fileData.mode == 1) {
		performSimulation(fileData);
	}
	else {
		std::cerr << "Invalid mode" << std::endl;
	}
	return 0;
}
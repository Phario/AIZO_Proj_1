#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
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
	double instanceTime[];
};
resultsInfo resultCalculator(double* instanceTime, int size) {
	resultsInfo result;
	result.avgTime = 0.0;
	result.minTime = instanceTime[0];
	result.maxTime = instanceTime[0];
	for (int i = 0; i < size; i++) {
		result.avgTime += instanceTime[i];
		if (instanceTime[i] < result.minTime) {
			result.minTime = instanceTime[i];
		}
		if (instanceTime[i] > result.maxTime) {
			result.maxTime = instanceTime[i];
		}
	}
	// Calculate median
	std::sort(instanceTime, instanceTime + size);
	if (size % 2 == 0) {
		result.medianTime = (instanceTime[size / 2 - 1] + instanceTime[size / 2]) / 2;
	}
	else {
		result.medianTime = instanceTime[size / 2];
	}
	// Calculate standard deviation
	double sum = 0.0;
	for (int i = 0; i < size; i++) {
		sum += (instanceTime[i] - result.avgTime) * (instanceTime[i] - result.avgTime);
	}
	result.stdDevTime = sqrt(sum / size);
	result.avgTime /= size;
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

	// generate a 3d array with arrays to test for each algorithm
	double*** testingArray = new double** [algorithmAmount]; //1d
	for (int i = 0; i < algorithmAmount; ++i) {
		testingArray[i] = new double* [fileData.instanceAmount]; //2d
		for (int j = 0; j < fileData.instanceAmount; ++j) {
			testingArray[i][j] = new double[fileData.size]; //3d
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
	for (int i = 0; i < fileData.instanceAmount; ++i) {
		switch (fileData.algorithm) {
		case 0:
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		default:
			break;
		}
	}

}
int main() {
	FileData fileData = loadFileData();
    if (fileData.mode == 0) performTest(fileData);
	else if (fileData.mode == 1) {
		performSimulation(fileData);
	}
	return 0;
}
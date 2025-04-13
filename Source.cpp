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
#include "progressbar.hpp"
/*
Structure of config file:
0		#mode:
10000	#size
0		#algorithm: 0-heapsort, 1-insertion sort, 2-quicksort, 3-binary insert sort
33		#amount: sorted in %
0		#data type: 0-int, 1-float
100		#instance amount
0		#direction: 0-random, 1-ascending, 2-descending
*/
// FileData structure to hold the configuration data
struct fileData {
	int mode;
	int size;
	int algorithm;
	int amountSorted;
	int dataType;
	int instanceAmount;
	int direction;
};
// resultsInfo structure to hold the results of the sorting algorithms
struct resultsInfo {
	double avgTime;
	double minTime;
	double maxTime;
	double medianTime;
	double stdDevTime;
	std::vector<double> instanceTime;
};
// amount of algorithms
int algorithmAmount = 4;
// calculates the results of the sorting algorithms
resultsInfo resultCalculator(const std::vector<double>& inputTimes) {
	resultsInfo result;
	// check if the input vector is empty
	if (inputTimes.empty()) {
		throw std::invalid_argument("Input vector is empty.");
	}
	result.instanceTime = inputTimes; // Copy original data
	int size = inputTimes.size();
	double sum = 0.0;
	result.minTime = inputTimes[0];
	result.maxTime = inputTimes[0];
	// Calculate average, min, and max
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
// Loads data from file
fileData loadFileData() {
	// initialize file data and file stream
	fileData fileData;
	std::string filePath;
	std::fstream fileStream;
	std::cout << "Enter the path to the config file: ";
	std::cin >> filePath;
	fileStream.open(filePath);
	if (!fileStream.is_open()) {
		std::cerr << "Error opening file: " << filePath << std::endl;
		fileData.mode = -1; // Indicate an error
		fileData.size = -1;
		fileData.algorithm = -1;
		fileData.amountSorted = -1;
		fileData.dataType = -1;
		fileData.instanceAmount = -1;
		fileData.direction = -1;
		fileStream.close();
		return fileData;
	}
	// Read the file line by line and parse it
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
				break;
			case 6:
				fileData.direction = value;
				break;
            default:
                break;
            }
            lineIndex++;
        }
    }
    fileStream.close();
	return fileData;
}
// performs the test according to the mode and data type
void performTest(fileData fileData) {
	// check if the fileData is valid
	if (fileData.dataType == 0) {
		// initialize tools
		Sorter<int> sorter;
		Generator<int> generator;
		int* A = nullptr;
		// generate a random array according to fileData
		if (fileData.direction == 0) {
			A = generator.generateRandomIntegerArray(fileData.size, fileData.amountSorted);
		}
		else if (fileData.direction == 1) {
			A = generator.generateAscDescTArray(fileData.size, true);
		}
		else if (fileData.direction == 2) {
			A = generator.generateAscDescTArray(fileData.size, false);
		}
		else {
			std::cerr << "Invalid direction" << std::endl;
			return;
		}
		// print the original array
		generator.printArray(A, fileData.size);
		// sort the array according to the algorithm and print the sorted array
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
	// if the data type is float
	else if (fileData.dataType == 1) {
		Generator<float> generator;
		Sorter<float> sorter;
		float* A = nullptr;
		A = generator.generateRandomFloatArray(fileData.size, fileData.amountSorted);
		generator.printArray(A, fileData.size);
		std::cout << "Quicksort:" << std::endl;
		sorter.quickSort(A, 0, fileData.size - 1);
		generator.printArray(A, fileData.size);
	}
	// if the data type is invalid
	else {
		std::cerr << "Invalid data type" << std::endl;
		return;
	}
}
// performs the simulation according to the mode and data type
resultsInfo* performSimulation(fileData fileData) {
	// initialize tools
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
	// fill the first dimension with generated arrays according to fileData
	for (int i = 0; i < fileData.instanceAmount; ++i) {
		if (fileData.direction == 0) { 
			A = generator.generateRandomIntegerArray(fileData.size, fileData.amountSorted); 
		}
		else if (fileData.direction == 1) {
			A = generator.generateAscDescTArray(fileData.size, true);
		}
		else if (fileData.direction == 2) {
			A = generator.generateAscDescTArray(fileData.size, false);
		}
		else {
			std::cerr << "Invalid direction" << std::endl;
			return nullptr;
		}
		// copy arrays to other columns
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
	// initialize the progress bar
	progressbar bar(algorithmAmount * fileData.instanceAmount);
	bar.set_todo_char(" ");
	bar.set_done_char("#");
	// sort each array
	for (int j = 0; j < algorithmAmount; j++) {
		for (int i = 0; i < fileData.instanceAmount; ++i) {
			switch (j) {
			// start clocks, perform sort, save results to results struct, rinse and repeat
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
			// update the progress bar
			bar.update();
		}
		
	}
	// calculate results for each algorithm
	quickSortResults = resultCalculator(quickSortResults.instanceTime);
	insertionSortResults = resultCalculator(insertionSortResults.instanceTime);
	heapSortResults = resultCalculator(heapSortResults.instanceTime);
	binaryInsertionSortResults = resultCalculator(binaryInsertionSortResults.instanceTime);
	// print out the data
	std::cout << "Array size: " << fileData.size << std::endl;
	std::cout << "Array amount: " << fileData.instanceAmount << std::endl;
	std::cout << "Amount sorted: " << fileData.amountSorted << "%" << std::endl;
	std::cout << "Memory allocated: " << fileData.instanceAmount * fileData.size * sizeof(int)*4 << " bytes" << std::endl;
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
			delete[] testingArray[i][j];
		}
		delete[] testingArray[i];
	}
	delete[] testingArray;
	// save results to array of results for easier processing
	resultsInfo* resultsInfoArray = new resultsInfo[algorithmAmount];
	resultsInfoArray[0] = heapSortResults;
	resultsInfoArray[1] = insertionSortResults;
	resultsInfoArray[2] = quickSortResults;
	resultsInfoArray[3] = binaryInsertionSortResults;
	// free the memory for the original arrays
	return resultsInfoArray;
}
// performs the simulation for float data type with quicksort
resultsInfo performFloatSimulation(fileData fileData) {
	// initialize tools
	Generator<float> generator;
	Sorter<float> sorter;
	float* A = nullptr;
	resultsInfo quickSortResults;
	// generate a 3d array with arrays to test for each algorithm
	float*** testingArray = new float** [algorithmAmount]; //1d
	for (int i = 0; i < algorithmAmount; ++i) {
		testingArray[i] = new float* [fileData.instanceAmount]; //2d
		for (int j = 0; j < fileData.instanceAmount; ++j) {
			testingArray[i][j] = new float[fileData.size]; //3d
		}
	}
	// fill the first dimension with generated arrays according to fileData
	for (int i = 0; i < fileData.instanceAmount; ++i) {
		if (fileData.direction == 0) {
			A = generator.generateRandomFloatArray(fileData.size, fileData.amountSorted);
		}
		else if (fileData.direction == 1) {
			A = generator.generateAscDescTArray(fileData.size, true);
		}
		else if (fileData.direction == 2) {
			A = generator.generateAscDescTArray(fileData.size, false);
		}
		else {
			std::cerr << "Invalid direction" << std::endl;
			return quickSortResults;
		}
		for (int j = 0; j < algorithmAmount; ++j) {
			for (int k = 0; k < fileData.size; ++k) {
				testingArray[j][i][k] = A[k];
			}
		}
	}
	progressbar bar(fileData.instanceAmount);
	bar.set_todo_char(" ");
	bar.set_done_char("#");
	// sort each array with quicksort, measure array sort time time in the instanceTime array
	quickSortResults.instanceTime.resize(fileData.instanceAmount);
	for (int i = 0; i < fileData.instanceAmount; ++i) {
		auto t1 = std::chrono::high_resolution_clock::now();
		sorter.quickSort(testingArray[0][i], 0, fileData.size - 1);
		auto t2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsedTime = t2 - t1;
		quickSortResults.instanceTime[i] = elapsedTime.count();
		bar.update();
	}
	// calculate results for quicksort
	quickSortResults = resultCalculator(quickSortResults.instanceTime);
	// print out the data
	std::cout << "Array size: " << fileData.size << std::endl;
	std::cout << "Array amount: " << fileData.instanceAmount << std::endl;
	std::cout << "Amount sorted: " << fileData.amountSorted << "%" << std::endl;
	std::cout << "Memory allocated: " << fileData.instanceAmount * fileData.size * sizeof(float) << " bytes" << std::endl;
	std::cout << "Algorithm: Quicksort" << std::endl;
	std::cout << "Average time: " << quickSortResults.avgTime << std::endl;
	std::cout << "Minimum time: " << quickSortResults.minTime << std::endl;
	std::cout << "Maximum time: " << quickSortResults.maxTime << std::endl;
	std::cout << "Median time: " << quickSortResults.medianTime << std::endl;
	std::cout << "Standard deviation: " << quickSortResults.stdDevTime << std::endl;
	// free the memory
	for (int i = 0; i < algorithmAmount; ++i) {
		for (int j = 0; j < fileData.instanceAmount; ++j) {
			delete[] testingArray[i][j];
		}
		delete[] testingArray[i];
	}
	delete[] testingArray;
	return quickSortResults;
}
// saves data to csv files
void saveDataToCSV(const std::string& filename, const resultsInfo& result) {
	// ask the user for the file path
	std::cout << "Enter the path to save the CSV file: ";
	std::string filePath;
	std::cin >> filePath;
	std::ofstream file(filePath + filename);
	// check if the file is open
	if (file.is_open()) {
		// write the data to the file
		file << "Instance no.;Time\n";
		for (size_t i = 0; i < result.instanceTime.size(); ++i) {
			file << i + 1 << ";" << result.instanceTime[i] << "\n";
		}
		file << "Average Time:;" << result.avgTime << "\n";
		file << "Minimum Time:;" << result.minTime << "\n";
		file << "Maximum Time:;" << result.maxTime << "\n";
		file << "Median Time:;" << result.medianTime << "\n";
		file << "Standard Deviation:;" << result.stdDevTime << "\n";
		file.close();
		std::cout << "Data saved to " << filePath + filename << std::endl;
	}
	else {
		// if the file is not open, print an error message
		std::cerr << "Error opening file for writing: " << filePath + filename << std::endl;
	}
}
// main program
int main() {
	// load data from file and perform tests according to mode
	fileData fileData = loadFileData();
	resultsInfo* resultsInfoArray = nullptr;
    if (fileData.mode == 0) performTest(fileData);
	else if (fileData.mode == 1 && fileData.dataType == 0) {
		resultsInfoArray = performSimulation(fileData);
		// save results to csv files
		saveDataToCSV("heapSortResults.csv", resultsInfoArray[0]);
		saveDataToCSV("insertionSortResults.csv", resultsInfoArray[1]);
		saveDataToCSV("quickSortResults.csv", resultsInfoArray[2]);
		saveDataToCSV("binaryInsertionSortResults.csv", resultsInfoArray[3]);
	}
	else if (fileData.mode == 1 && fileData.dataType == 1) {
		resultsInfo floatResults = performFloatSimulation(fileData);
		saveDataToCSV("floatResults.csv", floatResults);
	}
	else {
		std::cerr << "Invalid mode" << std::endl;
	}
	return 0;
}
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include "Algorithms.h"
#include "Generator.h"
struct FileData {
	int mode;
	int size;
	int algorithm;
	int amountSorted;
	int datatype;
};
/*
Structure of config file:
0	#mode:
10000	#size
0	#algorithm: 0-heapsort, 1-insertion sort, 2-quicksort
33	#amount: sorted in %
0	#data type: 0-int, 1-float
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
                fileData.datatype = value;
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
void performTest(FileData fileData) {
	Generator<int> generator;
	Sorter<int> sorter;
	int* A = nullptr;
	if (fileData.datatype == 0) {
		A = generator.generateRandomIntegerArray(fileData.size, fileData.amountSorted);
	}
	else if (fileData.datatype == 1) {
		A = generator.generateRandomFloatArray(fileData.size, fileData.amountSorted);
	}
	else {
		std::cerr << "Invalid data type" << std::endl;
		return;
	}
	if (fileData.datatype == 0) {
        switch (fileData.algorithm) {
        case 0:
            std::cout << "Heapsort:" << std::endl;
            sorter.heapSort(A);
            break;
		case 1:
			std::cout << "Insertionsort:" << std::endl;
			sorter.insertSort(A);
			break;
		case 2:
			std::cout << "Quicksort:" << std::endl;
			sorter.quickSort(A);
			break;
        }
		
	}
    else if (fileData.datatype == 1){
		std::cout << "Heapsort:" << std::endl;
		sorter.heapSort(A);
    }
}
void testMode() {
    
}
int main() {
	FileData fileData = loadFileData();
    performTest(fileData);
	return 0;
}
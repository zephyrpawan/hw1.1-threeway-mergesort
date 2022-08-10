/*
 * Author: Pawan Bhandari
 * Assignment Title: Homework 1.1: Three-way Merge Sort
 * Assignment Description: This program reads the list of integers from the input file provided by the user
 *      and sorts those integers using a three-way merge sort. Sorted list of integers is written to an output file
 *      named “sorted.txt.”
 * Due Date: 05/13/2012
 * Date Created: 05/10/2022
 * Date Last Modified:05/12/2022
 */

// Uses C++-2011 standard

/*
 * Data Abstraction:
 *       Output file stream is opened
 *       3 variables inputFileName, arrLength and inputArr are initialized
 *
 * Input:
 *       The program reads input as a string from the user as assigns it to inputFileName
 *
 * Process:
 *       Input file stream is opened to read from inputFileName
 *       Array named inputArr is created such that it contains integer from each line in inputFileName
 *       Sort inputArr by calling the threeWayMergeSort function
 *       Output file stream is opened to write sorted integers to a file named "sorted.txt"
 *
 * Output:
 *      Contents of input file are printed in the console
 *      Contents of sorted inputArr file are printed in the console
 *      Sorted list of integers are written to an output file named "sorted.txt"
 *
 * Assumptions:
 *      User only wants to sort one input file at a time
 *      User types input file name with extension and presses enter to sort
 *      Input file contains integer values only on each line (no special characters and whitespaces)
 */

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

//** function declarations
void threeWayMergeSort(int arr[], int start, int end);
void threeWayMerge(int arr[], int start, int end, int trisect1, int trisect2);
void printArray(int arr[], int arrLength, string arrName);
void writeToOutputFile(int arr[], string outputFileName);
string getFilenameFromUser();
int countLinesInFile(string inputFileName);
//**

int main()
{
    // reading input filename as entered by the user
    string inputFileName = getFilenameFromUser();

    // calculating the length of array
    int arrLength = countLinesInFile(inputFileName);

    // initializing empty array of length = number of lines in input file
    int inputArr[arrLength] = {};

    //**reading the contents of input file and saving as an inputArr[]
    ifstream inputFile(inputFileName);
    if(inputFile.is_open()) {
        string line;
        int lineNumber = 1;
        while(getline(inputFile, line)) {
            // handling cases where input file may have non integer values in a line
            try {
                inputArr[lineNumber - 1] = stoi(line);
                lineNumber++;
            } catch(std::invalid_argument&) {
                cout << "Invalid data in input file, please make sure input file contains integers only" << endl;
                return 1;
            }
        }
        inputFile.close();
    } else {
        cout << "Unable to open file :(" << endl;
    }
    //**

    // exit if array/file is empty
    if(arrLength == 0) {
        cout << "File is empty!" << endl;
        return 1;
    }

    // print input array before sorting
    printArray(inputArr, arrLength, " Input");

    // sort
    threeWayMergeSort(inputArr, 0, arrLength);

    // print input array after sorting
    printArray(inputArr, arrLength, "Sorted");

    //**writing sorted list to sorted.txt file
    ofstream outputData;
    outputData.open("sorted.txt");

    if(!outputData) {
        cerr << "Unable to open file!" << endl;
        exit(1);
    }
    cout << "Sorted list is written to the output the file sorted.txt" << endl << endl;

    for(int num : inputArr) {
        outputData << num << endl;
    }
    outputData.close();
    //**

    return 0;
}

//*************************************************************************************
// description: recursively splits an array into 3 parts and applies 3-way merge sort *
// return: void                                                                       *
// precondition: input array contains integer elements                                *
// postcondition: sorts an array                                                      *
//*************************************************************************************
void threeWayMergeSort(int arr[], int start, int end)
{
    // base case
    if(end - start < 2) {
        return;
    }

    // calculate split indexes and apply recursion for 3 array splits
    int trisect1 = start + (end - start) / 3;
    int trisect2 = start + ((end - start) / 3) * 2 + 1;
    threeWayMergeSort(arr, start, trisect1);
    threeWayMergeSort(arr, trisect1, trisect2);
    threeWayMergeSort(arr, trisect2, end);

    // merge
    threeWayMerge(arr, start, end, trisect1, trisect2);
}

//**********************************************************************************
// description: implements three-way merging of 3 sorted arrays                    *
// return: void                                                                    *
// precondition: input array contains integer elements                             *
// postcondition: merges arrays                                                    *
//**********************************************************************************
void threeWayMerge(int arr[], int start, int end, int trisect1, int trisect2)
{
    // initialize starting index for splitted arrays
    int leftStart = start;
    int midStart = trisect1;
    int rightStart = trisect2;

    // initialize an empty sorted array
    int sortedArr[end] = {};
    int sortedArrIndex = start;

    // compare splitted array elements as long as all are within bounds
    while((leftStart < trisect1) && (midStart < trisect2) && (rightStart < end)) {
        if(arr[leftStart] < arr[midStart]) {
            // case1: smallest elemnt in left array
            if(arr[leftStart] < arr[rightStart]) {
                sortedArr[sortedArrIndex] = arr[leftStart];
                leftStart++;
                sortedArrIndex++;
            }
            // case2: smallest element in right array
            else {
                sortedArr[sortedArrIndex] = arr[rightStart];
                rightStart++;
                sortedArrIndex++;
            }
        } else {
            // case3: smallest element in middle array
            if(arr[midStart] < arr[rightStart]) {
                sortedArr[sortedArrIndex] = arr[midStart];
                midStart++;
                sortedArrIndex++;
            }
            // case2: smallest element in right array
            else {
                sortedArr[sortedArrIndex] = arr[rightStart];
                rightStart++;
                sortedArrIndex++;
            }
        }
    }

    // When left array is merged but mid and right have values remaining
    while((midStart < trisect2) && (rightStart < end)) {
        if(arr[midStart] < arr[rightStart]) {
            sortedArr[sortedArrIndex] = arr[midStart];
            midStart++;
        } else {
            sortedArr[sortedArrIndex] = arr[rightStart];
            rightStart++;
        }
        sortedArrIndex++;
    }

    // When mid array is merged but left and right have values remaining
    while((leftStart < trisect1) && (rightStart < end)) {
        if(arr[leftStart] < arr[rightStart]) {
            sortedArr[sortedArrIndex] = arr[leftStart];
            leftStart++;
        } else {
            sortedArr[sortedArrIndex] = arr[rightStart];
            rightStart++;
        }
        sortedArrIndex++;
    }

    // When right array is merged but left and mid have values remaining
    while((leftStart < trisect1) && (midStart < trisect2)) {
        if(arr[leftStart] < arr[midStart]) {
            sortedArr[sortedArrIndex] = arr[leftStart];
            leftStart++;
        } else {
            sortedArr[sortedArrIndex] = arr[midStart];
            midStart++;
        }
        sortedArrIndex++;
    }

    //** copy any remaining elements from splitted arrays to the back of sorted array
    while(leftStart < trisect1) {
        sortedArr[sortedArrIndex] = arr[leftStart];
        leftStart++;
        sortedArrIndex++;
    }
    while(midStart < trisect2) {
        sortedArr[sortedArrIndex] = arr[midStart];
        midStart++;
        sortedArrIndex++;
    }
    while(rightStart < end) {
        sortedArr[sortedArrIndex] = arr[rightStart];
        rightStart++;
        sortedArrIndex++;
    }
    //**

    // copy all elements from sortedarray to input array
    for(int i = start; i < sortedArrIndex; i++) {
        arr[i] = sortedArr[i];
    }
}

//**********************************************************************************
// description: reads the full file name entered by user                           *
// return: string                                                                  *
// precondition: file exists on the same path as this code                         *
// postcondition: returns filename                                                 *
//**********************************************************************************
string getFilenameFromUser()
{
    string inputFileName = "";
    cout << "Please enter the input filename with extension (example: input.txt): ";
    cin >> inputFileName;
    cout << endl;

    return inputFileName;
}

//**********************************************************************************
// description: counts number of lines in a file                                   *
// return: int                                                                     *
// precondition: file exists on the same path as this code                         *
// postcondition: returns number of lines in the file                              *
//**********************************************************************************
int countLinesInFile(string inputFileName)
{
    ifstream inputFile(inputFileName);
    int lineCount = 0;
    if(inputFile.is_open()) {
        string line;
        while(getline(inputFile, line)) {
            lineCount++;
        }
        inputFile.close();
    } else {
        cout << "Unable to open file!" << endl;
    }
    return lineCount;
}

//***************************************************************
// description: prints the contents on an array to the console  *
// return: void                                                 *
// precondition: price and tax are valid                        *
// postcondition: none                                          *
//***************************************************************
void printArray(int arr[], int arrLength, string arrName)
{
    cout << arrName;
    cout << " List: {";
    for(int i = 0; i < arrLength; i++) {
        if(i == arrLength - 1) {
            cout << arr[i];
        } else {
            cout << arr[i];
            cout << ", ";
        }
    }
    cout << "}" << endl << endl;
}

/*
 * Web References:
 *      https://www.bgsu.edu/arts-and-sciences/computer-science/cs-documentation/writing-data-to-files-using-c-plus-plus.html
 *      https://www.w3schools.com/cpp/cpp_exceptions.asp
 *      https://www.youtube.com/watch?v=6pV2IF0fgKY&list=PLDN4rrl48XKpZkf03iYFl-O29szjTrs_O&index=33
 */
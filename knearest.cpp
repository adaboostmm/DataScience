/*
* This file provides the implementation for the K Nearest neighbor algorithm
* using two approaches; one approach uses Vector sort function to sort and another
* approach uses Priority Queue enqueue for sort and deque to pull k shortest distance
* out of priority queue.
* This program uses Stanford C++ libraries.
*/
#include "testing/SimpleTest.h"
#include <string>
#include "error.h"
#include "vector.h"
#include "strlib.h"
#include "grid.h"
#include <cmath>
#include "pqueue.h"
#include <iostream>
#include <fstream>
#include "filelib.h"
#include "map.h"
#include "set.h"

using namespace std;

/**
* @brief square2D helper function calculates the square of the difference
* between two numbers
* @param input are two numbers of type double.
* @return the square of the difference between those two numbers
*/
double square2D(double x, double a){
    return (x - a) * (x - a);
}

/**
* @brief pointToDistance helper function calculates the Eucidean distance between two
* multidimensional points
* @param input: training data as Grid, one test data point as vector, distance output as Map,
* containing label of training data with its distance from the test data point and all the
* distances of the test data point from all training data in the training data Grid.
* @return This function is of type void, as the main inputs are being passed as reference.
*/
void pointToDistance(Grid<double>& grid, Vector<double> point,
                     Map<int, Set<int>>& distanceMap, Vector<int>& allDistances){
    //Map<int, Set<double>> distanceMap;
    for (int r = 0; r < grid.numRows() ; r++){
        double intermediate = 0.0;
        for (int c = 0; c < grid.numCols() - 1; c++) {
            // Calculate Euclidean distance between the test point and all
            // training data points
            intermediate += square2D(grid[r][c], point[c]);
        }
        double finalDis = sqrt(intermediate);
        // Add the distances to a vector
        allDistances.add(finalDis);
       // Add the distances to a map with map key denoting label
        distanceMap[grid[r][grid.numCols()-1]].add(finalDis);
    }
    //cout << distanceMap << endl;
}

/**
* @brief buildDataGrid helper function reads a file and creates a multidimensional
* grid based on the number of rows and columns in the file.
* @param Reads the provided filename to build the grid object.
* @return It returns the grid of double.
*/
Grid<double> buildDataGrid(string& filename){
    ifstream in;
    if (!openFile(in, filename))
        error("cannot open file named "+ filename);

    Vector<string> lines;
    Grid<double> resultGrid;
    readEntireFile(in, lines);
    int numRows = lines.size();
    int numCols = stringSplit(lines[0], ",").size();
    // resize the grid with proper dimensions before populating it from the file
    resultGrid.resize(numRows, numCols);
    for ( int i = 0; i < lines.size() ; i++){
        string trainRow = lines[i];
        Vector<string> row = stringSplit(trainRow, ",");
        for (int c = 0; c <  row.size()  ; c++){
            resultGrid[i][c] = stringToReal(row[c]);
        }
    }
    return resultGrid;
}

/**
 * @brief getLabel helper function gets the label of the top K nearest distances
 * @param It takes distanceMap and distance as inputs where distance Map is a map that
 * contains the label as key with the set of distances as values.
 * distance is a vector that just contains the sorted distances
 * @return It returns the label index
 */
int getLabel(Map<int, Set<int>>& distanceMap, Vector<int> distance){
    Map<int, int> labelSet;
    int max = 0;
    int sum1 =0;
    int sum2 = 0;
    int index =0;

    // build a labelSet map with key as labels and values as how many
    // distances it's value set matches.
    for (int key: distanceMap){
        Set<int> output = distanceMap.get(key);
        for (int j =0; j < distance.size(); j++){
            if (output.contains(distance[j])){
                labelSet[key] += 1;
                output.remove(distance[j]);
            }
        }

        // get the label with majority vote
        // if there are ties, add the content and get the one with lowest content
        if ( labelSet[key] > max){
            Set<int> maxSet = distanceMap.get(key);// get the value of the map key with majority vote
            for (int i: maxSet){
                sum1 += i; // sum the contents of the map values
            }
            max = labelSet[key];// assign max to majority vote
            index = key;
        }
        else if ( labelSet[key] == max) {    // break the ties
            Set<int> maxSet = distanceMap.get(key);
            for (int i: maxSet){
                sum2 += i;
            }
            if (sum2 < sum1) {
                max = labelSet[key]; // keep that map key, whose values sum is less than the other
                index = key;
            }
        }
    }
    return index;
}
/**
 * @brief kNearestClassify predicts the labels of the test set by calling helper
 * functions pointToDistance and getLabel using KNN algorithm and it outputs both
 * the peredicted and actual values of the label for reference.
 *
 * It stores the final distance in a vector and uses vector sort to sort them and then
 * finds the top K nearest from sorted vector.
 *
 * @param It takes traing data and test datasets as multidimensional grid; it also
 * takes k as input which specifies how many nearest neighbor the program will find.
 * @return This function is of type void where training data grid is being passed as
 * reference. It outputs the predicted and actuial values oif the labels from the test
 * dataset.
 */
void kNearestClassify(Grid<double>& grid, Grid<double> testGrid, int k){
    if (k > grid.numRows()){
        error(" k is bigger than grid row size");
    }
    for (int r = 0; r < testGrid.numRows(); r++){
        Vector<double> testRow;
        for (int c = 0; c < testGrid.numCols() - 1; c++){
            testRow.add(testGrid[r][c]);
        }
        Vector<int> vecDistance;
        Vector<int> final;
        Map<int, Set<int>> distanceMap;
        pointToDistance(grid, testRow, distanceMap, vecDistance);
        // sort the array
        vecDistance.sort();
        // Take top k nearest
        for (int i =0; i< k ; i++){
            final.add(vecDistance[i]);
        }
        cout << r << ":-> " << "predicted-> " << getLabel(distanceMap, final) << " Actual-> " <<
                testGrid[r][testGrid.numCols() - 1] << endl;

    }
}

/**
 * @brief kNearestClassifyPQ is very similar to  kNearestClassify function above,
 * it predicts the labels of the test set by calling helper functions pointToDistance
 * and getLabel using KNN algorithm and it outputs both the peredicted and actual
 * values of the label for reference.
 *
 * kNearestClassifyPQ differs from kNearestClassify for storing the final distance as
 * it uses a priority queue to enqueue the values where the numbers get sorted automatically
 * based on the priority, then it dequeues K values from the priority queue.
 *
 * @param It takes traing data and test datasets as multidimensional grid; it also
 * takes k as input which specifies how many nearest neighbor the program will find.
 * @return This function is of type void where training data grid is being passed as
 * reference. It outputs the predicted and actuial values oif the labels from the test
 * dataset.
 */
void kNearestClassifyPQ(Grid<double>& grid, Grid<double> testGrid, int k){
    if (k > grid.numRows()){
        error(" k is bigger than grid row size");
    }
    for (int r = 0; r < testGrid.numRows(); r++){
        Vector<double> testRow;
        for (int c = 0; c < testGrid.numCols() - 1; c++){
            testRow.add(testGrid[r][c]);
        }
        Vector<int> vecDistance;
        Vector<int> final;
        Map<int, Set<int>> distanceMap;
        pointToDistance(grid, testRow, distanceMap, vecDistance);
        PriorityQueue<int> pq;
        // Once we enqueue priority queue is already sorted.
        for (int i =0; i < vecDistance.size(); i++){
            pq.enqueue(vecDistance[i], vecDistance[i]);
        }
        // Take top k nearest
        for (int i =0; i< k; i++){
            final.add(pq.dequeue());
        }
        cout << r << ":-> " << "predicted-> " << getLabel(distanceMap, final) << " Actual-> " <<
                testGrid[r][testGrid.numCols() - 1] << endl;

    }
}

/* * * * * Student Tests Below This Point * * * * */
STUDENT_TEST("square2D Test") {
    //Calculate difference of square of two numbers
    EXPECT_EQUAL(square2D(0,0), 0);
    EXPECT_EQUAL(square2D(5,3), 4);
    EXPECT_EQUAL(square2D(.3,-1), 1.69);
}

STUDENT_TEST("buildDataGrid for small training file") {
    //read a datafile and build a grid
    string simple_train = "res/simple_train.txt";
    Grid<double> trainDataS = buildDataGrid(simple_train);

    Grid<double> reference = {{8,9,1},
                              {10,8,1},
                             {6,8,8},
                              {5,9,3},
                              {7,7,8}};

    EXPECT_EQUAL(trainDataS, reference);
}

STUDENT_TEST("buildDataGrid for small test file") {
    //read a datafile and build a grid
    string simple_test = "res/simple_test.txt";
    Grid<double> testDataS = buildDataGrid(simple_test);

    Grid<double> reference = {{7.5,11,1}};
    EXPECT_EQUAL(testDataS, reference);
}

STUDENT_TEST("pointToDistance for test data from all the points in training file") {

    //Read the datafile and build the grid first
    string simple_train = "res/simple_train.txt";
    Grid<double> trainDataS = buildDataGrid(simple_train);

    //Calculate square root of Euclidean distance
    Map<int, Set<int>> distanceMap;
    Vector<int> allDistances;
    pointToDistance(trainDataS, {7.5,11} ,distanceMap,allDistances);
    Vector<int> allDistRef = {2, 3, 3, 3, 4};
    EXPECT_EQUAL(allDistances, allDistRef);
}

STUDENT_TEST("kNearestClassify for small train and test file") {
    //read a datafile and build a grid
    string simple_train = "res/simple_train.txt";
    string simple_test = "res/simple_test.txt";
    Grid<double> trainDataS = buildDataGrid(simple_train);
    Grid<double> testDataS = buildDataGrid(simple_test);
    kNearestClassify(trainDataS,testDataS, 3);
}

STUDENT_TEST("kNearestClassifyPQ for small train and test file") {
    //read a datafile and build a grid
    string simple_train = "res/simple_train.txt";
    string simple_test = "res/simple_test.txt";
    Grid<double> trainDataS = buildDataGrid(simple_train);
    Grid<double> testDataS = buildDataGrid(simple_test);
    kNearestClassifyPQ(trainDataS,testDataS, 3);
}

STUDENT_TEST("kNearestClassifyPQ for a large file end to end with TIME operation") {
    string trainFilename = "res/knearest_train.txt";
    Grid<double> trainData = buildDataGrid(trainFilename);
    string testFilename = "res/knearest_test.txt";
    Grid<double> testData = buildDataGrid(testFilename);
    Vector<int> allDistances;
    Map<int, Set<int>> distanceMap;

    cout << "***Run KNN with K=20 and compare predicted versus Actual *****" << endl;
    // Run K-nearest neighbor with 20 nearest neighb or and do the prediction
    kNearestClassify(trainData,testData, 20);

    cout << "***Second solution: Run KNN with K=20 and compare predicted versus Actual *****" << endl;
    // Run K-nearest neighbor with 20 nearest neighb or and do the prediction
    // Using second solution
    kNearestClassifyPQ(trainData,testData, 20);
    TIME_OPERATION(20, kNearestClassifyPQ(trainData,testData, 20));
    TIME_OPERATION(40, kNearestClassifyPQ(trainData,testData, 40));
    TIME_OPERATION(60, kNearestClassifyPQ(trainData,testData, 60));
    TIME_OPERATION(80, kNearestClassifyPQ(trainData,testData, 80));
    TIME_OPERATION(100, kNearestClassifyPQ(trainData,testData, 100));
    TIME_OPERATION(120, kNearestClassifyPQ(trainData,testData, 120));
    TIME_OPERATION(140, kNearestClassifyPQ(trainData,testData, 140));
    TIME_OPERATION(160, kNearestClassifyPQ(trainData,testData, 160));
    TIME_OPERATION(180, kNearestClassifyPQ(trainData,testData, 180));
}

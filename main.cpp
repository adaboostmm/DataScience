#include <iostream>
#include "console.h"
#include "testing/SimpleTest.h"
#include "strlib.h"
#include "grid.h"
#include "set.h"
#include "map.h"

using namespace std;
Grid<double> buildDataGrid(string& filename);
void kNearestClassify(Grid<double>& grid, Grid<double> testGrid,int k);
void kNearestClassifyPQ(Grid<double>& grid, Grid<double> testGrid,int k);
void pointToDistance(Grid<double>& grid, Vector<double> point,
                                      Map<int, Set<int>>& distanceMap, Vector<int>& allDistances);

int main()
{
        if (runSimpleTests(SELECTED_TESTS)) {
            return 0;
        }

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

    return 0;
}

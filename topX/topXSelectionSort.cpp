/* This program finds topX for small n using selection sort algorithm.
 * In selection sort, we find the smallest item in the liost and then swap it
 * with the left most unsorted element. While finding topX, instead of finding
 * the smallest element, the algorithm finds the largest element and sort trhe list in
 * descending order. Then it outputs topX, based on the value of X.
 * Worst performance is O(n^2).
 * Best performance is O(n^2).
 * But, if X is small, for large n, selection sort works fine.
 * This program uses Stanford C++ library.
*/
#include <iostream>
#include "vector.h"
#include "testing/SimpleTest.h"
#include "testing/TextUtils.h"

using namespace std;

void topXSelectionSort(Vector<int> &vec){
    for (int i =0; i< vec.size(); i++){
        int max = i;
        for (int j= i+1; j < vec.size(); j++){
            if (vec[j] > vec[max]){
                max = j;
            }
        }

        // swap the minimum
        if ( i != max){
            int temp = vec[i];
            vec[i] = vec[max];
            vec[max] = temp;
        }
    }
}

Vector<int> getTopX(int x, Vector<int> &vec){
    Vector<int> result;
    for (int i =0; i < x; i++){
        result.add(vec[i]);
    }
    return result;;
}

STUDENT_TEST("test selection sort"){
    Vector<int> vec = {3, 4, 2, 6, 7, 1};
    topXSelectionSort(vec);
    cout << vec << endl;
    cout << getTopX(2,vec)  << endl;
}

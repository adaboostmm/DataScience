/*
* This program finds the median from a vector of numbers. It uses min heap and max heap
* priority queue. First it enqueues the number into min heap; if size difference
* between min heap and max heap is more than 1, it dequeues from min heap and enqueues
* into max heap.
* At the end, if min heap has more element than max heap, it dequeues from
* min heap and output as Median. If max heap has more element than min heap,it dequeues
* from max heap and output as Median.If both min heap and max heap have same number of
* elements, it dequeues from min heap and max heap, take their average and output it as
* Median.
* This program uses Stanford C++ libraries. Stanford C++ library implements Priority
* Queue using min heap.
*/
#include <iostream>
#include "testing/SimpleTest.h"
#include "testing/TextUtils.h"
#include "vector.h"
#include "fstream"
#include "pqueue.h"
#include "set.h"

using namespace std;

double findMedian(Vector<double>& vec){
    PriorityQueue<double> minHeap;
    PriorityQueue<double> maxHeap;
    for(double val: vec){
        minHeap.enqueue(val, val);
        if (minHeap.size() - maxHeap.size() > 1){
            double num = minHeap.dequeue();
            maxHeap.enqueue(-num, -num);
        }
    }
    if (minHeap.size() > maxHeap.size()){
        return minHeap.dequeue();
    }
    else if ( minHeap.size() < maxHeap.size()){
        return (-maxHeap.dequeue());
    }
    return (minHeap.dequeue()-(maxHeap.dequeue()))/2;
}


STUDENT_TEST("Find Median on even"){
    Vector<double> v = {9,2,1,5,3,4,8,9,10};
    cout  << findMedian(v) << endl;
}

STUDENT_TEST("Find Median on odd"){
    Vector<double> v = {9,2,10,5,33};
    cout  << findMedian(v) << endl;
}

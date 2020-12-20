#include <iostream>
#include "testing/SimpleTest.h"
#include "testing/TextUtils.h"
#include <cmath>
#include "strlib.h"

using namespace std;
int KaratSuba(string strX, string strY, int n);

/**
 * @brief karatSubaRec
 * This program implement KaratSuba multiplication algorithm recursively.
 * For this implemebntation, it assumes that both x and y are of same length.
 * If both of the integers are of odd length, it adds a zero to eachj of the
 * numbers and at the end it divides the result by 100.
 * This program uses Stanford C++ library.
 * @param x is integer
 * @param y is integer
 * @return the product of x and y using the algorithm
 */

int karatSubaRec(int x, int y){
    int result = 0;
    string  xSplit =  integerToString(x);
    string  ySplit =  integerToString(y);

    if ( xSplit.length() != ySplit.length()){
        error("Length of the numbers are not the same");
    }
    if ( xSplit.length() == 1 && xSplit.length() == xSplit.length()){
        result = x * y;
    }
    else if ( xSplit.length() % 2 == 0){
            int n = xSplit.length();
            result = KaratSuba(xSplit, ySplit, n);
    }
    else if ( xSplit.length() % 2 != 0){
            int n = xSplit.length() + 1 ;
            xSplit = xSplit +'0';
            ySplit = ySplit + '0';
            result = KaratSuba(xSplit, ySplit, n);
            result = result/100;
    }
    return result;
}

/**
 * @brief KaratSuba function implements the meat of the algorithm.
 * Please refer tro Prof Tom Roughgarden's "Algorithms Illuminated"
 * book for KaratSuba multiplication algorithm reference. It implements
 * his pseudocode in C++.
 * @param strX
 * @param strY
 * @param n
 * @return
 */
int KaratSuba(string strX, string strY, int n){

    int a = stringToInteger(strX.substr(0, n/2));
    int b = stringToInteger(strX.substr(n/2));

    int c = stringToInteger(strY.substr(0, n/2));
    int d = stringToInteger(strY.substr(n/2));

    int p = a+b;
    int q = c+d;

    int left = karatSubaRec(a, c);
    int right = karatSubaRec(b, d);
    int middle = karatSubaRec(p, q);

    int adbc =  middle - left - right;
    return pow(10,n)*left+ pow(10, n/2)* adbc + right;
}


STUDENT_TEST("Test multipliocation several digits"){
    int result1 = karatSubaRec(26, 32);
    EXPECT_EQUAL(result1, 832);
    int result2 = karatSubaRec(2634, 3222);
    EXPECT_EQUAL(result2, 8486748);
    int result3 = karatSubaRec(2, 3);
    EXPECT_EQUAL(result3, 6);
    int result4 = karatSubaRec(0, 1);
    EXPECT_EQUAL(result4, 0);
    int result5 = karatSubaRec(223, 322);
    EXPECT_EQUAL(result5, 71806);
    EXPECT_ERROR(karatSubaRec(10, 1));
}

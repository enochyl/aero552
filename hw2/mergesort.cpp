//
//  main.cpp
//  mergesort
//
//  Created by Enoch Lee on 10/16/17.
//  Copyright © 2017 Enoch Lee. All rights reserved.
//

#include <iostream>
#include <ctime>
using namespace std;

void mergeSort(int* a, int l, int r);

int mergeSort(int* a, int n) {
    mergeSort(a,0,n-1);
    return 1;
}

void merge (int* a, int l, int m, int r) {
    int* temp = NULL;
    temp = new int[r-l+1];
    int i = l, j = m+1, k = 0;
    while (i <= m && j <= r)
        a[i] < a[j] ? (temp[k++] = a[i++]) : (temp[k++] = a[j++]);
    while (i <= m)
        temp[k++] = a[i++];
    while (j <= r)
        temp[k++] = a[j++];
    for (i = l; i <= r; i++)
        a[i] = temp[i-l];
    delete [] temp;
    temp = NULL;
}

void mergeSort(int* a, int l, int r) {
    int m = (l+r)/2;
    if (l < r) {
        mergeSort(a,  l,m);
        mergeSort(a,m+1,r);
        merge(a,l,m,r);
    }
}

int main() {
    int* testArray = NULL;
    srand(time(0)); // Randomized input
//    int n = 10; // 10 numbers
    int n = 100; // 100 numbers
//    int n = 1000; // 1000 numbers
    testArray = new int[n];
    printf("The unsorted array: \n");
    for (int i = 0; i < n; i++) {
        testArray[i] = rand() % 100 -50; // Negative numbers test case
//        testArray[i] = rand() % 100; // Positive numbers test case
        printf("%d ", testArray[i]);
    }
    printf("\n");
    mergeSort(testArray, n);
    printf("The sorted array: \n");
    for (int i = 0; i < n; i++) {
        printf("%d ", testArray[i]);
    }
    printf("\n");
    delete [] testArray;
    testArray = NULL;
    return 0;
}

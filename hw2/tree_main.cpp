//
//  tree_main.cpp
//  Tree
//
//  Created by Enoch Lee on 10/15/17.
//  Copyright © 2017 Enoch Lee. All rights reserved.
//

#include <iostream>
#include "Tree.h"
#include <ctime>
using namespace std;

int main() {
    srand(time(0)); // Randomized input
    int testCase = rand() %20 + 1;
    Tree testingTreeA, testingTreeB, testingTreeC;

    cout << "Number of TestTree1 elements (TestTree2 is randomized based on this value): " << testCase << endl;
    printf("\nTest Tree 1:\n");
    for (int i = 0; i < testCase; i++) {
        //int num = rand() % 1000;
        int num = rand() % 1000 - 500; // Negative test cases
        testingTreeA.insert(num);
    }

    cout << testingTreeA;
    int* arrayA = testingTreeA.toarray();
    printf("\nTesting toarray method...\n");
    for (int i = 0; i < testingTreeA.size(); i++)
        printf("%d ",arrayA[i]);
    printf("\n\nDeleting 10000, a number that is not in the set:\n");
    testingTreeA.erase(10000);
    printf("Resultant Test Tree 1:\n");
    cout << testingTreeA;
    printf("\nNothing happened (expected)\n");

    // Testing the default and copy constructor
    printf("\nTesting the copy constructor...\n");
    printf("Undefined test tree:\n");
    cout << testingTreeC;
    printf("(empty)\n");
    printf("Copying test tree 1 into an empty tree...\n");
    Tree testingTreeD(testingTreeA);
    cout << testingTreeD;


    printf("\n\nTest Tree 2:\n");
    for (int i = 0; i < testCase+rand()%50; i++) {
        int num = rand() % 1000;
        testingTreeB.insert(num);
    }
    cout << testingTreeB;

    // Testing if erase and member method are functioning normally
    string result;
    printf("\n\nInserting 500, a number that is (probably) not in the set:\n");
    testingTreeB.insert(500);
    printf("Resultant Test Tree 2:\n");
    cout << testingTreeB;
    printf("\nChecking if 500 exists in this set: ");
    result = (testingTreeB.member(500)) ? "True" : "False";
    cout << result;
    printf("\nDeleting the same number...\n");
    testingTreeB.erase(500);
    printf("Resultant Test Tree 2:\n");
    cout << testingTreeB;



    // Testing Intersection Method
    printf("\n\nInserted Intersection Numbers: ");
    for (int i = 0; i < testCase; i++) {
        int interNum = rand() % 1000 - 500;
        testingTreeA.insert(interNum);
        testingTreeB.insert(interNum);
        printf("%d ",interNum);
    }

    // toarray method is used inside unionOf and intersection method.
    Tree unionTree = testingTreeA.unionOf(testingTreeA, testingTreeB); // Union is a reserved word. UnionOf is used instead as the name of the function.
    Tree interTree = testingTreeA.intersection(testingTreeA, testingTreeB);

    cout << "\n\nUnion of Test tree A and Test tree B:\n" << unionTree;
    cout << "\n\nIntersection of Test tree A and Test tree B:\n" << interTree;

    printf("\n\nSize of unionTree: %d",unionTree.size());
    printf("\n\nSize of intersectTree: %d",interTree.size());

    printf("\n\n");
    return 0;
}

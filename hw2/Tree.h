//
//  Tree.h
//  Tree
//
//  Created by Enoch Lee on 10/15/17.
//  Copyright © 2017 Enoch Lee. All rights reserved.
//

#ifndef Tree_h
#define Tree_h

#include <iostream>
using namespace std;


struct treeNode {
    int data;
    treeNode *left;
    treeNode *right;
};

class Tree {
public:

    Tree();
    Tree(const Tree&);
    friend ostream& operator<<(ostream & os, const Tree & obj);
    void insert(int i);
    bool member(int i);
    int size();
    int* toarray();
    void erase(int i);
    Tree unionOf(Tree a, Tree b);
    Tree intersection(Tree a, Tree b);

private:

    treeNode* root;
    int* BSTArray;

    void printTree(ostream &os, treeNode* node) const;
    treeNode* copy(const treeNode* copyTree);
    void insert(int i, treeNode* node);
    int size(treeNode* root);
    int toarray(treeNode* node, int* arr, int i);
    treeNode* minValueNode(treeNode* root);
    treeNode* erase(treeNode* root, int i);
};

#endif /* Tree_h */

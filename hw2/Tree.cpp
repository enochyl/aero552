//
//  Tree.cpp
//  Tree
//
//  Created by Enoch Lee on 10/16/17.
//  Copyright © 2017 Enoch Lee. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include "Tree.h"
using namespace std;

Tree::Tree() {
    root = NULL;
}

Tree::Tree(const Tree& copyTree) {
    root = copy(copyTree.root);
}

treeNode* Tree::copy(const treeNode* copyTree) {
    if (copyTree == NULL)
        return NULL;
    treeNode* newNode = new treeNode;

    newNode -> data = copyTree -> data;

    newNode -> left = copy(copyTree -> left);
    newNode -> right = copy(copyTree -> right);
    return newNode;
}

void Tree::insert(int i) {
    if (root != NULL)
        insert(i,root);
    else {
        root = new treeNode;
        root -> data = i;
        root -> left = root -> right = NULL;
    }
}


bool Tree::member(int i) {
    treeNode* node = root;
    while (node != NULL) {
      if (node->data == i)
          return true;
      else {
        if (i > node -> data)
            node = node -> right;
        else
            node = node -> left;
      }
    }
    return false;
  }


int Tree::size() {
    if (root==NULL)
        return 0;
    else
        return(size(root->left) + size(root->right) + 1);
}

int* Tree::toarray() {
    int n = size();
    int *BSTArray = new int[n];
    toarray(root, BSTArray, 0);
    return BSTArray;
}


void Tree::erase(int i) {
    erase(root, i);
}


Tree Tree::unionOf(Tree a, Tree b) {
    Tree resultantTree;
    int* arrayA = a.toarray(); int* arrayB = b.toarray();

    for (int iter = 0; iter < a.size(); iter++)
        resultantTree.insert(arrayA[iter]);
    for (int iter = 0; iter < b.size(); iter++)
        resultantTree.insert(arrayB[iter]);

    return resultantTree;
}


Tree Tree::intersection(Tree a, Tree b) {
    Tree resultantTree;
    int* arrayA = a.toarray(); int* arrayB = b.toarray();
    int i = 0, j = 0;

    while (i < a.size() && j < b.size()) {
        if (arrayA[i] < arrayB[j])
            i++;
        else if (arrayB[j] < arrayA[i])
            j++;
        else {
            resultantTree.insert(arrayB[j++]);
        }
    }
    return resultantTree;
}


void Tree::insert(int i, treeNode *node) {
    if (i < node -> data) {
        if (node -> left != NULL)
            insert(i, node->left);
        else {
            node -> left = new treeNode;
            node -> left -> data = i;
            node -> left -> left = node -> left -> right = NULL;
        }
    }
    else if (i > node -> data) {
        if (node -> right != NULL)
            insert(i, node-> right);
        else {
            node -> right = new treeNode;
            node -> right -> data = i;
            node -> right -> right = node -> right -> left =  NULL;
        }
    }
};

int Tree::size(treeNode* root) {
    if (root == NULL)
        return 0;
    else
        return(size(root->left) + size(root->right) + 1);
}


void Tree::printTree(ostream& os, treeNode* node) const{
    if (node == NULL)
        return;
    printTree(os,node->left); // Traverse left then right
    printf("%d ", node->data);
    printTree(os,node->right);
}


ostream& operator<<(ostream& os, const Tree &obj) {
    obj.printTree(os, obj.root);
    return os;
}


int Tree::toarray(treeNode* node, int* arr, int i) {
    if(node == NULL)
        return i;
    if(node->left != NULL) // Traverse left then right
        i = toarray(node->left, arr, i);
    arr[i] = node->data;
    i++;
    if(node->right != NULL)
        i = toarray(node->right, arr, i);
    return i;
}


treeNode* Tree::minValueNode(treeNode* root) {
    treeNode* currentNode = root;
    while (currentNode->left != NULL)
        currentNode = currentNode->left;
    return currentNode;
}


treeNode* Tree::erase(treeNode* root, int i) {
    if (root == NULL)
        return root;
    if (i < root->data)
        root->left = erase(root->left, i);
    else if (i > root->data)
        root->right = erase(root->right, i);
    else {
        if (root -> left == NULL) {
            treeNode* temp = root -> right;
            return temp;
        }
        else if (root -> right == NULL) {
            treeNode* temp = root -> left;
            return temp;
        }

        treeNode* temp = minValueNode(root->right);
        root->data = temp->data;
        root->right = erase(root->right, temp->data);

    }
    return root;
}

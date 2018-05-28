//
//  dtree.cpp
//  Decision Tree ID3
//
//  Created by Enoch Lee on 11/15/17.
//  Copyright © 2017 Enoch Lee. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <ctime>
using namespace std;

class treeMatrix {
private:
    vector <vector <int> > Matrix;
    vector <int> Outputs;
    vector <int> uniqueOutputs;
public:
    treeMatrix() { };

    treeMatrix(string file) {
        ifstream data(file);
        string line;
        int item;
        vector <int> Row;
        // Label attributes 1 - 9, and 10 as the output
        Row.push_back(1); Row.push_back(2); Row.push_back(3); Row.push_back(4);
        Row.push_back(5); Row.push_back(6); Row.push_back(7); Row.push_back(8);
        Row.push_back(9); Row.push_back(10);
        Matrix.push_back(Row);
        Row.erase(Row.begin(),Row.end());
        while(!data.eof()) {
            getline(data, line);
            istringstream iss(line);
            while(iss >> item)
                Row.push_back(item);
            if(line.length()) {
                Matrix.push_back(Row);
                Row.erase(Row.begin(),Row.end());
            }
        }
        data.close();
    }


    int xlen() {
        return Matrix[0].size();
    }


    int ylen() {
        return Matrix.size();
    }


    bool isempty() {
        if (ylen() == 0 && xlen() == 0)
            return true;
        return false;
    }


    int element(int i, int j) {
        return Matrix[i][j];
    }


    void Print() {
        for(int i = 0; i < Matrix.size(); i++) {
            for(int j = 0; j < Matrix[0].size(); j++)
                cout << Matrix[i][j] << " ";
            printf("\n");
        }
    }


    void printVector(vector <int> vec) {
        for (vector<int>::const_iterator i = vec.begin(); i != vec.end(); ++i)
            cout << *i << ' ';
    }


    vector <int> getAttributes() {
        // Attributes are placed in the first row of the matrix
        vector <int> Attribute;
        for (int j = 0; j < Matrix[0].size()-1; j++)
            Attribute.push_back(Matrix[0][j]);
        return Attribute;
    }


    vector <int> getOutputs() {
        // Outputs are in the rightmost colomn of the matrix
        vector <int> Outputs;
        for (int i = 1; i < Matrix.size(); i++) {
            Outputs.push_back(Matrix[i][Matrix[0].size()-1]);
        }
        return Outputs;
    }


    vector <int> getUniques(vector <int> vec) {
        // Sort and return the unique vector
        sort(vec.begin(),vec.end());
        vec.erase(unique(vec.begin(),vec.end()), vec.end());
        return vec;
    }


    int getFrequentOutput (vector <int> Outputs) {
        vector <int> uniqueOutputs = getUniques(Outputs);
        int* count = new int[uniqueOutputs.size()]();
        // Using uniqueOutputs to find repeated values
        for (int i = 0; i < Outputs.size(); i++)
            for (int j = 0; j < uniqueOutputs.size(); j++)
                if (Outputs[i] == uniqueOutputs[j])
                    count[j] += 1;
        int index = 0;
        // Find the most frequent one
        for (int j = 0; j < uniqueOutputs.size(); j++)
            if (count[j] > count[index])
                index = j;
        delete[] count;
        return uniqueOutputs[index];
    }


    int getIndex(int attribute) {
        // Find the index of the attribute in the current tree matrix
        for (int j = 0; j < Matrix[0].size(); j++)
            if(Matrix[0][j] == attribute)
                return j;
        return -1;
    }


    map <int, vector<int> > getAttributeValues() {
        // Map the values of the attributes
        map <int, vector<int> > attribute_values;
        for (int j = 0; j < Matrix[0].size(); j++) {
            vector <int> individual_values;
            for (int i = 1; i < Matrix.size(); i++)
                individual_values.push_back(Matrix[i][j]);
            individual_values = getUniques(individual_values);
            attribute_values[Matrix[0][j]] = individual_values;
        }
        return attribute_values;
    }


    map <int, vector<int> > getAttributeOutputs(int attribute) {
        // For the possible outputs of a given attribute
        int index = getIndex(attribute);
        map <int, vector <int> > attribute_output;
        vector <int> attribute_values = getAttributeValues()[attribute];
        for (int iter = 0;  iter < attribute_values.size(); iter++) {
            vector <int> Row;
            for (int i = 1; i < Matrix.size(); i++)
                if (Matrix[i][index] == attribute_values[iter])
                    Row.push_back(Matrix[i][Matrix[0].size()-1]);
            attribute_output[attribute_values[iter]] = Row;
        }
        return attribute_output;
    }


    double computeEntropy(vector <int> Outputs) {
        vector <int> uniqueOutputs = getUniques(Outputs);
        double entropy = 0;
        // Zero entropy if size is zero
        if(uniqueOutputs.size() == 0)
            return 0;
        else {
            int* count = new int[uniqueOutputs.size()]();
            for (int i = 0; i < Outputs.size(); i++)
                for (int j = 0; j < uniqueOutputs.size(); j++)
                    if(Outputs[i] == uniqueOutputs[j])
                        count[j] += 1;
            double temp_entropy, P;
            for (int j = 0; j < uniqueOutputs.size(); j++) {
                if(count[j] == 0)
                    // Zero entropy if size is zero
                    temp_entropy = 0.;
                else {
                    P = (double)count[j]/(double)(Outputs.size());
                    temp_entropy = -P*log(P)/log(2.);
                }
                entropy += temp_entropy;
            }
            delete[] count;
        }
        return entropy;
    }


    void matrixReduction (treeMatrix Matrix_parent, int best_attribute, int subsetValue) {
        vector <int> Row;
        // Erase the current matrix to make space for matrix reduction based on parent matrix
        Matrix.erase(Matrix.begin(),Matrix.end());
        int index;
        for (int j = 0; j < Matrix_parent.xlen(); j++)
            if (Matrix_parent.element(0,j) != best_attribute)
                Row.push_back(Matrix_parent.element(0,j));
            else
                index = j;
        if (Row.size() != 0)
            Matrix.push_back(Row);

        for (int i = 1; i < Matrix_parent.ylen(); i++) {
            vector <int> Row;
            for (int j = 0; j < Matrix_parent.xlen(); j++)
                if (Matrix_parent.element(0,j) != best_attribute && Matrix_parent.element(i,index) == subsetValue)
                    Row.push_back(Matrix_parent.element(i,j));
            if(Row.size() != 0)
                Matrix.push_back(Row);
        }
    }
};


double computeInformationGain(treeMatrix Matrix, int attribute) {
    // Outputs of the original Matrix
    vector <int> Outputs = Matrix.getOutputs();
    // Associated entropy of the Outputs
    double entropy_n = Matrix.computeEntropy(Outputs);
    // Attribute values of the target attribute
    vector<int> attribute_values = Matrix.getAttributeValues()[attribute];
    // Associated ouputs of the target attribute
    map <int, vector<int> > attribute_output = Matrix.getAttributeOutputs(attribute);
    double entropy_np1, temp_entropy;
    for (int iter = 0; iter < attribute_values.size(); iter++) {
        vector <int> temp_output = attribute_output[attribute_values[iter]];
        temp_entropy = Matrix.computeEntropy(temp_output)*temp_output.size()/Outputs.size();
        entropy_np1 += temp_entropy;
    }
    return entropy_n - entropy_np1;
}


class dtree {

public:
    int node, branch;
    vector <dtree*> leaf;

    dtree(){
        node = -1;
        branch = -1;
    };


    dtree* buildTree (dtree* tree, treeMatrix Matrix) {
        // If tree is null, plant one (:
        if (tree == NULL)
            tree = new dtree();
        // If S is empty, return a single node with value Failure (-ve value);
        if (Matrix.isempty()) {
            tree -> node = -1;
            return tree;
        }
        // If S consists of records all with the same value for the target
        // attribute, return a single leaf node with that value;
        vector <int> uniqueOutputs = Matrix.getUniques(Matrix.getOutputs());
        if (uniqueOutputs.size() == 1) {
            tree -> node = uniqueOutputs[0];
            return tree;
        }
        // If R is empty, then return a single node with the value of the most
        // frequent of the values of the target attribute that are found in
        // records of S;
        if (Matrix.xlen() == 0) {
            int frequentOutput = Matrix.getFrequentOutput(Matrix.getOutputs());
            tree -> node = frequentOutput;
            return tree;
        }
        // Let A be the attribute with largest Gain(A,S) among attributes in R
        // Let {aj| j=1,2, .., m} be the values of attribute A;
        // Let {Sj| j=1,2, .., m} be the subsets of S consisting respectively
        // of records with value aj for A;
        // Return a tree with root labeled A and arcs labeled a1, a2, .., am
        // going respectively to the trees (ID3(R-{A}, C, S1), ID3(R-{A}, C, S2),
        // .....,ID3(R-{A}, C, Sm);
        // Recursively apply ID3 tosubsets {Sj| j=1,2, .., m}until they are empty

        // Finding the best attribute according to information gained
        double max_gain = -1, best_attribute = -1;
        vector <int> remaining_attributes = Matrix.getAttributes();
        for (int attri = 0; attri < remaining_attributes.size(); attri++) {
            double temp_gain = computeInformationGain(Matrix, remaining_attributes[attri]);
            if (temp_gain > max_gain) {
                max_gain = temp_gain;
                best_attribute = remaining_attributes[attri];
            }
        }
        // Based on the chosen "best attribute", populate the tree
        tree -> node = best_attribute;
        // Branches based on the chosen "best attributes"
        vector <int> subsetValues = Matrix.getAttributeValues()[best_attribute];
        treeMatrix newMatrix;
        // For each branch, recursively apply ID3
        for (int j = 0; j < subsetValues.size(); j++) {
            newMatrix.matrixReduction(Matrix, best_attribute, subsetValues[j]);
            dtree* newdtree = new dtree();
            newdtree -> branch = subsetValues[j];

            if (newMatrix.xlen() == 0)
                newdtree -> node = newMatrix.getFrequentOutput(newMatrix.getOutputs());
            else
                buildTree(newdtree, newMatrix);
            // Grow the tree
            tree -> leaf.push_back(newdtree);

        }
        return tree;
    }

    // Playing with spacing to print everything out. Recursively go through the decision tree.
    void printTree(int parent, dtree* t, int depth) {
        for(int i = 0; i < depth; i++) cout << "  ";
        if (t -> branch != -1) {
            cout << "x" << parent << "=" << t -> branch << endl;
            if (t -> leaf.size() == 0)
                for (int i = 0; i < depth; i++) cout << "  ";
        }
        if (t -> leaf.size() == 0)
            cout << "  return " << t -> node << endl;
        for(int i = 0; i < t -> leaf.size(); i++)
            printTree(t -> node, t->leaf[i], depth+1);
    }
};


dtree* buildDecisionTree(string file) {
    dtree* tree;
    treeMatrix startingMatrix(file);
    tree = tree -> buildTree(tree, startingMatrix);
    return tree;
}


void printTree(dtree* t) {
    t -> printTree(0,t, -1);
}


vector <int> input2vector (int* input) {
    vector <int> inputVector;
    for (int i = 0; i < 9; i++)
        inputVector.push_back(input[i]);
    return inputVector;
}


int traverseTree(dtree* t, vector <int> inputVector) {
    // Recursive traverse the dtree given an input. Returns the final node value based on the dtree and input.
    for (int k = 1; k <= 9; k++)
        if ( t -> leaf.size() == 0 && (t-> node == k))
            return k;
    for (int i = 0; i < inputVector.size(); i++)
        if (t -> node == i+1)
            for (int j = 0; j < t -> leaf.size(); j++)
                if ((t->leaf[j])->branch == inputVector[i])
                    return traverseTree(t -> leaf[j], inputVector);
    return -1;
}


int callTree(dtree* t, int* input) {
    vector <int> inputVector = input2vector(input);
    return traverseTree(t, inputVector);
}


int main() {

  // training sets generated randomly by MATLAB
  //    dtree* tree = buildDecisionTree("dtree1.txt"); // Random Example
  //    dtree* tree = buildDecisionTree("dtree2.txt"); // Repeated
  //    dtree* tree = buildDecisionTree("dtree3.txt"); // Training set of 10
      dtree* tree = buildDecisionTree("dtree4.txt"); // Training set of 100
  //    dtree* tree = buildDecisionTree("dtree5.txt"); // Training set of 1000
  //    dtree* tree = buildDecisionTree("dtree6.txt"); // Training set of 10000


      printf("Decision Tree (ID3): \n\n");
      printTree(tree);
      printf("\n");

      srand(time(0)); // Randomized inputs
      int* input = NULL;
      int n = 9;
      input = new int [n];
      printf("Random input:\n");
      for (int i = 0; i < n; i++) {
          input[i] = rand() % 9 + 1;
          printf("%d ", input[i]);
      }

      int output = callTree(tree, input);
      printf("\n\nOutput: ");
      cout << output;
      if (output < 0)
          cout << " (Failure)" << endl << endl;
      else
          cout << endl << endl;
      return 0;
}

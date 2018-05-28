#include <iostream>
#include <vector>
#include <queue>
#define SIZE 1000

using namespace std;

struct MinHeapNode {
    int data;
    int weight;
    MinHeapNode *left, *right;

    MinHeapNode(int data, int weight) {
        this->data = data;
        this->weight = weight;
        left = right = NULL;
    }
};

struct compare {
    bool operator()(MinHeapNode* l, MinHeapNode* r) {
        return (l -> weight > r -> weight);
    }
};

void printCodes(struct MinHeapNode* root, string str) {
    if (!root)
        return;
    if (root -> data != '$')
        cout << root->data << ": " << str << endl;

    printCodes(root->left, str + "0");
    printCodes(root->right, str + "1");
}

void huffman(int data[], int weight[]) {
    MinHeapNode *left, *right, *top;

    priority_queue<MinHeapNode*, vector<MinHeapNode*>, compare> minHeap;
    for (int i = 0; i < SIZE; i++)
        minHeap.push(new MinHeapNode(data[i],weight[i]));

    while (minHeap.size() != 1) {
        left = minHeap.top();
        minHeap.pop();
        right = minHeap.top();
        minHeap.pop();

        top = new MinHeapNode('$', left->weight + right->weight);
        top -> left = left;
        top -> right = right;
        minHeap.push(top);
    }
    printCodes(minHeap.top(), "");
}

int main(int argc, const char * argv[]) {

    FILE *f = fopen("huffman.txt", "r");
    char *a = new char[100];

    int *code = new int[SIZE];
    int *weight = new int[SIZE];

    int idx = 0;

    while (fgets(a,100,f)) {
        int n = atoi(a);
        code[idx] = idx+1;
        weight[idx] = n;
        idx++;
    }
    fclose(f);
    huffman(code, weight);

    return 0;

}

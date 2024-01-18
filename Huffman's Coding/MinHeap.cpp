#include "MinHeap.h"

// Constructor
MinHeap::MinHeap(int Maxsize)
{
    data = new Node*[Maxsize];
    leaves = new LeafNode*[Maxsize];
}

// Destructor
MinHeap::~MinHeap()
{
    delete[] data;
}

void MinHeap::insert(unsigned char inputCharater) 
{
    // check if the character already exists in a node
    for (int i = 0; i < heapSize; i++)
    {
        if (leaves[i]->character == inputCharater) 
        {
            leaves[i]->frequency++;
            return;
        }
    }

    // Make a new node
    LeafNode *newNode = new LeafNode(inputCharater);
    leaves[heapSize] = newNode;
    data[++heapSize]=newNode;
    minHeapify();
}

void MinHeap::insert(SubTreeNode *inputNode)
{
    data[++heapSize] = inputNode;
    minHeapify();
}

void MinHeap::minHeapify(Node *arr[],int size,int i) {
    int lChild = i*2;   // index for left child
    int rChild = i*2 + 1;   // index for right child

    if(i!= 1 && arr[i]->frequency < arr[i/2]->frequency)  //if i is not equal to 1 and child's frequency is less than parent's frequency
    {
        Node* temp = arr[i];  //swap
        arr[i] = arr[i/2];
        arr[i/2] = temp;
    }

    if(lChild <= size)
        minHeapify(arr,size,lChild);    //recursive call on left child
    if(rChild <= size)
        minHeapify(arr,size,rChild);    // recursive call on right child
}

Node* MinHeap::popMin() // returns the root node and removes it from the tree
{
    Node* temp = data[1]; // stores the top element
    data[1] = data[heapSize--]; // swaps last with first
    minHeapify();  // heapify
    return temp;    // return the previously top element
}


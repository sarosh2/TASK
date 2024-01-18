//
// Created by dell on 1/26/2021.
//

#ifndef HUFFMAN_S_CODING_MINHEAP_H
#define HUFFMAN_S_CODING_MINHEAP_H

#include <iostream>
#include "HeapNodes.h"

class MinHeap {
public:
    int heapSize = 0;

private:
    Node **data;
    LeafNode **leaves;

    // Methods
    void minHeapify(Node *arr[],int size,int i = 1);


public:
    MinHeap(int Maxsize);   // constructor
    ~MinHeap();     // destructor
    void insert(unsigned char inputCharater); // insert a new node in the heap
    void insert(SubTreeNode *inputNode); // insert a new node in the heap
    Node* popMin(); // returns the root node and removes it from the tree

    void debug_printHeap()
    {
        for (int i = 0; i < heapSize; i++)
        {
            std::cout << "c: " << leaves[i]->character << " f: " << leaves[i]->frequency << ": " << i << std::endl;
        }
    }

    // overloaded funtions for simplicity
    void minHeapify() {minHeapify(data,heapSize);}

};

#endif //HUFFMAN_S_CODING_MINHEAP_H


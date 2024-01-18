//
// Created by dell on 1/26/2021.
//

#ifndef HUFFMAN_S_CODING_TEXTDECOMPRESSOR_H
#define HUFFMAN_S_CODING_TEXTDECOMPRESSOR_H
#include <iostream>
#include <cmath>
#include "LinkedList.h"
#include "MinHeap.h"
#include "Queue.h"

class TextDecompressor
{
    FILE* inputFile;
    FILE* outputFile;
    Node* root;//root node of huffman tree
    long long int orignalFileSize = 0;
    long long int decompressedFileSize = 0;
    int chunkSize=1;
    int bitsToSkip = 0;

    ListNode<unsigned char>* makeHuffmansTree(ListNode<unsigned char>* charNode, Node** pTreeNode);
    void decryptAndReadTree();
    void decryptAndWriteText();
    void createOutputFile(const char* path);
    bool openInputFile(const char* path);
    void writeToFile(unsigned char c);
    long long int calculateFileSize(FILE *file);
    void calculatePaddingSize();
    void makeProgressBar(int state);
    void destroyHuffmanTree(Node *temp);

public:
    TextDecompressor();
    void decompress(const char *inputPath, const char* patch);
};

#endif //HUFFMAN_S_CODING_TEXTDECOMPRESSOR_H


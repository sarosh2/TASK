#ifndef HUFFMAN_S_CODING_TEXTCOMPRESSOR_H
#define HUFFMAN_S_CODING_TEXTCOMPRESSOR_H

#include <iostream>
#include <cmath>
#include "MinHeap.h"
#include "LinkedList.h"
#include "HashTable.h"
#include "Queue.h"



class TextCompressor 
{
    FILE *inputFile;
    MinHeap *heap;//used to create huffman tree
    HashTable* hTable;
    FILE *outputFile;
    Node *root;//root node of huffman tree
    int chunkSize = 1; // number of characters to be written or read at once
    long long int orignalFileSize = 0;
    long long int compressedFileSize = 0;
    bool fileIsValid = true;

    void makeHashTable(Node* node, std::string binaryString);
    LinkedList<bool>* makeLinkedListFromString(std::string binaryString);
    void convertHuffmanTree(LinkedList<unsigned char>* tree, Node* node);
    void makeHuffmansTree();
    void encryptAndWriteTree();
    void encryptAndWriteText();
    bool openInputFile(const char* path);
    void createOutputFile(const char *patch);
    void writeToFile(unsigned char c);
    long long int calculateFileSize(FILE *file);
    void makeProgressBar(int state);
    void destroyHuffmanTree(Node *temp);

public:
    TextCompressor();
    void compress(const char *inputPath, const char *patch);

};


#endif //HUFFMAN_S_CODING_TEXTCOMPRESSOR_H

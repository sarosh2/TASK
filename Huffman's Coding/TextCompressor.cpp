#include "TextCompressor.h"

using namespace std;

TextCompressor::TextCompressor()
{
    heap = new MinHeap(256);
    hTable = new HashTable();
    root = NULL;
    inputFile = NULL;
    outputFile = NULL;
}

LinkedList<bool>* TextCompressor::makeLinkedListFromString(std::string binaryString)
{
    LinkedList<bool>* binaryList = new LinkedList<bool>();
    for (char c : binaryString)
    {
        if(c == '0')
            binaryList->insertAtEnd(false);
        else
            binaryList->insertAtEnd(true);
    }

    return binaryList;
}

void TextCompressor::makeHashTable(Node* node, std::string binaryString)
{
    unsigned char c = node->getCharater();

    if (c != NULL)
    {
        hTable->insertCode(c, makeLinkedListFromString(binaryString));
        return;
    }

    Node* leftChild = node->getLeft();
    Node* rightChild = node->getRight();

    if (leftChild != NULL)
        makeHashTable(leftChild, binaryString + "0");
    if (rightChild != NULL)
        makeHashTable(rightChild, binaryString + "1");
}

void TextCompressor::makeHuffmansTree() 
{
    // making an arry of the chunksize to store the characters before writing
    unsigned char *characters = new unsigned char[chunkSize];
    int charactersRead = 0;

    // keep reading chracters into the character array till the number of chracters read at once is less than chunksize
    do
    {
        charactersRead = fread(characters,1,chunkSize,inputFile);
        for(int i=0;i<charactersRead;i++)// push each character into the heap
        {   
            if(characters[i] > 255 || characters[i] == 0)
            {
                cout << endl << "INVALID FILE FORMAT: The file contains characters outside the range of values 1 - 255." << endl;
                fileIsValid=false;
                return;
            }
            heap->insert(characters[i]);
        }
    }while (charactersRead==chunkSize);

    /* combine the smallest two nodes into a subtree node, and inserting it back into the heap
          until one node is left */
    while (heap->heapSize > 1) {
        Node *left = heap->popMin();
        Node *right;
        int frequency = 0;
        right = heap->popMin();
        frequency += right->frequency;

        frequency += left->frequency;
        SubTreeNode *newNode = new SubTreeNode(frequency, left, right);
        heap->insert(newNode);
    }
    // get the last node, it is the root of our huffman's tree
    root = heap->popMin();
}

unsigned char bitsToChar(bool* arr, int start)
{
    unsigned char ascii = 0;//ascii value of given byte

    //calculate ascii value of given byte
    for (int i = start; i < start + 8; i++)
        ascii += pow(2, 7 - (i - start)) * arr[i];

    return ascii;
}

bool* intToBits(int number,int size)
{
    bool *arr = new bool[size];
    //converting decimal to byte
    for (int i = size-1; i >= 0; i--)
    {
        arr[i] = number % 2;
        number /= 2;
    }
    return arr;
}


long long int TextCompressor::calculateFileSize(FILE *file)
{
    fseek(file, 0L, SEEK_END);
    long long int size = ftell(file);
    fseek(file, 0L, SEEK_SET);
    return  size;
}

void TextCompressor::convertHuffmanTree(LinkedList<unsigned char>* treeInLinkedList, Node* node)
{
    if (node != NULL)//base case occurs when node becomes NULL
    {
        char character = node->getCharater();
        if (character != NULL)
        {
            treeInLinkedList->insertAtEnd('1');
            treeInLinkedList->insertAtEnd(character);
        }
        else
        {
            treeInLinkedList->insertAtEnd('0');
        }
        convertHuffmanTree(treeInLinkedList, node->getLeft());
        convertHuffmanTree(treeInLinkedList, node->getRight());
    }
}

bool TextCompressor::openInputFile(const char* path)
{
        if ((inputFile = fopen(path, "rb")))
            return true;

        cout<<"Could Not open File: File does not exists."<<endl;
        return false;
}

void TextCompressor::createOutputFile(const char *path)
{
    try
    {
        outputFile = fopen(path,"wb");
    }
    catch (...)
    {
        cout << "Could not open output file" << std::endl;
        return;
    }
}


void TextCompressor::encryptAndWriteTree()
{
    LinkedList<unsigned char>* huffTreeList = new LinkedList<unsigned char>();
    convertHuffmanTree(huffTreeList, root);//converting huffman tree to character string

    int listSizeBitLength = (int)log2(huffTreeList->size) + 1;//how many bits it would take to store the tree size

    if (listSizeBitLength % 8 != 0)
        listSizeBitLength += (8 - listSizeBitLength % 8);//converting the number into a multiple of 8 by adding padding

    bool* listSizeBits = intToBits(huffTreeList->size, listSizeBitLength);//convert size of tree into a binary number

    //write the size of tree into file
    for (int i = 0; i < listSizeBitLength; i+= 8)
        writeToFile(bitsToChar(listSizeBits, i));

    huffTreeList->insertAtFront(255);//add character to show beginning of tree

    //write tree into file
    ListNode<unsigned char>* tempChar = huffTreeList->start;
    while (tempChar != NULL)
    {
        fwrite(&tempChar->data, sizeof(char), 1, outputFile);
        tempChar = tempChar->next;
    }
}

void TextCompressor::encryptAndWriteText()
{

    // array to read characters into
    char* characters = new char[chunkSize];
    // queue to keep the bits till we can make a byte
    Queue binaryQueue;
    int charactersRead=0;
    bool* binaryArray = new bool[8];

    do
    {   // read a chunk of characters
        charactersRead = fread(characters, 1, chunkSize, inputFile);
        // temporary node to get the binary code for the character from hash table
        for(int i=0;i<charactersRead;i++)
        {
            ListNode<bool> *llnode = hTable->getCode(characters[i])->start;
            // reading the code and enQueueing it in the binaryQueue

            while (llnode != NULL) {
                binaryQueue.enQueue(llnode->data);
                llnode = llnode->next;
            }
        }
        // if the Queue size gets bigger than 8, we can write a byte to the file

        while(binaryQueue.size>8) 
        {
            // dequeueing 8 bits into an array for bits to char;
            for (int i = 0; i < 8; i++)
                binaryArray[i] = binaryQueue.deQueue();

            unsigned char c = bitsToChar(binaryArray, 0);
            writeToFile(c);
        }
    }while (charactersRead == chunkSize);

    // queue still has bits in it, so we pad them with zeros
    // and store the padding size in the last 3 bits
    int paddingSize = 8 - binaryQueue.size;
    // if the space in the last character is less than 3, we need another byte

    if(paddingSize < 3)
        paddingSize += 8;

    // enqueueing the zeros
    for(int i = 0;i<paddingSize-3;i++)
        binaryQueue.enQueue(0);

    // enqueueing the size in binary
    bool *paddingSizeBinary = intToBits(paddingSize-3,3);
    for(int i = 0;i<3;i++)
        binaryQueue.enQueue(paddingSizeBinary[i]);

    // dequeue the last byte/bytes and writing them to the file
    bool* temp = new bool[8];
    for(int i = 0;!binaryQueue.isEmpty();i = (++i)%8)
    {
        temp[i] = binaryQueue.deQueue();
        if(i==7)
        {
            writeToFile(bitsToChar(temp, 0));
        }
    }
    delete[] binaryArray;
    delete[] characters;
}

void TextCompressor::writeToFile(unsigned char c)
{
    fwrite(&c,sizeof(char), 1, outputFile);
}

void TextCompressor::makeProgressBar(int state) {
    /*state 0: File Loaded
      state 1: Huffman Tree Generated
      state 2: Hash Table Generated
      state 3: Huffman Tree Stored Successfully
      state 4: Compression Finished Successfully
    */

    int progress;
    string message;

    switch (state)
    {
        case 0:
            message = "File Loaded.";
            progress = 5;
            break;

        case 1:
            message = "Huffman Tree Generated.";
            progress = 10;
            break;

        case 2:
            message = "Huffman Tree Stored Successfully.";
            progress = 20;
            break;

        case 3:
            message =  "Compression Finished Successfully.";
            progress = 100;
            break;

        default:
            cout << "Error. Current State " << state << " Not Identified." << endl;
            return;
    }


    int maxbar = 20;//max length of progress bar
    int barLength = progress * maxbar / 100;//determine the length of the progressbar

    system("CLS");
    cout << "Progress: " << progress << "%" << endl;
    for (int i = 0; i < barLength; i++)
        cout << (unsigned char)254;

    for (int i = 0; i < maxbar - barLength; i++)
        cout << '-';
    cout << endl;
    cout<<message<<endl;

}

void TextCompressor::destroyHuffmanTree(Node *temp)
{
    if(temp->getCharater() == NULL)
    {
        delete temp;
        return;
    }
    destroyHuffmanTree(temp->getLeft());
    destroyHuffmanTree(temp->getRight());
    delete temp;
}
void TextCompressor::compress(const char *inputPath, const char *outputPath)
{
    //compress files
    if(!openInputFile(inputPath))
        return;
    orignalFileSize = calculateFileSize(inputFile);
    chunkSize= sqrt(orignalFileSize);
    makeProgressBar(0);

    makeHuffmansTree();
    if(!fileIsValid)
    {
        fclose(inputFile);
        fclose(outputFile);
        return;
    }
    makeProgressBar(0);

    makeHashTable(root,"");
    makeProgressBar(1);

    createOutputFile(outputPath);
    fseek(inputFile,0L,SEEK_SET);// going back to start of input file
    encryptAndWriteTree();
    makeProgressBar(2);

    encryptAndWriteText();
    makeProgressBar(3);
    compressedFileSize = ftell(outputFile);
    fclose(inputFile);
    fclose(outputFile);
    destroyHuffmanTree(root);
    hTable->emptyTable();
    //print details of the compression
    //details printed
    cout << endl << "Details:" << endl;
    cout << "Original File Size: \t" << orignalFileSize << " Bytes" << endl;
    cout << "Compressed File Size: \t" << compressedFileSize << " Bytes" << endl;
    cout << 100 - (100 * (long long int)compressedFileSize / (long long int)orignalFileSize) << " % decrease" << endl;
}
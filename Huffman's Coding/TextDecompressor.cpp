#include "TextDecompressor.h"

using namespace std;

TextDecompressor::TextDecompressor()
{
    root = NULL;
    inputFile = NULL;
    outputFile = NULL;
}

ListNode<unsigned char>* TextDecompressor::makeHuffmansTree(ListNode<unsigned char>* charNode, Node** pTreeNode)
{
    unsigned char character = charNode->data;
    if (character == '0')//if current node is empty
    {
        Node* lChild = NULL;
        charNode = makeHuffmansTree(charNode->next, &lChild);//create left sub tree

        Node* rChild = NULL;
        charNode = makeHuffmansTree(charNode->next, &rChild);// create right sub tree

        *pTreeNode = new SubTreeNode(0, lChild, rChild);//create node
    }
    else//if node is character node
    {
        charNode = charNode->next;
        *pTreeNode = new LeafNode(charNode->data);
    }
    return charNode;
}

void TextDecompressor::decryptAndReadTree()
{
    LinkedList<unsigned char>* huffTreeList = new LinkedList<unsigned char>();//tree in linked list form
    LinkedList<unsigned char>* huffTreeSize = new LinkedList<unsigned char>();//tree size in linked list form

    //read from file and store in linked list
    unsigned char tempChar = NULL;
    fread(&tempChar, sizeof(char), 1, inputFile);

    //read tree size from file
    while (tempChar != 255)
    {
        huffTreeSize->insertAtEnd(tempChar);
        fread(&tempChar, sizeof(char), 1, inputFile);
    }

    //convert size from string of characters to integer
    int treeSize = 0;//size of huffman tree
    int byteNumber = huffTreeSize->size;//byte number of current list element, MSB at highest number

    ListNode<unsigned char>* tempNode = huffTreeSize->start;
    while (byteNumber != 0)
    {
        treeSize += pow(256, byteNumber - 1) * tempNode->data;
        byteNumber--;
        tempNode = tempNode->next;
    }

    //read tree using size calculated
    for (int i = 0; i < treeSize; i++)
    {
        fread(&tempChar, sizeof(char), 1, inputFile);
        huffTreeList->insertAtEnd(tempChar);
    }

    //use linked list to create huffman tree
    makeHuffmansTree(huffTreeList->start, &root);
}

void TextDecompressor::createOutputFile(const char *path)
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

bool TextDecompressor::openInputFile(const char* path)
{
    if ((inputFile = fopen(path, "rb")))
        return true;

        cout<<"Could Not open File: File does not exists."<<endl;
    return false;
}

bool* charToBits(unsigned char character)
{
    bool* arr = new bool[8];
    //converting decimal to byte
    for (int i = 7; i >= 0; i--)
    {
        arr[i] = character % 2;
        character /= 2;
    }
    return arr;
}

int bitsToInt(bool* arr, int start, int size)
{
    int toReturn = 0;
    for (int i = start; i < size; i++)
        toReturn += pow(2, (size - 1) - i) * arr[i];

    return toReturn;
}

long long int TextDecompressor::calculateFileSize(FILE *file)
{
    fseek(file, 0L, SEEK_END);
    long long int size = ftell(file);
    fseek(file, 0L, SEEK_SET);
    return  size;
}

void TextDecompressor::decryptAndWriteText()
{
    // making an array to read charaters into
    unsigned char* characters = new unsigned char[chunkSize];
    int charactersRead = 0;
    // a temp tree node to traverse the tree
    Node* tempTreeNode = root;//current node of tree
    // a queue for the read bits
    Queue q;

    do
    {   // reads a chunk of characters
        charactersRead = fread(characters, sizeof(char), chunkSize, inputFile);
        for(int i=0;i<charactersRead;i++)
        {
            bool* binArr = charToBits(characters[i]);//convert each character to binary
            for (int i = 0; i < 8; i++)
                q.enQueue(binArr[i]);   // enqueue each bit of the character
            while(q.size>16)    // only dequeue when the size is bigger than 16 so we dont dequeue the last 2 bytes
            {
                unsigned char temp;
                //if character reached, reset to root and write character in file
                if ((temp = tempTreeNode->getCharater())!= NULL)
                {
                    writeToFile(temp);
                    tempTreeNode = root;
                }
                //traverse through tree according to binary code
                if (q.deQueue())
                    tempTreeNode = tempTreeNode->getRight();
                else
                    tempTreeNode = tempTreeNode->getLeft();
            }
            delete[] binArr;
        }
    }while(charactersRead == chunkSize);    // if less characters are read, that means end of file

    while(q.size >= bitsToSkip) // keep traversing the tree until the padded bits
    {
        unsigned char temp;
        //if character reached, reset to root and write character in file
        if ((temp = tempTreeNode->getCharater())!= NULL)
        {
            writeToFile(temp);
            tempTreeNode = root;
        }
        //traverse through tree according to binary code
        if (q.deQueue())
            tempTreeNode = tempTreeNode->getRight();
        else
            tempTreeNode = tempTreeNode->getLeft();
    }

}

void TextDecompressor::calculatePaddingSize()
{
    // go to last character
    fseek(inputFile,-1,SEEK_END);
    // read last character
    unsigned char lastCharacter;
    fread(&lastCharacter,1,1,inputFile);
    // convert last character to binary
    bool *lcBinary = charToBits(lastCharacter);
    // last 3 bits tell us the padding size, we have to skip them and the padding
    bitsToSkip = bitsToInt(lcBinary,5,8 ) + 3;
    // go back to start of file;
    fseek(inputFile,0,SEEK_SET);
}

void TextDecompressor::writeToFile(unsigned char c)
{
    fwrite(&c,sizeof(c),1, outputFile);
}

void TextDecompressor::makeProgressBar(int state)
{
    /*state 0: File Loaded
      state 1: Huffman Tree Loaded
      state 2: Output File Created
      state 3: Decompression Finished Successfully
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
            message = "Huffman Tree Loaded.";
            progress = 10;
            break;

        case 2:
            message = "Output File Created.";
            progress = 20;
            break;

        case 3:
            message =  "Decompression Successful.";
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
void TextDecompressor::destroyHuffmanTree(Node *temp)
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

void TextDecompressor::decompress(const char *inputPath, const char *outputPath)
{
    //decompress file and print progress
    if(!openInputFile(inputPath))
        return;
    makeProgressBar(0);

    calculatePaddingSize();
    orignalFileSize = calculateFileSize(inputFile);
    chunkSize = sqrt(orignalFileSize);
    decryptAndReadTree();
    makeProgressBar(1);

    createOutputFile(outputPath);
    makeProgressBar(2);
    decryptAndWriteText();
    decompressedFileSize = ftell(outputFile);
    makeProgressBar(3);

    fclose(inputFile);
    fclose(outputFile);
    destroyHuffmanTree(root);

    //details printed
    cout << endl << "Details:" << endl;
    cout << "Original File Size: \t" << orignalFileSize << " Bytes" << endl;
    cout << "Compressed File Size: \t" << decompressedFileSize << " Bytes" << endl;
    cout << -100 + (100 * (long long int)decompressedFileSize / (long long int)orignalFileSize) << " % increase" << endl;
}
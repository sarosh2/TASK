#include "HashTable.h"

HashTable::HashTable() {
    table = new LinkedList<bool>* [maxChars];//characters that can be assigned a binary code

    //initializing each linkedlist
    for (int i = 0; i < maxChars; i++)
        table[i] = NULL;

}


void HashTable::insertCode(unsigned char character, LinkedList<bool>* code)
{
    if (table[character] != NULL)
    {
        table[character]->destroyList();
        delete table[character];
    }
    table[character] = code;
}

void HashTable::printTable()
{
    for (int i = 0; i < maxChars; i++)
    {
        std::cout << i << " ";
        if (table[i] != NULL)
        {
            table[i]->debug_printList();
        }
        std::cout << std::endl;
    }
}

void HashTable::emptyTable()
{
    for (int i = 0; i < maxChars; i++)
    {
        if (table[i] != NULL)
        {
            table[i]->destroyList();
            delete table[i];
            table[i] = NULL;
        }
    }
}
//
// Created by dell on 1/26/2021.
//

#ifndef HUFFMAN_S_CODING_HASHTABLE_H
#define HUFFMAN_S_CODING_HASHTABLE_H

#include <iostream>
#include "LinkedList.h"

class HashTable
{
public:
	LinkedList<bool>** table;//linked list used to store the code for each character as the codes can be of variable length
	const int maxChars = 256;
	HashTable();
	void insertCode(unsigned char character, LinkedList<bool>* code);
	void printTable();

	LinkedList<bool>* getCode(unsigned char character)
	{
	    return table[character];
	}

	void emptyTable();
};
#endif //HUFFMAN_S_CODING_HASHTABLE_H


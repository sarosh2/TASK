//
// Created by dell on 1/27/2021.
//

#ifndef HUFFMAN_S_CODING_LINKEDLIST_H
#define HUFFMAN_S_CODING_LINKEDLIST_H

#include <iostream>
template <typename T>
struct ListNode
{
    T data;//bool represents a single bit in the stream of encrypted text
    ListNode* next;

    ListNode<T>()
    {
        data = NULL;
        next = NULL;
    }
};

//linked list to store stream of bits in encrypted text
template <typename T>

class LinkedList
{


public:
    ListNode<T>* start=NULL;
    ListNode<T>* last=NULL;
    int size = 0;



    bool isEmpty()
    {
        //if there's no starting node, list is empty
            return start == NULL;
    }


    void insertAtFront(T value)
    {
        //create a node to add at front
        ListNode<T>* newnode = new ListNode<T>();
        newnode->data = value;

        //update both start and last in an empty list
        if (isEmpty())
        {
            start = newnode;
            last = newnode;
        }
            //update only start variable for non empty list
        else
        {
            newnode->next = start;
            start = newnode;
        }
        size++;
    }

    void insertAtEnd(T value)
    {
        ListNode<T>* newnode = new ListNode<T>();
        newnode->data = value;

        if (isEmpty())
        {
            start = newnode;
            last = newnode;
        }
        else
        {
            last->next = newnode;
            last = newnode;
        }
        size++;
     }

    bool deleteAtFront()
    {
        if (!isEmpty())
        {
            T tempData = start->data;
            ListNode<T>* tempNode = start;
            start = start->next;
            delete tempNode;
            size--;
            if (isEmpty())//update last if the list has been emptied
                last = NULL;
            return tempData;
        }
        else
        {
            std::cout << "List is empty" << std::endl;
            return NULL;
        }
    }

    void destroyList()
    {
        //iterate through list and delete everything
        if(isEmpty())
            return;
        ListNode<T>* node = start;
        while (node != NULL)
        {
            ListNode<T>* temp = node;
            node = node->next;
            delete temp;
        }
        start = NULL;
        last = NULL;
        size = 0;
    }

    void debug_printList()
    {
        ListNode<T> *temp = start;
        while(temp != NULL)
        {
            std::cout<< temp->data <<" ";
            temp = temp->next;
        }
    }
};

#endif //HUFFMAN_S_CODING_LINKEDLIST_H

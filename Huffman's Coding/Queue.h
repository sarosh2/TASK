//
// Created by dell on 1/29/2021.
//

#ifndef HUFFMAN_S_CODING_QUEUE_H
#define HUFFMAN_S_CODING_QUEUE_H

#include <iostream>


class Queue {


    // QueueNode
    struct QueueNode
    {
        bool data;//bool represents a single bit in the stream of encrypted text
        QueueNode* next;

        QueueNode(bool value, QueueNode* nextNode)
        {
            data = value;
            next = nextNode;
        }

    };

    QueueNode *first = NULL;
    QueueNode *last = NULL;

public:
    int size = 0;
 //   ~Queue(){   destroyQueue();}
    bool isEmpty(){     return size == 0;}

    void enQueue(bool value);
    bool deQueue();
    void destroyQueue();
};


#endif //HUFFMAN_S_CODING_QUEUE_H


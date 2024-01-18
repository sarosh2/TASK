//
// Created by dell on 1/29/2021.
//

#include "Queue.h"

void Queue::enQueue(bool value)
{

    QueueNode *newnode = new QueueNode(value,NULL);
    if (isEmpty())
    {
        first = newnode;
        last = newnode;
    }
    else
    {
        last->next = newnode;
        last = newnode;
    }
    size++;
}

bool Queue::deQueue()
{
    bool toReturn = first->data;
    QueueNode *temp = first;
    first = first->next;
    delete temp;
    size--;
    return toReturn;
}

void Queue::destroyQueue()
{
    //iterate through list and delete everything
    QueueNode *node = first;
    while (node != NULL)
    {
        QueueNode *temp = node;
        node = node->next;
        delete temp;
    }
    first = NULL;
    last = NULL;
    size = 0;
}


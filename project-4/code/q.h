/*===========================================
 *  Authors:
 *      Tyler Brockett
 *      Man Do
 *  Project:
 *      Project 4
 *  Description:
 *      Readers and Writers
 *  Class:
 *      CSE 430 Operating Systems - Partha Dasgupta
 *  Organization:
 *      Arizona State University
 ============================================*/

#ifndef CSE430_PROJECT_4_Q_H
#define CSE430_PROJECT_4_Q_H

#include <stdlib.h>
#include "tcb.h"

// Define an easy to use memory allocation method.
#define ALLOC(t) (t*) calloc(1, sizeof(t))

typedef struct Queue
{
    TCB_t * head;
} Queue;

// Global Queue variable
Queue * runQ;

// Declare all functions available
TCB_t * NewItem();
Queue * InitQueue();
void AddQueue(Queue * queue, TCB_t * tcb);
TCB_t * DelQueue(Queue * queue);
void RotateQ(Queue * queue);

/*==========================================
 *      Queue Function Implementations
 *==========================================*/

struct TCB_t * NewItem()
{
    /* Uses calloc macro. Calloc is not necessary over
     * malloc, since memset is called on the memory
     * later on in init_TCB. */
    TCB_t * element = ALLOC(TCB_t);
    element->prev = NULL;
    element->next = NULL;
    return element;
}

struct Queue * InitQueue()
{
    return ALLOC(Queue);
}

void AddQueue(Queue * queue, TCB_t * element)
{
    if(queue->head == NULL)
    {
        queue->head = element;
        queue->head->prev = queue->head;
        queue->head->next = queue->head;
    }
    else
    {
        TCB_t * tail = queue->head->prev;
        tail->next = element;
        element->prev = tail;
        tail = tail->next;

        // To make queue circular
        tail->next = queue->head;
        queue->head->prev = tail;
    }
}

struct TCB_t * DelQueue(Queue * queue)
{
    // No elements
    if(queue->head == NULL)
    {
        return NULL;
    }
    // One element
    else if (queue->head->next == queue->head)
    {
        TCB_t * temp = queue->head;
        queue->head = NULL;
        return temp;
    }
    // Multiple elements
    else
    {
        TCB_t * temp = queue->head;
        TCB_t * tail = queue->head->prev;

        // Only 1 element
        if(queue->head->next == queue->head)
        {
            queue->head = NULL;
        }
            // Multiple elements
        else
        {
            queue->head = queue->head->next;
            // To make queue circular
            queue->head->prev = tail;
            tail->next = queue->head;
        }
        return temp;
    }
}

void RotateQ(Queue * queue)
{
    queue->head = queue->head->next;
}

#endif //CSE430_PROJECT_4_Q_H

#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>

// Define the structure for a node
typedef struct queue_node_t {
    struct queue_node_t* next; // Pointer to the next node
} queue_node_t;

// Define the structure for a queue
typedef struct queue_t {
    queue_node_t* front; // Pointer to the front of the queue
    queue_node_t* rear;  // Pointer to the rear of the queue
    int           size;  // Current size of the queue
} queue_t;

typedef void (*callback)(queue_node_t* this);

// Function declarations
queue_node_t* QUEUE_NodeAlloc(size_t size);
void          QUEUE_NodeFree(queue_node_t* node, callback cbFreeNode);
void          QUEUE_NodeFreeAll(queue_node_t* front, callback cbFreeNode);
void          QUEUE_Constructor(queue_t* queue);
void          QUEUE_Destructor(queue_t* queue, callback cbFreeNode);
void          QUEUE_Enqueue(queue_t* queue, queue_node_t* node);
queue_node_t* QUEUE_Dequeue(queue_t* queue);
int           QUEUE_IsEmpty(queue_t* queue);
int           QUEUE_GetSize(queue_t* queue);

#endif // QUEUE_H

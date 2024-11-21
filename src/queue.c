#include "queue.h"


queue_node_t* QUEUE_NodeAlloc(size_t size)
{
    queue_node_t* node = (queue_node_t*) malloc(size);
    node->next         = NULL;
    return node;
}

void QUEUE_NodeFree(queue_node_t* node, callback cbFreeNode)
{
    node->next = NULL;
    cbFreeNode(node);
    free(node);
}

void QUEUE_NodeFreeAll(queue_node_t* front, callback cbFreeNode)
{
    while (front) {
        queue_node_t* next = front->next;
        QUEUE_NodeFree(front, cbFreeNode);
        front = next;
    }
}

void QUEUE_Constructor(queue_t* queue)
{
    queue->front = queue->rear = NULL;
    queue->size                = 0;
}

void QUEUE_Destructor(queue_t* queue, callback cbFreeNode)
{
    QUEUE_NodeFreeAll(queue->front, cbFreeNode);
    queue->front = queue->rear = NULL;
    queue->size                = 0;
}



// Function to enqueue a node to the queue
void QUEUE_Enqueue(queue_t* queue, queue_node_t* node)
{
    if (queue->rear == NULL) { // If the queue is empty
        queue->front = queue->rear = node;
    } else {
        queue->rear->next = node;
        queue->rear       = node;
    }
    queue->size++; // Increment the size
}

// Function to dequeue a node from the queue
queue_node_t* QUEUE_Dequeue(queue_t* queue)
{
    if (queue->front == NULL) { // If the queue is empty
        return NULL;
    }
    queue_node_t* temp = queue->front;
    queue->front       = queue->front->next;

    // If the queue becomes empty
    if (queue->front == NULL)
        queue->rear = NULL;

    queue->size--; // Decrement the size
    return temp;
}

// Function to check if the queue is empty
int QUEUE_IsEmpty(queue_t* queue)
{
    return (queue->size == 0);
}

// Function to get the current size of the queue
int QUEUE_GetSize(queue_t* queue)
{
    return queue->size;
}

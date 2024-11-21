#include "process_queue.h"
#include <stdlib.h>

void PROC_QUEUE_Init(process_queue_t* queue)
{
    queue->front = NULL;
    queue->rear  = NULL;
}

int PROC_QUEUE_IsEmpty(process_queue_t* queue)
{
    return queue->front == NULL;
}

void PROC_QUEUE_Enqueue(process_queue_t* queue, process_t* process)
{
    process_queue_node_t* new_node = (process_queue_node_t*) malloc(sizeof(process_queue_node_t));
    new_node->process              = process;
    new_node->next                 = NULL;

    if (queue->rear == NULL) {
        queue->front = new_node;
        queue->rear  = new_node;
    } else {
        queue->rear->next = new_node;
        queue->rear       = new_node;
    }
}

process_t* PROC_QUEUE_Dequeue(process_queue_t* queue)
{
    if (PROC_QUEUE_IsEmpty(queue)) {
        return NULL;
    }

    process_queue_node_t* temp    = queue->front;
    process_t*            process = temp->process;
    queue->front                  = queue->front->next;

    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    free(temp);
    return process;
}

process_t* PROC_QUEUE_GetByPID(process_queue_t* queue, int pid)
{
    process_queue_node_t* current = queue->front;
    while (current != NULL) {
        if (current->process->pid == pid) {
            return current->process;
        }
        current = current->next;
    }
    return NULL;
}

process_t* PROC_QUEUE_DequeueByPID(process_queue_t* queue, int pid)
{
    if (PROC_QUEUE_IsEmpty(queue)) {
        return NULL;
    }

    process_queue_node_t* current = queue->front;
    process_queue_node_t* prev    = NULL;

    while (current != NULL) {
        if (current->process->pid == pid) {
            if (prev == NULL) {
                queue->front = current->next;
            } else {
                prev->next = current->next;
            }

            if (current->next == NULL) {
                queue->rear = prev;
            }

            process_t* process = current->process;
            free(current);
            return process;
        }

        prev    = current;
        current = current->next;
    }

    return NULL;
}

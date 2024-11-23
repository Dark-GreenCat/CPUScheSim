#include "process_queue.h"
#include <stdlib.h>

process_node_t* PROC_QUEUE_NodeCreate(process_t* process)
{
    process_node_t* node = (process_node_t*) QUEUE_NodeAlloc(sizeof(process_node_t));
    node->process        = process;
    return node;
}

void PROC_QUEUE_Constructor(process_list_t* list)
{
    QUEUE_Constructor(&list->queue);
}

void PROC_QUEUE_FreeNode(queue_node_t* node)
{
    free(((process_node_t*) node)->process);
}

void PROC_QUEUE_Destructor(process_list_t* list)
{
    QUEUE_Destructor(&list->queue, PROC_QUEUE_FreeNode);
}

void PROC_QUEUE_RemoveNode(process_list_t* list, process_t* process)
{
    if (process == NULL) {
        return;
    }

    queue_t*      queue    = &list->queue;
    queue_node_t* current  = queue->front;
    queue_node_t* previous = NULL;

    // Traverse the queue to find the node with the matching process
    while (current != NULL) {
        process_node_t* current_node = (process_node_t*) current;

        // Check if the current node contains the specified process
        if (current_node->process == process) {
            // If the node to remove is the front of the queue
            if (previous == NULL) {
                // Move the front pointer to the next node
                queue->front = current_node->link.next;
                if (queue->front == NULL) {
                    // If the queue is now empty, reset the rear pointer as well
                    queue->rear = NULL;
                }
            } else {
                // Bypass the current node by updating the previous node's link
                previous->next = current_node->link.next;
                if (current_node->link.next == NULL) {
                    // If the node being removed is the last node, update the rear pointer
                    queue->rear = previous;
                }
            }

            // Free the node
            QUEUE_NodeFree(current, PROC_QUEUE_FreeNode);

            // Decrease the size of the queue
            queue->size--;
            return; // Node removed, exit function
        }

        // Move to the next node
        previous = current;
        current  = current->next;
    }

    // If we get here, the process was not found in the queue
    // Optionally, you could handle this case with an error message or return value
}

const process_node_t* PROC_QUEUE_GetNode(process_list_t* list, int pid)
{
    queue_t*      queue    = &list->queue;
    queue_node_t* current  = queue->front;

    // Traverse the queue to find the node with the matching process
    while (current != NULL) {
        process_node_t* current_node = (process_node_t*) current;

        // Check if the current node contains the specified process
        if (current_node->process->pid == pid) {
            return current_node;
        }

        // Move to the next node
        current  = current->next;
    }

    return NULL;
}



// void PROC_QUEUE_Init(process_queue_t* queue)
// {
//     queue->front = NULL;
//     queue->rear  = NULL;
// }

// int PROC_QUEUE_IsEmpty(process_queue_t* queue)
// {
//     return queue->front == NULL;
// }

// void PROC_QUEUE_Enqueue(process_queue_t* queue, process_t* process)
// {
//     process_queue_node_t* new_node = (process_queue_node_t*) malloc(sizeof(process_queue_node_t));
//     new_node->process              = process;
//     new_node->next                 = NULL;

//     if (queue->rear == NULL) {
//         queue->front = new_node;
//         queue->rear  = new_node;
//     } else {
//         queue->rear->next = new_node;
//         queue->rear       = new_node;
//     }
// }

// process_t* PROC_QUEUE_Dequeue(process_queue_t* queue)
// {
//     if (PROC_QUEUE_IsEmpty(queue)) {
//         return NULL;
//     }

//     process_queue_node_t* temp    = queue->front;
//     process_t*            process = temp->process;
//     queue->front                  = queue->front->next;

//     if (queue->front == NULL) {
//         queue->rear = NULL;
//     }

//     free(temp);
//     return process;
// }

// process_t* PROC_QUEUE_GetByPID(process_queue_t* queue, int pid)
// {
//     process_queue_node_t* current = queue->front;
//     while (current != NULL) {
//         if (current->process->pid == pid) {
//             return current->process;
//         }
//         current = current->next;
//     }
//     return NULL;
// }

// process_t* PROC_QUEUE_DequeueByPID(process_queue_t* queue, int pid)
// {
//     if (PROC_QUEUE_IsEmpty(queue)) {
//         return NULL;
//     }

//     process_queue_node_t* current = queue->front;
//     process_queue_node_t* prev    = NULL;

//     while (current != NULL) {
//         if (current->process->pid == pid) {
//             if (prev == NULL) {
//                 queue->front = current->next;
//             } else {
//                 prev->next = current->next;
//             }

//             if (current->next == NULL) {
//                 queue->rear = prev;
//             }

//             process_t* process = current->process;
//             free(current);
//             return process;
//         }

//         prev    = current;
//         current = current->next;
//     }

//     return NULL;
// }

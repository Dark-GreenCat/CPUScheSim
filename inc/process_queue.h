#ifndef PROCESS_QUEUE_H
#define PROCESS_QUEUE_H

#include "shared_defs.h"
#include "shared_types.h"
#include "queue.h"


typedef struct node {
    queue_node_t link;
    process_t*   process;
} process_node_t;

typedef struct {
    queue_t queue;
} process_list_t;

process_node_t* PROC_QUEUE_NodeCreate(process_t* process);
void PROC_QUEUE_NodeFreeAll(process_list_t* list);

#endif // PROCESS_QUEUE_H

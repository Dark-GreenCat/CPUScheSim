#ifndef PROCESS_QUEUE_H
#define PROCESS_QUEUE_H

#include "shared_defs.h"
#include "shared_types.h"

void       PROC_QUEUE_Init(process_queue_t* queue);
int        PROC_QUEUE_IsEmpty(process_queue_t* queue);
void       PROC_QUEUE_Enqueue(process_queue_t* queue, process_t* process);
process_t* PROC_QUEUE_Dequeue(process_queue_t* queue);
process_t* PROC_QUEUE_GetByPID(process_queue_t* queue, int pid);
process_t* PROC_QUEUE_DequeueByPID(process_queue_t* queue, int pid);

#endif // PROCESS_QUEUE_H

#include "scheduler_model.h"
#include "os_controller.h"

#include <stdlib.h>
#include <string.h>

void SCHED_MODEL_Init(scheduler_model_t* model)
{
    model->sched_algo                 = SCHED_ALGO_FCFS;
    model->process_list_size          = 0;
    model->next_process_request_index = 0;
    model->proc_observer_list_size    = 0;
    model->sched_observer_list_size   = 0;
    model->queue_observer_list_size   = 0;
    memset(model->proc_observer, (int) NULL, PROCESS_OBSERVER_SIZE * sizeof(model->proc_observer[0]));
    memset(model->queue_observer, (int) NULL, QUEUE_OBSERVER_SIZE * sizeof(model->queue_observer[0]));
    memset(model->sched_observer, (int) NULL, SCHEDULER_OBSERVER_SIZE * sizeof(model->sched_observer[0]));

    PROC_QUEUE_Init(&model->job_queue);
    PROC_QUEUE_Init(&model->ready_queue);
    PROC_QUEUE_Init(&model->device_queue);

    model->job_queue_delay   = 200;
    model->job_queue_counter = 0;

    model->elapsed_time_ms = 0;
}
void SCHED_MODEL_Simulate(scheduler_model_t* model)
{
    if (model->elapsed_time_ms == 0) {
        SCHED_MODEL_InitQueue(model, &model->job_queue);
    }

    int i = model->next_process_request_index;
    if (i < model->process_list_size && model->process_list[i].request_time_ms == model->elapsed_time_ms) {
        if (PROC_QUEUE_IsEmpty(&model->job_queue))
            model->job_queue_counter = 0;
        PROC_QUEUE_Enqueue(&model->job_queue, &model->process_list[i]);
        SCHED_MODEL_NotifyQueueObserver(model);
        model->next_process_request_index++;
    }

    if (!PROC_QUEUE_IsEmpty(&model->job_queue))
        model->job_queue_counter++;

    if (model->job_queue_counter == model->job_queue_delay) {
        SCHED_MODEL_QueueToQueue(model, &model->job_queue, &model->ready_queue);
        model->job_queue_counter = 0;
    }
    model->elapsed_time_ms++;
}

void SCHED_MODEL_SetAlgorithm(scheduler_model_t* model, scheduler_algorithm_e sched_algo)
{
    model->sched_algo = sched_algo;
}

void SCHED_MODEL_AddProcess(scheduler_model_t* model, const process_init_t* process)
{
    if (model->process_list_size >= PROCESS_LIST_SIZE_MAX)
        return;

    int index = model->process_list_size++;

    process_t* new_process = &model->process_list[index];

    *new_process = (process_t) {
        .pid             = process->pid,
        .priority        = process->priority,
        .request_time_ms = process->request_time_ms,
        .arrival_time_ms = process->request_time_ms,
        .pstate          = PROC_STATE_NEW,
        .prog_trace      = process->prog_trace,
    };
}

void SCHED_MODEL_DeleteProcess(scheduler_model_t* model, int pid)
{
    if (pid < 0 || pid >= model->process_list_size) {
        return;
    }

    for (int i = 0; i < model->process_list_size; i++) {
        if (model->process_list[i].pid == pid) {
            for (int j = i; j < model->process_list_size - 1; j++) {
                model->process_list[i] = model->process_list[j + 1];
            }

            model->process_list_size--;
            return;
        }
    }
}

void SCHED_MODEL_RegisterProcessObserver(scheduler_model_t* model, const process_observer_t* proc_observer)
{
    if (model->proc_observer_list_size >= PROCESS_OBSERVER_SIZE)
        return;

    int index = model->proc_observer_list_size++;

    model->proc_observer[index] = proc_observer;
}

void SCHED_MODEL_RegisterSchedulerObserver(scheduler_model_t* model, const scheduler_observer_t* sched_observer)
{
    if (model->sched_observer_list_size >= SCHEDULER_OBSERVER_SIZE)
        return;

    int index = model->sched_observer_list_size++;

    model->sched_observer[index] = sched_observer;
}

void SCHED_MODEL_RegisterQueueObserver(scheduler_model_t* model, const queue_observer_t* queue_observer)
{
    if (model->queue_observer_list_size >= QUEUE_OBSERVER_SIZE)
        return;

    int index = model->queue_observer_list_size++;

    model->queue_observer[index] = queue_observer;
}

void SCHED_MODEL_InitQueue(scheduler_model_t* model, process_queue_t* job_queue)
{
    // Sort the process list based on the request_time_ms
    for (int i = 0; i < model->process_list_size - 1; i++) {
        for (int j = i + 1; j < model->process_list_size; j++) {
            if (model->process_list[i].request_time_ms > model->process_list[j].request_time_ms) {
                process_t temp         = model->process_list[i];
                model->process_list[i] = model->process_list[j];
                model->process_list[j] = temp;
            }
        }
    }
}

void SCHED_MODEL_QueueToQueue(scheduler_model_t* model, process_queue_t* src_queue, process_queue_t* dest_queue)
{
    if (!PROC_QUEUE_IsEmpty(src_queue)) {
        process_t* process = PROC_QUEUE_Dequeue(src_queue);
        PROC_QUEUE_Enqueue(dest_queue, process);
    }

    SCHED_MODEL_NotifyQueueObserver(model);
}

void SCHED_MODEL_NotifyQueueObserver(scheduler_model_t* model)
{
    for (int i = 0; i < model->queue_observer_list_size; i++) {
        if (model->queue_observer[i]->type == PROC_QUEUE_JOB)
            model->queue_observer[i]->updateQueue(model->queue_observer[i], &model->job_queue);
        if (model->queue_observer[i]->type == PROC_QUEUE_READY)
            model->queue_observer[i]->updateQueue(model->queue_observer[i], &model->ready_queue);
    }
}

/**
 * @file scheduler_model.h
 * @brief Defines the core model and observer interfaces for the CPU Scheduler Simulator.
 *
 * This file provides data structures and functions to manage processes, queues,
 * and observers in the simulation. It also includes functions for initializing,
 * simulating, and configuring the scheduler.
 */

#ifndef SCHEDULER_MODEL_H
#define SCHEDULER_MODEL_H

#include "process_queue.h"
#include "shared_defs.h"
#include "shared_types.h"


// Observer array sizes
#define PROCESS_OBSERVER_SIZE   1
#define SCHEDULER_OBSERVER_SIZE 1
#define QUEUE_OBSERVER_SIZE     2


// Observer for process-related events
typedef struct {
    void (*updateProcessInfo)(void* this, const process_t* process);    // Updates process info
    void (*updateCurrentProcess)(void* this, const process_t* process); // Updates the current process
} process_observer_t;

// Observer for scheduler-related events
typedef struct {
    void (*updateChanges)(void* this, timestamp_t timestamp_ms, const process_t* process,
        const char* description); // Logs scheduler changes
} scheduler_observer_t;

// Core scheduler model structure
typedef struct {
    scheduler_algorithm_e sched_algo;                          // Selected scheduling algorithm
    process_t             process_list[PROCESS_LIST_SIZE_MAX]; // List of processes
    int                   process_list_size;                   // Number of processes in the list
    int                   next_process_request_index;
    process_observer_t*   proc_observer[PROCESS_OBSERVER_SIZE];    // Observers for process events
    int                   proc_observer_list_size;                 // Number of process observer
    scheduler_observer_t* sched_observer[SCHEDULER_OBSERVER_SIZE]; // Observers for scheduler events
    int                   sched_observer_list_size;                // Number of scheduler observer
    queue_observer_t*     queue_observer[QUEUE_OBSERVER_SIZE];     // Observers for queue events
    int                   queue_observer_list_size;                // Number of queue observer
    process_queue_t       job_queue;
    process_queue_t       ready_queue;
    process_queue_t       device_queue;
    int                   elapsed_time_ms;
} scheduler_model_t;


/* Public API - Short declaration */

void SCHED_MODEL_Init(scheduler_model_t* model);
void SCHED_MODEL_Simulate(scheduler_model_t* model);

void SCHED_MODEL_SetAlgorithm(scheduler_model_t* model, scheduler_algorithm_e sched_algo);
void SCHED_MODEL_AddProcess(scheduler_model_t* model, const process_init_t* process);
void SCHED_MODEL_DeleteProcess(scheduler_model_t* model, int pid);

void SCHED_MODEL_RegisterProcessObserver(scheduler_model_t* model, const process_observer_t* proc_observer);
void SCHED_MODEL_RegisterSchedulerObserver(scheduler_model_t* model, const scheduler_observer_t* sched_observer);
void SCHED_MODEL_RegisterQueueObserver(scheduler_model_t* model, const queue_observer_t* queue_observer);

/* Private API */
void SCHED_MODEL_InitQueue(scheduler_model_t* model, process_queue_t* job_queue);
void SCHED_MODEL_QueueToQueue(scheduler_model_t* model, process_queue_t* src_queue, process_queue_t* dest_queue);

void SCHED_MODEL_NotifyQueueObserver(scheduler_model_t* model);


/* Public API - Detailed description */

/**
 * @brief Initializes the scheduler model.
 *
 * This function sets up the scheduler model, initializing its fields to default values
 * and preparing it for simulation. It must be called before using other functions
 * in this module.
 *
 * @param model Pointer to the scheduler model to initialize.
 */
void SCHED_MODEL_Init(scheduler_model_t* model);

/**
 * @brief Runs the simulation for the scheduler model.
 *
 * This function starts the simulation of the scheduler model, processing
 * the current list of processes and applying the selected scheduling algorithm.
 *
 * @param model Pointer to the scheduler model to simulate.
 */
void SCHED_MODEL_Simulate(scheduler_model_t* model);

/**
 * @brief Sets the scheduling algorithm for the model.
 *
 * This function configures the scheduling algorithm that will be used during
 * the simulation. Supported algorithms are defined in the `scheduler_algorithm_e` enum.
 *
 * @param model Pointer to the scheduler model.
 * @param sched_algo The scheduling algorithm to set.
 */
void SCHED_MODEL_SetAlgorithm(scheduler_model_t* model, scheduler_algorithm_e sched_algo);

/**
 * @brief Adds a new process to the scheduler model.
 *
 * This function adds a new process, described by its initialization data, to
 * the scheduler model's process list.
 *
 * @param model Pointer to the scheduler model.
 * @param process Pointer to the initialization data for the process to add.
 */
void SCHED_MODEL_AddProcess(scheduler_model_t* model, const process_init_t* process);

/**
 * @brief Removes a process from the scheduler model.
 *
 * This function deletes a process with the specified process ID (PID) from
 * the scheduler model's process list.
 *
 * @param model Pointer to the scheduler model.
 * @param pid The process ID of the process to delete.
 */
void SCHED_MODEL_DeleteProcess(scheduler_model_t* model, int pid);

/**
 * @brief Registers a process observer with the scheduler model.
 *
 * This function allows an observer to register for updates related to
 * process events, such as state changes or other process-specific information.
 *
 * @param model Pointer to the scheduler model.
 * @param proc_observer Pointer to the process observer to register.
 */
void SCHED_MODEL_RegisterProcessObserver(scheduler_model_t* model, const process_observer_t* proc_observer);

/**
 * @brief Registers a scheduler observer with the scheduler model.
 *
 * This function allows an observer to register for updates about changes in
 * the scheduler's behavior, such as algorithm switches or process scheduling.
 *
 * @param model Pointer to the scheduler model.
 * @param sched_observer Pointer to the scheduler observer to register.
 */
void SCHED_MODEL_RegisterSchedulerObserver(scheduler_model_t* model, const scheduler_observer_t* sched_observer);

/**
 * @brief Registers a queue observer with the scheduler model.
 *
 * This function allows an observer to register for updates regarding the
 * state of queues in the scheduler, such as enqueue and dequeue events.
 *
 * @param model Pointer to the scheduler model.
 * @param queue_observer Pointer to the queue observer to register.
 */
void SCHED_MODEL_RegisterQueueObserver(scheduler_model_t* model, const queue_observer_t* queue_observer);


#endif
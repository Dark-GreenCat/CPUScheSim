/**
 * @file shared_types.h
 * @brief Defines shared data types for the CPU Scheduler Simulator.
 *
 * This file includes common data types used across the simulator, such as
 * structures for program traces and related constants. It provides a foundation
 * for managing process traces and simulation data.
 */

#ifndef SHARED_TYPES_H
#define SHARED_TYPES_H

#include "shared_defs.h"

// Maximum length for a program name
#define PROGRAM_NAME_SIZE_MAX 20
// Maximum number of trace records for a program
#define PROGRAM_TRACE_RECORDS_MAX 20
// Maximum number of processes in the scheduler
#define PROCESS_LIST_SIZE_MAX 10

// Typedef for representing timestamps in nanoseconds
typedef unsigned long long timestamp_t;

// Structure representing a single trace record in a program's execution
typedef struct {
    unsigned int    duration_ms; // Duration of the program state in milliseconds
    program_state_e prog_state;  // State of the program (CPU or I/O)
} program_trace_record_t;

// Structure representing the entire trace of a program's execution
typedef struct {
    char                   name[PROGRAM_NAME_SIZE_MAX];       // Program name
    int                    num_of_record;                     // Number of trace records (max PROGRAM_TRACE_RECORDS_MAX)
    int                    current_index;                     // Current index in the trace
    program_trace_record_t record[PROGRAM_TRACE_RECORDS_MAX]; // Array of trace records (fixed size)
} program_trace_t;

// Structure for initializing a process with its details
typedef struct {
    int             pid;             // Process ID
    int             priority;        // Process priority
    timestamp_t     request_time_ms; // Time at which the process was requested
    program_trace_t prog_trace;      // Execution trace of the process
} process_init_t;

// Structure representing the state of a process during simulation
typedef struct {
    int             pid;             // Process ID
    int             priority;        // Process priority
    timestamp_t     request_time_ms; // Time of request
    timestamp_t     arrival_time_ms; // Time of arrival in the scheduler
    process_state_e pstate;          // Current state of the process
    program_trace_t prog_trace;      // Execution trace of the process
} process_t;

typedef struct node {
    process_t*   process;
    struct node* next;
} process_queue_node_t;

typedef struct {
    process_queue_node_t* front;
    process_queue_node_t* rear;
} process_queue_t;

// Observer for queue-related events
typedef struct queue_observer_t {
    process_queue_e type;
    void (*updateQueue)(struct queue_observer_t* this, const process_queue_t* queue); // Updates queue state
} queue_observer_t;

#endif

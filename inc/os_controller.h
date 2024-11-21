/**
 * @file os_controller.h
 * @brief Header file for the OS controller.
 *
 * This module defines the OS controller, which acts as the intermediary
 * between the scheduler model and the simulation view. It provides functions
 * for managing the scheduler's state, interacting with processes, and controlling
 * the simulation.
 */

#ifndef OS_CONTROLLER_H
#define OS_CONTROLLER_H

#include "scheduler_model.h"
#include "sim_view.h"

// Structure representing the OS controller
typedef struct {
    sim_view_t         view;  // Simulation view for displaying scheduler and process information.
    scheduler_model_t* model; // Pointer to the scheduler model.
} os_controller_t;


/* Public API - Short declaration */
void OS_CTRL_Init(os_controller_t* controller, scheduler_model_t* model);

void OS_CTRL_AddProcess(view_interface_t* this, int pid, int request_time_ms, int total_cpu_burst_ms,
    int total_io_burst_ms, int num_of_cpu_burst);


#if 0
/* Public API - Detailed description */

/**
 * @brief Changes the scheduling algorithm in the scheduler model.
 *
 * This function allows the user to select a different scheduling algorithm
 * during the simulation.
 *
 * @param controller Pointer to the OS controller.
 */
void OS_CTRL_ChangeAlgorithm(os_controller_t* controller);

/**
 * @brief Adds a new process to the scheduler model.
 *
 * This function enables the addition of a new process, interacting
 * with the scheduler model to include it in the process list.
 *
 * @param controller Pointer to the OS controller.
 */
void OS_CTRL_AddProcess(
    void* this, int pid, int request_time_ms, int total_cpu_burst_ms, int total_io_burst_ms, int num_of_cpu_burst);

/**
 * @brief Deletes a process from the scheduler model.
 *
 * This function removes a process from the scheduler model based on its PID.
 *
 * @param controller Pointer to the OS controller.
 */
void OS_CTRL_DeleteProcess(os_controller_t* controller);

/**
 * @brief Displays detailed information about all processes.
 *
 * This function fetches the process details from the scheduler model and
 * displays them using the simulation view.
 *
 * @param controller Pointer to the OS controller.
 */
void OS_CTRL_DisplayProcessInfo(os_controller_t* controller);

/**
 * @brief Starts the scheduler simulation.
 *
 * This function initiates the simulation of the scheduler model,
 * running the selected scheduling algorithm and processing events.
 *
 * @param controller Pointer to the OS controller.
 */
void OS_CTRL_StartScheduler(os_controller_t* controller);

/**
 * @brief Enables context switching in the simulation.
 *
 * This function activates context switching functionality, allowing
 * the scheduler to add switching time between processes during the
 * simulation.
 *
 * @param controller Pointer to the OS controller.
 */
void OS_CTRL_EnableContextSwitchTime(os_controller_t* controller);
#endif

#endif

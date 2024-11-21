#include "os_controller.h"

void OS_CTRL_Start(os_controller_t* controller)
{

}

void OS_CTRL_AddProcess(os_controller_t* controller)
{
    sim_view_t* sim = &controller->view;

    process_init_t process = (process_init_t) {
        .pid = sim->pidCounter++,
        .priority = 0,
        .request_time_ms = sim->requestTimeInput,
        .prog_trace = {
            .name = "program",
            .num_of_record = 1,
            .current_index = 0,
            .record[0] = {
                .duration_ms = sim->cpuTimeInput,
                .prog_state = PROG_STATE_CPU,
            },
        },
    };

    SCHED_MODEL_AddProcess(controller->model, &process);
    if (sim->processCount < MAX_PROCESSES) {
        sim->processList[sim->processCount++] = (Process) { process.pid, process.request_time_ms, sim->ioTimeInput,
            process.prog_trace.record[0].duration_ms, process.prog_trace.record[0].duration_ms, sim->cpuNumberInput, NEW };
        // UpdateListViewContent(sim);
    }
}

void OS_CTRL_DeleteProcess(os_controller_t* controller)
{
    sim_view_t* sim = &controller->view;

    if (sim->activeItem >= 0 && sim->activeItem < sim->processCount) {
        for (int i = sim->activeItem; i < sim->processCount - 1; i++) {
            sim->processList[i] = sim->processList[i + 1];
        }
        sim->processCount--;
        sim->activeItem = -1;
        // UpdateListViewContent(sim);
        sim->PInfo[0] = '\0';
    } else {
        sim->showMessageBox = true;
        sim->messageType    = 2;
    }
}

void OS_CTRL_StartScheduler(os_controller_t* controller)
{
    sim_view_t* sim = &controller->view;

    SCHED_MODEL_Simulate(controller->model);

    // JobQueue(sim);
    // StartScheduler(sim);
    // TerminateProcessIfComplete(sim);
    // UpdateQueueStatus(sim);
    // sim->currentTime++;
    // if (AllProcessesTerminated(sim)) {
    //     sim->schedulerStarted = false;
    //     char logMessage[64];
    //     snprintf(logMessage, sizeof(logMessage), "All processes completed at time %d\n", sim->currentTime);
    //     strncat(sim->logContent, logMessage, sizeof(sim->logContent) - strlen(sim->logContent) - 1);
    //     for (int i = 0; i < sim->processCount; i++) {
    //         sim->processList[i].state        = NEW;
    //         sim->processList[i].cpuBurstTime = sim->processList[i].originalCpuBurstTime;
    //     }
    //     sim->currentTime = 0;
    // }
}

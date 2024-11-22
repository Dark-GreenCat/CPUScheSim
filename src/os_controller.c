#include "os_controller.h"

void OS_CTRL_Init(os_controller_t* controller, scheduler_model_t* model)
{
    controller->model               = model;
    view_interface_t view_interface = (view_interface_t) {
        .addProcess    = OS_CTRL_AddProcess,
        .removeProcess = OS_CTRL_RemoveProcess,
    };

    SCHED_MODEL_Init(controller->model);
    SIM_VIEW_Init(&controller->view, view_interface);
}

#warning "OS_CTRL_AddProcess currently has not supported {total_io_burst_ms, num_of_cpu_burst} yet"
void OS_CTRL_AddProcess(view_interface_t* this, int pid, int request_time_ms, int total_cpu_burst_ms,
    int total_io_burst_ms, int num_of_cpu_burst)
{
    os_controller_t* controller = (os_controller_t*) this;
    process_init_t process_info = {
        .pid = pid,
        .priority = 0,
        .request_time_ms = request_time_ms,
        .prog_trace = {
            .name = "",
            .current_index = 0,
            .num_of_record = 1,
            .record[0] = {
                .duration_ms = total_cpu_burst_ms,
                .prog_state = PROG_STATE_CPU,
            },
        },
    };

    SCHED_MODEL_AddProcess(controller->model, &process_info);
}

void OS_CTRL_RemoveProcess(view_interface_t* this, int pid)
{
    os_controller_t* controller = (os_controller_t*) this;
    SCHED_MODEL_DeleteProcess(controller->model, pid);
}

#define RAYGUI_IMPLEMENTATION

#include "os_controller.h"

int main()
{
    scheduler_model_t model;
    SCHED_MODEL_Init(&model);

    sim_view_t view = {
        // .job_observer = {
        //     .type = PROC_QUEUE_JOB,
        //     .updateQueue = SIM_VIEW_UpdateQueue,
        // },
        // .ready_observer = {
        //     .type = PROC_QUEUE_READY,
        //     .updateQueue = SIM_VIEW_UpdateQueue,
        // },
        .progress                = 0.0f,
        .currentTime             = 0,
        .pidCounter              = 1,
        .processCount            = 0,
        .baseWidth               = 800,
        .baseHeight              = 630,
        .showMessageBox          = false,
        .messageType             = 0,
        .selectedScheduler       = 0,
        .contextSwitchingEnabled = false,
        .activeItem              = -1,
        .scrollIndex             = 0,
        .currentSize             = 100,
        .logContent              = "",
        .queueStatus             = "",
        .Performance             = "",
        .cpuTimeInput            = 0,
        .ioTimeInput             = 0,
        .requestTimeInput        = 0,
        .cpuNumberInput          = 0,
        .PInfo                   = "",
        .listViewContent         = "",
        .cpuTimeEdit             = false,
        .ioTimeEdit              = false,
        .requestTimeEdit         = false,
        .cpuNumberEdit           = false,
        .baseTextSize            = 20,
        .schedulerStarted        = false,
        .runningProcessIndex     = -1,
        .processRunning          = false
    };

    SIM_VIEW_LaunchWindows(&view);

    // os_controller_t controller
    //     = { .model = &model, .view SCHED_MODEL_RegisterQueueObserver(controller.model,
    //     &controller.view.job_observer);
    // SCHED_MODEL_RegisterQueueObserver(controller.model, &controller.view.ready_observer);
    // OS_CTRL_Start(&controller);

    return 0;
}

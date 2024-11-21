#include "os_controller.h"

int main()
{
    scheduler_model_t model;
    os_controller_t controller;
    OS_CTRL_Init(&controller, &model);

    SIM_VIEW_LaunchWindows(&controller.view);

    SCHED_MODEL_Destructor(&model);
}

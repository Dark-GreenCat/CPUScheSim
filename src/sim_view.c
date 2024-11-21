#define RAYGUI_IMPLEMENTATION


#include "sim_view.h"
#include "os_controller.h"
#include <stdio.h>
#include <string.h>


void SIM_VIEW_Init(sim_view_t* view, view_interface_t interface)
{
    *view = (sim_view_t) {
        .interface               = interface,
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
        .processRunning          = false,
    };
}

void SIM_VIEW_LaunchWindows(sim_view_t* view)
{
    InitWindow(view->baseWidth, view->baseHeight, "CPU Scheduler Simulator");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // Calculate screen scaling
        int   width  = GetScreenWidth();
        int   height = GetScreenHeight();
        float scaleX = (float) width / view->baseWidth;
        float scaleY = (float) height / view->baseHeight;

        // Calculate the scaled text size based on window height
        int scaledTextSize = (int) (view->baseTextSize * scaleY * 0.6);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Title label
        GuiSetStyle(DEFAULT, TEXT_SIZE, scaledTextSize + 10); // Add extra size for the title
        GuiLabel((Rectangle) { 250 * scaleX, 20 * scaleY, 500 * scaleX, 30 * scaleY }, "CPU SCHEDULING SIMULATOR");

        // Normal labels and combo box
        GuiSetStyle(DEFAULT, TEXT_SIZE, scaledTextSize); // Reset text size for normal labels

        // Scheduler combo box
        if (GuiComboBox((Rectangle) { 20 * scaleX, 20 * scaleY, 210 * scaleX, 30 * scaleY },
                "FCFS;RR;SJF;SRTF;Priority Scheduling", &view->selectedScheduler)) { }

        // Add/Delete Process buttons
        if (GuiButton((Rectangle) { 20 * scaleX, 60 * scaleY, 100 * scaleX, 30 * scaleY }, "Add Process")) {
            view->interface.addProcess((view_interface_t*) view, view->pidCounter++, view->requestTimeInput,
                view->cpuTimeInput, view->ioTimeInput, view->cpuNumberInput);
        }
        if (GuiButton((Rectangle) { 130 * scaleX, 60 * scaleY, 100 * scaleX, 30 * scaleY }, "Delete Process")) {
            // OS_CTRL_DeleteProcess(controller);
        }

        // Toggle edit mode for different inputs
        if (CheckCollisionPointRec(
                GetMousePosition(), (Rectangle) { 130 * scaleX, 110 * scaleY, 100 * scaleX, 30 * scaleY })
            && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            view->cpuTimeEdit = true;
            view->ioTimeEdit = view->requestTimeEdit = view->cpuNumberEdit = false;
        }

        if (CheckCollisionPointRec(
                GetMousePosition(), (Rectangle) { 130 * scaleX, 150 * scaleY, 100 * scaleY, 30 * scaleY })
            && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            view->ioTimeEdit  = true;
            view->cpuTimeEdit = view->requestTimeEdit = view->cpuNumberEdit = false;
        }

        if (CheckCollisionPointRec(
                GetMousePosition(), (Rectangle) { 130 * scaleX, 190 * scaleY, 100 * scaleY, 30 * scaleY })
            && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            view->requestTimeEdit = true;
            view->cpuTimeEdit = view->ioTimeEdit = view->cpuNumberEdit = false;
        }

        if (CheckCollisionPointRec(
                GetMousePosition(), (Rectangle) { 130 * scaleX, 230 * scaleY, 100 * scaleY, 30 * scaleY })
            && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            view->cpuNumberEdit = true;
            view->cpuTimeEdit = view->ioTimeEdit = view->requestTimeEdit = false;
        }

        // User input fields for CPU, IO, Request Time, and CPU Burst Number
        GuiValueBox((Rectangle) { 130 * scaleX, 110 * scaleY, 100 * scaleX, 30 * scaleY }, "CPU time\t",
            &view->cpuTimeInput, 0, 300, view->cpuTimeEdit);
        GuiValueBox((Rectangle) { 130 * scaleX, 150 * scaleY, 100 * scaleX, 30 * scaleY }, "IO time\t",
            &view->ioTimeInput, 0, 300, view->ioTimeEdit);
        GuiValueBox((Rectangle) { 130 * scaleX, 190 * scaleY, 100 * scaleX, 30 * scaleY }, "Request time\t",
            &view->requestTimeInput, 0, 300, view->requestTimeEdit);
        GuiValueBox((Rectangle) { 130 * scaleX, 230 * scaleY, 100 * scaleX, 30 * scaleY }, "CPU burst Num\t",
            &view->cpuNumberInput, 0, 10, view->cpuNumberEdit);

        // Process Information TextBox
        GuiTextBox((Rectangle) { 20 * scaleX, 280 * scaleY, 200 * scaleX, 90 * scaleY }, view->PInfo,
            sizeof(view->PInfo), false);

        // Process list with scrollable view
        GuiListView((Rectangle) { 20 * scaleX, 360 * scaleY, 200 * scaleX, 200 * scaleY }, view->listViewContent,
            &view->scrollIndex, &view->activeItem);

        // Log display
        GuiLabel((Rectangle) { 250 * scaleX, 70 * scaleY, 500 * scaleX, 20 * scaleY }, "Changes: ");
        GuiTextBox((Rectangle) { 250 * scaleX, 90 * scaleY, 355 * scaleX, 270 * scaleY }, view->logContent,
            sizeof(view->logContent), false);
        GuiCheckBox((Rectangle) { 250 * scaleX, 400 * scaleY, 20 * scaleX, 20 * scaleY },
            "Enable Random Context Switching", &view->contextSwitchingEnabled);

        // Progress bars for processes
        for (int i = 0; i < view->processCount; i++) {
            float progress
                = 1.0f - (float) view->processList[i].cpuBurstTime / view->processList[i].originalCpuBurstTime;
            GuiProgressBar((Rectangle) { 615 * scaleX, (90 + 33 * i) * scaleY, 100 * scaleX, 30 * scaleY }, NULL, NULL,
                &progress, 0.0f, 1.0f);
            char label[20];
            sprintf(label, "P%d", i); // Label with process index
            DrawText(label, 730 * scaleX, (95 + 33 * i) * scaleY, 20 * scaleX, DARKGRAY);
        }

        // Display current process or CPU status
        if (view->processRunning && view->runningProcessIndex != -1) {
            char cpuStatus[64];
            snprintf(
                cpuStatus, sizeof(cpuStatus), "CPU status: P%d ", view->processList[view->runningProcessIndex].pid);
            GuiLabel((Rectangle) { 250 * scaleX, 430 * scaleY, 260 * scaleX, 20 * scaleY }, cpuStatus);
        } else {
            GuiLabel((Rectangle) { 250 * scaleX, 430 * scaleY, 260 * scaleX, 20 * scaleY }, "CPU status: Idle");
        }

        // Queue display
        GuiTextBox((Rectangle) { 250 * scaleX, 450 * scaleY, 260 * scaleX, 110 * scaleY }, view->queueStatus,
            sizeof(view->queueStatus), false);

        // Performance results display
        GuiTextBox((Rectangle) { 520 * scaleX, 450 * scaleY, 260 * scaleX, 110 * scaleY }, view->Performance,
            sizeof(view->Performance), false);
        GuiLabel((Rectangle) { 520 * scaleX, 430 * scaleY, 260 * scaleX, 20 * scaleY }, "Performance");

        // Start Scheduling Button
        if (GuiButton((Rectangle) { 250 * scaleX, 370 * scaleY, 200 * scaleX, 20 * scaleY }, "Start")) {
            snprintf(view->logContent, sizeof(view->logContent), "Start!\n");
            strncat(view->logContent, "Timestamp - Pid - Changes - State\n",
                sizeof(view->logContent) - strlen(view->logContent) - 1);
            view->schedulerStarted = true;
        }

        // Call the scheduler start function
        if (view->schedulerStarted) {
            // OS_CTRL_StartScheduler(controller);
        }

        // Export to CSV button
        if (GuiButton((Rectangle) { 650 * scaleX, 570 * scaleY, 100 * scaleX, 30 * scaleY }, "Export to .csv")) { }

        // Informations button
        if (GuiButton((Rectangle) { 20 * scaleX, 580 * scaleY, 100 * scaleX, 30 * scaleY }, "Informations")) {
            view->showMessageBox = true;
            view->messageType    = 1;
        }

        // Display MessageBox
        if (view->showMessageBox) {
            if (view->messageType == 1) {
                int result = GuiMessageBox((Rectangle) { 85 * scaleX, 70 * scaleY, 400 * scaleX, 200 * scaleY },
                    "#196#CPU SCHEDULING SIMULATOR", "Devs: Pham Tuan Phong 20214039, Tran Quang Huy 20210428", "OK");
                if (result >= 0) {
                    view->showMessageBox = false;
                    view->messageType    = 0;
                }
            } else if (view->messageType == 2) {
                int result = GuiMessageBox((Rectangle) { 85 * scaleX, 70 * scaleY, 400 * scaleX, 200 * scaleY },
                    "#196#CPU SCHEDULING SIMULATOR", "Please select a process to delete", "OK");
                if (result >= 0) {
                    view->showMessageBox = false;
                    view->messageType    = 0;
                }
            }
        }

        // Display selected process information
        if (view->activeItem != -1) {
            // UpdateProcessInfo(sim, sim->activeItem);
        }

        EndDrawing();
    }

    CloseWindow();
}

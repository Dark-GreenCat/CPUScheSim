#include "sim_view.h"
#include "os_controller.h"
#include <stdio.h>
#include <string.h>

void SIM_VIEW_LaunchWindows(sim_view_t* sim)
{
    InitWindow(sim->baseWidth, sim->baseHeight, "CPU Scheduler Simulator");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // Calculate screen scaling
        int   width  = GetScreenWidth();
        int   height = GetScreenHeight();
        float scaleX = (float) width / sim->baseWidth;
        float scaleY = (float) height / sim->baseHeight;

        // Calculate the scaled text size based on window height
        int scaledTextSize = (int) (sim->baseTextSize * scaleY * 0.6);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Title label
        GuiSetStyle(DEFAULT, TEXT_SIZE, scaledTextSize + 10); // Add extra size for the title
        GuiLabel((Rectangle) { 250 * scaleX, 20 * scaleY, 500 * scaleX, 30 * scaleY }, "CPU SCHEDULING SIMULATOR");

        // Normal labels and combo box
        GuiSetStyle(DEFAULT, TEXT_SIZE, scaledTextSize); // Reset text size for normal labels

        // Scheduler combo box
        if (GuiComboBox((Rectangle) { 20 * scaleX, 20 * scaleY, 210 * scaleX, 30 * scaleY },
                "FCFS;RR;SJF;SRTF;Priority Scheduling", &sim->selectedScheduler)) { }

        // Add/Delete Process buttons
        if (GuiButton((Rectangle) { 20 * scaleX, 60 * scaleY, 100 * scaleX, 30 * scaleY }, "Add Process")) {
            // OS_CTRL_AddProcess(controller);
        }
        if (GuiButton((Rectangle) { 130 * scaleX, 60 * scaleY, 100 * scaleX, 30 * scaleY }, "Delete Process")) {
            // OS_CTRL_DeleteProcess(controller);
        }

        // Toggle edit mode for different inputs
        if (CheckCollisionPointRec(
                GetMousePosition(), (Rectangle) { 130 * scaleX, 110 * scaleY, 100 * scaleX, 30 * scaleY })
            && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            sim->cpuTimeEdit = true;
            sim->ioTimeEdit = sim->arrivalTimeEdit = sim->cpuNumberEdit = false;
        }

        if (CheckCollisionPointRec(
                GetMousePosition(), (Rectangle) { 130 * scaleX, 150 * scaleY, 100 * scaleY, 30 * scaleY })
            && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            sim->ioTimeEdit  = true;
            sim->cpuTimeEdit = sim->arrivalTimeEdit = sim->cpuNumberEdit = false;
        }

        if (CheckCollisionPointRec(
                GetMousePosition(), (Rectangle) { 130 * scaleX, 190 * scaleY, 100 * scaleY, 30 * scaleY })
            && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            sim->arrivalTimeEdit = true;
            sim->cpuTimeEdit = sim->ioTimeEdit = sim->cpuNumberEdit = false;
        }

        if (CheckCollisionPointRec(
                GetMousePosition(), (Rectangle) { 130 * scaleX, 230 * scaleY, 100 * scaleY, 30 * scaleY })
            && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            sim->cpuNumberEdit = true;
            sim->cpuTimeEdit = sim->ioTimeEdit = sim->arrivalTimeEdit = false;
        }

        // User input fields for CPU, IO, Arrival Time, and CPU Burst Number
        GuiValueBox((Rectangle) { 130 * scaleX, 110 * scaleY, 100 * scaleX, 30 * scaleY }, "CPU time\t",
            &sim->cpuTimeInput, 0, 300, sim->cpuTimeEdit);
        GuiValueBox((Rectangle) { 130 * scaleX, 150 * scaleY, 100 * scaleX, 30 * scaleY }, "IO time\t",
            &sim->ioTimeInput, 0, 300, sim->ioTimeEdit);
        GuiValueBox((Rectangle) { 130 * scaleX, 190 * scaleY, 100 * scaleX, 30 * scaleY }, "Arrival time\t",
            &sim->arrivalTimeInput, 0, 300, sim->arrivalTimeEdit);
        GuiValueBox((Rectangle) { 130 * scaleX, 230 * scaleY, 100 * scaleX, 30 * scaleY }, "CPU burst Num\t",
            &sim->cpuNumberInput, 0, 10, sim->cpuNumberEdit);

        // Process Information TextBox
        GuiTextBox((Rectangle) { 20 * scaleX, 280 * scaleY, 200 * scaleX, 90 * scaleY }, sim->PInfo, sizeof(sim->PInfo),
            false);

        // Process list with scrollable view
        GuiListView((Rectangle) { 20 * scaleX, 360 * scaleY, 200 * scaleX, 200 * scaleY }, sim->listViewContent,
            &sim->scrollIndex, &sim->activeItem);

        // Log display
        GuiLabel((Rectangle) { 250 * scaleX, 70 * scaleY, 500 * scaleX, 20 * scaleY }, "Changes: ");
        GuiTextBox((Rectangle) { 250 * scaleX, 90 * scaleY, 355 * scaleX, 270 * scaleY }, sim->logContent,
            sizeof(sim->logContent), false);
        GuiCheckBox((Rectangle) { 250 * scaleX, 400 * scaleY, 20 * scaleX, 20 * scaleY },
            "Enable Random Context Switching", &sim->contextSwitchingEnabled);

        // Progress bars for processes
        for (int i = 0; i < sim->processCount; i++) {
            float progress = 1.0f - (float) sim->processList[i].cpuBurstTime / sim->processList[i].originalCpuBurstTime;
            GuiProgressBar((Rectangle) { 615 * scaleX, (90 + 33 * i) * scaleY, 100 * scaleX, 30 * scaleY }, NULL, NULL,
                &progress, 0.0f, 1.0f);
            char label[20];
            sprintf(label, "P%d", i); // Label with process index
            DrawText(label, 730 * scaleX, (95 + 33 * i) * scaleY, 20 * scaleX, DARKGRAY);
        }

        // Display current process or CPU status
        if (sim->processRunning && sim->runningProcessIndex != -1) {
            char cpuStatus[64];
            snprintf(cpuStatus, sizeof(cpuStatus), "CPU status: P%d ", sim->processList[sim->runningProcessIndex].pid);
            GuiLabel((Rectangle) { 250 * scaleX, 430 * scaleY, 260 * scaleX, 20 * scaleY }, cpuStatus);
        } else {
            GuiLabel((Rectangle) { 250 * scaleX, 430 * scaleY, 260 * scaleX, 20 * scaleY }, "CPU status: Idle");
        }

        // Queue display
        GuiTextBox((Rectangle) { 250 * scaleX, 450 * scaleY, 260 * scaleX, 110 * scaleY }, sim->queueStatus,
            sizeof(sim->queueStatus), false);

        // Performance results display
        GuiTextBox((Rectangle) { 520 * scaleX, 450 * scaleY, 260 * scaleX, 110 * scaleY }, sim->Performance,
            sizeof(sim->Performance), false);
        GuiLabel((Rectangle) { 520 * scaleX, 430 * scaleY, 260 * scaleX, 20 * scaleY }, "Performance");

        // Start Scheduling Button
        if (GuiButton((Rectangle) { 250 * scaleX, 370 * scaleY, 200 * scaleX, 20 * scaleY }, "Start")) {
            snprintf(sim->logContent, sizeof(sim->logContent), "Start!\n");
            strncat(sim->logContent, "Timestamp - Pid - Changes - State\n",
                sizeof(sim->logContent) - strlen(sim->logContent) - 1);
            sim->schedulerStarted = true;
        }

        // Call the scheduler start function
        if (sim->schedulerStarted) {
            // OS_CTRL_StartScheduler(controller);
        }

        // Export to CSV button
        if (GuiButton((Rectangle) { 650 * scaleX, 570 * scaleY, 100 * scaleX, 30 * scaleY }, "Export to .csv")) { }

        // Informations button
        if (GuiButton((Rectangle) { 20 * scaleX, 580 * scaleY, 100 * scaleX, 30 * scaleY }, "Informations")) {
            sim->showMessageBox = true;
            sim->messageType    = 1;
        }

        // Display MessageBox
        if (sim->showMessageBox) {
            if (sim->messageType == 1) {
                int result = GuiMessageBox((Rectangle) { 85 * scaleX, 70 * scaleY, 400 * scaleX, 200 * scaleY },
                    "#196#CPU SCHEDULING SIMULATOR", "Devs: Pham Tuan Phong 20214039, Tran Quang Huy 20210428", "OK");
                if (result >= 0) {
                    sim->showMessageBox = false;
                    sim->messageType    = 0;
                }
            } else if (sim->messageType == 2) {
                int result = GuiMessageBox((Rectangle) { 85 * scaleX, 70 * scaleY, 400 * scaleX, 200 * scaleY },
                    "#196#CPU SCHEDULING SIMULATOR", "Please select a process to delete", "OK");
                if (result >= 0) {
                    sim->showMessageBox = false;
                    sim->messageType    = 0;
                }
            }
        }

        // Display selected process information
        if (sim->activeItem != -1) {
            // UpdateProcessInfo(sim, sim->activeItem);
        }

        EndDrawing();
    }

    CloseWindow();
}

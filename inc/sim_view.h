#ifndef SIM_VIEW_H
#define SIM_VIEW_H


#include "raygui.h"
#include "raylib.h"


#define MAX_PROCESSES 10
#include "shared_defs.h"
#include "shared_types.h"
#include "stdbool.h"

typedef enum { NEW, READY, RUNNING, WAITING, TERMINATED } ProcessState;

typedef struct {
    int          pid;
    int          requestTime;
    int          ioBurstTime;
    int          cpuBurstTime;
    int          originalCpuBurstTime;
    int          cpuNumber;
    int          completionTime;
    int          waitingTime;
    int          turnaroundTime;
    ProcessState state;
} Process;

typedef struct __view_interface_t {
    void (*addProcess)(struct __view_interface_t* this, int pid, int request_time_ms, int total_cpu_burst_ms,
        int total_io_burst_ms, int num_of_cpu_burst);
} view_interface_t;

typedef struct {
    // queue_observer_t job_observer;
    // queue_observer_t ready_observer;
    view_interface_t interface;
    float            progress;                   // Progress of the simulation
    int              currentTime;                // Current time in the simulation
    int              pidCounter;                 // Process ID counter
    Process          processList[MAX_PROCESSES]; // List of processes
    int              processCount;               // Number of processes in the list
    int              baseWidth;                  // Base window width
    int              baseHeight;                 // Base window height
    bool             showMessageBox;             // Flag for displaying message box
    int              messageType;                // Message type (for the message box)
    int              selectedScheduler;          // The selected scheduler type
    bool             contextSwitchingEnabled;    // Flag for enabling context switching
    int              activeItem;                 // The currently active item in the simulation
    int              scrollIndex;                // Index for scrolling
    float            currentSize;                // Current size scaling factor
    char             logContent[5000];           // Content of the simulation log
    char             queueStatusJob[100];        // Status of the queue
    char             queueStatusReady[100];      // Status of the queue
    char             queueStatus[256];           // Status of the queue
    char             Performance[256];           // Performance metrics
    int              cpuTimeInput;               // CPU time input for new processes
    int              ioTimeInput;                // I/O time input for new processes
    int              requestTimeInput;           // Request time input for new processes
    int              cpuNumberInput;             // CPU number input (if applicable)
    char             PInfo[128];                 // Process information
    char             listViewContent[256];       // List view content (for process details)
    // Textbox edit states
    bool cpuTimeEdit;     // Flag for editing CPU time
    bool ioTimeEdit;      // Flag for editing I/O time
    bool requestTimeEdit; // Flag for editing request time
    bool cpuNumberEdit;   // Flag for editing CPU number
    // Define base text size for scaling
    int  baseTextSize;        // Base text size for scaling
    bool schedulerStarted;    // Flag indicating if the scheduler has started
    int  runningProcessIndex; // Index of the currently running process
    bool processRunning;      // Flag indicating if a process is running
} sim_view_t;


void SIM_VIEW_Init(sim_view_t* view, view_interface_t interface);
void SIM_VIEW_LaunchWindows(sim_view_t* view);

#endif
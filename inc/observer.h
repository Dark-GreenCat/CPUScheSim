#ifndef OBSERVER_H
#define OBSERVER_H

#include "process_queue.h"

typedef struct proc_obs_intf_t {
    void (*updateProcessList)(struct proc_obs_intf_t* this, const process_list_t* list);
    void (*updateRunningProcess)(struct proc_obs_intf_t* this, const process_t* process);
} proc_obs_intf_t;

#define PROC_SUBJ_MAX_OBSERVERS 1

typedef struct proc_subj_intf_t proc_subj_intf_t;
struct proc_subj_intf_t {
    proc_obs_intf_t* observer[PROC_SUBJ_MAX_OBSERVERS];
    int              observer_count;
    void (*registerObserver)(proc_subj_intf_t* this, proc_obs_intf_t* observer);
    void (*removeObserver)(proc_subj_intf_t* this, proc_obs_intf_t* observer);
    void (*notifyProcessList)(proc_subj_intf_t* this, const process_list_t* list);
    void (*notifyRunningProcess)(proc_subj_intf_t* this, const process_t* process);
};

#endif
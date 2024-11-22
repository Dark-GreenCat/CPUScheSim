#ifndef OBSERVER_H
#define OBSERVER_H

#include "process_queue.h"

typedef struct proc_obs_intf_t {
    void (*updateProcessList)(struct proc_obs_intf_t* this, const process_list_t* list);
    void (*updateRunningProcess)(struct proc_obs_intf_t* this, const process_t* process);
} proc_obs_intf_t;

typedef struct {
    proc_obs_intf_t observer[1];
    int             observer_count;
    void (*registerObserver)(struct proc_subj_intf_t* this, proc_obs_intf_t* observer);
    void (*removeObserver)(struct proc_subj_intf_t* this, proc_obs_intf_t* observer);
    void (*notifyProcessList)(struct proc_subj_intf_t* this, const process_list_t* list);
    void (*notifyRunningProcess)(struct proc_subj_intf_t* this, const process_t* process);
} proc_subj_intf_t;

#endif
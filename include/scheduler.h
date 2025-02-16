//
// Created by griot on 2/15/25.
//

#ifndef SCHEDULER_H
#define SCHEDULER_H
#include "process_queue.h"

typedef struct scheduler {
    ProcessQueue *queue;
} RoundRobinScheduler;

RoundRobinScheduler* create_round_robin_scheduler();
void destroy_round_robin_scheduler(RoundRobinScheduler *round_robin_scheduler);

#endif //SCHEDULER_H

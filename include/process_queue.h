//
// Created by banji on 1/25/2025.
//
#pragma once

#include <stdbool.h>
#include "process.h"

#ifndef QUEUE_H
#define QUEUE_H

/*=== The ProcessQueueState Enum and its Functions ===*/
typedef enum ProcessQueueState {
    PROCESS_QUEUE_IS_NOT_EMPTY,
    PROCESS_QUEUE_IS_EMPTY,
    PROCESS_QUEUE_IS_NULL,
    PROCESS_QUEUE_IS_FULL,
    PROCESS_QUEUE_CREATION_FAILED,
    PROCESS_QUEUE_MEMORY_ALLOCATION_FAILED,
    ENQUEUE_OPERATION_FAILED,DEQUEUE_OPERATION_FAILED,
    UNEXPECTED_PROCESS_QUEUE_ERROR
} ProcessQueueState;
// ProcessQueueState: Destruction Functions:
// NONE
// ProcessQueueState: Mutator Functions:
// NONE
// ProcessQueueState: Accessor Functions:
// NONE
// ProcessQueueState Boolean Functions:
// NONE
// ProcessQueue: ToString Function:
// NONE

// ProcessQueueState: toString
const char * process_queue_state_to_string(const ProcessQueueState process_queue_state);

typedef struct ProcessQueue {
    int size;
    ProcessNode * head;
    ProcessNode * tail;
    ProcessQueueState state;
} ProcessQueue;

// ProcessQueue: Creation functions:
ProcessQueue* createProcessQueue();

// ProcessQueue: Destruction functions:
// NONE

// ProcessQueue: Mutator functions:
Process * pop_process_queue(ProcessQueue* process_queue);
void clear_process_queue(ProcessQueue *queue);
bool enqueue_process(ProcessQueue* process_queue, Process* process);
Process* dequeue_process(ProcessQueue* process_queue);
void enqueue_process_node(ProcessQueue* process_queue, ProcessNode * process_node);
void delete_process_node(ProcessQueue* process_queue, const unsigned int process_id);
Process * select_process_tree(const Process * source);

// ProcessQueue: Accessor functions:
Process * find_process_by_id(const ProcessQueue* process_queue, const unsigned int process_id);
Process * find_process_by_name(const ProcessQueue* process_queue, const char * process_name);
void print_process_queue(const ProcessQueue* process_queue);

// ProcessQueue: Boolean functions:
bool process_queue_is_empty(const ProcessQueue* process_queue);

// ProcessQueue: ToString Functions
const char * process_queue_to_string(const ProcessQueue* queue);


/*=== The RoundRobinProcessQueue Data Type and Functions ===*/
typedef struct RoundRobinProcessQueue {
    ProcessQueue * queue;
} RoundRobinProcessQueue;

// RoundRobinProcessQueue: Creation Functions
RoundRobinProcessQueue * create_round_robin_process_queue ();

// RoundRobinProcessQueue: DestructionFunctions

// RoundRobinProcessQueue: Mutator Functions
bool join_round_robin_process_queue (const RoundRobinProcessQueue * round_robin_queue, Process * process);
Process * exit_round_robin_process_queue (const RoundRobinProcessQueue * roundRobin);

/*=== The PriorityProcessQueue Data Type and its Functions ===*/
typedef struct PriorityProcessQueue {
    ProcessQueue * queue;
} PriorityProcessQueue;

// PriorityProcessQueue: Creation functions
PriorityProcessQueue * create_priority_process_queue ();

// PriorityProcessQueue: Destruction functions:
void destroy_priority_process_queue (PriorityProcessQueue * process_queue);

// PriorityProcessQueue: Mutator Functions:
bool join_priority_process_queue (const PriorityProcessQueue * priority_queue, Process * process);
Process * exit_priority_process_queue (const PriorityProcessQueue * priority_queue);

// PriorityProcessQueue: Accessor Functions:
// NONE
// PriorityProcessQueue: Boolean Functions:
// NONE
// PriorityProcessQueue: ToString Functions:
// NONE
#endif //QUEUE_H

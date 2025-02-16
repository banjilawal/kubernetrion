//
// Created by banji on 1/25/2025.
//

#ifndef PROCESS_QUEUE_H
#define PROCESS_QUEUE_H

#pragma once
#include "process.h"

#define PROCESS_QUEUE_IS_EMPTY_MESSAGE "ProcessQueue is empty"
#define PROCESS_QUEUE_IS_NOT_EMPTY_MESSAGE "ProcessQueue is not empty"
#define PROCESS_QUEUE_IS_NULL_MESSAGE "ProcessQueue is NULL!"
#define PROCESS_QUEUE_IS_FULL_MESSAGE "ProcessQueue is full"
#define PROCESS_QUEUE_MEMORY_ALLOCATION_FAILED_MESSAGE "ProcessQueue memory allocation failed!"
#define PROCESS_QUEUE_PUSH_OPERATION_FAILED_MESSAGE "ProcessQueue push operation failed!"
#define PROCESS_QUEUE_POP_OPERATION_FAILED_MESSAGE "ProcessQueue pop operation failed!"
#define UNDEFINED_PROCESS_QUEUE_STATE_MESSAGE "ProcessQueue state is undefined!";

/*=== The ProcessQueueState Enum and its Functions ===*/
typedef enum ProcessQueueState {
    PROCESS_QUEUE_IS_NOT_EMPTY,
    PROCESS_QUEUE_IS_EMPTY,
    PROCESS_QUEUE_IS_NULL,
    PROCESS_QUEUE_IS_FULL,
    PROCESS_QUEUE_MEMORY_ALLOCATION_FAILED,
    PROCESS_QUEUE_PUSH_OPERATION_FAILED,
    PROCESS_QUEUE_POP_OPERATION_FAILED,
    UNEXPECTED_PROCESS_QUEUE_ERROR
} ProcessQueueState;
char* process_queue_state_to_string(const ProcessQueueState process_queue_state);

typedef struct ProcessQueue {
    int size;
    ProcessNode *head;
    ProcessNode *tail;
    ProcessQueueState state;
} ProcessQueue;

ProcessQueue* create_process_queue();
void clear_process_queue(ProcessQueue *queue);
void destroy_process_queue(ProcessQueue *process_queue);

unsigned int push_process_onto_queue(ProcessQueue *process_queue, Process *process);
Process* pop_process_from_queue(ProcessQueue *process_queue);

Process* find_process_by_id(const ProcessQueue *process_queue, const unsigned int process_id);
Process* find_process_by_name(const ProcessQueue *process_queue, const char *process_name);
void print_process_queue(const ProcessQueue *process_queue);

bool process_queue_is_empty(const ProcessQueue *process_queue);
char* process_queue_to_string(const ProcessQueue *process_queue);

/*=== The RoundRobinProcessQueue Data Type and Functions ===*/
typedef struct RoundRobinProcessQueue {
    ProcessQueue *queue;
} RoundRobinProcessQueue;

// RoundRobinProcessQueue: Creation Functions
RoundRobinProcessQueue * create_round_robin_process_queue ();

// RoundRobinProcessQueue: DestructionFunctions

// RoundRobinProcessQueue: Mutator Functions
bool enter_round_robin_process_queue (const RoundRobinProcessQueue *round_robin_queue, Process *process);
Process* exit_round_robin_process_queue (const RoundRobinProcessQueue *roundRobin);

/*=== The PriorityProcessQueue Data Type and its Functions ===*/
typedef struct PriorityProcessQueue {
    ProcessQueue* queue;
} PriorityProcessQueue;

// PriorityProcessQueue: Creation functions
PriorityProcessQueue* create_priority_process_queue ();

// PriorityProcessQueue: Destruction functions:
void destroy_priority_process_queue (PriorityProcessQueue* process_queue);

// PriorityProcessQueue: Mutator Functions:
bool join_priority_process_queue (const PriorityProcessQueue *priority_queue, Process *process);
Process* exit_priority_process_queue (const PriorityProcessQueue *priority_queue);

#endif //PROCESS_QUEUE_H

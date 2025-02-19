//
// Created by banji on 1/25/2025.
//

#ifndef PROCESS_QUEUE_H
#define PROCESS_QUEUE_H

#pragma once
struct Process;
typedef struct Process Process;

#pragma once
struct ProcessNode;
typedef struct ProcessNode ProcessNode;

#include <stdbool.h>


#define PROCESS_LIST_IS_EMPTY_MESSAGE "ProcessList is empty"
#define PROCESS_LIST_IS_NOT_EMPTY_MESSAGE "ProcessList is not empty"
#define PROCESS_LIST_IS_NULL_MESSAGE "ProcessList is NULL!"

#define PROCESS_LIST_MEMORY_ALLOCATION_FAILED_MESSAGE "ProcessList memory allocation failed!"
#define PROCESS_LIST_TAIL_INSERTION_FAILED_MESSAGE "ProcessList tail insertion failed!"
#define PROCESS_LIST_HEAD_INSERTION_FAILED_MESSAGE "ProcessList head insertion failed!"
#define PROCESS_LIST_ITEM_REMOVAL_FAILED_MESSAGE "ProcessList item removal failed!"
#define UNEXPECTED_PROCESS_LIST_STATE_MESSAGE "Unexpected ProcessList error!"

#define PROCESS_QUEUE_IS_NULL_MESSAGE "ProcessQueue is NULL!"
#define PROCESS_QUEUE_IS_EMPTY_MESSAGE "ProcessQueue is empty!"
#define PROCESS_QUEUE_MEMORY_ALLOCATION_FAILED_MESSAGE "ProcessQueue memory allocation failed!"
#define PROCESS_QUEUE_PUSH_OPERATION_FAILED_MESSAGE "ProcessQueue push operation failed!"
#define PROCESS_QUEUE_POP_OPERATION_FAILED_MESSAGE "ProcessQueue pop operation failed!"
#define UNDEFINED_PROCESS_QUEUE_STATE_MESSAGE "ProcessQueue state is undefined!"

typedef enum ProcessListState {
    PROCESS_LIST_IS_EMPTY,
    PROCESS_LIST_IS_NOT_EMPTY,
    PROCESS_LIST_IS_NULL,
    PROCESS_LIST_IS_MEMORY_ALLOCATION_FAILED,
    PROCESS_LIST_TAIL_INSERTION_FAILED,
    PROCESS_LIST_HEAD_INSERTION_FAILED,
    PROCESS_LIST_ITEM_REMOVAL_FAILED,
    UNEXPECTED_PROCESS_LIST_ERROR
} ProcessListState;
char * process_list_state_to_string(const ProcessListState state);

typedef struct ProcessLinkedList {
    int size;
    ProcessNode *head;
    ProcessNode *tail;
} ProcessLinkedList;

ProcessLinkedList* create_process_list();

unsigned int print_process_list(const ProcessLinkedList *process_list);
void destroy_process_list(ProcessLinkedList *process_linked_list);

unsigned int insert_process_at_list_head(ProcessLinkedList *process_list, Process *process);
unsigned int insert_process_at_list_tail(ProcessLinkedList *process_list, Process *process);

Process* remove_process_from_list(ProcessLinkedList *process_list, const unsigned int process_id);
Process* dequeue_from_process_list(ProcessLinkedList *process_list);
unsigned int enqueue_into_process_list(ProcessLinkedList *process_list, Process *process);
Process* search_process_list(const ProcessLinkedList *process_list, const unsigned int process_id);

bool is_empty_process_list(const ProcessLinkedList *process_list);


/*=== The ProcessQueueState Enum and its Functions ===*/
typedef enum ProcessQueueState {
    PROCESS_QUEUE_IS_NULL,
    PROCESS_QUEUE_IS_EMPTY,
    PROCESS_QUEUE_MEMORY_ALLOCATION_FAILED,
    PROCESS_QUEUE_PUSH_OPERATION_FAILED,
    PROCESS_QUEUE_POP_OPERATION_FAILED,
    UNEXPECTED_PROCESS_QUEUE_ERROR
} ProcessQueueState;
char* process_queue_state_to_string(const ProcessQueueState process_queue_state);

typedef struct ProcessQueue {
    char *name;
    ProcessLinkedList *processes;
    ProcessQueueState state;
} ProcessQueue;

ProcessQueue* create_process_queue(char *name);
void destroy_process_queue(ProcessQueue *process_queue);

unsigned int push_onto_process_queue(ProcessQueue *process_queue, Process *process);
Process* pop_from_process_queue(ProcessQueue *process_queue);

// /*=== The RoundRobinProcessQueue Data Type and Functions ===*/
// typedef struct RoundRobinProcessQueue {
//     ProcessQueue *processes;
// } RoundRobinProcessQueue;
//
// // RoundRobinProcessQueue: Creation Functions
// RoundRobinProcessQueue * create_round_robin_process_queue ();
//
// // RoundRobinProcessQueue: DestructionFunctions
//
// // RoundRobinProcessQueue: Mutator Functions
// bool enter_round_robin_process_queue (const RoundRobinProcessQueue *round_robin_queue, Process *process);
// Process* exit_round_robin_process_queue (const RoundRobinProcessQueue *roundRobin);
//
// /*=== The PriorityProcessQueue Data Type and its Functions ===*/
// typedef struct PriorityProcessQueue {
//     ProcessQueue* processes;
// } PriorityProcessQueue;
//
// // PriorityProcessQueue: Creation functions
// PriorityProcessQueue* create_priority_process_queue ();
//
// // PriorityProcessQueue: Destruction functions:
// void destroy_priority_process_queue (PriorityProcessQueue* process_queue);
//
// // PriorityProcessQueue: Mutator Functions:
// bool join_priority_process_queue (const PriorityProcessQueue *priority_queue, Process *process);
// Process* exit_priority_process_queue (const PriorityProcessQueue *priority_queue);

#endif //PROCESS_QUEUE_H

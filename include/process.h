//
// Created by banji on 12/10/2024.
//

#include "file.h"

#ifndef PROCESS_H
#define PROCESS_H

#define NAME_LENGTH 64
#define MAX_PRIORITY 100
#define MIN_PRIORITY 1
#define PROCESS_STRING_SIZE 256

//
// Created by banji on 12/10/2024.
//
#pragma once
#include <stdbool.h>

/*=== ProcessState Enum and Functions ===*/
typedef enum ProcessState {
    PROCESS_READY,
    PROCESS_RUNNING,
    PROCESS_BLOCKED,
    PROCESS_WAITING,
    PROCESS_FINISHED,
    PROCESS_IS_NULL,
    PROCESS_READING_FILE,
    PROCESS_WRITING_FILE,
    PROCESS_MEMORY_ALLOCATION_FAILED
} ProcessState;

// ProcessState: Destruction Functions:
// NONE
// ProcessState: Mutator Functions:
// NONE
// ProcessState: Accessor Functions:
// NONE
// ProcessState: Boolean Functions:
// NONE
// ProcessState: ToString Function:
// NONE
// ProcessState: toString Function
const char * process_state_to_string(const ProcessState process_state);

/*=== The Process Data Type and Functions ===*/
typedef struct Process {
    const char * name;
    File * file;
    unsigned int id;
    unsigned int number_of_child_processes;
    unsigned int priority;
    unsigned int initial_queue_entry_time;
    unsigned int milliseconds_remaining;
    struct Process * parent;
    struct Process * child;
    ProcessState state;
} Process;

// Process: Creation Functions
Process* create_process(
    const char* name,
    File* file,
    unsigned int id,
    unsigned int priority,
    unsigned int milliseconds_remaining,
    Process * parent
);

// Process: Destruction Functions:
void destroy_process(Process * process);

// Process: Mutator Functions:
void remove_child_process(Process * parent, Process * child);

// Process: Accessor Functions:
unsigned int get_process_id(const Process * process);

// Process: Boolean Functions:
bool processes_are_equal(const Process * a, const Process * b);

// Process: ToString Function:
const char * process_to_string(const Process* process);

/*=== ProcessNodeState Enum and Functions ===*/
typedef enum ProcessNodeState {
    PROCESS_NODE_IS_NULL,
    PROCESS_NODE_MEMORY_ALLOCATION_FAILED
} ProcessNodeState;

// ProcessNodeState: Destruction Functions:
// NONE
// ProcessNodeState: Mutator Functions:
// NONE
// ProcessNodeState: Accessor Functions:
// NONE
// ProcessNodeState: Boolean Functions:
// NONE
// ProcessNodeState: ToString Function:
// NONE

// ProcessNOdeState: toString Function
const char * process_node_to_string(const ProcessNodeState process_node_state);

/*=== The ProcessNode Data Type and It's Functions ===*/
typedef struct ProcessNode {
    Process * process;
    struct ProcessNode * next;
    struct ProcessNode * previous;
} ProcessNode;

// ProcessNode: Creation functions:
ProcessNode * create_process_node(Process * process);

// ProcessNode: Destruction functions:
void destroy_process_node(ProcessNode * process_node);

// ProcessNode: Mutator Functions:
// NONE
// ProcessNode: Accessor Functions:
// NONE
// Process: Boolean Functions:
// NONE

// ProcessNode: ToString Function:
// NONE

#endif //PROCESS_H

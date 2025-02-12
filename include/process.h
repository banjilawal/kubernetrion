//
// Created by banji on 12/10/2024.
//


#ifndef PROCESS_H
#define PROCESS_H

#pragma once
#include <stdbool.h>
#include "file.h"

#define NAME_LENGTH 64
#define MAX_PRIORITY 100
#define MIN_PRIORITY 1
#define PROCESS_STRING_SIZE 256

/*=== ProcessState Enum and Functions ===*/
typedef enum ProcessState {
    PROCESS_READY,
    PROCESS_RUNNING,
    PROCESS_READING_FILE_BLOCKED,
    PROCESS_WRITING_FILE_BLOCKED,
    PROCESS_WAITING_EVENT,
    PROCESS_FINISHED,
    PROCESS_IS_NULL,
    PROCESS_READING_FILE,
    PROCESS_WRITING_FILE,
    PROCESS_MEMORY_ALLOCATION_FAILED
} ProcessState;

// ProcessState: toString Function
const char * process_state_to_string(const ProcessState process_state);

/*=== The Process Data Type and Functions ===*/
typedef struct Process {
    const unsigned int id;
    const char * name;
    struct Process * parent;
    struct Process * child;
    File * reading_file;
    File * writing_file;
    unsigned int priority;
    unsigned int number_of_child_processes;
    unsigned int initial_queue_entry_time;
    unsigned int milliseconds_remaining;
    ProcessState state;
} Process;

// Process: Creation Functions
Process* create_process(
    const unsigned int id,
    const char * name,
    struct Process * parent,
    struct Process * child,
    File * reading_file,
    File * writing_file,
    const unsigned int priority,
    const unsigned int milliseconds_remaining
);

// Process: Destruction Functions:
void destroy_process(Process * process);

// Process: Mutator Functions:
File * unset_reading_file(Process * process);
File * unset_writing_file(Process * process);
void set_reading_file(Process * process, File * reading_file);
void set_writing_file(Process * process, File * writing_file);
void remove_child_process(Process * parent, Process * child);

// Process: Accessor Functions:
unsigned int get_process_id(const Process * process);
const char * get_process_name(const Process * process);

// Process: Boolean Functions:
bool processes_are_equal(const Process * a, const Process * b);

// Process: ToString Function:
const char * process_to_string(const Process* process);

/*=== ProcessNodeState Enum and Functions ===*/
typedef enum ProcessNodeState {
    PROCESS_NODE_IS_NULL,
    PROCESS_NODE_MEMORY_ALLOCATION_FAILED
} ProcessNodeState;

// ProcessNodeState: toString Function
const char * process_node_state_to_string(const ProcessNodeState process_node_state);

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

#endif //PROCESS_H

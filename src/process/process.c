#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "process.h"



#ifndef PROCESS_H
#define PROCESS_H

#define NAME_LENGTH 64
#define MAX_PRIORITY 100
#define MIN_PRIORITY 1

//
// Created by banji on 12/10/2024.
//
#pragma once
#include <stdbool.h>






#endif //PROCESS_H

// const char * errorStateToString(const enum ErrorState state) {
//     switch (state) {
//         case PROCESS_QUEUE_IS_NULL: return "Process queue is NULL!";
//         case PROCESS_NODE_IS_NULL: return "Process queue node is NULL!";
//         case PROCESS_IS_NULL: return "Process is NULL!";
//         case PROCESS_QUEUE_IS_EMPTY: return "Process queue is empty!";
//         default: return "Unknown error state!";
//     }
// }


/*=== ProcessState Enum and Functions ===*/
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
const char * process_state_to_string(const ProcessState process_state) {
    switch (process_state) {
        case PROCESS_READY: return "Ready";
        case PROCESS_RUNNING: return "Running";
        case PROCESS_BLOCKED: return "Blocked";
        case PROCESS_WAITING: return "Waiting";
        case PROCESS_FINISHED: return "Finished";
        case PROCESS_IS_NULL: return "Process is NULL!";
        case PROCESS_READING_FILE: return "Process is reading file";
        case PROCESS_WRITING_FILE: return "Process is writing file";
        case PROCESS_MEMORY_ALLOCATION_FAILED: return "Process memory allocation failed!";
        default: return "Unknown process_state";
    }
}

/*=== The Process Data Type and Functions ===*/

// Process: Creation Functions
Process * create_process(
    const char * name,
    File * file,
    const unsigned int id,
    const unsigned int priority,
    const unsigned int milliseconds_remaining,
    Process * parent
) {
    Process * process = (Process *) malloc(sizeof(Process));
    if (process == NULL) {
        printf("%s\n", process_state_to_string(PROCESS_MEMORY_ALLOCATION_FAILED));
        return NULL;
    }
    process->parent = parent;
    process->id = id;
    process->name = strdup(name);
    process->priority = priority;
    process->milliseconds_remaining = milliseconds_remaining;
    process->file = file;
    process->initial_queue_entry_time = 0;
    process->child = NULL;
    process->number_of_child_processes = 0;

    return process;
}


// Process: Destruction Functions:
void destroy_process(Process * process) {
    if (process != NULL && process->child == NULL && process->number_of_child_processes == 0) {
        process->file = NULL;
        process->parent = NULL;
        free((void *) process->name);
        free(process);
    }
}

// Process: Mutator Functions:
void remove_child_process(Process * parent, Process* child) {
    if (parent != NULL && child != NULL && child->child == NULL) {
        if (parent->child == child) {
            child->parent = NULL;
            parent->child = NULL;
            parent->number_of_child_processes--;
        }
    }
}

// Process: Accessor Functions:
unsigned int get_process_id(const Process * process) { return process->id; }

// Process: Boolean Functions:
bool processes_are_equal (const Process * a, const Process * b) { return (a == b || a->id == b->id); }

// Process: ToString Function:
const char * process_to_string(const Process * process) {
    if (process == NULL) {
        printf("%s.\n", process_state_to_string(PROCESS_IS_NULL));
        return NULL;
    }

    const int bufferSize = PROCESS_STRING_SIZE * sizeof(char);
    char * string = (char *) malloc(bufferSize);
    if (string == NULL) {
        printf("Could not allocate memory for process_to_string.\n");
        return NULL;
    }

    unsigned int parentId = 0;
    if (process->parent != NULL) { parentId = process->parent->id; }

    const char *format = "Process[address:%p, ParentID:%d ID:%d, Name:%s, State:%s, Priority:%d "
                         "RemainingMilliseconds:%d, CPU Cycles:%d, File:%s]";
    snprintf(
        string, bufferSize, format, (void *)process, parentId, process->id, process->name,
        process_state_to_string(process->state),
        process->priority, process->milliseconds_remaining,
        process->initial_queue_entry_time,
        (process->file != NULL && process->file->descriptor != NULL) ? process->file->descriptor->name : "NULL"
    );
    return string;
}

/*=== ProcessNodeState Enum and Functions ===*/

// ProcessNodeState: Destruction Functions:
// NONE
// ProcessNodeState: Mutator Functions:
// NONE
// ProcessNodeState: Accessor Functions:
// NONE
// ProcessNodeState: Boolean Functions:
// NONE

// ProcessNodeState: ToString Function:
const char * process_node_state_to_string(const ProcessNodeState process_node_state) {
    switch (process_node_state) {
        case PROCESS_NODE_IS_NULL: return "ProcessNode is NULL!";
        case PROCESS_NODE_MEMORY_ALLOCATION_FAILED: return "ProcessNode memory allocation failed!";
        default: return "Unknown process_state";
    }
}

/*=== The ProcessNode Data Type and It's Functions ===*/

// ProcessNode: Creation functions:
ProcessNode * create_process_node(Process * process) {
    if (process == NULL) {
        printf("%s.\n", process_node_to_string(PROCESS_NODE_MEMORY_ALLOCATION_FAILED));
        return NULL;
    }

    ProcessNode * processQueueNode = (ProcessNode *) malloc(sizeof(ProcessNode));
    if (processQueueNode == NULL) {
        printf("%s\n", process_node_to_string(PROCESS_NODE_MEMORY_ALLOCATION_FAILED));
        return NULL;
    }
    processQueueNode->process = process;
    processQueueNode->next = NULL;
    processQueueNode->previous = NULL;
    return processQueueNode;
}

// ProcessNode: Destruction functions:
void destroy_process_node(ProcessNode * process_node) {
    if (process_node != NULL) return;
    if (process_node->next == NULL && process_node->previous == NULL) {
        destroy_process_node(process_node->next);
        free(process_node->next);
        free(process_node->previous);
        destroy_process(process_node->process);
        free(process_node);
    }
}

// ProcessNode: Mutator Functions:
// NONE
// ProcessNode: Accessor Functions:
// NONE
// ProcessNode: Boolean Functions:
// NONE

// ProcessNode: ToString Function:
const char * process_node_to_string(const ProcessNodeState process_node_state) {
    switch (process_node_state) {
        case PROCESS_NODE_IS_NULL: return "ProcessNode is NULL!";
        case PROCESS_NODE_MEMORY_ALLOCATION_FAILED: return "ProcessNode memory allocation failed!";
        default: return "Unknown ProcessNode process_node_state!";
    }
}




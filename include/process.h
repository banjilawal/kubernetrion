//
// Created by banji on 12/10/2024.
//


#ifndef PROCESS_H
#define PROCESS_H

#ifdef __cplusplus
extern "C" {
#endif

#pragma once
#include <stdbool.h>
#include <limits.h>
#include <stddef.h>

#include "file.h"

/*
 * Forward declarations to avoid circular dependencies. They might not need to be in both files with the
 * dependency relationship
 */
// struct File;
// typedef struct File File;

// With forward declaration we don't need the include directive.
// #include "file.h"

/*===== CONSTANTS ====*/

#define MAX_PROCESS_NAME_LENGTH 64
#define PROCESS_STRING_SIZE 256

#define PROCESS_READY_MESSAGE "Process is ready"
#define PROCESS_RUNNING_MESSAGE "Process is running"
#define PROCESS_READING_FILE_IS_BLOCKED_MESSAGE "Reading file is blocked"
#define PROCESS_WRITING_FILE_IS_BLOCKED_MESSAGE "Writing blocked"
#define PROCESS_WAITING_EVENT_MESSAGE "Process is waiting event"
#define PROCESS_FINISHED_MESSAGE "Process is finished"
#define PROCESS_IS_NULL_MESSAGE "Process is NULL!"
#define PROCESS_READING_FILE_MESSAGE "Process is reading file"
#define PROCESS_WRITING_FILE_MESSAGE "Process is writing file"
#define PROCESS_MEMORY_ALLOCATION_FAILED_MESSAGE "Process memory allocation failed!"
#define PROCESS_PRIORITY_OUT_OF_RANGE_ERROR_MESSAGE "Process priority is out of range"
#define NULL_PROCESS_ERROR_MESSAGE "Process cannot be null!"
#define PROCESS_NAME_IS_NULL_ERROR_MESSAGE "Process name cannot be null"
#define UNDEFINED_PROCESS_STATE_MESSAGE "The Process state is undefined!"

#define PROCESS_NODE_IS_NULL_MESSAGE "ProcessNode is NULL!"
#define PROCESS_NODE_MEMORY_ALLOCATION_FAILED_MESSAGE "ProcessNode memory allocation failed!"
#define UNDEFINED_PROCESS_NODE_STATE_MESSAGE "The ProcessNode state is undefined!"
#define PROCESS_NODE_CREATION_FAILED_MESSAGE "ProcessNode creation failed!"

/*==== Enums ====*/

/*
 * Process priority
 */
typedef enum ProcessPriority {
    MIN_PRIORITY = 1,
    MAX_PRIORITY = 100
} ProcessPriority;

typedef enum ProcessErrorCode {
    PROCESS_OPERATION_SUCCESS = 0,
    PROCESS_MEMORY_ALLOCATION_FAILURE_ERROR = 1,
    PROCESS_IS_NULL_ILLEGAL_ARGUMENT_ERROR = 2,
    PROCESS_NAME_IS_NULL_ILLEGAL_ARGUMENT_ERROR = 3,
    PROCESS_PRIORITY_OUT_OF_RANGE_ERROR = 4,
    PROCESS_READING_FILE_IS_BLOCKED_EXCEPTION = 5,
    PROCESS_WRITING_FILE_IS_BLOCKED_EXCEPTION = 6,
    PROCESS_RELEASING_FILE_FAILED_EXCEPTION = 7,
    PROCESS_NODE_CREATION_FAILED_EXCEPTION = 8,
} ProcessErrorCode;

/*
 * Different states a process can have.
 */
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

// ProcessState Functions
/**
* Converts a process state enum value to its textual representation.
*
* @param process_state The process state to be converted.
* @return A constant string describing the process state.
*/
const char * process_state_to_string(const ProcessState process_state);

/*==== Data Types and their Functions */

/*
 * Describes a process
 */
typedef struct Process {
    unsigned int id;
    char *name;
    struct Process *parent;
    struct Process *child;
    File *file;
    unsigned int priority;
    unsigned int cpu_cycle_count;
    unsigned int number_of_child_processes;
    unsigned int milliseconds_remaining;
    ProcessState state;
} Process;

/* Process Functions */
Process* create_process(
    const unsigned int id, const char * name, Process *parent, Process *child,
    File *file, const unsigned int priority,const unsigned int milliseconds_remaining
);

void destroy_process(Process *process);
void destroy_child_process(Process *parent_process);

File* release_file(Process *process);
unsigned int read_file(Process *process, File *file);
unsigned int write_file(Process *process, File *file, unsigned int bytes_to_write);

bool processes_are_equal(const Process *a, const Process *b);
char* process_to_string(const Process *process);

/*=== ProcessNodeState Enum and Functions ===*/
typedef enum ProcessNodeState {
    PROCESS_NODE_IS_NULL,
    PROCESS_NODE_MEMORY_ALLOCATION_FAILED
} ProcessNodeState;
char* process_node_state_to_string(const ProcessNodeState process_node_state);

/*=== The ProcessNode Data Type and It's Functions ===*/
typedef struct ProcessNode {
    Process *process;
    struct ProcessNode *next;
    struct ProcessNode *previous;
} ProcessNode;

/* ProcessNode Functions */
ProcessNode* create_process_node(Process *process);
void destroy_process_node(ProcessNode *process_node);


#endif //PROCESS_H

// #ifndef PROCESS_H
// #define PROCESS_H
//
// #ifdef __cplusplus
// extern "C" {
// #endif
//
// #include <stdbool.h>  // For `bool` usage
// #include <stddef.h>   // For `NULL` usage
//
// // Forward declaration of the File type to avoid unnecessary dependencies
// struct File;
// typedef struct File File;
//
// // === CONSTANTS ===
// /**
//  * Maximum and minimum priority levels for a process.
//  */
// enum { MAX_PRIORITY = 100, MIN_PRIORITY = 1 };
//
// /**
//  * Maximum allowed length for a process name.
//  */
// #define MAX_PROCESS_NAME_LENGTH 64
//
// /**
//  * String size for process representation (e.g., in `process_to_string`).
//  */
// #define PROCESS_STRING_SIZE 256
//
// // === ENUMERATIONS ===
// /**
//  * Represents the state of a process in the system.
//  */
// typedef enum ProcessState {
//     PROCESS_READY,
//     PROCESS_RUNNING,
//     PROCESS_READING_FILE_BLOCKED,
//     PROCESS_WRITING_FILE_BLOCKED,
//     PROCESS_WAITING_EVENT,
//     PROCESS_FINISHED,
//     PROCESS_IS_NULL,
//     PROCESS_READING_FILE,
//     PROCESS_WRITING_FILE,
//     PROCESS_MEMORY_ALLOCATION_FAILED,
//     PROCESS_STATE_UNDEFINED // Defined for invalid or unexpected states
// } ProcessState;
//
// /**
//  * Represents the possible error states or statuses of a ProcessNode.
//  */
// typedef enum ProcessNodeState {
//     PROCESS_NODE_IS_NULL,
//     PROCESS_NODE_MEMORY_ALLOCATION_FAILED
// } ProcessNodeState;
//
// // === STRUCT DECLARATIONS ===
//
// /**
//  * Represents a process in the system.
//  */
// typedef struct Process {
//     const unsigned int id;               ///< Unique identifier for the process
//     const char *name;                    ///< Process name (maximum length MAX_PROCESS_NAME_LENGTH)
//     struct Process *parent;              ///< Parent process, or NULL if none
//     struct Process *child;               ///< Child process, or NULL if none
//     File *file_reading_from;                  ///< The file currently being read by the process, NULL if none
//     File *file_writing_to;                  ///< The file being written by the process, NULL if none
//     unsigned int priority;               ///< Process priority (MIN_PRIORITY to MAX_PRIORITY)
//     unsigned int cpu_cycle_count;        ///< Count of CPU cycles used by the process
//     unsigned int number_of_child_processes; ///< Number of child processes under this process
//     unsigned int milliseconds_remaining; ///< Time remaining for the process execution
//     ProcessState state;                  ///< Current state of the process
// } Process;
//
// /**
//  * A node structure used in a linked list of processes.
//  */
// typedef struct ProcessNode {
//     Process *process;                    ///< Pointer to the process represented by this node
//     struct ProcessNode *next;            ///< Pointer to the next node in the list
//     struct ProcessNode *previous;        ///< Pointer to the previous node in the list
// } ProcessNode;
//
// // === FUNCTION DECLARATIONS ===
// // Documentation is added for every function describing purpose, parameters, and return value
//
// // Process State Functions
// /**
//  * Converts a process state enum value to its textual representation.
//  *
//  * @param process_state The process state to be converted.
//  * @return A constant string describing the process state.
//  */
// const char *process_state_to_string(const ProcessState process_state);
//
// // Process Functions
// /**
//  * Creates a new Process instance.
//  *
//  * @param id The unique process ID.
//  * @param name The name of the process.
//  * @param parent Pointer to the parent process, or NULL if none.
//  * @param child Pointer to the child process, or NULL if none.
//  * @param file_reading_from Pointer to the file being read, or NULL if none.
//  * @param file_writing_to Pointer to the file being written, or NULL if none.
//  * @param priority The priority of the process (range MIN_PRIORITY to MAX_PRIORITY).
//  * @param milliseconds_remaining The execution time remaining for the process.
//  * @return Pointer to the newly created Process, or NULL on memory allocation failure.
//  */
// Process *create_process(
//     unsigned int id,
//     const char *name,
//     struct Process *parent,
//     struct Process *child,
//     File *file_reading_from,
//     File *file_writing_to,
//     unsigned int priority,
//     unsigned int milliseconds_remaining
// );
//
// /**
//  * Releases the memory used by a Process.
//  *
//  * @param process Pointer to the Process to be destroyed.
//  */
// void destroy_process(Process *process);
//
// /**
//  * Unsets the file the process is currently reading.
//  *
//  * @param process Pointer to the process.
//  * @return Pointer to the unset File, or NULL if no file was being read.
//  */
// File *release_file(Process *process);
//
// /**
//  * Unsets the file the process is writing.
//  *
//  * @param process Pointer to the process.
//  * @return Pointer to the unset File, or NULL if no file was being written.
//  */
// File *unset_file_writing_to(Process *process);
//
// /**
//  * Compares two processes for equality.
//  *
//  * @param a Pointer to the first process.
//  * @param b Pointer to the second process.
//  * @return `true` if the processes are equal, otherwise `false`.
//  */
// bool processes_are_equal(const Process *a, const Process *b);
//
// /**
//  * Converts a process to its string representation.
//  *
//  * @param process Pointer to the process.
//  * @return A dynamically allocated string describing the process. Caller must free the memory.
//  */
// char *process_to_string(const Process *process);
//
// // ProcessNode Functions
// /**
//  * Creates a new ProcessNode instance.
//  *
//  * @param process Pointer to the process to be associated with this node.
//  * @return Pointer to the newly created ProcessNode, or NULL on memory allocation failure.
//  */
// ProcessNode *create_process_node(Process *process);
//
// /**
//  * Releases the memory used by a ProcessNode.
//  *
//  * @param process_node Pointer to the ProcessNode to be destroyed.
//  */
// void destroy_process_node(ProcessNode *process_node);
//
// // ProcessNode State Functions
// /**
//  * Converts a ProcessNodeState enum value to its textual representation.
//  *
//  * @param process_node_state The ProcessNodeState enum value to convert.
//  * @return A constant string representing the ProcessNodeState.
//  */
// const char *process_node_state_to_string(const ProcessNodeState process_node_state);
//
// #ifdef __cplusplus
// }
// #endif
//
// #endif // PROCESS_H
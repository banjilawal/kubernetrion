#ifndef PROCESS_H
#define PROCESS_H
//
#ifdef __cplusplus
extern "C" {
#endif
//
#include <stdbool.h>  For `bool` usage
#include <stddef.h>   For `NULL` usage
//
Forward declaration of the File type to avoid unnecessary dependencies
struct File;
typedef struct File File;
//
=== CONSTANTS ===
/**
* Maximum and minimum priority levels for a process.
*/
enum { MAX_PRIORITY = 100, MIN_PRIORITY = 1 };
//
/**
* Maximum allowed length for a process name.
*/
#define MAX_PROCESS_NAME_LENGTH 64
//
/**
* String size for process representation (e.g., in `process_to_string`).
*/
#define PROCESS_STRING_SIZE 256
//
=== ENUMERATIONS ===
/**
* Represents the state of a process in the system.
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
   PROCESS_MEMORY_ALLOCATION_FAILED,
   PROCESS_STATE_UNDEFINED Defined for invalid or unexpected states
} ProcessState;
//
/**
* Represents the possible error states or statuses of a ProcessNode.
*/
typedef enum ProcessNodeState {
   PROCESS_NODE_IS_NULL,
   PROCESS_NODE_MEMORY_ALLOCATION_FAILED
} ProcessNodeState;
//
=== STRUCT DECLARATIONS ===
//
/**
* Represents a process in the system.
*/
typedef struct Process {
   const unsigned int id;               ///< Unique identifier for the process
   const char *name;                    ///< Process name (maximum length MAX_PROCESS_NAME_LENGTH)
   struct Process *parent;              ///< Parent process, or NULL if none
   struct Process *child;               ///< Child process, or NULL if none
   File *reading_file;                  ///< The file currently being read by the process, NULL if none
   File *writing_file;                  ///< The file being written by the process, NULL if none
   unsigned int priority;               ///< Process priority (MIN_PRIORITY to MAX_PRIORITY)
   unsigned int cpu_cycle_count;        ///< Count of CPU cycles used by the process
   unsigned int number_of_child_processes; ///< Number of child processes under this process
   unsigned int milliseconds_remaining; ///< Time remaining for the process execution
   ProcessState state;                  ///< Current state of the process
} Process;
//
/**
* A node structure used in a linked list of processes.
*/
typedef struct ProcessNode {
   Process *process;                    ///< Pointer to the process represented by this node
   struct ProcessNode *next;            ///< Pointer to the next node in the list
   struct ProcessNode *previous;        ///< Pointer to the previous node in the list
} ProcessNode;
//
=== FUNCTION DECLARATIONS ===
Documentation is added for every function describing purpose, parameters, and return value
//
Process State Functions
/**
* Converts a process state enum value to its textual representation.
*
* @param process_state The process state to be converted.
* @return A constant string describing the process state.
*/
const char *process_state_to_string(const ProcessState process_state);
//
Process Functions
/**
* Creates a new Process instance.
*
* @param id The unique process ID.
* @param name The name of the process.
* @param parent Pointer to the parent process, or NULL if none.
* @param child Pointer to the child process, or NULL if none.
* @param reading_file Pointer to the file being read, or NULL if none.
* @param writing_file Pointer to the file being written, or NULL if none.
* @param priority The priority of the process (range MIN_PRIORITY to MAX_PRIORITY).
* @param milliseconds_remaining The execution time remaining for the process.
* @return Pointer to the newly created Process, or NULL on memory allocation failure.
*/
Process *create_process(
   unsigned int id,
   const char *name,
   struct Process *parent,
   struct Process *child,
   File *reading_file,
   File *writing_file,
   unsigned int priority,
   unsigned int milliseconds_remaining
);
//
/**
* Releases the memory used by a Process.
*
* @param process Pointer to the Process to be destroyed.
*/
void destroy_process(Process *process);
//
/**
* Unsets the file the process is currently reading.
*
* @param process Pointer to the process.
* @return Pointer to the unset File, or NULL if no file was being read.
*/
File *unset_reading_file(Process *process);

/**
* Unsets the file the process is writing.
*
* @param process Pointer to the process.
* @return Pointer to the unset File, or NULL if no file was being written.
*/
File *unset_writing_file(Process *process);
//
/**
* Compares two processes for equality.
*
* @param a Pointer to the first process.
* @param b Pointer to the second process.
* @return `true` if the processes are equal, otherwise `false`.
*/
bool processes_are_equal(const Process *a, const Process *b);
//
/**
* Converts a process to its string representation.
*
* @param process Pointer to the process.
* @return A dynamically allocated string describing the process. Caller must free the memory.
*/
char *process_to_string(const Process *process);
//
ProcessNode Functions
/**
* Creates a new ProcessNode instance.
*
* @param process Pointer to the process to be associated with this node.
* @return Pointer to the newly created ProcessNode, or NULL on memory allocation failure.
*/
ProcessNode *create_process_node(Process *process);
//
/**
* Releases the memory used by a ProcessNode.
*
* @param process_node Pointer to the ProcessNode to be destroyed.
*/
void destroy_process_node(ProcessNode *process_node);
//
ProcessNode State Functions
/**
* Converts a ProcessNodeState enum value to its textual representation.
*
* @param process_node_state The ProcessNodeState enum value to convert.
* @return A constant string representing the ProcessNodeState.
*/
const char *process_node_state_to_string(const ProcessNodeState process_node_state);
//
#ifdef __cplusplus
}
#endif
//
#endif PROCESS_H

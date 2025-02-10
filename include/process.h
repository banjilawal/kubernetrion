//
// Created by banji on 12/10/2024.
//

#include "file.h"

#ifndef PROCESS_H
#define PROCESS_H

#define NAME_LENGTH 64
#define MAX_PRIORITY 100
#define MIN_PRIORITY 1

/* Error state enum and functions */
typedef enum ErrorState {
    // PROCESS_QUEUE_IS_NULL,
    // PROCESS_QUEUE_NODE_IS_NULL,
    // PROCESS_IS_NULL,
    // IS_EMPTY,
    UNKNOWN_ERROR
} ErrorState;
const char * errorStateToString(const ErrorState state);

/* ProcessState enum and functions */
typedef enum ProcessState {
    READY, RUNNING, BLOCKED, WAITING, FINISHED, PROCESS_IS_NULL, PROCESS_MEMORY_ALLOCATION_FAILED
} ProcessState;
const char * processStateToString(const ProcessState state);

/* Process data types and methods */
typedef struct Process {
    const char* name;
    File* file;
    unsigned int id;
    unsigned int childrenCount;
    unsigned int priority;
    unsigned int timeQueued;
    unsigned int remainingMilliseconds;
    struct Process * parent;
    struct Process * child;
    ProcessState state;
} Process;

Process* createProcess(
    const char* name,
    File* file,
    unsigned int id,
    unsigned int priority,
    unsigned int remainingMilliseconds,
    Process * parent
);
void killProcess(Process * process);
void removeChild(Process * parent, Process* child);
unsigned int getProcessID(const Process * process);
const char * processToString(const Process* process);

/* ProcessQueueNode type */
typedef enum ProcessQueueNodeState {
    PROCESS_QUEUE_NODE_IS_NULL, PROCESS_QUEUE_NODE_MEMORY_ALLOCATION_FAILED
} ProcessQueueNodeState;
const char * processQueueNodeStateToString(const ProcessQueueNodeState state);

typedef struct ProcessQueueNode {
    Process * process;
    struct ProcessQueueNode * next;
    struct ProcessQueueNode * previous;
} ProcessQueueNode;
ProcessQueueNode * createProcessQueueNode(Process * process);

/* Process Queue type and it's functions */
typedef enum ProcessQueueState {
    IS_NOT_EMPTY, IS_EMPTY, PROCESS_QUEUE_IS_NULL, PROCESS_QUEUE_MEMORY_ALLOCATION_FAILED
} ProcessQueueState;
const char * processQueueStateToString(const ProcessQueueState state);

typedef struct ProcessQueue {
    int size;
    ProcessQueueNode * head;
    ProcessQueueNode * tail;
    ProcessQueueState state;
} ProcessQueue;

ProcessQueue* createProcessQueue();

Process * popProcess(ProcessQueue* queue);
Process * findProcessById(const ProcessQueue* queue, const unsigned int processId);
Process * findProcessByName(const ProcessQueue* queue, const char * processName);

void clearQueue(ProcessQueue *queue);
void addProcessQueueNode(ProcessQueue* queue, ProcessQueueNode * node);
void deleteProcessQueueNode(ProcessQueue* queue, const unsigned int processId);

void printProcessQueue(const ProcessQueue* queue);

bool queueIsEmpty(const ProcessQueue* queue);
bool addProcess(ProcessQueue* queue, Process* process);
const char * processQueueToString(const ProcessQueue* queue);

Process * selectProcessTree(const Process * source);

#endif //PROCESS_H

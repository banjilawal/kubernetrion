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
    PROCESS_QUEUE_IS_NULL,
    PROCESS_QUEUE_NODE_IS_NULL,
    PROCESS_IS_NULL,
    PROCESS_QUEUE_IS_EMPTY,
    UNKNOWN_ERROR
} ErrorState;
const char * errorStateToString(const ErrorState state);

/* ProcessState enum and functions */
typedef enum ProcessState {READY, RUNNING, BLOCKED, WAITING, FINISHED} ProcessState;
const char * processStateToString(const ProcessState state);

/* Process data types and methods */
typedef struct Process {
    char* name;
    File* file;
    unsigned int id;
    unsigned int childrenCount;
    unsigned int priority;
    unsigned int CPUCycles;
    float remainingExecutionTime;
    struct Process * parent;
    struct Process * child;
    ProcessState state;
} Process;

Process* createProcess(
    Process * parent,
    const unsigned int id,
    const char* name,
    ProcessState state,
    unsigned int priority,
    float remainingExecutionTime,
    File * file
);
void killProcess(Process * process);
void removeChild(Process * parent, Process* child);
unsigned int getProcessID(const Process * process);
const char * processToString(const Process* process);

/* ProcessQueueNode type */
typedef struct ProcessQueueNode {
    Process * process;
    struct ProcessQueueNode * next;
    struct ProcessQueueNode * previous;
} ProcessQueueNode;
ProcessQueueNode * createProcessQueueNode(Process * process);

/* Process Queue type and it's functions */
typedef enum ProcessQueueState {NOT_EMPTY, EMPTY} ProcessQueueState;
const char * processQueueStateString(const ProcessQueueState state);

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

ProcessQueueNode * searchById(const ProcessQueue* queue, const unsigned int processId);
ProcessQueueNode * searchByName(const ProcessQueue* queue, const char * processName);
void printProcessQueue(const ProcessQueue* queue);

bool isEmpty(const ProcessQueue* queue);
bool addProcess(ProcessQueue* queue, Process* process);
const char * processQueueToString(const ProcessQueue* queue);


Process * selectProcessTree(const Process * source);

#endif //PROCESS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "process.h"

#define PROCESS_STRING_SIZE 256

const char * errorStateToString(const enum ErrorState state) {
    switch (state) {
        case PROCESS_QUEUE_IS_NULL: return "Process queue is NULL!";
        case PROCESS_QUEUE_NODE_IS_NULL: return "Process queue node is NULL!";
        case PROCESS_IS_NULL: return "Process is NULL!";
        case PROCESS_QUEUE_IS_EMPTY: return "Process queue is empty!";
        default: return "Unknown error state!";
    }
}

const char * processStateToString(const enum ProcessState state) {
    switch (state) {
        case READY: return "Ready";
        case RUNNING: return "Running";
        case BLOCKED: return "Blocked";
        case WAITING: return "Waiting";
        case FINISHED: return "Finished";
        default: return "Unknown state";
    }
}

/* Process functions */
Process* createProcess(
    Process * parent,
    const unsigned int id,
    const char* name,
    ProcessState state,
    unsigned int priority,
    float remainingExecutionTime,
    File * file
) {
    Process * process = (Process *) malloc(sizeof(Process));
    if (process == NULL) {
        printf("Could not allocate memory for process.\n");
        return NULL;
    }
    process->parent = parent;
    process->id = id;
    process->name = strdup(name);
    process->state = state;
    process->priority = priority;
    process->remainingExecutionTime = remainingExecutionTime;
    process->file = file;
    process->CPUCycles = 0;
    process->child = NULL;
    process->childrenCount = 0;

    return process;
}

void killProcess(Process * process) {
    if (process != NULL && process->child == NULL && process->childrenCount == 0) {
        process->file = NULL;
        process->parent = NULL;
        free(process->name);
        free(process);
    }
}

void removeChild(Process * parent, Process* child) {
    if (parent != NULL && child != NULL) {
        if (parent->child == child) {
            child->parent = NULL;
            parent->child = NULL;
            parent->childrenCount--;
        }
    }
}

const char * processToString(const Process * process) {
    if (process == NULL) {
        printf("%s.\n", errorStateToString(PROCESS_IS_NULL));
        return NULL;
    }

    const int bufferSize = PROCESS_STRING_SIZE * sizeof(char);
    char * string = (char *) malloc(bufferSize);
    if (string == NULL) {
        printf("Could not allocate memory for processToString.\n");
        return NULL;
    }

    const char *format = "Process[address:%p, ID:%d, Name:%s, State:%s, RemainingTime:%.2f, CPU Cycles:%d, File:%s]";
    snprintf(
        string, bufferSize, format, (void *)process, process->id, process->name,
        processStateToString(process->state), process->remainingExecutionTime, process->CPUCycles,
        (process->file != NULL && process->file->descriptor != NULL) ? process->file->descriptor->name : "NULL"
    );
    return string;
}

/* ProcessQueueNode functions */
ProcessQueueNode * createProcessQueueNode(Process * process) {
    if (process == NULL) {
        printf("%s.\n", errorStateToString(PROCESS_IS_NULL));
        return NULL;
    }

    ProcessQueueNode * processQueueNode = (ProcessQueueNode *) malloc(sizeof(ProcessQueueNode));
    if (processQueueNode == NULL) {
        printf("Could not allocate memory for processQueueNode.\n");
        return NULL;
    }
    processQueueNode->process = process;
    processQueueNode->next = NULL;
    processQueueNode->previous = NULL;
    return processQueueNode;
}

/* ProcessQueue functions */
ProcessQueue * createProcessQueue() {
    ProcessQueue *processQueue = (ProcessQueue *) malloc(sizeof(ProcessQueue));
    if (processQueue == NULL) {
        printf("Could not allocate memory for processQueue.\n");
        return NULL;
    }
    processQueue->head = NULL;
    processQueue->tail = NULL;
    processQueue->size = 0;
    processQueue->state = EMPTY;
    return processQueue;
}

Process * popProcess(ProcessQueue *queue) {
    if (queue == NULL) {
        printf("%s\n", errorStateToString(PROCESS_QUEUE_IS_NULL));
        return NULL;
    }
    if (isEmpty(queue)) {
        printf("%s\n", errorStateToString(PROCESS_QUEUE_IS_EMPTY));
        return NULL;
    }
    ProcessQueueNode * node = queue->head;
    node->next = NULL;
    node->previous = NULL;
    Process * process = queue->head->process;

    queue->head = queue->head->next;
    queue->size--;
    if (queue->size == 0) { clearQueue(queue); }
    free(node);
    return process;
}

bool addProcess(ProcessQueue* queue, Process* process) {
    if (queue == NULL) {
        printf("%s\n", errorStateToString(PROCESS_QUEUE_IS_NULL));
        return false;
    }
    if (process == NULL) {
        printf("%s\n", errorStateToString(PROCESS_QUEUE_NODE_IS_NULL));
        return false;
    }

    ProcessQueueNode * node = createProcessQueueNode(process);
    if (isEmpty(queue)) {
        queue->head = queue->tail = node;
    } else {
        node->previous = queue->tail;
        queue->tail->next = node;
        queue->tail = node;
    }
    queue->size++;
    queue->state = NOT_EMPTY;
    return true;
}

bool isEmpty(const ProcessQueue* queue) {
    if (queue == NULL) {
        printf("%s\n", errorStateToString(PROCESS_QUEUE_IS_NULL));
        return true;
    }
    if (queue->state == EMPTY || queue->size == 0 ||
        queue->head->previous == queue->tail ||
        queue->head == queue->tail && queue->tail == NULL
    ) {
        return true;
    } else { return false; }
}

void clearQueue(ProcessQueue *queue) {
    queue->size = 0;
    queue->state = EMPTY;
    queue->head = queue->tail = NULL;
}

Process * findProcessById(const ProcessQueue *queue, const unsigned int processId) {
    if (queue == NULL) {
        printf("%s\n", errorStateToString(PROCESS_QUEUE_IS_NULL));
        return NULL;
    }
    if (isEmpty(queue)) { return NULL; }

    ProcessQueueNode *cursor = queue->head;
    while (cursor != NULL) {
        if (cursor->process->id == processId) {
            return cursor->process;
        }
        cursor = cursor->next;
    }
    return NULL;
}

Process * findProcessByName(const ProcessQueue *queue, const char * name) {
    if (queue == NULL) {
        printf("%s\n", errorStateToString(PROCESS_QUEUE_IS_NULL));
        return NULL;
    }
    if (isEmpty(queue)) { return NULL; }

    ProcessQueueNode *cursor = queue->head;
    while (cursor != NULL) {
        if (strcmp(cursor->process->name, name) == 0) {
            return cursor->process;
        }
        cursor = cursor->next;
    }
    return NULL;
}

void deleteProcessQueueNode(ProcessQueue *queue, const unsigned int processId) {
    if (queue == NULL || queue->state == EMPTY) {
        printf("%s\n", errorStateToString(PROCESS_QUEUE_IS_EMPTY));
        return;
    }

    ProcessQueueNode *node = searchById(queue, processId);
    if (node == NULL) {
        printf("Process with ID %u not found!\n", processId);
        return;
    }

    if (node->previous != NULL) {
        node->previous->next = node->next;
    } else {
        queue->head = node->next;  // Deleting the head node
    }

    if (node->next != NULL) {
        node->next->previous = node->previous;
    } else {
        queue->tail = node->previous;  // Deleting the tail node
    }

    queue->size--;
    if (queue->size == 0) {
        queue->state = EMPTY;
    }

    const char * string = processToString(node->process);
    if (string != NULL) {
        printf("Deleted %s from the queue. Size is now %d\n", string, queue->size);
    }
    free((void *) string);
    free(node);
}

ProcessQueueNode * searchById(const ProcessQueue *queue, const unsigned int processId) {
    if (queue == NULL || queue->state == EMPTY) {
        return NULL;
    }

    ProcessQueueNode *cursor = queue->head;
    while (cursor != NULL) {
        if (cursor->process->id == processId) {
            return cursor;
        }
        cursor = cursor->next;
    }
    return NULL;
}

const char * processQueueToString(const ProcessQueue * queue) {
    if (queue == NULL) {
        printf("%s\n", errorStateToString(PROCESS_QUEUE_IS_NULL));
        return NULL;
    }

    const int bufferSize = (queue->size * PROCESS_STRING_SIZE) + 64;
    char * queueString = (char *) malloc(bufferSize);
    if (queueString == NULL) {
        printf("Could not allocate memory for processToString.\n");
        return NULL;
    }
    queueString[0] = '\0';

    const ProcessQueueNode * cursor = queue->head;
    int counter = 0;
    while (cursor != NULL) {
        const char * processString = processToString(cursor->process);
        if (processString != NULL) {
            int bytesWritten = sprintf(queueString + strlen(queueString + PROCESS_STRING_SIZE + 64), "%d %s\n", counter, processString);
            if (bytesWritten < 0 || bytesWritten >= bufferSize) {
                printf("Output exceeded queueString buffer size.\n");
                free(queueString);
                return NULL;
            }
            free((void *) processString);
        }
        cursor = cursor->next;
        counter++;
    }
    sprintf((queueString + 128), "\nqueue size:%d\n", queue->size);
    return queueString;
}

/** Prints the entire process queue */
void printProcessQueue(const ProcessQueue *queue) {
    if (queue == NULL) {
        return;
    }

    const ProcessQueueNode *cursor = queue->head;
    int counter = 0;
    while (cursor != NULL) {
        printf("%d: ", counter);
        const char * string = processToString(cursor->process);
        if (string != NULL) {
            printf("%s\n", string);
            free((void *) string);
        }
        cursor = cursor->next;
        counter++;
    }
    printf("QUEUE [address: %p, size: %d]\n", (void *)queue, queue->size);
}




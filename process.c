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
        case IS_EMPTY: return "Process queue is empty!";
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
        case PROCESS_IS_NULL: return "Process is NULL!";
        case PROCESS_MEMORY_ALLOCATION_FAILED: return "Process memory allocation failed!";
        default: return "Unknown state";
    }
}

/* Process functions */
Process* createProcess(
    const char* name,
    File* file,
    const unsigned int id,
    const unsigned int priority,
    const unsigned int remainingMilliseconds,
    Process * parent
) {
    Process * process = (Process *) malloc(sizeof(Process));
    if (process == NULL) {
        printf("%s\n", processStateToString(PROCESS_MEMORY_ALLOCATION_FAILED));
        return NULL;
    }
    process->parent = parent;
    process->id = id;
    process->name = strdup(name);
    process->priority = priority;
    process->remainingMilliseconds = remainingMilliseconds;
    process->file = file;
    process->timeQueued = 0;
    process->child = NULL;
    process->childrenCount = 0;

    return process;
}

void killProcess(Process * process) {
    if (process != NULL && process->child == NULL && process->childrenCount == 0) {
        process->file = NULL;
        process->parent = NULL;
        free((void *) process->name);
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
        printf("%s.\n", processStateToString(PROCESS_IS_NULL));
        return NULL;
    }

    const int bufferSize = PROCESS_STRING_SIZE * sizeof(char);
    char * string = (char *) malloc(bufferSize);
    if (string == NULL) {
        printf("Could not allocate memory for processToString.\n");
        return NULL;
    }

    unsigned int parentId = 0;
    if (process->parent != NULL) { parentId = process->parent->id; }



    const char *format = "Process[address:%p, ParentID:%d ID:%d, Name:%s, State:%s, Priority:%d RemainingMilliseconds:%d, CPU Cycles:%d, File:%s]";
    snprintf(
        string, bufferSize, format, (void *)process, parentId, process->id, process->name,
        processStateToString(process->state), process->priority, process->remainingMilliseconds, process->timeQueued,
        (process->file != NULL && process->file->descriptor != NULL) ? process->file->descriptor->name : "NULL"
    );
    return string;
}

/* ProcessQueueNode functions */
const char * processQueueNodeStateToString(const ProcessQueueNodeState state) {
    switch (state) {
        case PROCESS_QUEUE_NODE_IS_NULL: return "ProcessQueueNode is NULL!";
        case PROCESS_QUEUE_NODE_MEMORY_ALLOCATION_FAILED: return "ProcessQueueNode memory allocation failed!";
        default: return "Unknown ProcessQueueNode state!";
    }
}

ProcessQueueNode * createProcessQueueNode(Process * process) {
    if (process == NULL) {
        printf("%s.\n", processQueueNodeStateToString(PROCESS_QUEUE_NODE_MEMORY_ALLOCATION_FAILED));
        return NULL;
    }

    ProcessQueueNode * processQueueNode = (ProcessQueueNode *) malloc(sizeof(ProcessQueueNode));
    if (processQueueNode == NULL) {
        printf("%s\n", processQueueNodeStateToString(PROCESS_QUEUE_NODE_MEMORY_ALLOCATION_FAILED));
        return NULL;
    }
    processQueueNode->process = process;
    processQueueNode->next = NULL;
    processQueueNode->previous = NULL;
    return processQueueNode;
}

/* ProcessQueue functions */
const char * processQueueStateToString(const enum ProcessQueueState state) {
    switch (state) {
        case IS_EMPTY: return "Queue IS_EMPTY";
        case IS_NOT_EMPTY: return "Queue IS_NOT_EMPTY";
        case PROCESS_QUEUE_IS_NULL: return "Queue is NULL!";
        case PROCESS_QUEUE_MEMORY_ALLOCATION_FAILED: return "Queue memory allocation failed!";
        default: return "Unknown ProcessQueueState state!";
    }
}

ProcessQueue * createProcessQueue() {
    ProcessQueue *processQueue = (ProcessQueue *) malloc(sizeof(ProcessQueue));
    if (processQueue == NULL) {
        printf("Could not allocate memory for processQueue.\n");
        return NULL;
    }
    processQueue->head = NULL;
    processQueue->tail = NULL;
    processQueue->size = 0;
    processQueue->state = IS_EMPTY;
    return processQueue;
}

Process * popProcess(ProcessQueue *queue) {
    if (queue == NULL) {
        printf("%s\n", processQueueStateToString(PROCESS_QUEUE_IS_NULL));
        return NULL;
    }
    if (queueIsEmpty(queue)) {
        printf("%s\n", processQueueStateToString(IS_EMPTY));
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
        printf("%s\n", processQueueStateToString(PROCESS_QUEUE_IS_NULL));
        return false;
    }
    if (process == NULL) {
        printf("%s\n", processStateToString(PROCESS_IS_NULL));
        return false;
    }

    ProcessQueueNode * node = createProcessQueueNode(process);
    if (queueIsEmpty(queue)) {
        queue->head = queue->tail = node;
    } else {
        node->previous = queue->tail;
        queue->tail->next = node;
        queue->tail = node;
    }
    queue->size++;
    queue->state = IS_NOT_EMPTY;
    return true;
}

bool queueIsEmpty(const ProcessQueue* queue) {
    if (queue == NULL) {
        printf("%s\n", errorStateToString(PROCESS_QUEUE_IS_NULL));
        return true;
    }
    if (queue->state == IS_EMPTY || queue->size == 0 ||
        queue->head->previous == queue->tail ||
        queue->head == queue->tail && queue->tail == NULL
    ) {
        return true;
    } else { return false; }
}

void clearQueue(ProcessQueue *queue) {
    queue->size = 0;
    queue->state = IS_EMPTY;
    queue->head = queue->tail = NULL;
}

Process * findProcessById(const ProcessQueue *queue, const unsigned int processId) {
    if (queue == NULL) {
        printf("%s\n", processQueueStateToString(PROCESS_QUEUE_IS_NULL));
        return NULL;
    }
    if (queueIsEmpty(queue)) { return NULL; }

    const ProcessQueueNode * cursor = queue->head;
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
    if (queueIsEmpty(queue)) { return NULL; }

    ProcessQueueNode *cursor = queue->head;
    while (cursor != NULL) {
        if (strcmp(cursor->process->name, name) == 0) {
            return cursor->process;
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




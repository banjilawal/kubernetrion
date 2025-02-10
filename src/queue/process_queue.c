
#include <stdio.h>
#include <stdlib.h>
#include "process_queue.h"

#include <string.h>


/*=== FileNodeDiscoveryState Enum and Functions ===*/
typedef enum FileNodeDiscoveryState {UNDISCOVERED, DISCOVERED, PROCESSED} FileNodeDiscoveryState;

/*=== The FileSearchMetrics Data Type and Functions ===*/
typedef struct FileSearchMetrics {
    FileNodeDiscoveryState state;
    unsigned int startTime;
    unsigned int finishTime;
} FileSearchMetrics;

// FileSearchMetric: Creation Functions
FileSearchMetrics * initialize_file_search_metrics ();

// FileSearchMetric: Destruction Functions:
// NONE
// FileSearchMetric: Mutator Functions:
// NONE
// FileSearchMetric: Accessor Functions:
// NONE
// FileSearchMetric: Boolean Functions:
// NONE
// FileSearchMetric: ToString Function:
// NONE


/*=== ProcessQueueState Enum and Functions ===*/
// ProcessQueueState: Destruction Functions:
// NONE
// ProcessQueueState: Mutator Functions:
// NONE
// ProcessQueueState: Accessor Functions:
// NONE
// ProcessQueueState Boolean Functions:
// NONE
// ProcessQueue: ToString Function:
// NONE

// ProcessQueueState: toString
const char * process_queue_state_to_string(const enum ProcessQueueState process_queue_state) {
    switch (process_queue_state) {
        case PROCESS_QUEUE_IS_EMPTY: return "ProcessQueue is empty";
        case PROCESS_QUEUE_IS_NOT_EMPTY: return "ProcessQueue is not empty";
        case PROCESS_QUEUE_IS_NULL: return "ProcessQueue is NULL!";
        case PROCESS_QUEUE_MEMORY_ALLOCATION_FAILED: return "ProcessQueue memory allocation failed!";
        default: return "Unknown ProcessQueueState process_queue_state!";
    }
}

/*=== The File Data Type and It's Functions ===*/
typedef struct File {
    FileDescriptor * descriptor;
    FileSearchMetrics * search_metrics;
    struct File * next;
    struct File * previous;
} File;



// File: Destruction functions:
void destroy_file (File * file);

// File: Mutator Functions
void set_file_name (File * file, const char * name);

// File: Accessor Functions
unsigned int get_file_id (const File * file);
const char * get_file_name (const File * file);

// File: Boolean Functions
bool is_empty_file (const File * file);
bool files_are_equal (const File * a, const File * b);



// ProcessQueue: Creation functions:
ProcessQueue * createProcessQueue() {
    ProcessQueue *processQueue = (ProcessQueue *) malloc(sizeof(ProcessQueue));
    if (processQueue == NULL) {
        printf("Could not allocate memory for processQueue.\n");
        return NULL;
    }
    processQueue->head = NULL;
    processQueue->tail = NULL;
    processQueue->size = 0;
    processQueue->state = PROCESS_QUEUE_IS_EMPTY;
    return processQueue;
}

Process * pop_process_queue(ProcessQueue *process_queue) {
    if (process_queue == NULL) {
        printf("%s\n", process_queue_state_to_string(PROCESS_QUEUE_IS_NULL));
        return NULL;
    }
    if (process_queue_is_empty(process_queue)) {
        printf("%s\n", process_queue_state_to_string(PROCESS_QUEUE_IS_EMPTY));
        return NULL;
    }
    ProcessNode * node = process_queue->head;
    node->next = NULL;
    node->previous = NULL;
    Process * process = process_queue->head->process;

    process_queue->head = process_queue->head->next;
    process_queue->size--;
    if (process_queue->size == 0) { clear_process_queue(process_queue); }
    free(node);
    return process;
}

bool enqueue_process(ProcessQueue* process_queue, Process* process) {
    if (process_queue == NULL) {
        printf("%s\n", process_queue_state_to_string(PROCESS_QUEUE_IS_NULL));
        return false;
    }
    if (process == NULL) {
        printf("%s\n", process_state_to_string(PROCESS_IS_NULL));
        return false;
    }

    ProcessNode * node = create_process_node(process);
    if (process_queue_is_empty(process_queue)) {
        process_queue->head = process_queue->tail = node;
    } else {
        node->previous = process_queue->tail;
        process_queue->tail->next = node;
        process_queue->tail = node;
    }
    process_queue->size++;
    process_queue->state = PROCESS_QUEUE_IS_NOT_EMPTY;
    return true;
}

bool process_queue_is_empty(const ProcessQueue* queue) {
    if (queue == NULL) {
        printf("%s\n", process_queue_state_to_string(PROCESS_QUEUE_IS_NULL));
        return true;
    }
    if (queue->state == PROCESS_QUEUE_IS_EMPTY || queue->size == 0 ||
        queue->head->previous == queue->tail ||
        queue->head == queue->tail && queue->tail == NULL
    ) {
        return true;
    } else { return false; }
}

void clear_process_queue(ProcessQueue *queue) {
    queue->size = 0;
    queue->state = PROCESS_QUEUE_IS_EMPTY;
    queue->head = queue->tail = NULL;
}

Process * find_process_by_id(const ProcessQueue *process_queue, const unsigned int process_id) {
    if (process_queue == NULL) {
        printf("%s\n", process_queue_state_to_string(PROCESS_QUEUE_IS_NULL));
        return NULL;
    }
    if (process_queue_is_empty(process_queue)) { return NULL; }

    const ProcessNode * cursor = process_queue->head;
    while (cursor != NULL) {
        if (cursor->process->id == process_id) {
            return cursor->process;
        }
        cursor = cursor->next;
    }
    return NULL;
}

Process * find_process_by_name(const ProcessQueue *process_queue, const char * name) {
    if (process_queue == NULL) {
        printf("%s\n", process_queue_state_to_string(PROCESS_QUEUE_IS_NULL));
        return NULL;
    }
    if (process_queue_is_empty(process_queue)) { return NULL; }

    ProcessNode *cursor = process_queue->head;
    while (cursor != NULL) {
        if (strcmp(cursor->process->name, name) == 0) {
            return cursor->process;
        }
        cursor = cursor->next;
    }
    return NULL;
}

const char * process_queue_to_string(const ProcessQueue * queue) {
    if (queue == NULL) {
        printf("%s\n", process_queue_state_to_string(PROCESS_QUEUE_IS_NULL));
        return NULL;
    }

    const int bufferSize = (queue->size * PROCESS_STRING_SIZE) + 64;
    char * queueString = (char *) malloc(bufferSize);
    if (queueString == NULL) {
        printf("Could not allocate memory for process_to_string.\n");
        return NULL;
    }
    queueString[0] = '\0';

    const ProcessNode * cursor = queue->head;
    int counter = 0;
    while (cursor != NULL) {
        const char * processString = process_to_string(cursor->process);
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

/** Prints the entire process process_queue */
void print_process_queue(const ProcessQueue *process_queue) {
    if (process_queue == NULL) {
        return;
    }

    const ProcessNode *cursor = process_queue->head;
    int counter = 0;
    while (cursor != NULL) {
        printf("%d: ", counter);
        const char * string = process_to_string(cursor->process);
        if (string != NULL) {
            printf("%s\n", string);
            free((void *) string);
        }
        cursor = cursor->next;
        counter++;
    }
    printf("QUEUE [address: %p, size: %d]\n", (void *)process_queue, process_queue->size);
}

/*=== The RoundRobinProcessQueue Data Type and Functions ===*/

// RoundRobinProcessQueue: Creation Functions
RoundRobinProcessQueue * create_round_robin_process_queue () {
    RoundRobinProcessQueue * roundRobin = (RoundRobinProcessQueue *) malloc(sizeof(RoundRobinProcessQueue *));
    if (roundRobin == NULL) {
        printf("%s\n", process_queue_state_to_string(PROCESS_QUEUE_IS_NULL));
        return NULL;
    }
    roundRobin->queue = createProcessQueue();
    return roundRobin;
}

// RoundRobinProcessQueue: Destruction Functions:
// NONE

// RoundRobinProcessQueue: Mutator Functions:
bool join_round_robin_process_queue (const RoundRobinProcessQueue * round_robin_queue, Process * process) {
  if (round_robin_queue == NULL || process_queue_is_empty(round_robin_queue->queue)) {
    printf("%s\n", process_queue_state_to_string(PROCESS_QUEUE_IS_NULL));
    return false;
  }
  if (process == NULL) {
    printf("%s\n", process_state_to_string(PROCESS_IS_NULL));
    return false;
  }
  enqueue_process(round_robin_queue->queue, process);
  return true;
}

Process * popRoundRobin (const RoundRobinProcessQueue * roundRobin) {
    if (roundRobin == NULL || roundRobin->queue == NULL) {
        printf("%s\n", process_queue_state_to_string(PROCESS_QUEUE_IS_NULL));
        return NULL;
    }
    if (roundRobin->queue->state == PROCESS_QUEUE_IS_EMPTY) {
        printf("%s\n", process_queue_state_to_string(PROCESS_QUEUE_IS_EMPTY));
        return NULL;
    }
    return pop_process_queue(roundRobin->queue);
}

// RoundRobinProcessQueue: Accessor Functions:
// NONE
// RoundRobinProcessQueue: Boolean Functions:
// NONE
// RoundRobinProcessQueue: ToString Function:
// NONE

/*=== The PriorityProcessQueue Data Type and It's Functions ===*/

// PriorityProcessQueue: Creation functions
PriorityProcessQueue * create_priority_process_queue () {
  PriorityProcessQueue * priorityQueue = (PriorityProcessQueue *) malloc(sizeof(PriorityProcessQueue));
  if (priorityQueue == NULL) {
    printf("%s\n", process_queue_state_to_string(PROCESS_QUEUE_IS_NULL));
    return NULL;
  }
  priorityQueue->queue = createProcessQueue();
  return priorityQueue;
}

// FileList: Destruction functions:
void destroy_priority_process_queue (PriorityProcessQueue * process_queue) {
    free(process_queue);
}

// PriorityProcessQueue: Mutator Functions:
bool join_priority_process_queue (const PriorityProcessQueue * priority_queue, Process * process) {
  if (priority_queue == NULL || priority_queue->queue == NULL) {
    printf("%s\n", process_queue_state_to_string(PROCESS_QUEUE_IS_NULL));
    return false;
  }
  if (process == NULL) {
    printf("%s\n", process_state_to_string(PROCESS_IS_NULL));
    return false;
  }
  if (process_queue_is_empty(priority_queue->queue) || priority_queue->queue->tail->process->priority >= process->priority) {
    enqueue_process(priority_queue->queue, process);
    return true;
  }
  ProcessNode * node = create_process_node(process);
  if (node == NULL) {
    printf("%s\n", process_node_to_string(PROCESS_NODE_IS_NULL));
    return false;
  }
  ProcessNode * cursor = priority_queue->queue->tail;
  if (cursor == NULL) {
    printf("%s\n", process_node_to_string(PROCESS_NODE_IS_NULL));
    return false;
  }
  while (cursor != NULL) {
    if (cursor->process->priority > process->priority) {
      node->next = cursor;
      node->previous = cursor->previous;
      cursor->previous = node;

      priority_queue->queue->size++;
      return true;
    }
    cursor = cursor->next;
  }
  return false;
}

Process * exit_priority_process_queue (const PriorityProcessQueue * priority_queue) {
  if (priority_queue == NULL || priority_queue->queue == NULL) {
    printf("%s\n", process_queue_state_to_string(PROCESS_QUEUE_IS_NULL));
    return NULL;
  }
  if (priority_queue->queue->state == PROCESS_QUEUE_IS_EMPTY) {
    printf("%s\n", process_queue_state_to_string(PROCESS_QUEUE_IS_EMPTY));
    return NULL;
  }
  return pop_process_queue(priority_queue->queue);
}

// PriorityProcessQueue: Accessor Functions:
// NONE

// PriorityProcessQueue: Boolean Functions:
// NONE

// PriorityProcessQueue: ToString Functions:
// NONE


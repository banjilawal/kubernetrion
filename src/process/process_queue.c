
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process_queue.h"



/*
 * Function: process_queue_state_to_string
 * ----------------------------------------
 * @description:
 * String describing process queue's state
 * @param: process_queue_state: ProcessQueueState
 *
 * @return: char*
 */
char* process_queue_state_to_string(const ProcessQueueState process_queue_state) {
    switch (process_queue_state) {
        case PROCESS_QUEUE_IS_EMPTY: return PROCESS_QUEUE_IS_EMPTY_MESSAGE;
        case PROCESS_QUEUE_IS_NOT_EMPTY: return PROCESS_QUEUE_IS_NOT_EMPTY_MESSAGE;
        case PROCESS_QUEUE_IS_NULL: return PROCESS_QUEUE_IS_NULL_MESSAGE;
        case PROCESS_QUEUE_MEMORY_ALLOCATION_FAILED: return PROCESS_IS_NULL_MESSAGE;
        case PROCESS_QUEUE_IS_FULL: return PROCESS_QUEUE_IS_FULL_MESSAGE;
        case PROCESS_QUEUE_PUSH_OPERATION_FAILED: return PROCESS_QUEUE_PUSH_OPERATION_FAILED_MESSAGE;
        case PROCESS_QUEUE_POP_OPERATION_FAILED: return PROCESS_QUEUE_POP_OPERATION_FAILED_MESSAGE;
        default:
            fprintf(stderr, "%s\n", UNDEFINED_PROCESS_QUEUE_STATE_MESSAGE);
            return UNDEFINED_PROCESS_QUEUE_STATE_MESSAGE;
    }
}

/*
 *  Function: create_process_queue
 *  -------------------------------
 *  @definition:
 *  Creates an empty process queue
 *  -------------------------------
 *  @return ProcessQueue*
 */
ProcessQueue * create_process_queue() {
    ProcessQueue * process_queue = (ProcessQueue *) calloc(1, sizeof(ProcessQueue));
    if (process_queue == NULL) {
        fprintf(stderr, "%s\n", PROCESS_QUEUE_MEMORY_ALLOCATION_FAILED_MESSAGE);
        free(process_queue);
        return NULL;
    }

    /* Initialize ProcessQueue fields */
    process_queue->head = NULL;
    process_queue->tail = NULL;
    process_queue->size = 0;
    process_queue->state = PROCESS_QUEUE_IS_EMPTY;

    /* Return ProcessQueue instance */
    return process_queue;
}

// ProcessQueue: Destruction functions:
void destroy_process_queue(ProcessQueue * process_queue) {
    if (process_queue == NULL) return;

    ProcessNode* cursor = process_queue->head;
    while (cursor != NULL) {

    }
  free(process_queue);
}

// ProcessQueue: Mutator functions:
void clear_process_queue (ProcessQueue *queue) {
    queue->size = 0;
    queue->state = PROCESS_QUEUE_IS_EMPTY;
    queue->head = queue->tail = NULL;
}

bool enter_process_queue (ProcessQueue* process_queue, Process* process) {
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

Process * exit_process_queue(ProcessQueue *process_queue) {
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

// ProcessQueue: Accessor functions:
Process* find_process_by_id(const ProcessQueue *process_queue, const unsigned int process_id) {
    if (process_queue == NULL) {
        printf("%s\n", process_queue_state_to_string(PROCESS_QUEUE_IS_NULL));
        return NULL;
    }
    if (process_queue_is_empty(process_queue)) { return NULL; }

    const ProcessNode *cursor = process_queue->head;
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

// ProcessQueue: Boolean functions:
bool process_queue_is_empty(const ProcessQueue* queue) {
    if (queue == NULL) {
        printf("%s\n", process_queue_state_to_string(PROCESS_QUEUE_IS_NULL));
        return true;
    }
    if (queue->state == PROCESS_QUEUE_IS_EMPTY || queue->size == 0 ||
        queue->head->previous == queue->tail || queue->head == queue->tail && queue->tail == NULL
    ) { return true; }
    return false;
}

// ProcessQueue: ToString Functions
char * process_queue_to_string(const ProcessQueue * process_queue) {
    if (process_queue == NULL) {
        printf("%s\n", process_queue_state_to_string(PROCESS_QUEUE_IS_NULL));
        return NULL;
    }

    const int buffer_size = (process_queue->size * PROCESS_STRING_SIZE) + 64;
    char * queue_string = (char *) malloc(buffer_size);
    if (queue_string == NULL) {
        printf("Could not allocate memory for process_to_string.\n");
        return NULL;
    }
    queue_string[0] = '\0';

    const ProcessNode * cursor = process_queue->head;
    int counter = 0;
    while (cursor != NULL) {
        const char * process_string = process_to_string(cursor->process);
        if (process_string != NULL) {
            const int bytes_written = sprintf(queue_string
                + strlen(queue_string + PROCESS_STRING_SIZE + 64),
                "%d %s\n", counter, process_string
            );
            if (bytes_written < 0 || bytes_written >= buffer_size) {
                printf("Output exceeded queueString buffer size.\n");
                free(queue_string);
                return NULL;
            }
            free((void *) process_string);
        }
        cursor = cursor->next;
        counter++;
    }
    sprintf((queue_string + 128), "\nqueue size:%d\n", process_queue->size);
    return queue_string;
}

/** Prints the entire process process_queue */


/*=== The RoundRobinProcessQueue Data Type and Functions ===*/

// RoundRobinProcessQueue: Creation Functions
RoundRobinProcessQueue * create_round_robin_process_queue () {
    RoundRobinProcessQueue * roundRobin = malloc(sizeof(RoundRobinProcessQueue *));
    if (roundRobin == NULL) {
        printf("%s\n", process_queue_state_to_string(PROCESS_QUEUE_IS_NULL));
        return NULL;
    }
    roundRobin->queue = create_process_queue();
    return roundRobin;
}

// RoundRobinProcessQueue: Destruction Functions:
void destroy_round_robin_process_queue(RoundRobinProcessQueue * round_robin_queue) {
    if (round_robin_queue == NULL) return;
    destroy_process_queue(round_robin_queue->queue);
    free(round_robin_queue);
}

// RoundRobinProcessQueue: Mutator Functions:
bool enter_round_robin_process_queue (const RoundRobinProcessQueue * round_robin_queue, Process * process) {
  if (round_robin_queue == NULL || process_queue_is_empty(round_robin_queue->queue)) {
    printf("%s\n", process_queue_state_to_string(PROCESS_QUEUE_IS_NULL));
    return false;
  }
  if (process == NULL) {
    printf("%s\n", process_state_to_string(PROCESS_IS_NULL));
    return false;
  }
  enter_process_queue(round_robin_queue->queue, process);
  return true;
}

Process * exit_round_robin_queue (const RoundRobinProcessQueue * round_robin_queue) {
    if (round_robin_queue == NULL || round_robin_queue->queue == NULL) {
        printf("%s\n", process_queue_state_to_string(PROCESS_QUEUE_IS_NULL));
        return NULL;
    }
    if (round_robin_queue->queue->state == PROCESS_QUEUE_IS_EMPTY) {
        printf("%s\n", process_queue_state_to_string(PROCESS_QUEUE_IS_EMPTY));
        return NULL;
    }
    return exit_process_queue(round_robin_queue->queue);
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
  priorityQueue->queue = create_process_queue();
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
    enter_process_queue(priority_queue->queue, process);
    return true;
  }

  ProcessNode * node = create_process_node(process);
  if (node == NULL) {
    printf("%s\n", process_node_state_to_string(PROCESS_NODE_IS_NULL));
    return false;
  }
  ProcessNode * cursor = priority_queue->queue->tail;
  if (cursor == NULL) {
    printf("%s\n", process_node_state_to_string(PROCESS_NODE_IS_NULL));
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
  return exit_process_queue(priority_queue->queue);
}


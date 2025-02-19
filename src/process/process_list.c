
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "process.h"
#include "process_list.h"

/*
 * Function: process_list_state_to_string
 * ----------------------------------------
 * @definition
 * Returns a message corresponding to the linked list's state
 * -----------------------------------------------------------
 * @param process_list_state: ProcessListState
 *
 * @return bool
 */
char * process_list_state_to_string(ProcessListState process_list_state) {
    switch (process_list_state) {
        case PROCESS_LIST_IS_EMPTY: return PROCESS_LIST_IS_EMPTY_MESSAGE;
        case PROCESS_LIST_IS_NOT_EMPTY: return PROCESS_LIST_IS_NOT_EMPTY_MESSAGE;
        case PROCESS_LIST_IS_NULL: return PROCESS_LIST_IS_NULL_MESSAGE;
        case PROCESS_LIST_IS_MEMORY_ALLOCATION_FAILED: return PROCESS_LIST_MEMORY_ALLOCATION_FAILED_MESSAGE;
        case PROCESS_LIST_TAIL_INSERTION_FAILED: return PROCESS_LIST_TAIL_INSERTION_FAILED_MESSAGE;
        case PROCESS_LIST_HEAD_INSERTION_FAILED: return PROCESS_LIST_HEAD_INSERTION_FAILED_MESSAGE;
        case PROCESS_LIST_ITEM_REMOVAL_FAILED: return PROCESS_LIST_ITEM_REMOVAL_FAILED_MESSAGE;
        default:
            fprintf(stderr, "%s\n", UNEXPECTED_PROCESS_LIST_STATE_MESSAGE);
        return UNEXPECTED_PROCESS_LIST_STATE_MESSAGE;
    }
}

/*
 * Function: create_process_list
 * -------------------------------------
 * @definition
 * Creates an empty linked list of ProcessNodes
 * ----------------------------------------------
 * @return ProcessLinkedList
 */
ProcessLinkedList* create_process_list() {
    ProcessLinkedList *process_list = (ProcessLinkedList*) calloc(1, sizeof(ProcessLinkedList));

    if (process_list == NULL) {
        fprintf(stderr,"ProcessLinkedList memory allocation failed\n");
        free(process_list);
        return NULL;
    }

    /* Initialize the ProcessLinkedList */
    process_list->head = NULL;
    process_list->tail = NULL;
    process_list->size = 0;

    /* Return the process_linked_list */
    return process_list;
}

/*
 * Function: insert_process_at_list_head
 * ----------------------------------------------
 * @definition
 * Adds unique Process to ProcessLinkedList at index then increments the size
 * --------------------------------------------
 * @param process_list: ProcessLinkedList
 * @pram process: Process
 *
 * @return unsigned int
 */
unsigned int insert_process_at_list_head(ProcessLinkedList *process_list, Process *process) {
    if (process_list == NULL) {
        fprintf(stderr, "Cannot add process it null ProcessList\n");
        return  10;
    }

    if (process == NULL) {
        fprintf(stderr, "Cannot add null process to ProcessList\n");
        return 11;
    }

    if (search_process_list(process_list, process->id) != NULL) {
        fprintf(stderr, "Process already exists in the process list\n");
        return 13;
    }

    ProcessNode *process_node = create_process_node(process);
    if (process_node == NULL) {
        fprintf(stderr, "Failed to create ProcessNode. ProcessLinkedList add operation failed\n");
        free(process_node);
        return 14;
    }

    if (is_empty_process_list(process_list)) {
        process_list->head = process_node;
        process_list->tail = process_node;
        process_node->next = NULL;
        process_node->previous = NULL;
    } else {
        process_list->head->next->previous = process_node;
        process_node->next = process_list->head->next;
    }

    process_node->previous = process_list->head;
    process_list->head = process_node;
    process_list->size++;
    return 0;
}

/*
 * Function:insert_process_at_list_tail
 * ----------------------------------------------
 * @definition
 * Adds unique Process to ProcessLinkedList at the tail then increments the size
 * -----------------------------------------------------------------------------
 * @param process_list: ProcessLinkedList
 * @pram process: Process
 *
 * @return unsigned int
 */
unsigned int insert_process_at_list_tail(ProcessLinkedList *process_list, Process *process) {
    // Handle null ProcessLinkedList
    if (process_list == NULL) {
        fprintf(stderr, "Cannot add process to a null ProcessLinkedList\n");
        return 10; // Error code for null ProcessLinkedList
    }

    // Handle null Process
    if (process == NULL) {
        fprintf(stderr, "Cannot add a null process to the ProcessLinkedList\n");
        return 11; // Error code for null Process
    }

    // Check if the process already exists in the list
    if (search_process_list(process_list, process->id) != NULL) {
        fprintf(stderr, "Process already exists in the ProcessLinkedList\n");
        return 13; // Error code for duplicate Process
    }

    // Allocate memory for a new ProcessNode
    ProcessNode *process_node = create_process_node(process);
    if (process_node == NULL) {
        fprintf(stderr, "Failed to create ProcessNode. ProcessLinkedList add operation failed\n");
        return 14; // Error code for ProcessNode creation failure
    }

    // Handle the case where the list is empty
    if (is_empty_process_list(process_list)) {
        process_node->next = NULL;
        process_node->previous = NULL;
        process_list->head = process_node;
        process_list->tail = process_node;
    } else {
        // Add the new ProcessNode at the tail of the list
        process_node->next = NULL; // Since this is the tail, next is NULL
        process_node->previous = process_list->tail; // Point to the current tail
        if (process_list->tail != NULL) {
            process_list->tail->next = process_node; // Link current tail's next to the new node
        }
        process_list->tail = process_node; // Update the tail to the new node
    }

    // Increment the size of the list
    process_list->size++;

    return 0; // Success
}

/*
 * Function: destroy_process_list
 * -------------------------------
 * @definition
 * Destroy ProcessLinkedList and cleanup afterwards.
 * --------------------------------------------------
 *
 * @return ProcessLinkedList
 */
void destroy_process_list(ProcessLinkedList *process_list) {
    if (process_list == NULL) return;

    ProcessNode *cursor = process_list->head;
    while (cursor != NULL) {
        ProcessNode *next_node = cursor->next;
        destroy_process_node(cursor);
        cursor = next_node;
    }
    free(process_list);
}

/*
 * Function: remove_process_from_list
 * -------------------------------
 * @definition
 * Remove a process from the list cleanly then decrease the list's size
 * ----------------------------------------------------------------------
 * @param process_list: ProcessLinkedList
 * @param process_id: const unsigned int
 * @return ProcessLinkedList
 */
Process* remove_process_from_list(ProcessLinkedList *process_list, const unsigned int process_id) {

    /* Handle null process_list parameter*/
    if (process_list == NULL) {
        fprintf(stderr, "Cannot search process list null\n");
        return NULL;
    }

    /* Handle removal from empty ProcessList */
    if (is_empty_process_list(process_list)) {
        fprintf(stderr, "Process list is empty.\n");
        return NULL;
    }

    ProcessNode *cursor = process_list->head;
    while (cursor != NULL) {

        /*  Steps to remove the node and return the process if found */
        if (cursor->process->id == process_id) {

            /* Need to check if cursor->previous == NULL or cursor->next == NULL */
            if (cursor->previous != NULL) {
                cursor->previous->next = cursor->next;
            } else {
                process_list->head = cursor->next;
            }
             if (cursor->next != NULL) {
                 cursor->next->previous = cursor->previous;
             } else {
                 process_list->tail = cursor->previous;
             }

            Process *process = cursor->process;
            free(cursor);
            process_list->size--;
            return process;
        }
        cursor = cursor->next;
    }
    return NULL;
}

/*
 * Function: dequeue_from_process_list
 * -------------------------------
 * @definition
 * Remove the process at the head of the list
 * ----------------------------------------------------------------------
 * @param process_list: ProcessLinkedList

 * @return Process
 */
Process* dequeue_from_process_list(ProcessLinkedList *process_list) {

    /* Handle null process_list parameter*/
    if (process_list == NULL) {
        fprintf(stderr, "Cannot search process list null\n");
        return NULL;
    }
 //   printf("removing process %d of %d process lisr\n", 0, process_list->size);

    /* Handle removal from empty ProcessList */
//     if (is_empty_process_list(process_list)) {
//         fprintf(stderr, "Process list is empty.\n");
//         return NULL;
//     }
//
//   printf("is at the list's head %s", process_to_string(process_list->head->process));
     ProcessNode *node = process_list->head;
     process_list->head = process_list->head->next;
     process_list->head->next->previous = NULL;

     Process *process = node->process;
     // printf("%s process is at the head\n", process_to_string(process));
     node->next = NULL;
     node->previous = NULL;
     process_list->size--;

  //   destroy_process_node(node);

    return process;
}

/*
 * Function: enqueue_from_process_list
 * -------------------------------
 * @definition
 * Remove the process at the head of the list
 * ----------------------------------------------------------------------
 * @param process_list: ProcessLinkedList

 * @return Process
 */
unsigned int enqueue_into_process_list(ProcessLinkedList *process_list, Process *process) {

    /* Handle null process parameter */
    if (process == NULL) {
        fprintf(stderr, "Cannot add null process to the process list\n");
        return 30;
    }

    if (search_process_list(process_list, process->id) != NULL) {
        fprintf(stderr, "Process already exists in the process list\n");
        return 35;
    }

    /* Handle null process_list parameter*/
    if (process_list == NULL) {
        fprintf(stderr, "Cannot search process list null\n");
        return 31;
    }

    ProcessNode *node = create_process_node(process);
    if (node == NULL) {
        fprintf(stderr, "Failed to create ProcessNode\n");
        return 32;
    }

    if (is_empty_process_list(process_list)) {
        process_list->head = node;
        process_list->tail = node;
        node->next = NULL;;
        node->previous = NULL;;
    } else {
        process_list->tail->next = node;
        node->previous = process_list->tail;
        node->next = NULL;
    }
    process_list->size++;

    return 0;
}

bool is_empty_process_list(const ProcessLinkedList *process_list) {
    if (process_list == NULL) {
        fprintf(stderr, "Null ProcessLinkedList cannot be empty\n");
        return false;
    }
    if (process_list->size == 0) return true;
    return process_list->head == NULL || process_list->tail == NULL;
}

/*
 * Function: search_process_list
 * ------------------------------
 * @definition
 * Search the list for a process with the targeted id. The search does not
 * change the list.
 * ----------------------------------------------------
 * @param process_list: const ProcessLinkedList
 * @param process_id: const unsigned int
 *
 * @return Process*
 */
Process* search_process_list(const ProcessLinkedList *process_list, const unsigned int process_id) {
    if (process_list == NULL) {
        fprintf(stderr, "%s. Cannot search for a process in a null processList", PROCESS_LIST_IS_NULL_MESSAGE);
        return NULL;
    }

    ProcessNode *cursor = process_list->head;
    while (cursor != NULL) {
        if (cursor->process->id == process_id) { return cursor->process; }
        cursor = cursor->next;
    }
    return NULL;
}

/*
 * Function: print_process_list
 * ------------------------------
 * @definition
 * Prints the items in a ProcessLinkedList
 * -----------------------------------------
 * @param process_list: ProcessList*
 *
 * @return void
 */
unsigned int print_process_list(const ProcessLinkedList *process_list) {
    if (process_list == NULL) {
        fprintf(stderr, "Null ProcessLinkedList cannot be printed\n");
        return 50;
    }

    unsigned int count = 0;
    ProcessNode *cursor = process_list->head;
    while (cursor != NULL) {
        printf("index:%d %s\n", count, process_to_string(cursor->process));
        cursor = cursor->next;
        count++;
    }
    printf("list address:%p list size:%d\n", process_list->head, process_list->size);
    return 0;
}

/*
 * Function: destroy_process_node_list
 * -------------------------------------
 * @description
 * ProcessNodes naturally form a linked list. This function destroys all nodes
 * in the linked list recursively.
 * ----------------------------------------------------------------------------
 * @param process_node: ProcessNode*
 *
 * @return void
 */
void destroy_process_node_list(ProcessLinkedList *process_list) {
    if (process_list == NULL) return;

    ProcessNode *cursor = process_list->head;

    /* Destroy ProcessNodes ahead of process_node */
    while (cursor != NULL) {
        ProcessNode *next_node = cursor->next;
        destroy_process_node(cursor);
        cursor = next_node;
    }
    free(process_list);
}

/*
 * Function: process_queue_state_to_string
 * ----------------------------------------
 * @description:
 * String describing process processes's state
 * @param: process_queue_state: ProcessQueueState
 *
 * @return: char*
 */
char* process_queue_state_to_string(const ProcessQueueState process_queue_state) {
    switch (process_queue_state) {
        case PROCESS_QUEUE_IS_NULL: return PROCESS_QUEUE_IS_NULL_MESSAGE;
        case PROCESS_QUEUE_MEMORY_ALLOCATION_FAILED: return PROCESS_IS_NULL_MESSAGE;
        case PROCESS_QUEUE_PUSH_OPERATION_FAILED: return PROCESS_QUEUE_PUSH_OPERATION_FAILED_MESSAGE;
        case PROCESS_QUEUE_POP_OPERATION_FAILED: return PROCESS_QUEUE_POP_OPERATION_FAILED_MESSAGE;
        default:
            fprintf(stderr,"%s\n", UNDEFINED_PROCESS_QUEUE_STATE_MESSAGE);
            return UNDEFINED_PROCESS_QUEUE_STATE_MESSAGE;
    }
}

/*
 *  Function: create_process_queue
 *  -------------------------------
 *  @definition:
 *  Creates an empty process processes
 *  -------------------------------
 *
 *  @return ProcessQueue*
 */
ProcessQueue * create_process_queue(char *name) {

    if (name == NULL) { name = ""; }

    /*
     * Create the ProcessLinkedList which is a member of ProcessQueue. If the
     * creation fails return NULL to the called
     */
    ProcessLinkedList *process_linked_list = create_process_list();
    if (process_linked_list == NULL) {
        fprintf(stderr, "%s. Failed to create ProcessLinkedList\n", PROCESS_LIST_MEMORY_ALLOCATION_FAILED_MESSAGE);
        return NULL;
    }

    /*
     * Assign memory to ProcessQueue then a  pointer to the base address. If this
     * fails free process_linked_list
     *
     */
    ProcessQueue * process_queue = (ProcessQueue *) calloc(1, sizeof(ProcessQueue));
    if (process_queue == NULL) {
        fprintf(stderr, "%s\n", PROCESS_QUEUE_MEMORY_ALLOCATION_FAILED_MESSAGE);
        free(process_linked_list);
        return NULL;
    }

    /* Initialize ProcessQueue fields */
    process_queue->name = name;
    process_queue->processes = process_linked_list;
    process_queue->state = PROCESS_QUEUE_IS_EMPTY;

    /* Return ProcessQueue instance */
    return process_queue;
}

/*
 * Function: destroy_process_queue
 * ---------------------------------
 * @definition
 * Destroys a ProcessQueue instance then cleans up
 * ------------------------------------------------
 * @param process_queue: ProcessQueue*
 *
 * @return void
 */
void destroy_process_queue(ProcessQueue * process_queue) {
    if (process_queue == NULL) return;

    /* Destroy the ProcessList influence and free the ProcessQueue's memory*/
    destroy_process_list(process_queue->processes);
    free(process_queue);
}

/*
 * Function push_onto_process_queue
 * ----------------------------------
 * @definition
 * Add a process to the processes's tail
 * ----------------------------------
 * @param process_queue: ProcessQueue*
 * @param process: Process*
 *
 * ---------------------------------
 * @return void
 */
unsigned int push_onto_process_queue(ProcessQueue *process_queue, Process *process) {
    if (process_queue == NULL) {
        fprintf(stderr, "%s. %s\n", PROCESS_QUEUE_IS_NULL_MESSAGE, PROCESS_QUEUE_PUSH_OPERATION_FAILED_MESSAGE);
        return PROCESS_QUEUE_PUSH_OPERATION_FAILED;
    }

    if (process == NULL) {
        fprintf(stderr, "%s %s\n", PROCESS_IS_NULL_MESSAGE, PROCESS_QUEUE_PUSH_OPERATION_FAILED_MESSAGE);
        return PROCESS_IS_NULL_ILLEGAL_ARGUMENT_ERROR;
    }

    return insert_process_at_list_tail(process_queue->processes, process);
}


/*
 * Function: pop_from_process_queue
 * -----------------------------------
 * @definition
 * Removes the process at the head of the processes
 * ----------------------------------------------
 * @param process_queue: ProcessQueue*
 *
 * @return: Process*
 */
Process* pop_from_process_queue(ProcessQueue *process_queue) {
    if (process_queue == NULL) {
        fprintf(stderr, "%s. %s\n", PROCESS_QUEUE_IS_NULL_MESSAGE, PROCESS_QUEUE_PUSH_OPERATION_FAILED_MESSAGE);
        return NULL;
    }

    return dequeue_from_process_list(process_queue->processes);
}



//
// // RoundRobinProcessQueue: Creation Functions
// RoundRobinProcessQueue * create_round_robin_process_queue () {
//     RoundRobinProcessQueue * roundRobin = malloc(sizeof(RoundRobinProcessQueue *));
//     if (roundRobin == NULL) {
//         printf("%s\n", process_queue_state_to_string(PROCESS_QUEUE_IS_NULL));
//         return NULL;
//     }
//     roundRobin->processes = create_process_queue();
//     return roundRobin;
// }
//
// // RoundRobinProcessQueue: Destruction Functions:
// void destroy_round_robin_process_queue(RoundRobinProcessQueue * round_robin_queue) {
//     if (round_robin_queue == NULL) return;
//     destroy_process_queue(round_robin_queue->processes);
//     free(round_robin_queue);
// }
//
// // RoundRobinProcessQueue: Mutator Functions:
// bool enter_round_robin_process_queue (const RoundRobinProcessQueue * round_robin_queue, Process * process) {
//   if (round_robin_queue == NULL || process_queue_is_empty(round_robin_queue->processes)) {
//     printf("%s\n", process_queue_state_to_string(PROCESS_QUEUE_IS_NULL));
//     return false;
//   }
//   if (process == NULL) {
//     printf("%s\n", process_state_to_string(PROCESS_IS_NULL));
//     return false;
//   }
//   enter_process_queue(round_robin_queue->processes, process);
//   return true;
// }
//
// Process * exit_round_robin_queue (const RoundRobinProcessQueue * round_robin_queue) {
//     if (round_robin_queue == NULL || round_robin_queue->processes == NULL) {
//         printf("%s\n", process_queue_state_to_string(PROCESS_QUEUE_IS_NULL));
//         return NULL;
//     }
//     if (round_robin_queue->processes->state == PROCESS_QUEUE_IS_EMPTY) {
//         printf("%s\n", process_queue_state_to_string(PROCESS_QUEUE_IS_EMPTY));
//         return NULL;
//     }
//     return exit_process_queue(round_robin_queue->processes);
// }
//
// // RoundRobinProcessQueue: Accessor Functions:
// // NONE
// // RoundRobinProcessQueue: Boolean Functions:
// // NONE
// // RoundRobinProcessQueue: ToString Function:
// // NONE
//
// /*=== The PriorityProcessQueue Data Type and It's Functions ===*/
//
// // PriorityProcessQueue: Creation functions
// PriorityProcessQueue * create_priority_process_queue () {
//   PriorityProcessQueue * priorityQueue = (PriorityProcessQueue *) malloc(sizeof(PriorityProcessQueue));
//   if (priorityQueue == NULL) {
//     printf("%s\n", process_queue_state_to_string(PROCESS_QUEUE_IS_NULL));
//     return NULL;
//   }
//   priorityQueue->processes = create_process_queue();
//   return priorityQueue;
// }
//
// // FileList: Destruction functions:
// void destroy_priority_process_queue (PriorityProcessQueue * process_queue) {
//     free(process_queue);
// }
//
// // PriorityProcessQueue: Mutator Functions:
// bool join_priority_process_queue (const PriorityProcessQueue * priority_queue, Process * process) {
//   if (priority_queue == NULL || priority_queue->processes == NULL) {
//     printf("%s\n", process_queue_state_to_string(PROCESS_QUEUE_IS_NULL));
//     return false;
//   }
//   if (process == NULL) {
//     printf("%s\n", process_state_to_string(PROCESS_IS_NULL));
//     return false;
//   }
//   if (process_queue_is_empty(priority_queue->processes) || priority_queue->processes->tail->process->priority >= process->priority) {
//     enter_process_queue(priority_queue->processes, process);
//     return true;
//   }
//
//   ProcessNode * node = create_process_node(process);
//   if (node == NULL) {
//     printf("%s\n", process_node_state_to_string(PROCESS_NODE_IS_NULL));
//     return false;
//   }
//   ProcessNode * cursor = priority_queue->processes->tail;
//   if (cursor == NULL) {
//     printf("%s\n", process_node_state_to_string(PROCESS_NODE_IS_NULL));
//     return false;
//   }
//   while (cursor != NULL) {
//     if (cursor->process->priority > process->priority) {
//       node->next = cursor;
//       node->previous = cursor->previous;
//       cursor->previous = node;
//
//       priority_queue->processes->size++;
//       return true;
//     }
//     cursor = cursor->next;
//   }
//   return false;
// }
//
// Process * exit_priority_process_queue (const PriorityProcessQueue * priority_queue) {
//   if (priority_queue == NULL || priority_queue->processes == NULL) {
//     printf("%s\n", process_queue_state_to_string(PROCESS_QUEUE_IS_NULL));
//     return NULL;
//   }
//   if (priority_queue->processes->state == PROCESS_QUEUE_IS_EMPTY) {
//     printf("%s\n", process_queue_state_to_string(PROCESS_QUEUE_IS_EMPTY));
//     return NULL;
//   }
//   return exit_process_queue(priority_queue->processes);
// }


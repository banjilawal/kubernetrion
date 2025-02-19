#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "process.h"


const char * process_string_format = "Process["
    "address:%p"
    " parentId:%d"
    " id:%d"
    " name:%s"
    " state:%s"
    " priority:%d"
    " milliseconds_remaining:%d"
    " cpu_cycle_count:%d"
    " file:%s]";

/*
 * Function: print_allocation_failure
 * -----------------------------------
 * @description:
 * Consistent reporting about memory allocation failures.
 * ----------------------------------------------------------------
 * @Param data_type: const char
 *
 * @return void
 */
void print_allocation_failure(const char *data_type) {
    fprintf(stderr, "Memory allocation for %s failed!\n", data_type);
}


/*
 * Function: process_state_to_string
 * ----------------------------------
 * @description:
 * String describing ProcessState value
 * -----------------------------------------------
 * @Param process_state: ProcessState
 * return: const char *
 * Gives a string describing the equivalent ProcessState enum's numeric value.
 * @param process_state _to_string(ProcessState process_state)
 *
 * @return const char *
 */
const char * process_state_to_string(const ProcessState process_state) {
    switch (process_state) {
        case PROCESS_READY: return PROCESS_READY_MESSAGE;
        case PROCESS_RUNNING: return PROCESS_RUNNING_MESSAGE;
        case PROCESS_READING_FILE_BLOCKED: return PROCESS_READING_FILE_IS_BLOCKED_MESSAGE;
        case PROCESS_WRITING_FILE_BLOCKED: return PROCESS_WRITING_FILE_IS_BLOCKED_MESSAGE;
        case PROCESS_WAITING_EVENT: return PROCESS_WAITING_EVENT_MESSAGE;
        case PROCESS_FINISHED: return PROCESS_FINISHED_MESSAGE;
        case PROCESS_IS_NULL: return PROCESS_IS_NULL_MESSAGE;
        case PROCESS_READING_FILE: return PROCESS_READING_FILE_MESSAGE;
        case PROCESS_WRITING_FILE: return PROCESS_WRITING_FILE_MESSAGE;
        case PROCESS_MEMORY_ALLOCATION_FAILED: return PROCESS_MEMORY_ALLOCATION_FAILED_MESSAGE;
        default:
            printf("%s: %d\n", UNDEFINED_PROCESS_STATE_MESSAGE, process_state);
            return UNDEFINED_PROCESS_STATE_MESSAGE;
    }
}

/*
 * Function: create_process
 * -------------------------
 * @description:
 * Creates instance of Process
 * --------------------------------------
 * @param id: const unsigned int
 * @param name: const *char
 * @param parent: Process*
 * @param child: Process*
 * @param file: File*
 * @param priority: const unsigned int
 * @param milliseconds_remaining: const unsigned int
 *
 * @return: Process*
 */
Process * create_process(
    const unsigned int id,
    const char *name,
    Process *parent,
    Process *child,
    File *file,
    const unsigned int priority,
    const unsigned int milliseconds_remaining
) {
    if (name == NULL) {
        fprintf(stderr, "%s. Process creation failed", PROCESS_NAME_IS_NULL_ERROR_MESSAGE);
        return NULL;
    }

    if (priority < MIN_PRIORITY || priority > MAX_PRIORITY) {
        fprintf(stderr, "%s\n", PROCESS_PRIORITY_OUT_OF_RANGE_ERROR_MESSAGE);
        return NULL;
    }

    Process * process = calloc(1, sizeof(Process));
    if (process == NULL) {
        fprintf(stderr, "%s\n", process_state_to_string(PROCESS_MEMORY_ALLOCATION_FAILED));
        return NULL;
    }

    /*
     * If the process' name is null after assignment report error and free
     * assigned memory
     */
    process->name =  strdup(name);
    if (process->name == NULL) {
        fprintf(stderr, "%s\n", PROCESS_NAME_IS_NULL_ERROR_MESSAGE);
        if (process->file != NULL) free(process->file);
        free(process);
        return NULL;
    }

    /*
     * Assure bidirectional relationship between process and its' parent; if
     * parent is not null.
    */
    if (parent != NULL) {
        parent->child = process;
        process->parent = parent;
    } else { process->parent = NULL; }

    /* Set remaining parameters */
    process->id = id;
    process->child = NULL;
    process->file = file;
    process->priority = priority;
    process->cpu_cycle_count = 0;
    process->number_of_child_processes = 0;
    process->milliseconds_remaining = milliseconds_remaining;
    process->state = PROCESS_READY;

    /* Return process */
    return process;
}

/*
 * Function: destroy_process
 * --------------------------
 * @description:
 * Recursively destroy a process and its' children.
 * --------------------------------------------------
 * @param process:  Process*
 *
 * @return void
 */
void destroy_process(Process **process) {
    if (process == NULL) return;

    /* Recursively destroy child processes */
    destroy_process(&(*process)->child);

    /* Release file */
    release_file(*process);

    /* Free the process*/
    free((*process)->name);
    free(process);
    *process = NULL;
}

/*
 * Function: destroy_child_process
 * ---------------------------------
 * @description:
 * Kills child processes and cleans up afterwards
 * ------------------------------------------------
 * @param parent_process: Process*
 *
 * @return void
 */
void destroy_child_process(Process * parent_process) {
    if (parent_process == NULL) return;

    Process *current_child = parent_process->child;
    Process *next_child = NULL;

    while (current_child != NULL) {
        next_child = current_child->child;
        destroy_process(&current_child);
        current_child = next_child;
    }
    parent_process->child = NULL;
}

/*
 * Function: read_file
 * ---------------------
 * @description:
 * Reads from a file. Returns 0 on success FILE_READ_FAILED otherwise
 * --------------------------------------------------------------------
 * @param process: Process*
 * @param file: File*
 *
 * @return unsigned int
 */
unsigned int read_file(Process  *process, File *file) {
    if (process == NULL) {
        fprintf(stderr, "%s. Process reading failed", NULL_PROCESS_ERROR_MESSAGE);
        return PROCESS_IS_NULL_ILLEGAL_ARGUMENT_ERROR;
    }

    if (file == NULL) {
        fprintf(stderr, "%s. File reading failed", file_state_to_string(FILE_IS_NULL));
        return FILE_IS_NULL_ERROR_CODE;
    }

    if (processes_are_equal(process, file->reader)) { return PROCESS_OPERATION_SUCCESS; }

    if (file->reader != NULL) {
        fprintf(stderr, "%s. file in use by another process", PROCESS_READING_FILE_IS_BLOCKED_MESSAGE);
        return PROCESS_READING_FILE_IS_BLOCKED_EXCEPTION;
    }

    process->file = file;
    return set_file_reader(file, process);
}

/*
 * Function: write_file
 * ---------------------
 * @description:
 * Locks file for writing n bytes to
 * ----------------------------------
 * @param process: Process*
 * @param file: File*
 * @param bytes_to_write: unsigned int
 *
 * @return unsigned int
 */
unsigned int write_file(Process *process, File *file, unsigned int bytes_to_write) {

    if (process == NULL) {
        fprintf(stderr, "%s. Process writing failed", NULL_PROCESS_ERROR_MESSAGE);
        return PROCESS_IS_NULL_ILLEGAL_ARGUMENT_ERROR;
    }

    if (processes_are_equal(process, file->writer)) { return PROCESS_OPERATION_SUCCESS; }

    if (file->reader != NULL) {
        fprintf(stderr, "%s. file in use by another process", PROCESS_READING_FILE_IS_BLOCKED_MESSAGE);
        return PROCESS_WRITING_FILE_IS_BLOCKED_EXCEPTION;
    }

    process->file = file;
    return set_file_writer(file, process, bytes_to_write);
}

/*
 * Function: release_file
 * -----------------------
 * @description:
 * Releases lock on file
 * ------------------------
 * @param process: Process
 *
 * @return File*
 */
File* release_file(Process * process) {

    /* Handle case: process parameter is null */
    if (process == NULL) {
        fprintf(stderr, "%s. Process releasing failed", NULL_PROCESS_ERROR_MESSAGE);
        return NULL;
    }

    /* Handle case: file parameter is null */
    if (process->file == NULL) {
        fprintf(stderr, "%s. No file is associated with the process", file_state_to_string(FILE_IS_NULL));
        return NULL;
    }

    File *file = process->file;

    /* Check if process is reader or writer to correctly disassociate process from file */
    if (file->reader == process) { file->reader = NULL; }
    else if (file->writer == process) { file->writer = NULL; }
    else {
        fprintf(stderr, "Process is not associated with file's reader or writer\n");
    }

    /* Nullify process' file */
    process->file = NULL;

    /* Return file */
    return file;
}

/*
 * Function: processes_are_equal
 *--------------------------------
 * @definition
 * Checks if two processes are equal
 * -----------------------------------
 * @param a: Process*
 * @param b: Process*
 *
 * @return bool
 */
bool processes_are_equal (const Process * a, const Process * b) {
    if (a == b) return true;
    if (a == NULL || b == NULL) return false;
    return a->id == b->id;
}

/*
 * Function: process_to_spring
 * -----------------------------
 * @definition
 * String representation of Process
 * ----------------------------------
 * @param process: Process*
 *
 * @return char*
 */
char * process_to_string(const Process * process) {
    if (process == NULL) {
        fprintf(stderr, "%s cannot represent null process as string\n", process_state_to_string(PROCESS_IS_NULL));
        return NULL;
    }

    /* Assign space for process' string */
    const size_t buffer_size = PROCESS_STRING_SIZE * sizeof(char);
    char * string = (char *) calloc(buffer_size, 1);

    /*
     * Get parent's process id for the string. If parent is null parent_id = 0
     */
    unsigned int parent_id = 0;
    if (process->parent != NULL) { parent_id = process->parent->id; }

    const char *file_name = process->file == NULL
        || process->file->descriptor == NULL ? "None" : process->file->descriptor->name;

    snprintf(
        string,
        buffer_size,
        process_string_format,
        (void *) process,
        parent_id,
        process->id,
        process->name,
        process_state_to_string(process->state),
        process->priority,
        process->milliseconds_remaining,
        process->cpu_cycle_count,
        file_name
    );
    return string;
}

/*
 * Function: process_node_state_to_string
 * --------------------------------------
 * @definition:
 * A string describing matching the ProcessNodeState enum value
 * -------------------------------------------------------------
 * @param process_node_state: ProcessNodeState
 *
 * @return char *
 */
char * process_node_state_to_string(const ProcessNodeState process_node_state) {
    switch (process_node_state) {
        case PROCESS_NODE_IS_NULL: return PROCESS_NODE_IS_NULL_MESSAGE;
        case PROCESS_NODE_MEMORY_ALLOCATION_FAILED: return PROCESS_NODE_MEMORY_ALLOCATION_FAILED_MESSAGE;
        default: {
            printf("%s\n", UNDEFINED_PROCESS_NODE_STATE_MESSAGE);
            return UNDEFINED_PROCESS_NODE_STATE_MESSAGE;
        }
    }
}

/*
 * Function: create_process_node
 * ------------------------------
 * @description
 * Creates a ProcessNode
 * --------------------------------
 * @precondition
 * process is not null
 * ---------------------------------
 * @postcondition
 * ProcessNode contains Process that is not changed.
 * -------------------------------------------------
 * @param process: Process*
 *
 * @return ProcessNode*
 */
ProcessNode* create_process_node(Process *process) {

    /* Handle null process function argument */
    if (process == NULL) {
        fprintf(stderr, "%s. %scannot create a node for a null process\n",
            PROCESS_NODE_CREATION_FAILED_MESSAGE, PROCESS_IS_NULL_MESSAGE);
        return NULL;
    }


    /* Allocate memory to a ProcessNode. Set pointer to the base address */
    ProcessNode *process_node = (ProcessNode*) calloc(1, sizeof(ProcessNode));
    if (process_node == NULL) {
        fprintf(stderr, "%s for pid %d\n", PROCESS_NODE_MEMORY_ALLOCATION_FAILED_MESSAGE, process->id);
        return NULL;
    }

    /* Set ProcessNode fields and return the entity */
    process_node->process = process;
    process_node->next = NULL;
    process_node->previous = NULL;

    return process_node;
}

/*
 * Function: destroy_process_node
 * -------------------------------
 * Destroys and cleans up a processNode. Links destruction target's next and previous
 * links to each other prior to destruction. The function preserves the target's
 * neighboring ProcessNodes
 * ---------------------------------------------------------------------------------
 * @param process_node: ProcessNode*
 *
 * @return void
 */
void destroy_process_node(ProcessNode *process_node) {

    /* If process_node is null nothing to do. Return */
    if (process_node == NULL) return;

    ProcessNode *cursor = process_node;
    while (cursor != NULL) {
        ProcessNode *next_node = cursor->next;
        destroy_process_node(cursor);
        free(cursor);
        cursor = next_node;
    }
}

/*
 * Function: process_nodes_are_equal
 * ---------------------------------
 * @definition
 * Checks if two ProcessNode instances are equal
 * ---------------------------------------------------------------------------------
 * @param a: const ProcessNode*
 * @param b: const ProcessNode*
 *
 * @return bool
 */
bool process_nodes_are_equal(const ProcessNode *a, const ProcessNode *b) {
    if (a == b) return true;
    if (a == NULL || b == NULL) return false;
    return processes_are_equal(a->process, b->process);
}


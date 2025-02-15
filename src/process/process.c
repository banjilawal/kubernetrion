#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "process.h"


const char * process_string_format = "Process["
    " address:%p"
    " parentId:%d"
    " id:%d"
    " name:%s"
    " state:%s"
    " priority:%d"
    " milliseconds_remaining:%d"
    " cpu_cycle_count:%d"
    " file_reading_from:%s "
    " file_writing_to:%s]";

/*
 * Function: Consistent reporting about memory allocation failures.
 * ----------------------------------------------------------------
 * @Param data_type: const char
 * return: void
 */
void print_allocation_failure(const char *data_type) {
    fprintf("Memory allocation for %s failed!\n", data_type);
}


/*
 * Function: String describing ProcessState value
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
        case PROCESS_READING_FILE_BLOCKED: return PROCESS_READING_FILE_BLOCKED_MESSAGE;
        case PROCESS_WRITING_FILE_BLOCKED: return PROCESS_WRITING_FILE_BLOCKED_MESSAGE;
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
 * Function: Creates instance of Process
 * --------------------------------------
 * @param id: const unsigned int
 * @param name: const *char
 * @param parent: Process*
 * @param child: Process*
 * @param file: File*
 * @param priority: const unsigned int
 * @param milliseconds_remaining: const unsigned int
 *
 * @return: Process
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
        free(process);
        return NULL;
    }

    /*
     * If the process' name is null after assignment report error and free
     * assigned memory
     */
    process->name =  strdup(name);
    if (process->name == NULL) {
        fprintf(stderr, "%s\n", PROCESS_NAME_IS_NULL_ERROR_MESSAGE);
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
 * Function: Recursively destroy a process and its' children.
 * @param process:  Process*
 *
 * @return: void
 */
void destroy_process(Process * process) {
    if (process == NULL) return;

    /* Recursively destroy child processes */
    destroy_process(process->child);

    /* Release file */
    release_file(process);

    /* Free the process*/
    free(process);
    process = NULL;
}

/*
 *set_reading(Process * process, File * file)
 *return: void
 */
void set_reading_file(Process * process, File * file) {
    if (process == NULL) return;
    if (files_are_equal(process->file_reading_from, file)) return;
    if (file->reader != NULL) {
        printf("file %s is in use", file->descriptor->name);
        return;
    }
    process->file_reading_from = NULL;
    if (file != NULL) {
        process->file_reading_from = file;
        file->reader = process;
    }
}

/*
 *set_reading_file(Process * process, File * file)
 *return: void
 */
void set_writing_file (Process * process, File * file) {
    if (process == NULL) return;
    if (files_are_equal(process->file_writing_to, file)) return;
    if (file->writer != NULL) {
        printf("file %s is in use", file->descriptor->name);
        return;
    }
    process->file_writing_to = NULL;
    if (file != NULL) {
        process->file_writing_to = file;
        file->writer = process;
    }
}

/*
 *unset_read_file(Process * process)
 *return: File *
 */
File * release_file(Process * process) {
    if (process == NULL) return NULL;
    if (process->file_reading_from == NULL) return NULL;
    File * file = process->file_reading_from;
    process->file_reading_from = NULL;
    file->reader = NULL;
    return file;
}

/*
 *unset_file_writing_to(Process * process)
 *return: File *
 */
File * unset_file_writing_to(Process * process) {
    if (process == NULL) return NULL;
    if (process->file_writing_to == NULL) return NULL;
    File * file = process->file_writing_to;
    process->file_writing_to = NULL;
    return file;
}

/*
 *kill_child_process(process * parent_process)
 *return void:
 */
void kill_child_process(Process * parent) {
    if (parent == NULL) return;
    if (parent->child == NULL) return;
    destroy_process(parent->child);
}

/*
 *get_process_id(const Process * process)
 *return: unsigned int
 */
unsigned int get_process_id(const Process * process) { return process->id; }

/*
 *get_process_name(const Process * process)
 *return: char *
 */
char * get_process_name(const Process * process) { return (char *) process->name; }

/*
 *processes_are_equal(const Process * a, Process * b)
 *return: bool
 */
bool processes_are_equal (const Process * a, const Process * b) {
    if (&a == &b) return true;
    if (a == NULL || b == NULL) return false;
    return a->id == b->id;
}

/*
 *process_to_string (const Process * process)
 *return: char *
 */
char * process_to_string(const Process * process) {
    if (process == NULL) {
        printf("%s.\n", process_state_to_string(PROCESS_IS_NULL));
        return NULL;
    }

    const int bufferSize = PROCESS_STRING_SIZE * sizeof(char);
    char * string = (char *) malloc(bufferSize);
    if (string == NULL) {
        printf("Could not allocate memory for process_to_string.\n");
        return NULL;
    }

    unsigned int parentId = 0;
    if (process->parent != NULL) { parentId = process->parent->id; }

    snprintf(
        string,
        bufferSize,
        process_string_format,
        (void *) process,
        parentId,
        process->id,
        process->name,
        process_state_to_string(process->state),
        process->priority,
        process->milliseconds_remaining,
        process->cpu_cycle_count,
        file_to_string(process->file_reading_from),
        file_to_string(process->file_writing_to)
    );
    return string;
}

/*
 * process_node_state_to_string (const ProcessNodeState process_node_state)
 * return: char *
 */
char * process_node_state_to_string(const ProcessNodeState state) {
    switch (state) {
        case PROCESS_NODE_IS_NULL: return PROCESS_NODE_IS_NULL_MESSAGE;
        case PROCESS_NODE_MEMORY_ALLOCATION_FAILED: return PROCESS_NODE_MEMORY_ALLOCATION_FAILED_MESSAGE;
        default: {
            printf("%s.\n", UNDEFINED_PROCESS_NODE_STATE_MESSAGE);
            return UNDEFINED_PROCESS_NODE_STATE_MESSAGE;
        }
    }
}

/*
 *create_process_node(Process * process)
 *return: ProcessNode *
 */
ProcessNode * create_process_node(Process * process) {

    if (process == NULL || process->name == NULL) {
        printf("%s cannot create a node for a null process\n", process_state_to_string(PROCESS_IS_NULL));
        return NULL;
    }

    ProcessNode * process_node = (ProcessNode *) malloc(sizeof(ProcessNode));
    if (process_node == NULL) {
        printf("%s for processId:%d\n",
            process_node_state_to_string(PROCESS_NODE_MEMORY_ALLOCATION_FAILED),
            process->id);
        return NULL;
    }
    process_node->process = process;
    process_node->next = NULL;
    process_node->previous = NULL;

    return process_node;
}

/*
 *destroy_process_node(ProcessNode * process_node)
 */
void destroy_process_node(ProcessNode * process_node) {
    if (process_node == NULL) return;

    process_node->previous->next = process_node->next;
    process_node->next->previous = process_node->previous;

    process_node->next = NULL;
    process_node->previous = NULL;

    destroy_process(process_node->process);
    free(process_node);
}
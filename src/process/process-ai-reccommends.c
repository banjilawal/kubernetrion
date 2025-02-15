#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "process.h"

const char *process_string_format = "Process["
    " address:%p"
    " parentId:%d"
    " id:%d"
    " name:%s"
    " state:%s"
    " priority:%d"
    " milliseconds_remaining:%d"
    " cpu_cycle_count:%d"
    " reading_file:%s"
    " writing_file:%s]";

void print_allocation_failure(const char *data_type) {
    fprintf(stderr, "Memory allocation for %s failed!\n", data_type);
}

/*=== ProcessState Enum and Functions ===*/

const char *process_state_to_string(const ProcessState process_state) {
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
            fprintf(stderr, "%s: %d\n", UNDEFINED_PROCESS_STATE_MESSAGE, process_state);
            return UNDEFINED_PROCESS_STATE_MESSAGE;
    }
}

/*=== The Process Data Type and Functions ===*/

Process *create_process(
    const unsigned int id,
    const char *name,
    struct Process *parent,
    struct Process *child,
    File *reading_file,
    File *writing_file,
    const unsigned int priority,
    const unsigned int milliseconds_remaining
) {
    if (name == NULL) {
        fprintf(stderr, "The process name cannot be null. Process creation failed.\n");
        return NULL;
    }

    if (priority < MIN_PRIORITY || priority > MAX_PRIORITY) {
        fprintf(stderr, "Priority must be between %d and %d.\n", MIN_PRIORITY, MAX_PRIORITY);
        return NULL;
    }

    Process *process = calloc(1, sizeof(Process));  // Use calloc for zero-initialized memory.
    if (process == NULL) {
        fprintf(stderr, "%s\n", process_state_to_string(PROCESS_MEMORY_ALLOCATION_FAILED));
        return NULL;
    }

    process->id = id;
    process->name = strdup(name);
    if (process->name == NULL) {
        print_allocation_failure("process name");
        free(process);
        return NULL;
    }

    if (parent != NULL) {
        parent->child = process;
        process->parent = parent;
    }

    process->child = NULL;
    process->reading_file = reading_file;
    process->writing_file = writing_file;
    process->priority = priority;
    process->milliseconds_remaining = milliseconds_remaining;
    process->cpu_cycle_count = 0;
    process->number_of_child_processes = 0;
    process->state = PROCESS_RUNNING;

    return process;
}

void destroy_process(Process *process) {
    if (process == NULL) return;

    destroy_process(process->child);

    unset_reading_file(process);
    unset_writing_file(process);

    free(process->name);  // Ensure proper memory cleanup.
    free(process);
}

/* Eliminate redundant checks and improve consistency */
void set_reading_file(Process *process, File *file) {
    if (process == NULL || file == NULL) return;

    if (files_are_equal(process->reading_file, file)) return;

    if (file->reader != NULL) {
        fprintf(stderr, "File %s is already in use.\n", file->descriptor->name);
        return;
    }

    unset_reading_file(process);
    process->reading_file = file;
    file->reader = process;
}

void set_writing_file(Process *process, File *file) {
    if (process == NULL || file == NULL) return;

    if (files_are_equal(process->writing_file, file)) return;

    if (file->writer != NULL) {
        fprintf(stderr, "File %s is already in use.\n", file->descriptor->name);
        return;
    }

    unset_writing_file(process);
    process->writing_file = file;
    file->writer = process;
}

File *unset_reading_file(Process *process) {
    if (process == NULL || process->reading_file == NULL) return NULL;

    File *file = process->reading_file;
    process->reading_file = NULL;
    file->reader = NULL;

    return file;
}

File *unset_writing_file(Process *process) {
    if (process == NULL || process->writing_file == NULL) return NULL;

    File *file = process->writing_file;
    process->writing_file = NULL;

    return file;
}

void kill_child_process(Process *parent) {
    if (parent == NULL || parent->child == NULL) return;

    destroy_process(parent->child);
    parent->child = NULL;
}

unsigned int get_process_id(const Process *process) {
    if (process == NULL) {
        fprintf(stderr, "Invalid process pointer.\n");
        return 0;
    }
    return process->id;
}

char *get_process_name(const Process *process) {
    if (process == NULL) return NULL;
    return process->name;
}

bool processes_are_equal(const Process *a, const Process *b) {
    if (a == NULL || b == NULL) return false;
    return a->id == b->id;
}

char *process_to_string(const Process *process) {
    if (process == NULL) {
        fprintf(stderr, "%s.\n", process_state_to_string(PROCESS_IS_NULL));
        return NULL;
    }

    const int bufferSize = PROCESS_STRING_SIZE * sizeof(char);
    char *string = (char *) malloc(bufferSize);
    if (string == NULL) {
        fprintf(stderr, "Could not allocate memory for process_to_string.\n");
        return NULL;
    }

    unsigned int parentId = process->parent ? process->parent->id : 0;

    snprintf(
        string,
        bufferSize,
        process_string_format,
        (void *)process,
        parentId,
        process->id,
        process->name,
        process_state_to_string(process->state),
        process->priority,
        process->milliseconds_remaining,
        process->cpu_cycle_count,
        file_to_string(process->reading_file),
        file_to_string(process->writing_file)
    );

    return string;
}

ProcessNode *create_process_node(Process *process) {
    if (process == NULL || process->name == NULL) {
        fprintf(stderr, "%s cannot create a node for a null process.\n",
                process_state_to_string(PROCESS_IS_NULL));
        return NULL;
    }

    ProcessNode *process_node = calloc(1, sizeof(ProcessNode));  // Use calloc for zero-initialized memory.
    if (process_node == NULL) {
        fprintf(stderr, "%s for processId:%d\n",
                process_node_state_to_string(PROCESS_NODE_MEMORY_ALLOCATION_FAILED),
                process->id);
        return NULL;
    }

    process_node->process = process;
    return process_node;
}

void destroy_process_node(ProcessNode *process_node) {
    if (process_node == NULL) return;

    if (process_node->previous) process_node->previous->next = process_node->next;
    if (process_node->next) process_node->next->previous = process_node->previous;

    destroy_process(process_node->process);
    free(process_node);
} 

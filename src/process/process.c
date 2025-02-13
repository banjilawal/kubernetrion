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
    " CPU Cycles:%d"
    " reading_file:%s "
    " writing_file:%s]";

void print_allocation_failure(const char *data_type) {
    printf("Memory allocation for %s failed!\n", data_type);
}

/*=== ProcessState Enum and Functions ===*/

// ProcessState: toString Function
const char * process_state_to_string(const ProcessState process_state) {
    switch (process_state) {
        case PROCESS_READY: return "Process is ready";
        case PROCESS_RUNNING: return "Process is running";
        case PROCESS_READING_FILE_BLOCKED: return "Reading file blocked";
        case PROCESS_WRITING_FILE_BLOCKED: return "Writing blocked";
        case PROCESS_WAITING_EVENT: return "Process is waiting event";
        case PROCESS_FINISHED: return "Process is finished";
        case PROCESS_IS_NULL: return "Process is NULL!";
        case PROCESS_READING_FILE: return "Process is reading file";
        case PROCESS_WRITING_FILE: return "Process is writing file";
        case PROCESS_MEMORY_ALLOCATION_FAILED: return "Process memory allocation failed!";
        default:
            printf("Unknown process state: %d\n", process_state);
            return "Unknown process_state";
    }
}

/*=== The Process Data Type and Functions ===*/

// Process: Creation Functions
Process * create_process(
    const unsigned int id,
    const char * name,
    struct Process * parent,
    struct Process * child,
    File * reading_file,
    File * writing_file,
    const unsigned int priority,
    const unsigned int milliseconds_remaining
) {
    Process * process = (Process *) malloc(sizeof(Process));
    if (process == NULL) {
        printf("%s\n", process_state_to_string(PROCESS_MEMORY_ALLOCATION_FAILED));
        return NULL;
    }
    if (name == NULL) {
        printf("The process name cannot be null. Process creation failed");
        return NULL;
    }

    if (parent != NULL) {
        parent->child = process;
        process->parent = parent;
    } else { process->parent = NULL; }

    if (priority < MIN_PRIORITY || priority > MAX_PRIORITY) {
        printf("Priority must be between %d and %d.\n", MIN_PRIORITY, MAX_PRIORITY);
        free(process);
        return NULL;
    }

    *((unsigned int *)&process->id) = id;  // Only allowed during initialization
    * ((char **)&process->name) =  strdup(name);

    process->child = NULL;
    process->reading_file = reading_file;
    process->writing_file = writing_file;
    process->priority = priority;
    process->milliseconds_remaining = milliseconds_remaining;

    process->number_of_queue_entries = 0;
    process->number_of_child_processes = 0;
    process->state = PROCESS_RUNNING;
    return process;
}

// Process: Destruction Functions:
void destroy_process(Process * process) {
    if (process == NULL) return;
    destroy_process(process->child);
    unset_reading_file(process);
    unset_writing_file(process);
    free((unsigned int *)process->id);
    free((char *)process->name);
    free(process);
}

// Process: Mutator Functions:
void set_reading_file(Process * process, File * file) {
    if (process == NULL) return;
    if (files_are_equal(process->reading_file, file)) return;
    if (file->reader != NULL) {
        printf("file %s is in use", file->descriptor->name);
        return;
    }
    process->reading_file = NULL;
    if (file != NULL) {
        process->reading_file = file;
        file->reader = process;
    }
}

void set_writing_file (Process * process, File * file) {
    if (process == NULL) return;
    if (files_are_equal(process->writing_file, file)) return;
    if (file->writer != NULL) {
        printf("file %s is in use", file->descriptor->name);
        return;
    }
    process->writing_file = NULL;
    if (file != NULL) {
        process->writing_file = file;
        file->writer = process;
    }
}

File * unset_reading_file(Process * process) {
    if (process == NULL) return NULL;
    if (process->reading_file == NULL) return NULL;
    File * file = process->reading_file;
    process->reading_file = NULL;
    file->reader = NULL;
    return file;
}

File * unset_writing_file(Process * process) {
    if (process == NULL) return NULL;
    if (process->writing_file == NULL) return NULL;
    File * file = process->writing_file;
    process->writing_file = NULL;
    return file;
}

void kill_child_process(Process * parent) {
    if (parent == NULL) return;
    if (parent->child == NULL) return;
    destroy_process(parent->child);
}

// Process: Accessor Functions:
unsigned int get_process_id(const Process * process) { return process->id; }

// Process: Boolean Functions:
bool processes_are_equal (const Process * a, const Process * b) {
    if (&a == &b) return true;
    if (a == NULL || b == NULL) return false;
    return a->id == b->id;
}

// Process: ToString Function:
const char * process_to_string(const Process * process) {
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
        process->number_of_queue_entries,
        file_to_string(process->reading_file),
        file_to_string(process->writing_file)
    );
    return string;
}

/*=== ProcessNodeState Enum and Functions ===*/

// ProcessNodeState: ToString Function:
const char * process_node_state_to_string(const ProcessNodeState state) {
    switch (state) {
        case PROCESS_NODE_IS_NULL: return "ProcessNode is NULL!";
        case PROCESS_NODE_MEMORY_ALLOCATION_FAILED: return "ProcessNode memory allocation failed!";
        default: return "Unknown process_state";
    }
}

/*=== The ProcessNode Data Type and It's Functions ===*/

// ProcessNode: Creation functions:
ProcessNode * create_process_node(Process * process) {
    if (process == NULL || process->name == NULL) {
        printf("%s cannot create a node for a null process\n", process_state_to_string(PROCESS_IS_NULL));
        return NULL;
    }

    ProcessNode * process_node = (ProcessNode *) malloc(sizeof(ProcessNode));
    if (process_node == NULL) {
        printf("%s for processId:%d\n", process_node_state_to_string(PROCESS_NODE_MEMORY_ALLOCATION_FAILED), process->id);
        return NULL;
    }
    process_node->process = process;
    process_node->next = NULL;
    process_node->previous = NULL;
}

// ProcessNode: Destruction functions:
void destroy_process_node(ProcessNode * process_node) {
    if (process_node == NULL) return;

    ProcessNode * next = process_node->next;
    ProcessNode * previous = process_node->previous;

    next->previous = NULL;
    previous->next = NULL;

    destroy_process(process_node->process);
    free(process_node);
}
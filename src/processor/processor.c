#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "process.h"
#include "processor.h"

/*=== Processor Helper Functions ===*/
ProcessState  random_process_state() {
    srand(time(NULL));
    const int outcome = rand() % 100 + 1;
    if (outcome < 33) return PROCESS_READY;
    else if (outcome >= 33 && outcome < 66) return PROCESS_BLOCKED;
    else return PROCESS_WAITING;
}

/*=== ProcessorState Enum and Functions ===*/

// ProcessorState: Creation Functions:
// NONE
// ProcessorState: Destruction Functions:
// NONE
// ProcessorState: Mutator Functions:
// NONE
// ProcessorState: Accessor Functions:
// NONE
// ProcessorState: ToString Functions:
const char * processor_state_to_string(const enum ProcessorState processor_state) {
    switch (processor_state) {
        case PROCESSOR_EXECUTING: return "Processor Executing";
        case PROCESSOR_FAILED: return "Processor Failed";
        case PROCESSOR_IS_NULL: return "Processor is NULL";
        case PROCESSOR_MEMORY_ALLOCATION_FAILED: return "Processor Memory Allocation Failed";
        default: return "Unknown ProcessorState";
    }
}

/*=== The Process Data Type and Functions ===*/

// ProcessorState: Creation Functions:
Processor * create_processor (const unsigned int id) {
    Processor * processor = (Processor *) malloc(sizeof(Processor));
    if (processor == NULL) {
        printf("%s\n", processor_state_to_string(PROCESSOR_MEMORY_ALLOCATION_FAILED));
        exit(0);
    }
    processor->id = id;
    processor->cycles = 0;
    return processor;
}

// Processor: Destruction Functions:
void destroy_processor (void * processor) { free(processor); }

// Processor: Mutator Functions:
// NONE
// Processor: Accessor Functions:
// NONE

// Processor: Boolean Functions:
bool execute_process (Processor * processor, Process * process) {
    if (processor == NULL) {
        printf("%s\n", processor_state_to_string(PROCESSOR_IS_NULL));
        return false;
    }
    if (process == NULL) {
        printf("%s\n", process_state_to_string(PROCESS_IS_NULL));
        return false;
    }
    if (process->state == PROCESS_READY) {
        process->state = PROCESS_RUNNING;
        process->milliseconds_remaining--;
    }
    processor->cycles++;
    process->initial_queue_entry_time++;
    if (process->milliseconds_remaining == 0) {
        process->state = PROCESS_FINISHED;
        process->file = NULL;
        free(process);
    } else {
        process->state = random_process_state();
    }
    return true;
}

// Processor: ToString Functions:
// NONE
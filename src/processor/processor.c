
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "../include/process.h"
#include "../include/processor.h"

const char * processor_state_to_string(const enum ProcessorState state) {
    switch (state) {
        case EXECUTING: return "Processor Executing";
        case PROCESSOR_FAILED: return "Processor Failed";
        case PROCESSOR_IS_NULL: return "Processor is NULL";
        case PROCESSOR_MEMORY_ALLOCATION_FAILED: return "Processor Memory Allocation Failed";
        default: return "Unknown ProcessorState";
    }
}

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

enum ProcessState random_process_state () {
    srand(time(NULL));
    const int outcome = rand() % 100 + 1;
    if (outcome < 15) return PROCESS_READY;
    else if (outcome < 30) return PROCESS_WAITING_EVENT;
    else if (outcome < 45) return PROCESS_WRITING_FILE_BLOCKED;
    else if (outcome < 60) return PROCESS_READING_FILE_BLOCKED;
    else if (outcome < 75) return PROCESS_READING_FILE;
    else return PROCESS_WRITING_TO_FILE;
}

Process * execute (Processor * processor, Process * process) {
    if (processor == NULL) {
        printf("%s\n", processor_state_to_string(PROCESSOR_IS_NULL));
        return NULL;
    }
    if (process == NULL) {
        printf("%s\n", process_state_to_string(PROCESS_IS_NULL));
        return NULL;
    }
    if (process->state == PROCESS_READY) {
        process->state = PROCESS_RUNNING;
        process->milliseconds_remaining--;
    }
    processor->cycles++;
    process->initial_queue_entry_time++;
    if (process->milliseconds_remaining == 0) {
        process->state = PROCESS_FINISHED;
        process->reading_file = NULL;
        process->writing_file = NULL;
    } else {
        process->state = random_process_state();
    }
    return process;
}
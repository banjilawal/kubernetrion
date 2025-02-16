
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "process.h"
#include "processor.h"
#include "process_generator.h"

char * processor_state_to_string(const enum ProcessorState processor_state) {
    switch (processor_state) {
        case PROCESSOR_EXECUTING: return "Processor Executing";
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

void destroy_processor (Processor * processor) {
    if (processor != NULL) return;
    free(processor);
    processor = NULL;
}



Process * execute_process (Processor * processor, Process * process) {
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
    }
    processor->cycles++;
    process->cpu_cycle_count++;
    if (process->milliseconds_remaining == 0) {
        process->state = PROCESS_FINISHED;
        process->file = NULL;
    } else {
        process->state = random_process_state();
    }
    process->milliseconds_remaining--;
    return process;
}
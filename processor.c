
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "process.h"
#include "include/processor.h"

const char * processorStateToString(const enum ProcessorState state) {
    switch (state) {
        case EXECUTING: return "Processor Executing";
        case PROCESSOR_FAILED: return "Processor Failed";
        case PROCESSOR_IS_NULL: return "Processor is NULL";
        case PROCESSOR_MEMORY_ALLOCATION_FAILED: return "Processor Memory Allocation Failed";
        default: return "Unknown ProcessorState";
    }
}

Processor * createProcessor (const unsigned int id) {
    Processor * processor = (Processor *) malloc(sizeof(Processor));
    if (processor == NULL) {
        printf("%s\n", processorStateToString(PROCESSOR_MEMORY_ALLOCATION_FAILED));
        exit(0);
    }
    processor->id = id;
    processor->cycles = 0;
    return processor;
}

enum ProcessState randomProcessState () {
    srand(time(NULL));
    const int outcome = rand() % 100 + 1;
    if (outcome < 33) return READY;
    else if (outcome >= 33 && outcome < 66) return BLOCKED;
    else return WAITING;
}

Process * execute (Processor * processor, Process * process) {
    if (processor == NULL) {
        printf("%s\n", processorStateToString(PROCESSOR_IS_NULL));
        return NULL;
    }
    if (process == NULL) {
        printf("%s\n", processStateToString(PROCESS_IS_NULL));
        return NULL;
    }
    if (process->state == READY) {
        process->state = RUNNING;
        process->remainingMilliseconds--;
    }
    processor->cycles++;
    process->timeQueued++;
    if (process->remainingMilliseconds == 0) {
        process->state = FINISHED;
        process->file = NULL;
    } else {
        process->state = randomProcessState();
    }
    return process;
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "process.h"
#include "processor.h"
#include "round_robin.h"

Processor * createProcessor () {
    Processor * processor = (Processor *) malloc(sizeof(Processor));
    if (processor == NULL) {
        printf("Cannot allocate memory to create processor\n");
        exit(0);
    }
    processor->queue = createRoundRobin();
    processor->nextProcessId = 0;
    processor->CYCLES = 0;
    return processor;
}

Process * execute (Processor * processor) {
    if (processor == NULL || processor->queue == NULL) {
        printf("No processes to execute\n");
        return NULL;
    }
    Process * process = dequeue(processor->queue);
    if (process->state == READY) {
        const float prevExecTime = process->remainingExecutionTime;
        process->state = RUNNING;
        process->remainingExecutionTime--;
        process->state = randomState();
    } else {
        process->state = randomState();
    }
    processor->CYCLES++;
    return process;
}

void addProcess (Processor * processor, Process * process) {
    if (processor == NULL || processor->queue == NULL) {
        printf("Processor or queue is null\n");
        return;
    }
    if (process == NULL) {
        printf("Process is null\n");
        return;
    }
    enqueue(processor->queue, process);
}
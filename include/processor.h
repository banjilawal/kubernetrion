//
// Created by banji on 1/25/2025.
//

#ifndef PROCESSOR_H
#define PROCESSOR_H

typedef enum ProcessorState {
    EXECUTING,
    PROCESSOR_FAILED,
    PROCESSOR_IS_NULL,
    PROCESSOR_MEMORY_ALLOCATION_FAILED
} ProcessorErrorState;
const char * processorStateToString(const enum ProcessorState state);

typedef struct Processor {
    unsigned int id;
    unsigned int cycles;
} Processor;
Processor * createProcessor (const unsigned int id);
Process *  execute (Processor * processor, Process * process);
ProcessState randomProcessState ();

#endif //PROCESSOR_H

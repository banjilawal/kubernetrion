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
const char * processor_state_to_string(const enum ProcessorState processor_state);

typedef struct Processor {
    unsigned int id;
    unsigned int cycles;
} Processor;
Processor * create_processor (const unsigned int id);
Process *  execute_process (Processor * processor, Process * process);

ProcessState random_process_state ();

#endif //PROCESSOR_H

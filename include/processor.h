//
// Created by banji on 1/25/2025.
//

#ifndef PROCESSOR_H
#define PROCESSOR_H

typedef enum ProcessorState {
    PROCESSOR_EXECUTING,
    PROCESSOR_FAILED,
    PROCESSOR_IS_NULL,
    PROCESSOR_MEMORY_ALLOCATION_FAILED
} ProcessorState;
char* processor_state_to_string(ProcessorState processor_state);

typedef struct Processor {
    unsigned int id;
    unsigned int cycles;
} Processor;

Processor* create_processor(const unsigned int processor_id);
void destroy_processor(Processor *processor);
Process*  execute_process(Processor *processor, Process *process);

#endif //PROCESSOR_H

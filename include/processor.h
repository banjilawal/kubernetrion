//
// Created by banji on 1/25/2025.
//

#ifndef PROCESSOR_H
#define PROCESSOR_H

/*=== Processor Helper Functions ===*/
ProcessState random_process_state();

/*=== ProcessorState Enum and Functions ===*/
typedef enum ProcessorState {
    PROCESSOR_EXECUTING,
    PROCESSOR_FAILED,
    PROCESSOR_IS_NULL,
<<<<<<< HEAD
    PROCESSOR_MEMORY_ALLOCATION_FAILED, PROCESSOR_WAITING
} ProcessorState;
=======
    PROCESSOR_MEMORY_ALLOCATION_FAILED
} ProcessorErrorState;
const char * processor_state_to_string(const enum ProcessorState processor_state);
>>>>>>> development

// ProcessorState: Creation Functions:
// NONE
// ProcessorState: Destruction Functions:
// NONE
// ProcessorState: Mutator Functions:
// NONE
// Processor: Accessor Functions:
// NONE
// ProcessorState: Boolean Functions:
// NONE

// ProcessorState: toString Function
const char * processor_state_to_string(const ProcessorState processor_state);

/*=== The Process Data Type and Functions ===*/
typedef struct Processor {
    unsigned int id;
    unsigned int cycles;
} Processor;
<<<<<<< HEAD

// Processor: Creation Functions:
Processor * create_processor (const unsigned int id);

// Processor: Destruction Functions:
void destroy_processor (Processor * processor);

// Processor: Mutator Functions:
// Processor: Accessor Functions:
// NONE

// Processor: Boolean Functions:
bool execute_procss (Processor * processor, Process * process);

// Processor: toString Function
// NONE
=======
Processor * create_processor (const unsigned int id);
Process *  execute_process (Processor * processor, Process * process);

ProcessState random_process_state ();
>>>>>>> development

#endif //PROCESSOR_H

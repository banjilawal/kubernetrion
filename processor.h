//
// Created by banji on 1/25/2025.
//

#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "round_robin.h"

typedef struct Processor {
    RoundRobin * queue;
    unsigned int nextProcessId;
    unsigned int CYCLES;
} Processor;

Processor * createProcessor ();
Process * execute (Processor * processor);
void addProcess (Processor * processor, Process * process);


#endif //PROCESSOR_H

//
// Created by griot on 2/11/25.
//

#ifndef PROCESS_GENERATOR_H
#define PROCESS_GENERATOR_H

//
// Created by griot on 1/31/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "process.h"
#include "process_list.h"

#define MAX_MILLISECONDS 20
#define MIN_MILLISECONDS 5
#define STARTING_PROCESS_NUMBER 0

// unsigned int nextId = 1;

const char* random_process_name();
ProcessState random_process_state();
ProcessQueue* process_stream(unsigned int stream_size);
unsigned int random_priority();
unsigned int random_milliseconds();

Process* random_process(Process *parent);

#endif //PROCESS_GENERATOR_H

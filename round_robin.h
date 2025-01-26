//
// Created by banji on 1/25/2025.
//
#pragma once

#include <stdbool.h>
#include "process.h"


#ifndef ROUND_ROBIN_H
#define ROUND_ROBIN_H

typedef struct RoundRobin {
    ProcessQueue * queue;
} RoundRobin;


RoundRobin * createRoundRobin ();
static bool enqueue (RoundRobin * roundRobin, Process * process);
static Process * dequeue (RoundRobin * roundRobin);

#endif //ROUND_ROBIN_H

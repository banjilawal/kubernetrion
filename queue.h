//
// Created by banji on 1/25/2025.
//
#pragma once

#include <stdbool.h>
#include "process.h"

#ifndef QUEUE_H
#define QUEUE_H

typedef struct RoundRobin {
    ProcessQueue * queue;
} RoundRobin;
RoundRobin * createRoundRobin ();
bool addToRoundRobinQueue (const RoundRobin * roundRobin, Process * process);
Process * popRoundRoundRobin (const RoundRobin * roundRobin);

typedef struct PriorityQueue {
    ProcessQueue * queue;
} PriorityQueue;
PriorityQueue * createPriorityQueue ();
bool addToPriorityQueue (const PriorityQueue * priorityQueue, Process * process);
Process * popPriorityQueue (const PriorityQueue * priorityQueue);

#endif //QUEUE_H

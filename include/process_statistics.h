//
// Created by griot on 2/9/25.
//

#pragma once
#include <stddef.h>

#include "file.h"
#include "process.h"
#ifndef PROCESS_STATISTICS
#define PROCESS_STATISTICS


typedef struct process_statistics {

    unsigned int totalCycles;
    unsigned int blockCount;
    unsigned int priority;
    unsigned int executionCycles;
    unsigned int queueEntryTime;
    ProcessState processState;
    File * file;
} ProcessMetrics;
ProcessMetrics * createProcessMetrics(Process * process);

typedef struct processCollection {
    size_t size;
    const ProcessMetrics * rows;
    unsigned int totalProcesses;
} ProcessCollection;
ProcessCollection * createProcessCollection(size_t size);
void addProcessMetrics(ProcessCollection * table, Process * process);
void removeProcessMetrics(ProcessCollection * table, Process * process);
ProcessMetrics * getProcessMetrics(ProcessCollection * table, size_t index, unsigned int processId);

typedef struct processStatistics {
    ProcessCollection * rows;
    unsigned int totalProcesses;
    unsigned int maxBlockCount;
    unsigned int minBlockCount;
    unsigned int averageBlockCount;
    unsigned int minCycleRuns;
    unsigned int maxCycleRuns;
    unsigned int averageCycleRuns;
    unsigned int oldestPID;
} ProcessStatistics;

#endif //PROCESS_STATISTICS

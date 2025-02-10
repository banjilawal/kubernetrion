//
// Created by griot on 2/9/25.
//

#pragma once
#include <stddef.h>

#include "file.h"
#include "process.h"

#ifndef PROCESS_TABLE
#define PROCESS_TABLE


/*=== The ProcessData Data Type and its Functions ===*/
typedef struct ProcessData {
    unsigned int totalCycles;
    unsigned int blockCount;
    unsigned int priority;
    unsigned int executionCycles;
    unsigned int queueEntryTime;
    ProcessState processState;
    File * file;
} ProcessData;

// ProcessData: Creation Functions:
ProcessData * createt_process_data (Process * process);

// ProcessStatistics: Destruction Functions:
void destroy_process_data (ProcessData * process_data);

// ProcessStatistics: Mutator Functions:
// NONE
// ProcessStatistics: Accessor Functions:
// NONE

// ProcessStatistics:Boolean Functions:
bool process_data_are_equal(const ProcessData * a, const ProcessData * b);

// ProcessStatistics: ToString Function:
const char * process_data_to_string (ProcessData * process_data);

/*=== The ProcessDataNode Data Type and its Functions ===*/
typedef struct ProcessDataNode {
  ProcessData * data;
  struct ProcessDataNode * next;
  struct ProcessDataNode * previous;
} ProcessDataNode;

// ProcessDataNode: Creation Functions:
ProcessDataNode * createt_process_data_node (Process * process);

// ProcessStatistics: Destruction Functions:
void destroy_process_data_node (ProcessDataNode * process_data_node);

// ProcessDataNode: Mutator Functions:
// NONE
// ProcessDataNode: Accessor Functions:
// NONE

// ProcessDataNode:Boolean Functions:
bool process_data_nodes_are_equal(const ProcessDataNode * a, const ProcessDataNode * b);

// ProcessDataNode: ToString Function:
const char * process_data_node_to_string (ProcessDataNode * process_data_node);

/*=== The ProcessTable Data Type and its Functions ===*/

typedef struct ProcessTable {
    ProcessDataNode * head;
    ProcessDataNode * tail;
    unsigned int totalProcesses;
} ProcessTable;

// ProcessTablea: Creation Functions:
ProcessTable * create_process_table ();

// ProcesTable: Destruction Functions:
void destroy_process_table (ProcessTable * process_table);

// ProcessTable: Mutator Functions:
void add_process_data (ProcessTable * process_table, const Process * process);
void remove_process_data (ProcessTable * process_table, const unsigned int process_id);

// ProcessTable: Accessor Functions:
ProcessData * find_process_data_by_pid (ProcessTable * process_table, const unsigned int process_id);

// ProcessTable:Boolean Functions:
// NONE
// ProcessTable: ToString Function:
const char * process_table_to_string (ProcessTable * process_table);


//typedef struct processStatistics {
//    ProcessCollection * rows;
//    unsigned int totalProcesses;
//    unsigned int maxBlockCount;
//    unsigned int minBlockCount;
//    unsigned int averageBlockCount;
//    unsigned int minCycleRuns;
//    unsigned int maxCycleRuns;
//    unsigned int averageCycleRuns;
//    unsigned int oldestPID;
//} ProcessStatistics;

#endif //PROCESS_TABLE

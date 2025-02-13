//
// Created by griot on 2/9/25.
//

#ifndef PROCESS_TABLE
#define PROCESS_TABLE

#pragma once

#include "file.h"
#include "process.h"

#define PROCESS_RECORD_STRING_SIZE 256

/*=== The ProcessRecord Data Type and its Functions ===*/
typedef struct ProcessRecord {
    const char * name;
    const unsigned int pid;
    const unsigned int cpu_cycle_count;
    const unsigned int priority;
    const unsigned int milliseconds_remaining;
    const ProcessState state;
} ProcessRecord;

// ProcessExecutionRecord: Creation Functions:
ProcessRecord * create_process_record (Process * process);

// ProcessPriorityRecord: Destruction Functions:
void destroy_process_record (const ProcessRecord * process_record);

// ProcessPriorityRecord: Mutator Functions:
// NONE
// ProcessPriorityRecord: Accessor Functions:
unsigned int process_record_get_pid(const ProcessRecord* process_record);
unsigned int process_record_get_priority(const ProcessRecord* process_record);
ProcessState process_record_get_state(const ProcessRecord* process_record);
const char *  process_record_get_name(const ProcessRecord* process_record);
unsigned int process_record_get_cpu_cycle_count(const ProcessRecord* process_record);
ProcessState process_record_get_milliseconds_remaining(const ProcessRecord* process_record);

// ProcessyRecord:Boolean Functions:
bool process_records_are_equal (const ProcessRecord * a, const ProcessRecord * b);

// ProcessPriorityRecord: ToString Function:
const char * process_record_to_string (ProcessRecord * process_record);

/*=== The ProcessExecutionRecord Data Type and its Functions ===*/

typedef struct ProcessExecutionRecord {
    const unsigned int pid;
    unsigned int number_of_ready_cycles;
    unsigned int number_of_running_cycles;
    unsigned int number_of_blocked_reads;
    unsigned int number_of_blocked_writes;
    unsigned int number_of_waiting_cycles;
    unsigned int number_of_reads;
    unsigned int number_of_writes;
    unsigned int total_cpu_cycles;
    ProcessState current_process_state;
} ProcessExecutionRecord;

// ProcessExecutionRecord: Creation Functions:
ProcessExecutionRecord * create_process_execution_record (Process * process);

// ProcessExecutionRecord: Destruction Functions:
void destroy_process_execution_record (ProcessExecutionRecord * process_execution_record);

// ProcessExecutionRecord: Mutator Functions:
// NONE
// ProcessExecutionRecord: Accessor Functions:
// NONE

// ProcessExecutionRecord:Boolean Functions:
bool process_execution_records_are_equal(const ProcessExecutionRecord * a, const ProcessExecutionRecord * b);

// ProcessExecutionRecord: ToString Function:
const char * process_execution_record_to_string (ProcessExecutionRecord * process_execution_record);

/*=== The ProcessDataNode Data Type and its Functions ===*/
typedef struct ProcessDataNode {
  ProcessRecord * process_record;
  struct ProcessDataNode * next;
  struct ProcessDataNode * previous;
} ProcessDataNode;

// ProcessDataNode: Creation Functions:
ProcessDataNode * create_process_data_node (Process * process);

// ProcessDataNode: Destruction Functions:
void destroy_process_data_node (ProcessDataNode * process_data_node);

// ProcessDataNode: Mutator Functions:
// NONE
// ProcessDataNode: Accessor Functions:
const ProcessRecord * get_process_record(const ProcessDataNode * process_data_node);

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

// ProcessTable: Creation Functions:
ProcessTable * create_process_table ();

// ProcessTable: Destruction Functions:
void destroy_process_table (ProcessTable * process_table);

// ProcessTable: Mutator Functions:
void add_process_data (ProcessTable * process_table, const Process * process);
void remove_process_data (ProcessTable * process_table, const unsigned int process_id);

// ProcessTable: Accessor Functions:
ProcessTable * filer_process_table_by_pid (ProcessTable * process_table, const unsigned int process_id);

// ProcessTable:Boolean Functions:
bool process_table_is_empty (ProcessTable * process_table);
// ProcessTable: ToString Function:
const char * process_table_to_string (ProcessTable * process_table);

#endif //PROCESS_TABLE

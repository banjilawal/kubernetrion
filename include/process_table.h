//
// Created by griot on 2/9/25.
//

#ifndef PROCESS_TABLE
#define PROCESS_TABLE

#pragma once

#include "file.h"
#include "process.h"

#define PROCESS_RECORD_STRING_SIZE 256

#define PROCESS_RECORD_IS_NULL_MESSAGE "The ProcessRecord is NULL"
#define PROCESS_RECORD_MEMORY_ALLOCATION_FAILED_MESSAGE "ProcessRecord memory allocation failed"

#define PROCESS_DATA_NODE_IS_NULL_MESSAGE "The ProcessDataNode is NULL"
#define PROCESS_DATA_NODE_MEMORY_ALLOCATION_FAILED_MESSAGE "ProcessNode memory allocation failed"

#define PROCESS_TABLE_IS_NULL_MESSAGE "The ProcessTable is NULL"
#define PROCESS_TABLE_MEMORY_ALLOCATION_FAILED_MESSAGE "ProcessTable memory allocation failed"

/*=== The ProcessRecord Data Type and its Functions ===*/
typedef struct ProcessRecord {
    char * name;
    unsigned int pid;
    unsigned int cpu_cycle_count;
    unsigned int priority;
    unsigned int milliseconds_remaining;
    ProcessState state;
} ProcessRecord;

/* ProcessRecord Functions */
ProcessRecord * create_process_record (Process * process);
void destroy_process_record (ProcessRecord * process_record);
ProcessRecord * copy_process_record (const ProcessRecord * process_record);
bool process_records_are_equal (const ProcessRecord * a, const ProcessRecord * b);
const char * process_record_to_string (const ProcessRecord * process_record);

/*=== The ProcessDataNode Data Type and its Functions ===*/
typedef struct ProcessDataNode {
  ProcessRecord * process_record;
  struct ProcessDataNode * next;
  struct ProcessDataNode * previous;
} ProcessDataNode;

/* ProcessDataNode Functions */
ProcessDataNode * create_process_data_node (ProcessRecord * process_record);
void destroy_process_data_node (ProcessDataNode * process_data_node);
ProcessDataNode * copy_process_data_node (const ProcessDataNode * process_data_node);
bool process_data_nodes_are_equal(const ProcessDataNode * a, const ProcessDataNode * b);
const char * process_data_node_to_string (const ProcessDataNode * process_data_node);

/*=== The ProcessTable Data Type and its Functions ===*/

typedef struct ProcessTable {
    ProcessDataNode * head;
    ProcessDataNode * tail;
    unsigned int number_of_rows;
} ProcessTable;

// ProcessTable: Creation Functions:
ProcessTable * create_process_table ();

// ProcessTable: Destruction Functions:
void destroy_process_table (ProcessTable * process_table);

// ProcessTable: Mutator Functions:
void add_process_data_node_to_table (ProcessTable * process_table, const ProcessDataNode * process_data_node);


// ProcessTable: Accessor Functions:
ProcessTable * filer_process_table_by_pid (const ProcessTable * process_table, const unsigned int process_id);

// ProcessTable:Boolean Functions:
bool process_table_is_empty (const ProcessTable * process_table);

// ProcessTable: ToString Function:
const char * process_table_to_string (const ProcessTable * process_table);

#endif //PROCESS_TABLE

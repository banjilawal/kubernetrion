//
// Created by griot on 2/9/25.
//

#pragma once

#include <stdlib.h>
#include <stdio.h>

#include "process.h"
#include "process_table.h"

/*=== The ProcessPriorityRecord Data Type and its Functions ===*/
// ProcessExecutionRecord: Creation Functions:
ProcessPriorityRecord * create_process_priority_record (Process * process) {
  if (process == NULL) {
    printf("%s\n", process_state_to_string(PROCESS_IS_NULL));
    return NULL;
  }
  ProcessPriorityRecord * priority_record = (ProcessPriorityRecord *) malloc (sizeof (ProcessPriorityRecord));
  if (priority_record == NULL) {
    printf("Failed to allocate memory for ProcessPriorityRecord\n");
    return NULL;
  }
  *(unsigned int *)&priority_record->pid = get_process_id(process); // Casting away const
  *(unsigned int *)&priority_record->priority = process->priority;  // Casting away const
  *(ProcessState *)&priority_record->state = process->state;
  return priority_record;
}

//// ProcessPriorityRecord: Destruction Functions:
void destroy_process_priority_record (ProcessPriorityRecord * process_priority_record) {
  if (process_priority_record == NULL) return;
  free(process_priority_record);
}
//
//// ProcessPriorityRecord: Mutator Functions:


//// ProcessPriorityRecord: Accessor Functions:
unsigned int process_priority_record_get_pid(const ProcessPriorityRecord *priority_record) {
  return priority_record->pid;
}

unsigned int process_priority_record_get_priority(const ProcessPriorityRecord *priority_record) {
  return priority_record->priority;
}

ProcessState process_priority_record_get_state(const ProcessPriorityRecord *priority_record) {
  return priority_record->state;
}

//// ProcessPriorityRecord:Boolean Functions:
//bool process_priority_records_are_equal (const ProcessPriorityRecord * a, const ProcessPriorityRecord * b);
//
//// ProcessPriorityRecord: ToString Function:
//const char * process_priority_record_to_string (ProcessPriorityRecord * process_priority_record);
//
///*=== The ProcessExecutionRecord Data Type and its Functions ===*/
//
//typedef struct ProcessExecutionRecord {
//    unsigned int pid;
//    unsigned int number_of_ready_cycles;
//    unsigned int number_of_running_cycles;
//    unsigned int number_of_blocked_reads;
//    unsigned int number_of_blocked_writes;
//    unsigned int number_of_waiting_cycles;
//    unsigned int number_of_reads;
//    unsigned int number_of_writes;
//    unsigned int total_cpu_cycles;
//    ProcessState current_process_state;
//} ProcessExecutionRecord;
//
//// ProcessExecutionRecord: Creation Functions:
//ProcessExecutionRecord * create_process_execution_record (Process * process);
//
//// ProcessExecutionRecord: Destruction Functions:
//void destroy_process_execution_record (ProcessExecutionRecord * process_execution_record);
//
//// ProcessExecutionRecord: Mutator Functions:
//// NONE
//// ProcessExecutionRecord: Accessor Functions:
//// NONE
//
//// ProcessExecutionRecord:Boolean Functions:
//bool process_execution_records_are_equal(const ProcessExecutionRecord * a, const ProcessExecutionRecord * b);
//
//// ProcessExecutionRecord: ToString Function:
//const char * process_execution_record_to_string (ProcessExecutionRecord * process_execution_record);
//
///*=== The ProcessDataNode Data Type and its Functions ===*/
//typedef struct ProcessDataNode {
//  ProcessPriorityRecord * priority_record;
//  ProcessExecutionRecord * execution_record;
//  struct ProcessDataNode * next;
//  struct ProcessDataNode * previous;
//} ProcessDataNode;
//
//// ProcessDataNode: Creation Functions:
//ProcessDataNode * create_process_data_node (Process * process);
//
//// ProcessDataNode: Destruction Functions:
//void destroy_process_data_node (ProcessDataNode * process_data_node);
//
//// ProcessDataNode: Mutator Functions:
//// NONE
//// ProcessDataNode: Accessor Functions:
//// NONE
//
//// ProcessDataNode:Boolean Functions:
//bool process_data_nodes_are_equal(const ProcessDataNode * a, const ProcessDataNode * b);
//
//// ProcessDataNode: ToString Function:
//const char * process_data_node_to_string (ProcessDataNode * process_data_node);
//
///*=== The ProcessTable Data Type and its Functions ===*/
//
//typedef struct ProcessTable {
//    ProcessDataNode * head;
//    ProcessDataNode * tail;
//    unsigned int totalProcesses;
//} ProcessTable;
//
//// ProcessTable: Creation Functions:
//ProcessTable * create_process_table ();
//
//// ProcessTable: Destruction Functions:
//void destroy_process_table (ProcessTable * process_table);
//
//// ProcessTable: Mutator Functions:
//void add_process_data (ProcessTable * process_table, const Process * process);
//void remove_process_data (ProcessTable * process_table, const unsigned int process_id);
//
//// ProcessTable: Accessor Functions:
//ProcessDataNode * find_process_data_node (ProcessTable * process_table, const unsigned int process_id);
//
//// ProcessTable:Boolean Functions:
//// NONE
//// ProcessTable: ToString Function:
//const char * process_table_to_string (ProcessTable * process_table);
//
//#endif //PROCESS_TABLE

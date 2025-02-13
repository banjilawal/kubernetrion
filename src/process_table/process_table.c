//
// Created by griot on 2/9/25.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "process.h"
#include "process_table.h"

const char * process_record_string_format = "ProcessRecord["
    " pid:%d"
    " name:%s"
    " state:%s"
    " priority:%d"
    " milliseconds_remaining:%d"
    " cpu_cycle_count:%d]";


/*=== The ProcessRecord Data Type and its Functions ===*/
// ProcessRecord: Creation Functions:
ProcessRecord * create_process_priority_record (Process * process) {
  if (process == NULL) {
    printf("%s\n", process_state_to_string(PROCESS_IS_NULL));
    return NULL;
  }
  ProcessRecord * process_record = (ProcessRecord *) malloc (sizeof (ProcessRecord));
  if (process_record == NULL) {
    printf("Failed to allocate memory for ProcessRecord\n");
    return NULL;
  }    const char * name;


  *(const char **)&process_record->name = get_process_name(process)
  *(unsigned int *)&process_record->pid = get_process_id(process); 
  *(unsigned int *)&process_record->milliseconds_remaining = process->milliseconds_remaining;
  *(unsigned int *)&process_record->cpu_cycle_count = process->cpu_cycle_count;
  *(unsigned int *)&process_record->priority = process->priority; 
  *(ProcessState *)&process_record->state = process->state;
  return process_record;
}

// ProcessPriorityRecord: Destruction Functions:
void destroy_process_priority_record (ProcessRecord * process_record) {
  if (process_record == NULL) return;
  free(process_record);
}

//// ProcessPriorityRecord: Mutator Functions:


// ProcessPriorityRecord: Accessor Functions:
unsigned int process_record_get_pid(const ProcessRecord* process_record) {
  return process_record->pid;
}

unsigned int process_record_get_priority(const ProcessRecord* process_record) {
  return process_record->priority;
}

ProcessState process_record_get_state(const ProcessRecord* process_record) {
  return process_record->state;
}

const char *  process_record_get_name(const ProcessRecord* process_record) {
  return process_record->name;
}

unsigned int process_record_get_cpu_cycle_count(const ProcessRecord* process_record) {
  return process_record->cpu_cycle_count;
}

ProcessState process_record_get_milliseconds_remaining(const ProcessRecord* process_record) {
  return process_record->milliseconds_remaining;
}

// ProcessRecord:Boolean Functions:
bool process_priority_records_are_equal (const ProcessRecord * a, const ProcessRecord * b) {
  if (a == b) return true;
  if (a == NULL || b == NULL) return false;
  return process_record_get_pid(a) == process_record_get_pid(b)  &&
    process_record_get_cpu_cycle_count(a) == process_record_get_cpu_cycle_count(b) &&
    process_record_get_milliseconds_remaining(a) == process_record_get_milliseconds_remaining(b) &&
    process_record_get_priority(a) == process_record_get_priority(b) &&
    process_record_get_state(a) == process_record_get_state(b);
}

// ProcessPriorityRecord: ToString Function:
const char * process_record_to_string (ProcessRecord * process_record) {
  const char * process_record_string_format = "ProcessRecord["

  if (process_record == NULL) {
    printf("process_record is null");
    return NULL;
  }

  const int bufferSize = PROCESS_RECORD_STRING_SIZE * sizeof(char);
  char * string = (char *) malloc(bufferSize);
  if (string == NULL) {
    printf("Could not allocate memory for process_record_to_string.\n");
    return NULL;
  }
  snprintf(
      string,
      bufferSize,
      process_record_string_format,
      process_record_get_pid(process_record),
      process_record_get_name(process_record),
      process_record_get_state(process_record),
      process_record_get_priority(process_record),
      process_record_get_milliseconds_remaining(process_record),
      process_record_get_cpu_cycle_count(process_record)
  );
  return string;
}

/*=== The ProcessDataNode Data Type and its Functions ===*/

// ProcessDataNode: Creation Functions:
ProcessDataNode * create_process_data_node (ProcessRecord * process_record) {
  if (process_record == NULL) {
    printf("Cannot create process node. the record is null.\n");
    return NULL;
  }

  ProcessDataNode * process_data_node = (ProcessDataNode *) malloc (sizeof (ProcessDataNode));
  if (process_data_node == NULL) {
    printf("Failed to allocate memory for process_data_node.\n");
    return NULL;
  }

  process_data_node->process_record = process_record;
  process_data_node->next = NULL;
  process_data_node->previous = NULL;
  return process_data_node;
}

// ProcessDataNode: Destruction Functions:
void destroy_process_data_node (ProcessDataNode * process_data_node) {
  if (process_data_node == NULL) return;
  process_data_node->next->previous = process_data_node->previous;
  process_data_node->previous->next = process_data_node->next;
  process_data_node->previous = NULL;
  process_data_node->next = NULL;
  free(process_data_node);
}

//// ProcessDataNode: Mutator Functions:
//// NONE
//// ProcessDataNode: Accessor Functions:
ProcessDataNode * copy_process_data_node (ProcessDataNode * process_data_node) {
  if (process_data_node == NULL) return NULL;
  ProcessDataNode * copy = (ProcessDataNode *) malloc (sizeof (ProcessDataNode));
  if (copy == NULL) {
    printf("Failed to allocate memory for copy_process_data_node.\n");
    return NULL;
  }
  copy = create_process_data_node(process_data_node->process_record);
  return copy;
}

ProcessRecord * get_process_data_node_record (const ProcessDataNode * process_data_node) {
  return process_data_node->process_record;
}

unsigned int process_data_node_get_pid(const ProcessDataNode * process_data_node) {
  return process_record_get_pid(process_data_node->process_record);
}

unsigned int process_data_node_get_priority(const ProcessDataNode * process_data_node) {
  return process_record_get_priority(process_data_node->process_record);
}

ProcessState process_data_node_get_state(const ProcessDataNode * process_data_node) {
  return process_record_get_state(process_data_node->process_record);
}

const char * process_data_node_get_name(const ProcessDataNode * process_data_node) {
  return process_record_get_name(process_data_node->process_record);
}

unsigned int process_data_node_get_cpu_cycle_count(const ProcessDataNode * process_data_node) {
  return process_record_get_cpu_cycle_count(process_data_node->process_record);
}

ProcessState process_data_node_get_milliseconds_remaining(const ProcessDataNode * process_data_node) {
  return process_record_get_milliseconds_remaining(process_data_node->process_record);
}

// ProcessDataNode:Boolean Functions:
bool process_data_nodes_are_equal(const ProcessDataNode * a, const ProcessDataNode * b) {
  if (a == b) return true;
  if (a == NULL || b == NULL) return false;
  return process_priority_records_are_equal(get_process_data_node_record(a), get_process_data_node_record(b));
}


// ProcessDataNode: ToString Function:
const char * process_data_node_to_string (ProcessDataNode * process_data_node) {
  return process_record_to_string(get_process_data_node_record(process_data_node));
}

///*=== The ProcessTable Data Type and its Functions ===*/
//
//typedef struct ProcessTable {
//    ProcessDataNode * head;
//    ProcessDataNode * tail;
//    unsigned int totalProcesses;
//} ProcessTable;
//
//// ProcessTable: Creation Functions:
ProcessTable * create_process_table () {
  ProcessTable * process_table = (ProcessTable *) malloc (sizeof (ProcessTable));
  if (process_table == NULL) {
    printf("Failed to allocate memory for process_table.\n");
    return NULL;
  }
  process_table->head = NULL;
  process_table->tail = NULL;
  process_table->totalProcesses = 0;
  return process_table;
}

// ProcessTable: Destruction Functions:
void destroy_process_table (ProcessTable * process_table) {
  if (process_table == NULL) return;
  ProcessDataNode *cursor = process_table->head;
  while (cursor != NULL) {
      ProcessDataNode * next = cursor->next;
      destroy_process_data_node(cursor);
      cursor = next;
  }
  free(process_table);
}

// ProcessTable: Mutator Functions:
void add_process_data (ProcessTable * process_table, const Process * process) {
  if (process_table == NULL) return;
  if (process == NULL) return;
  ProcessDataNode * node = create_process_data_node(process);
  if (node == NULL) return;
  if (process_table->head == NULL) {
      process_table->head = node;
      process_table->tail = node;
  } else {
      process_table->tail->next = node;
      process_table->tail = node;
  }
  process_table->totalProcesses++;
}

// ProcessTable: Accessor Functions:
// ProcessTable * filer_process_table_by_pid (ProcessTable * process_table, const unsigned int process_id) {
//   if (process_table == NULL) {
//     printf("The process table is null. It canot be filtered.\n");
//     return NULL;
//   }
//   if (process_table_is_empty(process_table)) {
//     printf("The process table is empty. It can't be filtered.\n");
//     return NULL;
//   }
//
//   ProcessTable * matches = create_process_table();
//   if (matches == NULL) {
//     printf("Failed to allocate memory for process table filter results");
//     return NULL;
//   }
//
//   ProcessDataNode * cursor = process_table->head;
//   while (cursor != NULL) {
//     if (cursor->)
// }
//
//// ProcessTable:Boolean Functions:
bool process_table_is_empty (ProcessTable * process_table) {
  if (process_table == NULL) {
    printf("The process table is null. Null items cannot be empty");
    return false;
  }
  if (process_table->totalProcesses == 0) return true;
  return process_table->head == NULL ||
    ((process_table->head == process_table-> tail) && process_table->tail == NULL);
}
//// ProcessTable: ToString Function:
//const char * process_table_to_string (ProcessTable * process_table);
//
//#endif //PROCESS_TABLE

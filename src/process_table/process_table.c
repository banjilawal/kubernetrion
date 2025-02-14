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
ProcessRecord * create_process_priority_record (const Process * process) {
  if (process == NULL) {
    printf("%s\n", process_state_to_string(PROCESS_IS_NULL));
    return NULL;
  }

  ProcessRecord * process_record = (ProcessRecord *) malloc (sizeof (ProcessRecord));
  if (process_record == NULL) {
    printf("Failed to allocate memory for ProcessRecord\n");
    free(process_record);
    return NULL;
  }

  process_record->name = (char *) get_process_name(process);;
  process_record->pid = get_process_id(process);
  process_record->milliseconds_remaining = process->milliseconds_remaining;
  process_record->cpu_cycle_count = process->cpu_cycle_count;
  process_record->priority = process->priority;
  process_record->state = process->state;

  return process_record;
}

/*
 *destroy_process_record(ProcessRecord * process_record)
 *return: void
 */
void destroy_process_record (ProcessRecord * process_record) {
  if (process_record == NULL) return;
  free(process_record);
}

/*
 *copy_process_record(const ProcessRecord * processRecord)
 *return: ProcessRecord *
 */
ProcessRecord * copy_process_record (const ProcessRecord * process_record) {

  // If process_record is null the copy fails
  if (process_record == NULL) return NULL;
}
/*
 *process_records_are_equal(const ProcessRecord *a, const ProcessRecord *b)
 *return: bool
 */
bool process_records_are_equal (const ProcessRecord * a, const ProcessRecord * b) {
  if (a == b) return true;
  if (a == NULL || b == NULL) return false;
  return a->pid == b->pid && a->cpu_cycle_count == b->cpu_cycle_count &&
    a->milliseconds_remaining == b->milliseconds_remaining && a->priority == b->priority &&
    a->state == b->state && strcmp(a->name, b->name) == 0;
}

/*
 *process_record_to_string(const ProcessRecord * process_record)
 *return: bool
 */
const char * process_record_to_string (const ProcessRecord * process_record) {

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
      process_record->pid, process_record->name, process_record->priority, process_record->milliseconds_remaining,
      process_record->cpu_cycle_count
  );
  return string;
}

/*
 *create_process_data_node(ProcessRecord * process_record)
 *return: ProcessNode *
 */
ProcessDataNode * create_process_data_node (ProcessRecord * process_record) {

  // Handle null process_record
  if (process_record == NULL) {
    printf("process_record is null. Cannot create process_data_node.\n");
    return NULL;
  }

  // Assign memory to ProcessDataNode and its' pointer
  ProcessDataNode * process_data_node = (ProcessDataNode *) malloc (sizeof (ProcessDataNode));

  // If process_data_node was not created clean up and return
  if (process_data_node == NULL) {
    printf("%s\n", PROCESS_DATA_NODE_IS_NULL_MESSAGE);
    free(process_data_node);
    process_data_node = NULL;
    return NULL;
  }

  // Set fields in the ProcessRecord instance and return
  process_data_node->process_record = process_record;
  process_data_node->next = NULL;
  process_data_node->previous = NULL;

  return process_data_node;
}

/*
 *destroy_process_data_node(ProcessDataNode * process_data_node)
 *return: void
 */
void destroy_process_data_node (ProcessDataNode * process_data_node) {

  // If the ProcessDataNode is already null. Nothing to do, return.
  if (process_data_node == NULL) return;

  // Destroy ProcessDataNode's record
  destroy_process_record(process_data_node->process_record);

  // Free the memory allocated to the ProcessDataNode. Remove the pointer to the address.
  free(process_data_node);
  process_data_node = NULL;
}

/*
 *copy_process_data_node(const ProcessDataNode * process_data_node)
 *return: ProcessDataNode *
 */
ProcessDataNode * copy_process_data_node (const ProcessDataNode * process_data_node) {

  if (process_data_node == NULL) {
    printf("%s. Cannot copy process data_node", PROCESS_DATA_NODE_IS_NULL_MESSAGE);
    return NULL;
  }

  ProcessDataNode * copy = (ProcessDataNode *) malloc (sizeof (ProcessDataNode));
  if (copy == NULL) {
    printf("%s\n", PROCESS_DATA_NODE_MEMORY_ALLOCATION_FAILED_MESSAGE);
    destroy_process_data_node(copy);
    copy = NULL;
    return NULL;
  }

  copy = create_process_data_node(process_data_node->process_record);
  return copy;
}

/*
 *process_data_nodes_are_equal(const ProcessDataNode * a, const ProcessDataNode * b)
 *return bool
 */
bool process_data_nodes_are_equal(const ProcessDataNode * a, const ProcessDataNode * b) {
  if (a == b) return true;
  if (a == NULL || b == NULL) return false;
  return process_records_are_equal( a->process_record, b->process_record);
}


/*
 *process_data_node_to_string(const ProcessDataNode * process_data_node)
 *return: const char *
 */
const char * process_data_node_to_string (const ProcessDataNode * process_data_node) {
  if (process_data_node == NULL || process_data_node->process_record == NULL) {
    printf("%s. Cannot print process data_node", PROCESS_DATA_NODE_IS_NULL_MESSAGE);
    return NULL;
  }

  return process_record_to_string(process_data_node->process_record);
}


/*
 *create_process_table()
 *return ProcessTable *
 */
ProcessTable * create_process_table () {

  // Assign a pointer to memory space that fits a ProcessTable
  ProcessTable * process_table = (ProcessTable *) malloc (sizeof (ProcessTable));

  // If there was not enough memory free what was assigned; print a message, and return null
  if (process_table == NULL) {
    printf("%s\n", PROCESS_TABLE_MEMORY_ALLOCATION_FAILED_MESSAGE);
    destroy_process_table(process_table);
    free(process_table);
    return NULL;
  }

  // Initialize ProcessTable's fields and return the instance
  process_table->head = NULL;
  process_table->tail = NULL;
  process_table->number_of_rows = 0;

  return process_table;
}


/*
 *destroy_process_table
 *return: void
 */
void destroy_process_table (ProcessTable * process_table) {

  // If ProcessTable is already null. Nothing to do return to te the caller
  if (process_table == NULL) return;

  // Create a cursor that traverses the table and destroy each node
  ProcessDataNode *cursor = process_table->head;
  while (cursor != NULL) {
      ProcessDataNode * next = cursor->next;
      destroy_process_data_node(cursor);
      cursor = next;
  }

  // Destroy head and tail
  destroy_process_data_node(process_table->head);
  destroy_process_data_node(process_table->tail);

  // free the memory assigned
  free(process_table);

  // Set the pointer to null
  process_table = NULL;
}

/*
 *add_process_data_node_to_table(ProcessTable * process_table, ProcessDataNode * process_data_node)
 *return: void
 *return: void
 */
void add_process_data_node_to_table (ProcessTable * process_table, ProcessDataNode * process_data_node) {

  // Cannot add to a null ProcessTable
  if (process_table == NULL) {
    printf("%s. Cannot add process data.\n", PROCESS_TABLE_IS_NULL_MESSAGE);
    return;
  }

  // Cannot add null ProcessDataNode
  if (process_data_node == NULL) {
    printf("%s. Cannot add to the process table.\n", PROCESS_DATA_NODE_IS_NULL_MESSAGE);
    return;
  }

  // If ProcessTable is empty head and tail are the ProcessDataNode instance
  if (process_table_is_empty(process_table)) {
    process_table->head = process_data_node;
    process_table->tail = process_data_node;
  }
// If ProcessTable is not empty add ProcessDataNode at the tail
  else {
    process_table->tail->next->previous = process_data_node;
    process_data_node->next = process_table->tail->next;
    process_table->tail = process_data_node;
  }

  // Increment total_rows in ProcessTable after P
  process_table->number_of_rows++;
}

/*
 *filter_process_table_by_pid(const ProcessTable * processTable, const unsigned int process_id)
 *return: ProcessTable *
 */
ProcessTable * filer_process_table_by_pid (const ProcessTable * process_table, const unsigned int process_id) {

  // If ProcessTable is null there is nothing to filter
  if (process_table == NULL) {
    printf("%s. It cannot be filtered.\n", PROCESS_TABLE_IS_NULL_MESSAGE);
    return NULL;
  }

  //
  if (process_table_is_empty(process_table)) {
    printf("%s. It can't be filtered.\n", PROCESS_TABLE_IS_EMPTY_MESSAGE);
    return NULL;
  }

  // Create a ProcessTable to hold records matching process_id
  ProcessTable * matches = create_process_table();

  // If matches is null the memory allocation failed, Assure matches is destroy and return
  if (matches == NULL) {
    printf("%s for storing pid filter results", PROCESS_TABLE_IS_NULL_MESSAGE);
    destroy_process_table(matches);
    matches = NULL;
    return NULL;
  }

  ProcessDataNode * cursor = process_table->head;
  while (cursor != NULL) {
    if (cursor->process_record->pid == process_id) {
      add_process_data_node_to_table(matches, copy_process_data_node(cursor));
    }
    cursor = cursor->next;
  }
  return matches;
}


/*
 *process_table_is_empty
 *return: bool
 */
bool process_table_is_empty (const ProcessTable * process_table) {

  // Null ProcessTable is not empty. It does not exist
  if (process_table == NULL) {
    printf("%s. Null items cannot be empty", PROCESS_TABLE_IS_NULL_MESSAGE);
    return false;
  }

  // Checking if ProcessTable is empty by number of rows
  if (process_table->number_of_rows == 0) return true;

  //T Checking if ProcessTable is empty by head or tail being null
  return process_table->head == NULL ||
    ((process_table->head == process_table-> tail) && process_table->tail == NULL);
}

/*
 *process_table_to_string (ProcessTable * process_table)
 *return: const char *
 */


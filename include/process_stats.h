//
// Created by griot on 2/10/25.
//

#ifndef PROCESS_STATISTICS_H
#define PROCESS_STATISTICS_H

#pragma once
#include "process_table.h"


/*=== The PriorityElevationSummary Data Type and its Functions ===*/

typedef struct PriorityElevationSummary {
    unsigned int total_number_of_promotions;
    unsigned int lowest_number_of_promotions;
    unsigned int highest_number_of_promotions;
    unsigned int avg_number_of_promotions;
    unsigned int least_promoted_pid;
    unsigned int most_promoted_pid;
} PriorityElevationSummary;

// PriorityElevationSummary: Creation Functions:
PriorityElevationSummary * get_priority_elevation_summary(const ProcessTable * process_table);

// PriorityElevationSummary: Destruction Functions:
void destroy_priority_elevation_summary(PriorityElevationSummary * elevation_summary);

// PriorityElevationSummary: ToString Function:
const char * process_elevation_summary_to_string(const PriorityElevationSummary * elevation_summary);

/*=== The PriorityDemotionSummary Data Type and its Functions ===*/

typedef struct PriorityDemotionSummary {
    unsigned int total_number_of_demotions;
    unsigned int lowest_number_of_demotions;
    unsigned int highest_number_of_demotions;
    unsigned int avg_number_of_demotions;
    unsigned int least_demoted_pid;
    unsigned int most_demoted_pid;
} PriorityDemotionSummary;

// PriorityDemotionSummary: Creation Functions:
PriorityDemotionSummary * get_priority_demotion_summary(const ProcessTable * process_table);

// PPriorityDemotionSummary: Destruction Functions:
void destroy_priority_demotion_summary(PriorityDemotionSummary * demotion_summary);

// PriorityDemotionSummary: ToString Function:
const char * process_demotion_summary_to_string(const PriorityDemotionSummary * demotion_summary);


/*=== The ReadyCyclesSummary Data Type and its Functions ===*/

typedef struct ReadyCyclesSummary {
    unsigned int total_number_of_ready_times;
    unsigned int min_number_of_ready_times;
    unsigned int max_number_of_ready_times;
    unsigned int avg_number_of_ready_times;;
    unsigned int total_number_of_running_times;
} ReadyCyclesSummary;

// ReadyCyclesSummary: Creation Functions:
ReadyCyclesSummary * get_ready_cycles_summary(const ProcessTable * process_table);

// ReadyCyclesSummary: Destruction Functions:
void destroy_ready_cycles_summary(ReadyCyclesSummary * ready_cycles_summary);

// ReadyCyclesSummary: ToString Function:
const char * ready_cycles_summary_to_string(const ReadyCyclesSummary * ready_cycles_summary);


/*=== The RunningCyclesSummary Data Type and its Functions ===*/

typedef struct RunningCyclesSummary {
    unsigned int total_running_cycles;
    unsigned int lowest_number_of_running_cycles;
    unsigned int highest_number_of_running_cycles;
    unsigned int avg_number_of_running_cycles;;
} RunningCyclesSummary;

// RunningCyclesSummary: Creation Functions:
RunningCyclesSummary * get_running_cycles_summary(const ProcessTable * process_table);

// RunningCyclesSummary: Destruction Functions:
void destroy_running_cycles_summary(RunningCyclesSummary * running_cycles_summary);

// RunningCyclesSummary: ToString Function:
const char * running_cycles_summary_to_string(const RunningCyclesSummary * running_cycles_summary);


/*=== The BlockedReadsSummary Data Type and its Functions ===*/

typedef struct BlockedReadsSummary {
    unsigned int total_blocked_reads;
    unsigned int min_blocks_till_reading;
    unsigned int max_blocks_till_reading;
    unsigned int avg_blocks_till_reading;
    unsigned int most_blocked_pid;
} BlockedReadsSummary;

// BlockedReadsSummary: Creation Functions:
BlockedReadsSummary * get_blocked_reads_summary(const ProcessTable * process_table);

// BlockedReadsSummary: Destruction Functions:
void destroy_blocked_reads_summary(BlockedReadsSummary * blocked_reads_summary);

// BlockedReadsSummary: ToString Function:
const char * blocked_reads_summary_to_string(const BlockedReadsSummary * blocked_reads_summary);


/*=== The BlockedWritesSummary Data Type and its Functions ===*/

typedef struct BlockedWritesSummary {
    unsigned int total_blocked_writes;
    unsigned int min_blocks_till_writing;
    unsigned int max_blocks_till_writing;
    unsigned int avg_blocks_till_writing;
    unsigned int most_blocked_pid;
} BlockedWritesSummary;

// BlockedWritesSummary: Creation Functions:
BlockedWritesSummary * get_blocked_writes_summary(const ProcessTable * process_table);

// BlockedWritesSummary: Destruction Functions:
void destroy_blocked_writes_summary(BlockedWritesSummary * blocked_writes_summary);

// BlockedWritesSummary: ToString Function:
const char * blocked_writes_summary_to_string(const BlockedWritesSummary * blocked_writes_summary);


/*=== The WaitingCyclesSummary Data Type and its Functions ===*/
typedef struct WaitingCyclesSummary {
    unsigned int total_waiting_cycles;
    unsigned int shortest_event_waiting_time;
    unsigned int longest_event_waiting_time;
    unsigned int avg_event_waiting_time;
    unsigned int longest_waiting_pid;
} WaitingCyclesSummary;

// WaitingCyclesSummary: Creation Functions:
WaitingCyclesSummary * get_wating_cycles_summary(const ProcessTable * process_table);

// WaitingCyclesSummary: Destruction Functions:
void destroy_waiting_cycles_summary(WaitingCyclesSummary * waiting_cycles_summary);

// WaitingCyclesSummary: ToString Function:
const char * waiting_cycles_summary_to_string(const WaitingCyclesSummary * waiting_cycles_summary;

/*=== The ReadsSummary Data Type and its Functions ===*/
typedef struct ReadsSummary {
    unsigned int total_reads;
    unsigned int shortest_reading_time;
    unsigned int longest_reading_time;
    unsigned int avg_reading_time;
} ReadsSummary;

// ReadsSummary: Creation Functions:
ReadsSummary * get_reads_summary(const ProcessTable * process_table);

// ReadsSummary: Destruction Functions:
void destroy_reads_summary(ReadsSummary * reads_summary);

// ReadsSummary: ToString Function:
const char * reads_summary_to_string(const ReadsSummary * reads_summary);


/*=== The WritesSummary Data Type and its Functions ===*/
typedef struct WritesSummary {
    unsigned int total_writes;
    unsigned int shortest_writing_time;
    unsigned int longest_writing_time;
    unsigned int avg_writing_time;;
} WritesSummary;

// WritesSummary: Creation Functions:
WritesSummary * get_writes_summary(const ProcessTable * process_table);

// WritesSummary: Destruction Functions:
void destroy_writes_summary(WritesSummary * writes_summary);

// WritesSummary: ToString Function:
const char * writes_summary_to_string(const WritesSummary * writes_summary);


/*=== The ProcessorCyclesSummary Data Type and its Functions ===*/
typedef struct processorCyclesSummary {
    unsigned int total_processor_cycles;
    unsigned int min_cycles_to_completion;
    unsigned int max_cycles_to_completion;
    unsigned int avg_cycles_to_completion;
} ProcessCyclesSummary;

// ProcessCyclesSummary: Creation Functions:
ProcessCyclesSummary * get_process_cycles_summary(const ProcessTable * process_table);

// ProcessCyclesSummary: Destruction Functions:
void destroy_blocked_reads_summary(ProcessCyclesSummary * process_cycles_summary);

// ProcessCyclesSummary: ToString Function:
const char * process_cycles_summary_to_string(const ProcessCyclesSummary * process_cycles_summary);


/*=== The ProcessStats Data Type and its Functions ===*/
typedef struct processStats {

    unsigned int total_processor_cycles;
    unsigned int min_processor_cycles;
    unsigned int max_processor_cycles;
    unsigned int avg_processor_cycles;
    unsigned int total_blocked_read;
    unsigned int min_blocks;
    unsigned int max_blocks;
    unsigned int avg_blocks;
    unsigned int maxBlockCount;
    unsigned int minBlockCount;
    unsigned int averageBlockCount;
    unsigned int minCycleRuns;
    unsigned int maxCycleRuns;
    unsigned int averageCycleRuns;

    unsigned int oldestPID;
} ProcessStats;
// ProcessTable: Creation Functions:
// ProcessTable: Destruction Functions:
// ProcessTable: Accessor Functions:
// ProcessTable:Boolean Functions:
// ProcessTable: ToString Function:

#endif //PROCESS_STATISTICS_H

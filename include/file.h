//
// Created by banji on 12/10/2024.
//


#ifndef FILE_H
#define FILE_H

#pragma once
#include <stdbool.h>

#define MAX_NAME_LENGTH 20

#define FILE_ID_EXISTS_ERROR_CODE 1010
#define FILE_NAME_EXISTS_ERROR_CODE 2020
#define NULL_FILE_ERROR_CODE 3030

/*=== FileState Enum and Functions ===*/
typedef enum FileState {
    PROCESS_READING_FROM_FILE,
    PROCESS_WRITING_TO_FILE,
    FILE_IS_CLOSED,
    FILE_IS_OPEN,
    FILE_IS_NULL,
    FILE_MEMORY_ALLOCATION_FAILED
} FileState;
// FileState: Destruction Functions:
// NONE
// FileState: Mutator Functions:
// NONE
// FileState: Accessor Functions:
// NONE
// FileState: Boolean Functions:
// NONE
// FileState: ToString Function:
// NONE
// FileState: toString
const char * file_state_to_string (const enum FileState state);

/*=== FileNodeDiscoveryState Enum and Functions ===*/
typedef enum FileNodeDiscoveryState {UNDISCOVERED, DISCOVERED, PROCESSED} FileNodeDiscoveryState;

/*=== The FileSearchMetrics Data Type and Functions ===*/
typedef struct FileSearchMetrics {
    FileNodeDiscoveryState state;
    unsigned int startTime;
    unsigned int finishTime;
} FileSearchMetrics;

// FileSearchMetric: Creation Functions
FileSearchMetrics * initialize_file_search_metrics ();

// FileSearchMetric: Destruction Functions:
// NONE
// FileSearchMetric: Mutator Functions:
// NONE
// FileSearchMetric: Accessor Functions:
// NONE
// FileSearchMetric: Boolean Functions:
// NONE
// FileSearchMetric: ToString Function:
// NONE

/*=== The FileDescriptor Data Type and Functions ===*/
typedef struct FileDescriptor {
    char * name;
    unsigned int id;
    unsigned int megabytes;
    FileState state;
} FileDescriptor;

// FileDescriptor: Creation Functions
FileDescriptor * create_file_descriptor (char * name, unsigned int id, unsigned int megabytes);

// FileDescriptor: Destruction Functions:
void destroy_file_descriptor (FileDescriptor * file_descriptor);

// FileDescriptor: Mutator Functions:
// NONE
// FileDescriptor: Accessor Functions:
// NONE
// FileDescriptor: Boolean Functions:
// NONE

// FileDescriptor: ToString Function:
const char * file_descriptor_to_string (const FileDescriptor * fileMetadata);

/*=== The File Data Type and It's Functions ===*/
typedef struct File {
    FileDescriptor * descriptor;
    FileSearchMetrics * search_metrics;
    struct File * next;
    struct File * previous;
} File;

// File: Creation functions:
File * create_file (char * name, unsigned int id, unsigned int megabytes);

// File: Destruction functions:
void destroy_file (File * file);

// File: Mutator Functions
void set_file_name (File * file, const char * name);

// File: Accessor Functions
unsigned int get_file_id (const File * file);
const char * get_file_name (const File * file);

// File: Boolean Functions
bool is_empty_file (const File * file);
bool files_are_equal (const File * a, const File * b);

// File: ToString Function
const char * file_to_string (const File * file);

/*=== The FileList Data Type and It's Functions ===*/
typedef struct FileList {
    File * head;
    File * tail;
    unsigned int size;
    unsigned int totalMegabytes;
} FileList;

// FileList: Creation functions
FileList * create_file_list (void);

// FileList: Destruction functions:
void destroy_file_list (FileList * file_list);

// FileList: Mutator Functions
bool add_to_file_list (FileList * file_list, File * file);
bool delete_from_file_list (FileList * file_list, const unsigned int file_id);
File * pop_from_file_list (FileList * file_list, const unsigned int file_id);

// FileList: Accessor Functions
File * file_list_name_search (const FileList * file_list, const char * name);
File * file_list_id_search (const FileList * file_list, const unsigned int file_id);
void print_file_list (const FileList * fileList);

// FileList: Boolean Functions
bool is_empty_file_list (const FileList * file_list);

// FileList: ToString Functions:
// NONE

#endif //FILE_H

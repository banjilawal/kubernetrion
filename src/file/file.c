//
// Created by banji on 12/10/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "file.h"

#include "process.h"

// FileState: toString
const char * file_state_to_string (const FileState state) {
    switch (state) {
        case PROCESS_READING_FROM_FILE: return "PROCESS_READING_FROM_FILE";
        case PROCESS_WRITING_TO_FILE: return "PROCESS_WRITING_TO_FILE";
        case FILE_IS_CLOSED: return "FILE_IS_CLOSED";
        case FILE_IS_OPEN: return "FILE_IS_OPEN";
        case FILE_IS_NULL: return "FILE_IS_NULL";
        case FILE_DESCRIPTOR_IS_NULL: return "FILE_DESCRIPTOR_IS_NULL";
        case FILE_MEMORY_ALLOCATION_FAILED: return "FILE_MEMORY_ALLOCATION_FAILED";
        case FILE_DESCRIPTOR_MEMORY_ALLOCATION_FAILED: return "FILE_DESCRIPTOR_MEMORY_ALLOCATION_FAILED";
        default: return "UNKNOWN FILE ERROR";
    }
}

/*=== The FileDescriptor Data Type and Functions ===*/

// FileDescriptor: Creation Functions
FileDescriptor * create_file_descriptor (const char * name, const unsigned int id, const unsigned int megabytes) {
    FileDescriptor * descriptor = (FileDescriptor *) malloc(sizeof(FileDescriptor));
    if (descriptor == NULL) {
        printf("%s", file_state_to_string(FILE_DESCRIPTOR_MEMORY_ALLOCATION_FAILED));
        return NULL;
    }

    if (name == NULL) {
        printf("Name is NULL. File creation failed");
        return NULL;
    }

    *(unsigned int *)&descriptor->id = id;
    descriptor->state = FILE_IS_CLOSED;
    descriptor->name = strdup(name);
    descriptor->megabytes = megabytes;
    return descriptor;
}

// FileDescriptor: Destruction Functions:
void destroy_file_descriptor (FileDescriptor * file_descriptor) {
    free((unsigned int *)file_descriptor->id);
    free(file_descriptor);
}

// FileDescriptor: Mutator Functions:
// NONE

// FileDescriptor: Accessor Functions
unsigned int get_file_descriptor_id (const FileDescriptor * file_descriptor) { return file_descriptor->id; }

const char * file_descriptor_to_string (const FileDescriptor * file_descriptor) {
    if (file_descriptor == NULL) return file_state_to_string(FILE_DESCRIPTOR_IS_NULL);

    const int bufferSize = 512;
    char * string = malloc(bufferSize * sizeof(char));
    if (string == NULL) {
        printf("%s", file_state_to_string(FILE_DESCRIPTOR_MEMORY_ALLOCATION_FAILED));
        exit(0);
    }

    const char * format = "id:%d, name:%s megabytes:%d state:%s";
    snprintf(
        string, bufferSize,
        format, file_descriptor->id,
        file_descriptor->name,
        file_descriptor->megabytes,
        file_state_to_string(file_descriptor->state)
    );
    return string;
}

// FileDescriptor: Boolean Functions:
bool files_descriptors_are_equal (const FileDescriptor * a, const FileDescriptor * b) {
    if (a == NULL || b == NULL) return false;
    return a->id == b->id
        && strcmp(a->name, b->name) == 0
        && a->megabytes == b->megabytes
        && strcmp(a->name, b->name) == 0;
}

/*=== The File Data Type and It's Functions ===*/

// File: Creation functions
File * create_file (const unsigned int id, const char * name, const unsigned int megabytes, Process * writer) {
    File * file = (File *) malloc(sizeof(File));
    if (file == NULL) {
        printf(file_state_to_string(FILE_DESCRIPTOR_MEMORY_ALLOCATION_FAILED));
        exit(0);
    }
    file->descriptor = create_file_descriptor(name, id, megabytes);
    file->next = NULL;
    file->previous = NULL;
    file->writer = writer;
    return file;
}

// File: Destruction functions:
void destroy_file (File * file) {
    if (file == NULL) return;
    file->next->previous = file->previous;
    file->previous->next = file->next;

    file->next = NULL;
    file->previous = NULL;
    destroy_file_descriptor(file->descriptor);
    free(file);
}

// File Mutator Functions
void set_file_name (const File * file, const char * name) { file->descriptor->name = strdup(name); }

bool set_file_reader (const File * file, const Process * process) {
    if (file == NULL) return false;
    if (processes_are_equal(file->reader, process)) return true;
    if (file->reader != NULL) return false;
    set_reading_file(process, file);
    return false;
}

bool set_file_writer (const File * file, const Process * writer, const unsigned int megabytes_to_write) {
    return false;
}

// File: Accessor Functions
unsigned int get_file_id (const File * file) { return get_file_descriptor_id(file->descriptor); }
const char * get_file_name (const File * file) { return file->descriptor->name; }

// File: Boolean Functions
bool is_empty_file (const File * file) {
    if (file == NULL) {
        printf("A null file is empty");
        return true;
    }
    return file->descriptor->megabytes == 0;
}

bool files_are_equal(const File * a, const File * b) {
    if ( &a == &b ) return true;
    if (a == NULL || b == NULL) return false;
    return files_descriptors_are_equal(a->descriptor, b->descriptor);
}

// File: ToString Function
const char * file_to_string (const File * file) {
    if (file == NULL) return file_state_to_string(FILE_IS_NULL);

    const int bufferSize = 1024;
    char * string = (char *) malloc(bufferSize * sizeof(char));
    if (string == NULL) {
        printf("Cannot allocate memory to fileString()");
        exit(0);
    }

    const char * format = "File[%s]";
    snprintf(string, bufferSize, format, file_descriptor_to_string(file->descriptor));
    return string;
}

/*=== The FileList Data Type and It's Functions ===*/

// FileList: Creation functions
FileList * create_file_list (void) {
    FileList * fileList = (FileList *) malloc(sizeof(FileList));
    if (fileList == NULL) {
        printf("Cannot allocate memory to create_file_list()");
        exit(0);
    }
    fileList->head = NULL;
    fileList->tail = NULL;
    fileList->size = 0;
    fileList->totalMegabytes = 0;
    return fileList;
}

// FileList: Destruction functions
void destroy_file_list (FileList * file_list) {
    if (file_list == NULL) return;
    file_list->totalMegabytes = 0;
    free(file_list->head);
    free(file_list->tail);
    free(file_list);
}

// FileList: Mutator Functions
bool add_to_file_list (FileList * file_list, File * file) {
    if (file_list == NULL || file == NULL) return false;
    if (file_list->head == NULL) {
        file_list->head = file;
        file_list->tail = file;
    } else {
        file_list->tail->next = file;
        file->previous = file_list->tail;
        file_list->tail = file;
    }
    file_list->size++;
    file_list->totalMegabytes += file->descriptor->megabytes;
    return true;
}

bool delete_from_file_list (FileList * file_list, const unsigned int file_id) {
    File  *  file = file_list_id_search(file_list, file_id);
    if (file_list == NULL || file == NULL) return false;

    if (file == file_list->head) {
        file_list->head = file->next;
    }
    if (file == file_list->tail) {
        file_list->tail = file->previous;
    }
    if (file->previous != NULL) {
        file->previous->next = file->next;
    }
    file_list->totalMegabytes -= file->descriptor->megabytes;
    file_list->size--;

    free(file->descriptor);
    file->descriptor = NULL;
    free(file);
    file = NULL;
    return true;
}

File * pop_from_file_list (FileList * file_list, const unsigned int file_id) {
    File  *  file = file_list_id_search(file_list, file_id);
    if (file == NULL) return NULL;

    if (file == file_list->head) {
        file_list->head = file->next;
    }
    if (file == file_list->tail) {
        file_list->tail = file->previous;
    }
    if (file->previous != NULL) {
        file->previous->next = file->next;
    }
    file_list->totalMegabytes -= file->descriptor->megabytes;
    file_list->size--;
    return file;
}

// FileList: Accessor Functions
File * file_list_name_search (const FileList * file_list, const char * fileName) {
    if (is_empty_file_list(file_list)) return NULL;
    File * cursor = file_list->head;
    while (cursor != NULL) {
        if (strcmp(cursor->descriptor->name, fileName) == 0) return cursor;
        cursor = cursor->next;
    }
    free(cursor);
    cursor = NULL;
    return NULL;
}

File * file_list_id_search (const FileList * file_list, const unsigned int file_id) {
    if (is_empty_file_list(file_list)) return NULL;
    File * cursor = file_list->head;
    while (cursor != NULL) {
        if (get_file_id(cursor) == file_id) return cursor;
        cursor = cursor->next;
    }
    free(cursor);
    cursor = NULL;
    return NULL;
}

void print_file_list (const FileList * file_list) {
    if (file_list == NULL) {
        printf("NULL File List");
        return;
    }
    File * cursor = file_list->head;
    while (cursor != NULL) {
        printf("%s\n", file_to_string(cursor));
        cursor = cursor->next;
    }
    printf("%d files %d MB\n", file_list->size,file_list->totalMegabytes);
    free(cursor);
    cursor = NULL;
}

// FileList: Boolean Functions
bool is_empty_file_list (const FileList * file_list) {
    return file_list->head == NULL || file_list->tail == NULL || file_list->totalMegabytes == 0;
}

// FileList: ToString Functions:
// NONE



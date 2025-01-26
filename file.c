//
// Created by banji on 12/10/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "file.h"

const char * fileStateToString (const FileState state) {
    switch (state) {
        case READ_FROM: return "READ_FROM";
        case WRITTEN_TO: return "WRITTEN_TO";
        case CLOSED: return "CLOSED";
        default: return "UNKNOWN ERROR";
    }
}

FileDescriptor * createDescriptor (char * name, unsigned int id, unsigned int megabytes) {
    FileDescriptor * descriptor = (FileDescriptor *) malloc(sizeof(FileDescriptor));
    if (descriptor == NULL) {
        printf("Cannot allocate memory to createDescriptor()");
        exit(0);
    }
    descriptor->id = id;
    descriptor->state = CLOSED;
    descriptor->name = strdup(name);
    descriptor->megabytes = megabytes;
    return descriptor;
}

const char * fileMetadataToString (const FileDescriptor * fileMetadata) {
    if (fileMetadata == NULL) return "NULL File Metadata";

    const int bufferSize = 512;
    char * string = (char *) malloc(bufferSize * sizeof(char));
    if (string == NULL) {
        printf("Cannot allocate memory to fileMetadataString()");
        exit(0);
    }

    const char * format = "id:%d, name:%s megabytes:%d state:%s";
    snprintf(
        string, bufferSize,
        format, fileMetadata->id,
        fileMetadata->name,
        fileMetadata->megabytes,
        fileStateToString(fileMetadata->state)
    );
    return string;
}

SearchParams * initSearchParams () {
    SearchParams * params = (SearchParams *) malloc(sizeof(SearchParams));
    if (params == NULL) {
        printf("Cannot allocate memory to initSearchParams()");
        exit(0);
    }
    params->state = UNDISCOVERED;
    params->startTime = 0;
    params->finishTime = 0;
    return params;
}

File * createFile (char * name, unsigned int id, unsigned int megabytes) {
    File * file = (File *) malloc(sizeof(File));
    if (file == NULL) {
        printf("Cannot allocate memory to createFile()");
        exit(0);
    }
    file->descriptor = createDescriptor(name, id, megabytes);
    file->searchParams = initSearchParams();
    file->next = NULL;
    file->prev = NULL;
    return file;
}

unsigned int getFileId (const File * file) {
    return file->descriptor->id;
}

bool areSameFile (const File * a, const File * b) {
    return a->descriptor->id == b->descriptor->id &&a->descriptor->megabytes == b->descriptor->megabytes
        && strcmp(a->descriptor->name, b->descriptor->name) == 0;
}

const char * fileToString (const File * file) {
    if (file == NULL) return "NULL File";

    const int bufferSize = 1024;
    char * string = (char *) malloc(bufferSize * sizeof(char));
    if (string == NULL) {
        printf("Cannot allocate memory to fileString()");
        exit(0);
    }

    const char * format = "File[address:%p %s]";
    snprintf(string, bufferSize, format, (void *) file, fileMetadataToString(file->descriptor));
    return string;
}


FileList * createFileList (void) {
    FileList * fileList = (FileList *) malloc(sizeof(FileList));
    if (fileList == NULL) {
        printf("Cannot allocate memory to createFileList()");
        exit(0);
    }
    fileList->head = NULL;
    fileList->tail = NULL;
    fileList->size = 0;
    fileList->totalMegabytes = 0;
    return fileList;
}

bool isEmptyFileList (const FileList * fileList) {
    return fileList->head == NULL || fileList->tail == NULL || fileList->totalMegabytes == 0;
}

bool addToFileList (FileList * fileList, File * file) {
    if (fileList == NULL || file == NULL) return false;
    if (fileList->head == NULL) {
        fileList->head = file;
        fileList->tail = file;
    } else {
        fileList->tail->next = file;
        file->prev = fileList->tail;
        fileList->tail = file;
    }
    fileList->size++;
    fileList->totalMegabytes += file->descriptor->megabytes;
    return true;
}

bool removeFromFileList (FileList * fileList, const unsigned int fileId) {
    File  *  file = findFileById(fileList, fileId);
    if (fileList == NULL || file == NULL) return false;

    if (file == fileList->head) {
        fileList->head = file->next;
    }
    if (file == fileList->tail) {
        fileList->tail = file->prev;
    }
    if (file->prev != NULL) {
        file->prev->next = file->next;
    }
    fileList->totalMegabytes -= file->descriptor->megabytes;
    fileList->size--;

    free(file->descriptor);
    file->descriptor = NULL;
    free(file);
    file = NULL;
    return true;
}

File * popFromFileList (FileList * fileList, const unsigned int fileId) {
    File  *  file = findFileById(fileList, fileId);
    if (file == NULL) return NULL;

    if (file == fileList->head) {
        fileList->head = file->next;
    }
    if (file == fileList->tail) {
        fileList->tail = file->prev;
    }
    if (file->prev != NULL) {
        file->prev->next = file->next;
    }
    fileList->totalMegabytes -= file->descriptor->megabytes;
    fileList->size--;
    return file;
}

File * findFileById (const FileList * fileList, const unsigned int fileId) {
    if (isEmptyFileList(fileList)) return NULL;
    File * cursor = fileList->head;
    while (cursor != NULL) {
        if (getFileId(cursor) == fileId) return cursor;
        cursor = cursor->next;
    }
    free(cursor);
    cursor = NULL;
    return NULL;
}

File * findFileByName (const FileList * fileList, const char * fileName) {
    if (isEmptyFileList(fileList)) return NULL;
    File * cursor = fileList->head;
    while (cursor != NULL) {
        if (strcmp(cursor->descriptor->name, fileName) == 0) return cursor;
        cursor = cursor->next;
    }
    free(cursor);
    cursor = NULL;
    return NULL;
}

void printfileList (const FileList * fileList) {
    if (fileList == NULL) {
        printf("NULL File List");
        return;
    }
    File * cursor = fileList->head;
    while (cursor != NULL) {
        printf("%s\n", fileToString(cursor));
        cursor = cursor->next;
    }
    printf("%d files %d MB\n", fileList->size, fileList->totalMegabytes);
    free(cursor);
    cursor = NULL;
}




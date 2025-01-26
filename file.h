//
// Created by banji on 12/10/2024.
//
#pragma once
#include <stdbool.h>

#ifndef FILE_H
#define FILE_H
#define MAX_NAME_LENGTH 20

#define FILE_ID_EXISTS_ERROR_CODE 1010
#define FILE_NAME_EXISTS_ERROR_CODE 2020
#define NULL_FILE_ERROR_CODE 3030

typedef enum FileState {READ_FROM, WRITTEN_TO, CLOSED} FileState;
typedef enum DiscoveryState {UNDISCOVERED, DISCOVERED, PROCESSED} DiscoveryState;
const char * fileStateToString (const enum FileState state);

typedef struct SearchParams {
    DiscoveryState state;
    unsigned int startTime;
    unsigned int finishTime;
} SearchParams;

SearchParams * initSearchParams ();

typedef struct FileDescriptor {
    char * name;
    unsigned int id;
    unsigned int megabytes;
    FileState state;
} FileDescriptor;

FileDescriptor * createDescriptor (char * name, unsigned int id, unsigned int megabytes);
const char * fileMetadataToString (const FileDescriptor * fileMetadata);

typedef struct File {
    FileDescriptor * descriptor;
    SearchParams * searchParams;
    struct File * next;
    struct File * prev;
} File;

File * createFile (char * name, unsigned int id, unsigned int megabytes);
unsigned int getFileId (const File * file);
const char * getFileName (const File * file);
void setFileName (File * file, const char * name);
bool isEmptyFile (const File * file);
bool areSameFile (const File * a, const File * b);
const char * fileToString (const File * file);

typedef struct FileList {
    File * head;
    File * tail;
    unsigned int size;
    unsigned int totalMegabytes;
} FileList;

FileList * createFileList (void);
bool isEmptyFileList (const FileList * fileList);
void printfileList (const FileList * fileList);
bool addToFileList (FileList * fileList, File * file);
bool removeFromFileList (FileList * fileList, const unsigned int fileId);
File * popFromFileList (FileList * fileList, const unsigned int fileId);
File * findFileByName (const FileList * fileList, const char * name);
File * findFileById (const FileList * fileList, const unsigned int fileId);

#endif //FILE_H

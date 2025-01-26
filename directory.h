//
// Created by banji on 12/10/2024.
//
#pragma once
#include "file.h"
#include <stdbool.h>

#ifndef DIRECTORY_H
#define DIRECTORY_H
#define MAX_SUB_DIRECTORIES 100

#define NULL_DIRECTORY_ERROR_CODE 4040

typedef struct Directory {
    File * details;
    FileList * files;
    struct Directory * parent;
    struct Directory ** children;
    unsigned int childrenCount;
} Directory;

bool setFileCapacity(Directory * directory, const unsigned int capacity);

Directory * createDirectory (Directory * parent, char * name, unsigned int id);
unsigned int getDirectoryId (const Directory * directory);
const char * getDirectoryName (const Directory * directory);
unsigned int getDirectoryMegabytes (const Directory * directory);
bool addFile (Directory * directory, File * file);
bool removeFile (Directory * directory, const unsigned int fileId);
void moveFile (Directory * source, Directory * destination, const unsigned int fileId);

bool isEmptyDirectory (const Directory * directory);
bool areSameDirectory (const Directory * a, const Directory * b);
const char * directoryToString (const Directory * directory);
const unsigned int getDirectoryChildrenCount (const Directory * directory);

typedef struct DirectoryTree {
    Directory * root;
    unsigned int directoryCount;
} DirectoryTree;

DirectoryTree * createDirectoryTree (Directory * root);
void addDirectory(const DirectoryTree * tree, Directory * directory);
void removeDirectory(const DirectoryTree * tree, Directory * directory);
void moveDirectory (const DirectoryTree * source, const DirectoryTree * destination, const unsigned int directoryId);

File * fileIdSearch (const DirectoryTree * tree, const unsigned int fileId);
File * fileNameSearch (const DirectoryTree * tree, const char * name);
Directory * findDirectoryById (const DirectoryTree * directoryTree, const unsigned int directoryId);
Directory * findDirectoryByName (const DirectoryTree * directoryTree, const char * name);
DirectoryTree * discoverDirectoryTree (Directory * root);

bool isEmptyDirectoryTree (const DirectoryTree * directoryTree);

unsigned int getDirectoryTreeMegaBytes (const DirectoryTree * directoryTree);

const char * directoryTreeToString (const DirectoryTree * directoryTree);
void printDirectoryTree (const DirectoryTree * directoryTree);

#endif //DIRECTORY_H

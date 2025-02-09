//
// Created by banji on 12/10/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "include/file.h"
#include "include/directory.h"

Directory * createDirectory (Directory * parent, char * name, unsigned int id) {
    Directory * directory = (Directory *) malloc(sizeof(Directory));
    if (directory == NULL) {
        printf("Cannot allocate memory to createDirectory()");
        exit(0);
    }
    directory->details = createFile(name, id, 0);
    directory->files = createFileList();
    directory->parent = parent;
    directory->children = (Directory * *) malloc(sizeof(Directory * ) * (MAX_SUB_DIRECTORIES));
    directory->childrenCount = 0;
    return directory;
}

bool setFileCapacity (Directory * directory, const unsigned int capacity) {
    if (directory == NULL) return false;
    if (directory->files == NULL) {
      directory->files = createFileList();
    }
    directory->files->size = capacity;
    return true;
}

unsigned int getDirectoryId (const Directory * directory) {
    return getFileId(directory->details);
}

bool areSameDirectory (const Directory * a, const Directory * b) {
    return areSameFile(a->details, b->details);
}

bool addFile (Directory * directory, File * file) {
  if (directory == NULL) {
    printf("Cannot add file to NULL directory");
    return false;
  }
  if (directory->details == NULL) {
    printf("Directory details is null. Cannot add file to the directory");
    return false;
  }
  if (directory->files == NULL) {
    directory->files = createFileList();
  }
  if (file == NULL) {
    printf("Cannot null file to directory %s", directory->details->descriptor->name);
    exit(NULL_FILE_ERROR_CODE);
  }
  return addToFileList(directory->files, file);
}

bool removeFile (Directory * directory, const unsigned int fileId) {
  if (directory == NULL) return false;
  return removeFromFileList(directory->files, fileId);
}

void moveFile (Directory * source, Directory * destination, const unsigned int fileId) {
    File * file = popFromFileList(source->files, fileId);
    if (file == NULL) return;
    addFile(destination, file);
}

const char * directoryToString (const Directory * directory) {
    if (directory == NULL) return "NULL Directory";
    const int bufferSize = 256;
    char * string = (char *) malloc(bufferSize * sizeof(char));
    if (string == NULL) {
        printf("Cannot allocate memory to directoryToString()");
        exit(0);
    }

    const char * format = "Directory[address:%p id:%d name:%s, total files:%d size:%d MB]";
    snprintf(
        string,
        bufferSize,
        format,
        (void *) directory,
        fileToString(directory->details),
        directory->files->size,
        directory->files->totalMegabytes);
    return string;
}

DirectoryTree * createDirectoryTree (Directory * root) {
    DirectoryTree * directoryTree = (DirectoryTree *) malloc(sizeof(DirectoryTree));
    if (directoryTree == NULL) {
        printf("Cannot allocate memory to createDirectoryTree()");
        exit(0);
    }
    directoryTree->root = root;
    directoryTree->directoryCount = 0;
    return directoryTree;
}

void addDirectory (const DirectoryTree * directoryTree, Directory * directory) {
    if (directoryTree == NULL || directory == NULL) return;
    directory->parent = directoryTree->root;

    if (findDirectoryById(directoryTree, getDirectoryId(directory)) != NULL) {
      printf("Directory with id %d already exists\n", getDirectoryId(directory));
      exit(FILE_ID_EXISTS_ERROR_CODE);
    }

    if (findDirectoryByName(directoryTree, directory->details->descriptor->name) != NULL) {
      printf("Directory with name %s already exists\n", directory->details->descriptor->name);
      exit(FILE_NAME_EXISTS_ERROR_CODE);
    }

    int counter = 0;
    while (counter < directoryTree->root->childrenCount || directoryTree->root->children[counter] != NULL) {
      counter++;
    }
    directoryTree->root->children[counter] = directory;
    directoryTree->root->childrenCount++;
}

void deleteDirectory (const DirectoryTree * directoryTree, const unsigned int directoryId) {
    Directory * directory = findDirectoryById(directoryTree, directoryId);
    if (directory == NULL) return;
    for (int i = 0; i < directoryTree->root->childrenCount; i++) {
      if (getFileId(directoryTree->root->children[i]->details) == directoryId) {
        directoryTree->root->children[i] = NULL;
        directoryTree->root->childrenCount--;
      }
    }
}

Directory * findDirectoryById (const DirectoryTree * directoryTree, const unsigned int directoryId) {
    if (directoryTree == NULL) return NULL;
    return NULL;
}

Directory * findDirectoryByName (const DirectoryTree * directoryTree, const char * directoryName) {
    if (directoryTree == NULL) return NULL;
}

void printDirectoryTree (const DirectoryTree * directoryTree) {
    if (directoryTree == NULL) return;
    Directory * cursor = directoryTree->root;
    while (cursor != NULL) {
        printf("%s\n", directoryToString(cursor));
    }
}

void moveDirectory (const DirectoryTree * source, const DirectoryTree * destination, const unsigned int directoryId) {
    if (source == NULL || destination == NULL) return;
    if (source == destination || areSameDirectory(source->root, destination->root)) return;
    Directory * directory = findDirectoryById(source, directoryId);
    directory->parent = destination->root;
    deleteDirectory(source, directoryId);
    addDirectory(destination, directory);
}





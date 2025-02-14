//
// Created by banji on 12/10/2024.
//
#ifndef DIRECTORY_H
#define DIRECTORY_H

#pragma once
#include <stdbool.h>
#include "file.h"

#define MAX_SUB_DIRECTORIES 100

#define NULL_DIRECTORY_ERROR_CODE 4040

/*=== The DirectoryTree Data Type and It's Functions ===*/
typedef struct Directory {
    File * details;
    FileList * files;
    struct Directory * parent;
    struct Directory ** children;
    unsigned int number_of_children;
} Directory;

// Directory: Creation functions
Directory * create_directory (Directory * parent, char * name, unsigned int id);

// Directory: Destruction functions:
void destroy_directory (Directory * directory);

// Directory: Mutator Functions
bool add_file (Directory * directory, File * file);
bool remove_file (Directory * directory, const unsigned int file_id);
bool move_file (Directory * source, Directory * destination, unsigned int file_id);
bool set_directory_capacity(Directory * directory, const unsigned int capacity);

// Directory: Accessor Functions
unsigned int get_directory_by_id (const Directory * directory);
const char * get_directory_by_name (const Directory * directory);
unsigned int get_directory_megabytes (const Directory * directory);

unsigned int get_number_of_subdirectories(const Directory *directory);

// Directory: Boolean Functions
bool is_empty_directory (const Directory * directory);
bool directories_are_equal (const Directory * a, const Directory * b);

// Directory: ToString Functions:
const char * directory_to_string (const Directory * directory);

/*=== The Directory Data Type and It's Functions ===*/
typedef struct DirectoryTree {
    Directory * root;
    unsigned int directoryCount;
} DirectoryTree;

// DirectoryTree: Creation functions
DirectoryTree * create_directory_tree (Directory * root);

// DirectoryTree: Destruction functions:
void destroy_directory_tree (DirectoryTree * directory_tree);

// DirectoryTree: Mutator Functions
void add_directory_to_tree(const DirectoryTree* directory_tree, Directory * directory);
void delete_directory_from_tree(const DirectoryTree* directory_tree, const unsigned int directory_id);
bool move_directory_to_tree (const DirectoryTree * source, const DirectoryTree * destination, const unsigned int directory_id);

// DirectoryTree: Accessor Functions
unsigned int get_directory_tree_mega_bytes (const DirectoryTree* directory_tree);
File * directory_tree_file_id_search (const DirectoryTree* directory_tree, const unsigned int file_id);
File * directory_tree_file_name_search (const DirectoryTree* directory_tree, const char * file_name);

Directory * find_directory_by_id (const DirectoryTree* directory_tree, const unsigned int directory_id);
Directory * find_directory_by_name(const DirectoryTree* directory_tree, const char * directory_name);

DirectoryTree * directory_tree_dfs (Directory * root);
DirectoryTree * directory_tree_bfs (Directory * root);
void print_directory_tree (const DirectoryTree* directory_tree);

// DirectoryTree: Boolean Functions
bool is_empty_directory_tree (const DirectoryTree* directory_tree);

// DirectoryTree: ToString Functions:
const char* directory_tree_to_string (const DirectoryTree* directory_tree);

#endif //DIRECTORY_H

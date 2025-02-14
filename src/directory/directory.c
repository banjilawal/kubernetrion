//
// Created by banji on 12/10/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "file.h"
#include "directory.h"

/*=== The Directory Data Type and It's Functions ===*/

// Directory: Creation functions
Directory * create_directory (Directory * parent, char * name, unsigned int id) {
    Directory * directory = (Directory *) malloc(sizeof(Directory));
    if (directory == NULL) {
        printf("Cannot allocate memory to create_directory()");
        exit(0);
    }
    directory->details = create_file(id, name, 0, NULL);
    directory->files = create_file_list();
    directory->parent = parent;
    directory->children = (Directory * *) malloc(sizeof(Directory * ) * (MAX_SUB_DIRECTORIES));
    directory->number_of_children = 0;
    return directory;
}

// Directory: Destruction functions:
void destroy_directory (Directory * directory) {
    destroy_file_list(directory->files);
    free(directory);
}

// Directory: Mutator Functions
bool add_file (Directory * directory, File * file) {
    if (directory == NULL) {
        printf("Cannot add file to NULL directory");
        return false;
    }
    if (directory->details == NULL) {
        printf("Directory details is null. Cannot add file to the directory");
        return false;
    }
    if (directory->files == NULL) {
        directory->files = create_file_list();
    }
    if (file == NULL) {
        printf("Cannot null file to directory %s", directory->details->descriptor->name);
        exit(NULL_FILE_ERROR_CODE);
    }
    return add_to_file_list(directory->files, file);
}

bool remove_file (Directory * directory, const unsigned int fileId) {
    if (directory == NULL) return false;
    return delete_from_file_list(directory->files, fileId);
}

bool move_file (Directory * source, Directory * destination, const unsigned int fileId) {
    File * file = pop_from_file_list(source->files, fileId);
    if (file == NULL) return false;
    return add_file(destination, file);
}

bool set_directory_capacity (Directory * directory, const unsigned int capacity) {
    if (directory == NULL) return false;
    if (directory->files == NULL) {
        directory->files = create_file_list();
    }
    directory->files->size = capacity;
    return true;
}

// Directory: Accessor Functions
unsigned int get_directory_id (const Directory * directory) {
    return get_file_id(directory->details);
}

const char * get_directory_name (const Directory * directory) {
    return directory->details->descriptor->name;
}
unsigned int get_directory_megabytes (const Directory * directory) {
    return directory->files->size;
}

unsigned int get_number_of_subdirectories(const Directory *directory) {
    return directory->number_of_children;
}

// Directory: Boolean Functions
bool is_empty_directory (const Directory * directory) {
    if (directory == NULL) return false;
    return directory->number_of_children == 0 && directory->files->size == 0;
}

bool directories_are_equal (const Directory * a, const Directory * b) {
    if (a == NULL || b == NULL) return false;
    return files_are_equal(a->details, b->details);
}

// Directory: ToString Functions:
const char * directory_to_string (const Directory * directory) {
    if (directory == NULL) return "NULL Directory";
    const int bufferSize = 256;
    char * string = (char *) malloc(bufferSize * sizeof(char));
    if (string == NULL) {
        printf("Cannot allocate memory to directory_to_string()");
        exit(0);
    }

    const char * format = "Directory[address:%p id:%d name:%s, total files:%d size:%d MB]";
    snprintf(
        string,
        bufferSize,
        format,
        (void *) directory,
        file_to_string(directory->details),
        directory->files->size,
        directory->files->totalMegabytes);
    return string;
}

/*=== The DirectoryTree Data Type and It's Functions ===*/

// DirectoryTree: Creation functions
DirectoryTree * create_directory_tree (Directory * root) {
    DirectoryTree * directoryTree = (DirectoryTree *) malloc(sizeof(DirectoryTree));
    if (directoryTree == NULL) {
        printf("Cannot allocate memory to create_directory_tree()");
        exit(0);
    }
    directoryTree->root = root;
    directoryTree->directoryCount = 0;
    return directoryTree;
}

// DirectoryTree: Destruction functions:
void destroy_directory_tree(DirectoryTree * directory_tree) {
    if (directory_tree == NULL) return;
    free(directory_tree->root);
}

// DirectoryTree: Mutator Functions
void add_directory_to_tree (const DirectoryTree * directory_tree, Directory * directory) {
    if (directory_tree == NULL || directory == NULL) return;
    directory->parent = directory_tree->root;

    const unsigned int directory_id = get_directory_id(directory);
    if (find_directory_by_id(directory_tree, directory_id) != NULL) {
        printf("Directory with id %d already exists\n", directory_id);
        exit(FILE_ID_EXISTS_ERROR_CODE);
    }

    if (find_directory_by_name(directory_tree, directory->details->descriptor->name) != NULL) {
        printf("Directory with name %s already exists\n", directory->details->descriptor->name);
        exit(FILE_NAME_EXISTS_ERROR_CODE);
    }

    int counter = 0;
    while (counter < directory_tree->root->number_of_children || directory_tree->root->children[counter] != NULL) {
        counter++;
    }
    directory_tree->root->children[counter] = directory;
    directory_tree->root->number_of_children++;
}

void delete_directory_from_tree (const DirectoryTree * directory_tree, const unsigned int directory_id) {
    Directory * directory = find_directory_by_id(directory_tree, directory_id);
    if (directory == NULL) return;
    for (int i = 0; i < directory_tree->root->number_of_children; i++) {
        if (get_file_id(directory_tree->root->children[i]->details) == directory_id) {
            directory_tree->root->children[i] = NULL;
            directory_tree->root->number_of_children--;
        }
    }
}

bool move_directory_to_tree (const DirectoryTree * source, const DirectoryTree * destination, unsigned int directory_id) {
    if (source == NULL || destination == NULL) return false;
    if (source == destination || directories_are_equal(source->root, destination->root)) return false;

    Directory * directory = find_directory_by_id(source, directory_id);
    if (directory == NULL) {
        printf("Directory with id %d does not exist. move operation failed\n", directory_id);
        return false;
    }
    if (directory->parent != NULL) {
        delete_directory_from_tree(source, directory_id);
    }
    add_directory_to_tree(destination, directory);
    return true;
}

// DirectoryTree: Accessor Functions
unsigned int get_directory_tree_mega_bytes (const DirectoryTree* directory_tree) {
    return directory_tree->directoryCount * sizeof(Directory);
}

File * directory_tree_file_id_search (const DirectoryTree* directory_tree, const unsigned int file_id) {
    return NULL;
}

File * directory_tree_file_name_search (const DirectoryTree* directory_tree, const char * file_name) {
    return NULL;
}


Directory * find_directory_by_id (const DirectoryTree * directory_tree, const unsigned int directory_id) {
    if (directory_tree == NULL) return NULL;
    return NULL;
}

Directory * find_directory_by_name (const DirectoryTree * directory_tree, const char * directory_name) {
    if (directory_tree == NULL) return NULL;
    return NULL;
}

void print_directory_tree (const DirectoryTree * directoryTree) {
    if (directoryTree == NULL) return;
    Directory * cursor = directoryTree->root;
    while (cursor != NULL) {
        printf("%s\n", directory_to_string(cursor));
    }
}
DirectoryTree * directory_tree_dfs (Directory * root) { return NULL; }
DirectoryTree * directory_tree_bfs (Directory * root) { return NULL; }


// DirectoryTree: Boolean Functions
bool is_empty_directory_tree (const DirectoryTree* directory_tree) { return false; }

// DirectoryTree: ToString Functions:
const char* directory_tree_to_string (const DirectoryTree* directory_tree) { return get_directory_name(directory_tree->root); }









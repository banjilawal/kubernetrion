//
// Created by griot on 2/10/25.
//

#ifndef FILE_B_TREE_H
#define FILE_B_TREE_H

#pragma once
#include "file.h"
#include <stdbool.h>
#include <stdio.h>

/*=== The FileBTreeNode Data Type and its Functions ===*/

typedef struct FileBTreeNode {
    File * parent;
    struct FileBTreeNode *left_child;
    struct FileBTreeNode *right_child;
} FileBTreeNode;

// FileBTreeNode: Destruction Functions:
FileBTreeNode * create_file_b_tree_node(const File * parent);

// FileBTree Mutator Functions:
void destroy_file_b_tree_node(const FileBTreeNode * file_b_tree_node);

// FileBTreeNode: Accessor Functions:
// NONE
// FileBTreeNode Boolean Functions:
// NONE
// ProcessQueue: ToString Function:
const char * file_b_tree_node_to_string(const FileBTreeNode * file_b_tree_node);

#endif //FILE_B_TREE_H

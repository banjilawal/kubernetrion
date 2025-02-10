//
// Created by griot on 2/10/25.
//
#pragma once

#include "directory.h"

#ifndef DIRECTORY_B_TREE_H
#define DIRECTORY_B_TREE_H

/*=== The DirectoryBTreeNode Data Type and its Functions ===*/

typedef struct DirectoryBTreeNode {
  Directory * parent;
  struct DirectoryBTreeNode *left_child;
  struct DirectoryBTreeNode *right_child;
} DirectoryBTreeNode;

// DirectoryBTreeNode: Destruction Functions:
DirectoryBTreeNode * create_directory_b_tree_node(const Directory * saource);

// DirectoryBTreeNode: Mutator Functions:
void destroy_directory_b_tree_node(const DirectoryBTreeNode * directory_b_tree_node);

// DirectoryBTreeNode: Accessor Functions:
// NONE
// DirectoryBTreeNode Boolean Functions:
// NONE
// ProcessQueue: ToString Function:
const char * directory_b_tree_node_to_string(const DirectoryBTreeNode * directory_b_tree_node);

#endif //DiIRECTORY_B_TREE_H

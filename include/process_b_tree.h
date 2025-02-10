//
// Created by griot on 2/10/25.
//
#pragma once

#include "process.h"

#ifndef PROCESS_B_TREE_H
#define PROCESS_B_TREE_H

/*=== The ProcessQueue Data Type and its Functions ===*/

typedef struct ProcessBTreeNode {
    Process *process;
    struct ProcessBTreeNode *left_child;
    struct ProcessBTreeNode *right_child;
} ProcessBTreeNode;

// ProcessBTreeNode: Destruction Functions:
ProcessBTreeNode * create_process_b_tree_node(const Process * parent);

// ProcessBTreeNode: Mutator Functions:
void destroy_process_b_tree_node(const ProcessBTreeNode * process_b_tree_node);

// ProcessBTreeNode: Accessor Functions:
// NONE
// ProcessBTreeNode Boolean Functions:
// NONE
// ProcessQueue: ToString Function:
const char * process_b_tree_node_to_string(const ProcessBTreeNode * process_b_tree_node);

#endif //PROCESS_B_TREE_H

//
// Created by griot on 2/11/25.
//

#pragma once
#include "graph_vertex.h"

#include <stdio.h>
#include <stdlib.h>


const char * graph_vertex_state_to_string (const GraphVertexState state) {
    switch (state) {
        case UNDISCOVERED: return "UNDISCOVERED";
        case DISCOVERED: return "DISCOVERED";
        case PROCESSED: return "PROCESSED";
        case GRAPH_VERTEX_IS_NULL: return "GRAPH_VERTEX_IS_NULL";
        case GRAPH_VERTEX_MEMORY_ALLOCATION_FAILED: return "GRAPH_VERTEX_MEMORY_ALLOCATION_FAILED";
    }
}

/*=== The GraphVertex Data Type and Functions ===*/

// FileSearchMetric: Creation Functions
GraphVertex * create_graph_vertex (const void * data, const unsigned int data_size, const GraphVertex * parent, const GraphVertex * child) {
    GraphVertex * vertex = (GraphVertex *) malloc(sizeof(GraphVertex) + data_size);
    if (vertex == NULL) {
        printf("%s", graph_vertex_state_to_string(GRAPH_VERTEX_MEMORY_ALLOCATION_FAILED));
        exit(0);
    }
    vertex->data = data;
    vertex->child = child;
    vertex->predecessor = parent;
    vertex->state = UNDISCOVERED;
    vertex->startTime = 0;
    vertex->finishTime = 0;
    return vertex;
}

// FileSearchMetric: Destruction Functions:
// NONE
// FileSearchMetric: Mutator Functions:
// NONE
// FileSearchMetric: Accessor Functions:
// NONE
// FileSearchMetric: Boolean Functions:
// NONE

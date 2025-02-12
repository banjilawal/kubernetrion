//
// Created by griot on 2/11/25.
//

#ifndef GRAPH_VERTEX_H
#define GRAPH_VERTEX_H
typedef enum GraphVertexState {
    UNDISCOVERED,
    DISCOVERED,
    PROCESSED,
    GRAPH_VERTEX_IS_NULL,
    GRAPH_VERTEX_MEMORY_ALLOCATION_FAILED
} GraphVertexState;
const char * graph_vertex_state_to_string(GraphVertexState );

/*=== The GraphVertex Data Type and Functions ===*/
typedef struct GraphVertex {
    void *data;
    GraphVertexState state;
    unsigned int startTime;
    unsigned int finishTime;
    struct GraphVertex * predecessor;
    struct GraphVertex * child;
} GraphVertex;

#endif //GRAPH_VERTEX_H

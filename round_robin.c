
#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "round_robin.h"

RoundRobin * createRoundRobin () {
  RoundRobin * roundRobin = (RoundRobin *) malloc(sizeof(RoundRobin));
  if (roundRobin == NULL) {
    printf("%s\n", errorStateToString(PROCESS_QUEUE_IS_NULL));
    return NULL;
  }
  roundRobin->queue = createProcessQueue();
  return roundRobin;
}

static bool enqueue (RoundRobin * roundRobin, Process * process) {
  if (roundRobin == NULL || roundRobin->queue == NULL) {
    printf("%s\n", errorStateToString(PROCESS_QUEUE_IS_NULL));
    return false;
  }
  if (process == NULL) {
    printf("%s\n", errorStateToString(PROCESS_IS_NULL));
    return false;
  }
  addProcessQueueNode(roundRobin->queue, createProcessQueueNode(process));
  return true;
}

static Process * dequeue (RoundRobin * roundRobin) {
    if (roundRobin == NULL || roundRobin->queue == NULL) {
        printf("%s\n", errorStateToString(PROCESS_QUEUE_IS_NULL));
        return NULL;
    }
    if (roundRobin->queue->state == EMPTY) {
        printf("%s\n", errorStateToString(PROCESS_QUEUE_IS_EMPTY));
        return NULL;
    }
    return popProcess(roundRobin->queue);
}

#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

RoundRobin * createRoundRobin () {
  RoundRobin * roundRobin = (RoundRobin *) malloc(sizeof(RoundRobin));
  if (roundRobin == NULL) {
    printf("%s\n", errorStateToString(PROCESS_QUEUE_IS_NULL));
    return NULL;
  }
  roundRobin->queue = createProcessQueue();
  return roundRobin;
}

bool addToRoundRobinQueue (const RoundRobin * roundRobin, Process * process) {
  if (roundRobin == NULL || queueIsEmpty(roundRobin->queue)) {
    printf("%s\n", errorStateToString(PROCESS_QUEUE_IS_NULL));
    return false;
  }
  if (process == NULL) {
    printf("%s\n", errorStateToString(PROCESS_IS_NULL));
    return false;
  }
  addProcess(roundRobin->queue, process);
  return true;
}

Process * popRoundRobin (const RoundRobin * roundRobin) {
    if (roundRobin == NULL || roundRobin->queue == NULL) {
        printf("%s\n", errorStateToString(PROCESS_QUEUE_IS_NULL));
        return NULL;
    }
    if (roundRobin->queue->state == IS_EMPTY) {
        printf("%s\n", processQueueStateToString(IS_EMPTY));
        return NULL;
    }
    return popProcess(roundRobin->queue);
}

PriorityQueue * createPriorityQueue () {
  PriorityQueue * priorityQueue = (PriorityQueue *) malloc(sizeof(PriorityQueue));
  if (priorityQueue == NULL) {
    printf("%s\n", errorStateToString(PROCESS_QUEUE_IS_NULL));
    return NULL;
  }
  priorityQueue->queue = createProcessQueue();
  return priorityQueue;
}

bool addToPriorityQueue (const PriorityQueue * priorityQueue, Process * process) {
  if (priorityQueue == NULL || priorityQueue->queue == NULL) {
    printf("%s\n", errorStateToString(PROCESS_QUEUE_IS_NULL));
    return false;
  }
  if (process == NULL) {
    printf("%s\n", errorStateToString(PROCESS_IS_NULL));
    return false;
  }
  if (queueIsEmpty(priorityQueue->queue) || priorityQueue->queue->tail->process->priority >= process->priority) {
    addProcess(priorityQueue->queue, process);
    return true;
  }
  ProcessQueueNode * node = createProcessQueueNode(process);
  if (node == NULL) {
    printf("%s\n", errorStateToString(PROCESS_QUEUE_NODE_IS_NULL));
    return false;
  }
  ProcessQueueNode * cursor = priorityQueue->queue->tail;
  if (cursor == NULL) {
    printf("%s\n", errorStateToString(PROCESS_QUEUE_NODE_IS_NULL));
    return false;
  }
  while (cursor != NULL) {
    if (cursor->process->priority > process->priority) {
      node->next = cursor;
      node->previous = cursor->previous;
      cursor->previous = node;

      priorityQueue->queue->size++;
      return true;
    }
    cursor = cursor->next;
  }
  return false;
}

Process * popPriorityQueue (const PriorityQueue * priorityQueue) {
  if (priorityQueue == NULL || priorityQueue->queue == NULL) {
    printf("%s\n", processQueueStateToString(PROCESS_QUEUE_IS_NULL));
    return NULL;
  }
  if (priorityQueue->queue->state == IS_EMPTY) {
    printf("%s\n", processQueueStateToString(IS_EMPTY));
    return NULL;
  }
  return popProcess(priorityQueue->queue);
}
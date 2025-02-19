
 #include <stdio.h>
 #include <stdlib.h>
 #include <time.h>
 #include <unistd.h>

#include "process.h"
#include "processor.h"
#include "process_list.h"
#include "process_generator.h"

// #define MAX_PROCESS_NAME_LENGTH 10
//  #define BUFFER_SIZE 1024
//  #define MAX_CHILDREN 5
#define PROCESS_QUEUE_SIZE 5

 // unsigned int STARTING_PROCESS_NUMBER = 0;
 unsigned int lastId = 0;
 unsigned int CYCLES = 0;


int main(int argc, char *argv[]) {
    srand(time(NULL));

    Process *process = random_process(NULL);
    ProcessLinkedList *process_list = create_process_list();
    insert_process_at_list_head(process_list, process);
    printf("%d processes in list\n", process_list->size);
    print_process_list(process_list);

    ProcessQueue *ready_queue = create_process_queue("ready_queue");
    ProcessQueue *waiting_queue = create_process_queue("waiting_queue");
    ProcessQueue *blocked_queue = create_process_queue("blocked_queue");

    for (int i = 0; i < 20; i++) {
        process = random_process(NULL);
        insert_process_at_list_tail(process_list, process);
        printf("added process %s id:%d\n", process->name, process->id);
    }
    printf("%d processes in the list\n", process_list->size);
    // print_process_list(process_list);

    const Process *search_result = search_process_list(process_list, 5);
    if (search_result == NULL) {
        printf("no process found\n");
    } else {
        printf("found process %s pid:%d\n", search_result->name, search_result->id);
    }

    printf("dequeued %s\n", process_to_string(dequeue_from_process_list(process_list)));
    const unsigned int deletion_count = process_list->size;
    printf("%d deletions to the list\n", deletion_count);
    for (int i = 0; i < deletion_count; i++) {
        process = dequeue_from_process_list(process_list);
        if (process->state == PROCESS_READY) {
            printf("%s %d in ready queue \n", process->name, process->id);
            push_onto_process_queue(ready_queue, process);
        } else if (process->state == PROCESS_WAITING_EVENT) {
            printf("%s %d in waiting queue \n", process->name, process->id);
            push_onto_process_queue(waiting_queue, process);
        } else if (process->state == PROCESS_READING_FILE_BLOCKED || process->state == PROCESS_WRITING_FILE_BLOCKED) {
            printf("%s %d in blocked queue \n", process->name, process->id);
            push_onto_process_queue(blocked_queue, process);
        } else {
            insert_process_at_list_tail(process_list, process);
            printf("%d requeued %s\n",i, process->name);
        }
    }
    printf("process_list size:%d\n", process_list->size);
    printf("%s size:%d\n", ready_queue->name, ready_queue->processes->size);
    printf("%s size:%d\n", blocked_queue->name, blocked_queue->processes->size);
    printf("%s size:%d\n", waiting_queue->name, waiting_queue->processes->size);

    // printf("\n-------------------------------------------------------\n");

    return(0);
}

//
// Created by griot on 1/31/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "process.h"
#include "process_generator.h"
#include "process_names.h"
#include "process_queue.h"

unsigned int nextId = 1;

unsigned int random_priority() {
    return (rand() % 100);
}

unsigned int random_milliseconds() {
    return (rand() % (MAX_MILLISECONDS - MIN_MILLISECONDS)) + MIN_MILLISECONDS;
}

const char * random_process_name () {
    unsigned int index= rand() % (PROCESS_NAME_COUNT - 1);
    printf("index: %u\n", index);
    const char * name =  process_names[index];
    printf("%s name: %s\n", process_names[index], name);
    return name;
}

ProcessState random_process_state () {
    const int outcome = rand() % 100 + 1;
    if (outcome < 15) return PROCESS_READY;
    else if (outcome < 30) return PROCESS_WAITING_EVENT;
    else if (outcome < 45) return PROCESS_WRITING_FILE_BLOCKED;
    else if (outcome < 60) return PROCESS_READING_FILE_BLOCKED;
    else if (outcome < 75) return PROCESS_READING_FILE;
    else return PROCESS_WRITING_FILE;
}

Process * random_process(Process * parent) {
    const unsigned int id = nextId;
    nextId++;

    const char *name = random_process_name();
    const unsigned int priority = random_priority();
    const unsigned int milliseconds_remaining = random_milliseconds();
    File *file = NULL;
    Process *child = NULL;

    Process * process = create_process(id, name, parent, child, file, priority, milliseconds_remaining);
    process->state = random_process_state();
    return process;
}

ProcessQueue * generate_process_queue (const unsigned int number_of_processes) {
    ProcessQueue * queue = create_process_queue();

    for (int i = 0; i < number_of_processes; i++) {
        Process * process = random_process(NULL);
        enter_process_queue(queue, random_process(NULL));
    }
    return queue;
}
//
// int main(void) {
//     srand(time(NULL));
//     Process * root = random_process(NULL);
//     Process * child = random_process(root);
//     printf("generated process:%s\n", process_to_string(root));
//     printf("generated process:%s\n", process_to_string(child));
//
//     Process * previous = child;
//     for (int i = 0; i < 10; i++) {
//         Process * next = random_process(previous);
//         if (i % 2 == 0) {
//             previous = next;
//         } else { previous = random_process(NULL); }
//         printf("%d\n\tPREVIOUS:%s\n\tNEXT:%s\n", i, process_to_string(previous), process_to_string(next));
//     }
//     destroy_process(root);
//     return 0;
// }
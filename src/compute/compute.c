
 #include <stdio.h>
 #include <stdlib.h>
 #include <time.h>
 #include <unistd.h>

#include "process_queue.h"
#include "process_generator.h"

// #define NAME_LENGTH 10
//  #define BUFFER_SIZE 1024
//  #define MAX_CHILDREN 5

 // unsigned int STARTING_PROCESS_NUMBER = 0;
 unsigned int lastId = 0;
 unsigned int CYCLES = 0;

int main(void) {
    srand(time(NULL));
    Process * parent = NULL;
    ProcessQueue * process_queue = create_process_queue();
    for (int i = 0; i < 32; i++) {
        Process * process = random_process(NULL);
        // printf("name:%s ID:%d\n", process->name, process->id);
        process_to_string(process);
        join_process_queue(process_queue, process);
        // if (i % 6 == 0) { parent = process; }
    }
    // print_process_queue(process_queue);

    free(process_queue);
    return 0;
}

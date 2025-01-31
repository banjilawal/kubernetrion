//
// #include <stdio.h>
// #include <stdlib.h>
// #include <time.h>
// #include <unistd.h>
//
// #define NAME_LENGTH 10
// #define BUFFER_SIZE 1024
// #define MAX_CHILDREN 5
//
// unsigned int STARTING_PROCESS_NUMBER = 0;
// unsigned int lastId = 0;
// unsigned int CYCLES = 0;
//
// enum ProcessState {READY, RUNNING, WAITING, FREE};
// enum QueueState { NOT_EMPTY, EMPTY};
//
//
// typedef struct Process {
//     unsigned int id;
//     char* name;
//     enum ProcessState state;
//     float remainingExecutionTime;
//     unsigned int numberOfCycles;
// } Process;
//
// typedef struct Node {
//     Process* process;
//     struct Node* next;
//     struct Node* prev;
// } Node;
//
// typedef struct Queue {
//     Node* head;
//     Node* tail;
//     int size;
// } LinkedList;
//
// static Process* createProcess (enum ProcessState state, const float totalExecutionTime) { //(const char* name, const enum State state, const float totalExecutionTime) {
//     Process* process = (Process*) malloc(sizeof(Process));
//     if (process == NULL) return NULL;
//
//     process->name = (char*) malloc(NAME_LENGTH * sizeof(char));
//     if (process->name == NULL) {
//         free(process);
//         return NULL;
//     }
//     const int id = lastId++;;
//     snprintf(process->name, NAME_LENGTH, "PID-%d", id);
//     process->id = id;
//     process->state = state;
//     process->remainingExecutionTime = totalExecutionTime;
//     process->numberOfCycles = 0;
//     return process;
// }
//
// Node* createNode(Process * process) {
//     Node* node = (Node*)malloc(sizeof(Node));
//     if (node == NULL) return NULL;
//     node->next = NULL;
//     node->prev = NULL;
//     node->process = process;
// }
//
// LinkedList* createLinkedList() {
//     LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));
//     if (list == NULL) return NULL;
//     list->head = NULL;
//     list->tail = NULL;
//     list->size = 0;
//     return list;
// }
//
// enum QueueState isEmpty (const LinkedList* list) {
//     if (list == NULL || list->size == 0 || list->head == NULL) return 1;
//     else return 0;
// }
//
// void enqueue (LinkedList* list, Node* node) {
//     if (list == NULL) return;
//     if (isEmpty(list) == 1) {
//         list->head = node;
//         list->tail = node;
//     } else {
//         node->prev = list->tail;
//         list->tail->next = node;
//         list->tail = node;
//     }
//     list->size++;
// }
//
// Process* dequeue (LinkedList* list) {
//     Process * process = NULL;
//     if (isEmpty(list) == 1) {
//         printf("The list is empty. Nothing to remove.\n");
//         return NULL;
//     }
//     Node *node = list->head;
//     list->head = list->head->next;
//     node->next = NULL;
//     process = node->process;
//
//     if (isEmpty(list) == 1) {
//         list->tail = NULL;
//         list->head = NULL;
//     }
//     list->size--;
//     free(node);
//     return process;
// }
//
// // const Process* peek (const LinkedList* list) {
// //     if (list == NULL || list->size == 0 || list->head == NULL || isEmpty(list)) return NULL;
// //     return list->head->process;
// // }
//
// static const char * randomName () {
//     const char *names[128] = {
//         "explorer.exe", "svchost.exe", "cmd.exe", "notepad.exe", "chrome.exe", "msedge.exe", "firefox.exe", "powershell.exe",
//         "winlogon.exe", "csrss.exe","lsass.exe", "taskhost.exe", "dwm.exe", "rundll32.exe", "services.exe","smss.exe",
//         "SearchIndexer.exe", "spoolsv.exe", "wmiprvse.exe", "audiodg.exe","msiexec.exe", "SystemSettings.exe", "Taskmgr.exe",
//         "ShellExperienceHost.exe", "svchost_network.exe","RuntimeBroker.exe", "mstsc.exe", "SearchProtocolHost.exe", "dllhost.exe",
//         "conhost.exe","init", "systemd", "sshd", "cron", "nginx","apache2", "mysqld", "docker", "Xorg", "pulseaudio","cupsd", "dhclient",
//         "NetworkManager", "rsyslogd", "gdm","snapd", "dbus-daemon", "udisksd", "gvfsd", "gvfsd-metadata","modem-manager", "polkitd",
//         "bluetoothd", "kworker", "thermald","python", "java", "node", "perl", "ruby","gcc", "make", "cmake", "git", "vim","emacs", "nano",
//         "code", "subl", "gimp","blender", "ffmpeg", "obs", "htop", "iotop","strace", "lsof", "netstat", "ping", "wget","traceroute",
//         "scp", "rsync", "mount", "umount","parted", "fdisk", "mkfs", "e2fsck", "chkconfig","service", "systemctl", "journalctl", "kill",
//         "pkill","sleep", "uptime", "ps", "jobs", "fg","bg", "killall", "nohup", "tmux", "watch","locate", "find", "grep", "sed", "awk",
//         "xfce4-session", "lightdm", "sddm", "plymouth", "udisksctl","mount.ntfs", "udevd", "syslogd", "rsyslog", "acpid","cron.daily",
//         "iptables", "firewalld", "auditd", "systemd-journald"
//     };
//     return names[rand() % 127];
// }
//
// const char * printState (const enum ProcessState state) {
//     switch (state) {
//         case FREE: return "FREE";
//         case READY: return "READY";
//         case RUNNING: return "RUNNING";
//         case WAITING: return "WAITING";
//         default: return "UNKNOWN";
//     }
// }
//
// static float randomTime () {
//     return (rand() % 10) + 1.00;
// }
//
// static const char * processToString (const Process* process) {
//     const int bufferSize = 128;
//     char* string = malloc(bufferSize);
//     if (string == NULL) {
//         printf("Could not assign memory to create process toString\n");
//         return NULL;
//     }
//     snprintf(
//         string,
//         bufferSize,
//         "Process[ID:%d Name:%s State:%s TotalExecutionTime:%.2f]",
//         process->id,
//         process->name,
//         printState(process->state),
//         process->remainingExecutionTime
//     );
//     return string;
// }
//
// static Process * randomProcess () {
//     return createProcess(randomName(), randomTime()); // createProcess(randomName(), randomState(), totalExecutionTime());
// }
//
// static LinkedList * randomLinkedList (const unsigned int size) {
//     LinkedList* list = createLinkedList();
//     for (int i = 0; i < size; i++) {
//         enqueue(list, createNode(randomProcess()));
//     }
//     return list;
// }
//
// static void printProcess (const Process * process) {
//     printf("Process[ID:%d Name:%s State:%s TotalExecutionTime:%.2f]",
//         process->id,
//         process->name,
//         printState(process->state),
//         process->remainingExecutionTime);
// }
//
// void printNode (Node * node) {
//     if (node == NULL) return;
//     const char * processString = processToString(node->process);
//     printf("Node[PREVIOUS_ADDRESS:%p ADDRESS:%p PROCESS:%s NEXT_ADDRESS:%p\n",(void *) node->prev, (void *) node, processString, (void *) node->next);
// }
//
// void printQueue (const LinkedList* list) {
//     // Node* cursor = (Node*) malloc(sizeof(Node));
//     if (list == NULL || isEmpty(list) == 1) return;
//     Node* cursor = list->head;
//     int counter = 0;
//
//     while (cursor != NULL) {
//         printf("%d ", counter);
//         printf("%s\n", processToString(cursor->process));
//         // printNode(cursor);
//         cursor = cursor->next;
//         counter++;
//     }
//     printf("QUEUE ADDRESS:%p SIZE:%d\n", list, list->size);
// }
//
// Process * execute (LinkedList* queue) {
//     if (queue == NULL || isEmpty(queue) == 1) {
//         printf("No processes to execute\n");
//         return NULL;
//     }
//     Process * process = dequeue(queue);
//     if (process->state == READY) {
//         const float prevExecTime = process->remainingExecutionTime;
//         process->state = RUNNING;
//         process->remainingExecutionTime--;
//         process->state = randomState();
//         printf("\n%s executed for 1 CPU cycle. previous run time=%.1f new run time=%.1f current state: %s\n",
//             process->name, prevExecTime, process->remainingExecutionTime, printState(process->state));
//     } else {
//         printf("\n%s is not ready to run. It\'s state is %s. Run time is still %.1f\n", process->name, printState(process->state), process->remainingExecutionTime);
//         if (process->state == FREE || process->state == WAITING) process->state = randomState();
//     }
//     return process;
// }
//
// static int main() {
//     srand(time(NULL));
//     // int counter = 0;
//     // while (counter < 10) {
//     //     printf("%s\n", printState(randomState()));
//     //     counter++;
//     // }
//     // printf("%s %s %.2fs\n", randomName(), printState(state()), randomTime());
//     // Process * process = randomProcess();
//     // // printf("%s\n", processToString(process));
//     LinkedList* list = randomLinkedList(5);
//     // printf("The Process Queue:\n");
//     // printList(list);
//     //
//     int cycle = 0;
//     unsigned int maxQueueSize = list->size;
//     while (isEmpty(list) == 0) {
//         if (list->size > maxQueueSize) maxQueueSize = list->size;
//         printf("CYCLE:%d QUEUE_SIZE:%d\n", cycle, list->size);
//         Process * runningProcess = execute(list);
//         runningProcess->numberOfCycles++;
//         if (runningProcess->remainingExecutionTime > 0) {
//             // printf("\n%s OLD STATE:%s\n", runningProcess->name, printState(runningProcess->state));
//             // if (runningProcess->state != READY) runningProcess->state = randomState();
//             printf("%s is going back in the queue state:%s execTime:%.1f\n", runningProcess->name, printState(runningProcess->state), runningProcess->remainingExecutionTime);
//             enqueue(list, createNode(runningProcess));
//         } else {
//             runningProcess->state = FREE;
//             printf("%s HAS FINISHED RUNNING! State is %s\n", runningProcess->name, printState(runningProcess->state));
//         }
//         cycle++;
//         if (cycle < 200 && (1 + rand() % 5 == 5)) {
//             Process * process = randomProcess();
//             printf("\tADDING %s at CYCLE %d\n", process->name, cycle);
//             enqueue(list, createNode(randomProcess()));
//         }
//     }
//
//     // // Process * current = peek(list);
//     // Process * current = list->head->process;
//     // const int numberServed = list->size;
//     // while (list->size > 0 || current != NULL); {
//     //     printf("queue size:%d\n", list->size);
//     //     current = list->head->process;
//     //     printf("Before DEQUE:PID:%d %s, executionTime:%.2f status:%s\n", current->id, current->name, current->totalExecutionTime, printState(current->state));
//     //     current = dequeue(list);
//     //     printf("after deque head process:%d %s executionTime:%.2f status:%s\n", current->id,  current->name, current->totalExecutionTime, printState(current->state));
//     //     if (isEmpty(list) || list->size == 0) {
//     //         printf("No processes left exiting\n");
//     //         break;
//     //     }
//     //     if (current->totalExecutionTime > 0) {
//     //         current->state = READY;
//     //         printf("reenquing %s\n", current->name);
//     //         enqueue(list, current);
//     //     } else {
//     //         printf("%s has finished executing %f\n", current->name, current->totalExecutionTime);
//     //     }
//     // }
//     printQueue(list);
//     printf("It took %d cycles to complete %d processes. The queue size started at 5. It's largest size was %d\n", cycle, lastId, maxQueueSize);
//     // int count = 0;
//     // while (count < 20) {
//     //     printf("%d\n", rand() % 5);
//     //     count++;
//     // }
//     // free(current);
//
//     free(list);
//     return 0;
// }
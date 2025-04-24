// must compile with: gcc  -std=c99 -Wall -o oslab oslab.c

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* WE will use the three-state model */
#define READY   0
#define RUNNING 1
#define BLOCKED 2
#define MAXNUM 9999 //pid must be less than this number

/* string of process states */
char states[3][10] = {"ready", "running", "blocked"};

/* Information about each process */
/* Important: Feel free to add any new fields you want. But do not remove any of the cited one. */
struct process_info{
    int pid; // process ID, cannot be negative
    int state; //state of the process: ready, running, blocked
    int arrival_time; // time the process arrives
    int arrival_time_copy;
    int complete_time; //time the process finishes
    int turnaround_time; // complete_time - arrival_time
    int cpu1; //first computing interval
    int io; // io interval
    int cpu2; // second computing interval
    int done; // This flag is initialized to 0 and is set to 1 when the process is done
    int remaining_cpu1;
    int remaining_cpu2;
    int remaining_io;
};


/**** Global Variables ***/

struct process_info * process_list; // array containing all processes info
int num_processes = 0;  // total number of processes
int done_processes = 0;
int active_flag = 0;
int list[MAXNUM];
int listSize = 0;


/* You will implement the following functions */
/* Feel free to add more functions if you need. */

void process_file(FILE *); // Reads the input file and fill the process_list
int scheduler(int); // Returns the next process to be scheduled for execution (i.e. its state becomes RUNNING)
int update_running_process(int, int);
void update_blocked_processes(void); //decrement the io field in all blocked processes.
void print_processes(FILE *, int);


/**********************************************************************/


int main(int argc, char *argv[]){

    FILE * fp; //for creating the output file
    int time =0; //The clock
    int running_process_index = -1; // index of the process currently on the cpu
    int cpu_active = 0; // Incremented each cycle the CPU is busy
    
    
    // Check that the command line is correct
    if(argc != 2){
        
        printf("usage:  ./oslab filename\n");
        printf("filename: the processese information file\n");
        exit(1);
    }
    
    // Process the input command line.
    
    // Check that the file specified by the user exists and open it
    if( !(fp = fopen(argv[1],"r")))
    {
        printf("Cannot open file %s\n\n", argv[1]);
        exit(1);
    }
    
    process_file(fp);
    
    
    // Process output file

    char outputFileName[1024];
    
        
    sprintf(outputFileName, "%s%d", argv[1], num_processes);
    
    
    FILE *outputFile = fopen(outputFileName, "w");
    

    while (done_processes != num_processes ) {
        running_process_index = scheduler(time);
        print_processes(outputFile, time);
        
        if (running_process_index != -1) {
            
            time = update_running_process(running_process_index, time);
            cpu_active += 1;
            
        } else {
            time++;
        }
        update_blocked_processes();
    }
    
    
    
    float cpu_util = (float) cpu_active/((float) time);
    printf("num processes = %d\n", num_processes);
    printf("cpu utilization = %f\n", cpu_util);
    printf("total time = %d\n", time);

    for (int i = 0; i < num_processes; i++) {
        printf("process %d: turnaround time = %d\n", process_list[list[i]].pid, process_list[list[i]].turnaround_time );
    }
    
fclose(outputFile);

    
return 0;
}


/**********************************************************************/
/* The following function does the following:
- Reads the number of process from the input file and save it in the global variable: num_processes
- Allocates space for the processes list: process_list
- Read the file and fill up the info in the process_info array
- Keeps reading the file and fill up the information in the process_list
*/

void process_file(FILE * fp){

int i = 0;
fscanf(fp, "%d", &num_processes);

if((process_list = malloc(num_processes * sizeof(struct process_info) ))== NULL){
    printf("Failure to allocate process list of %d processes\n", num_processes);
    exit(1);
}

while( fscanf(fp,"%d %d %d %d %d", &process_list[i].pid, &process_list[i].cpu1, &process_list[i].io, &process_list[i].cpu2, &process_list[i].arrival_time) == 5){
    
    process_list[i].done = 0;
    process_list[i].remaining_cpu1 = process_list[i].cpu1;
    process_list[i].remaining_cpu2 = process_list[i].cpu2;
    process_list[i].remaining_io = process_list[i].io;
    process_list[i].state = READY;
    process_list[i].arrival_time_copy = process_list[i].arrival_time;
    i++;
   }
}



/**********************************************************************/



int scheduler(int time){
    
int curr_id = MAXNUM;
int curr_index = -1;

for (int i = 0; i < num_processes; i++) {
    struct process_info current_process = process_list[i];
    
    
    if (process_list[i].state == RUNNING) {
        curr_id = current_process.pid;
        curr_index = i;
        
        if (process_list[i].cpu2 <= 0) {
            process_list[i].state = BLOCKED;
            process_list[i].done = 1;
        }
                
    }
    if (process_list[i].pid < curr_id && process_list[i].state == READY && process_list[i].arrival_time == 0) {
        curr_id = current_process.pid;
        curr_index = i;
                }
    }
    
    
    if (curr_index != -1){
        process_list[curr_index].state = RUNNING;

    }
    return curr_index;
}


/**********************************************************************/


int update_running_process(int running_process_index, int currentTime){
    

    if (running_process_index == -1) {
        return -1;
    }
    
    
    
    struct process_info *running_process = &process_list[running_process_index];

    
    // Setting to blocked for CPU1
    
    if (running_process -> cpu1 >= 1) {
        running_process -> cpu1 -= 1;
        if (running_process -> cpu1 <= 0)
            running_process -> state = BLOCKED;

    }
    
    if (running_process -> io == 0 && running_process -> cpu2 >= 1) {
        running_process -> cpu2 -= 1;
        
    }
    
    // Setting to done for CPU2
    
    if (running_process -> cpu2 <= 0) {
        running_process -> state = BLOCKED;
        running_process -> done = 1;
        done_processes += 1;
        running_process -> complete_time = currentTime;
        running_process -> turnaround_time = running_process -> complete_time - running_process -> arrival_time_copy;
        list[listSize] = running_process_index;
        listSize+=1;

    }
    
    currentTime += 1;

    
    return currentTime;
    
    }

/**********************************************************************/



void update_blocked_processes(void){
    
    for (int i = 0; i < num_processes; i++) {
    
        struct process_info *curr_process = &process_list[i];
        
        if (process_list[i].arrival_time > 0){
            process_list[i].arrival_time -= 1;
        }
        
        if (curr_process -> io >= 1 && curr_process -> state == BLOCKED) {
            curr_process -> io -= 1;
        }
        
        else if (curr_process -> done != 1 && curr_process -> io == 0){
            curr_process -> state = READY;
        }
    }
}

/**********************************************************************/


void print_processes(FILE *output, int time) {
    
    
    fprintf(output, "time:%d\n", time);
    
    for (int i = 0; i < num_processes; i++) {
          if (process_list[i].state == RUNNING) {
              fprintf(output, "process : %d : %s\n", process_list[i].pid, states[process_list[i].state]);
              break;
          }
      }

    for (int i = 0; i < num_processes; i++) {
        
        struct process_info print_process = process_list[i];
        
        if (process_list[i].arrival_time == 0 && process_list[i].done != 1 && process_list[i].state != RUNNING){
            fprintf(output, "process : %d : %s\n", print_process.pid,states[print_process.state] );
 


        }
    }
    fprintf(output, "\n");

    
}


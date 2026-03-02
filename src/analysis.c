#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "dyn_array.h"
#include "processing_scheduling.h"

#define FCFS "FCFS"
#define P "P"
#define RR "RR"
#define SJF "SJF"
#define SRT "SRT"

// Add and comment your analysis code in this function.
// THIS IS NOT FINISHED.
int main(int argc, char **argv) 
{
	if (argc < 3) 
	{
		printf("%s <pcb file> <schedule algorithm> [quantum]\n", argv[0]);
		return EXIT_FAILURE;
	}
	//Load data
	dyn_array_t *ready_queue = load_process_control_blocks(argv[1]);
	//Create an empty ScheduleResult_t
	ScheduleResult_t *result = malloc(sizeof(ScheduleResult_t));

	bool success = false;
	// Invalid inputs
	if(result==NULL||ready_queue==NULL){
		printf("%s <pcb file> <schedule algorithm> [quantum]\n", argv[0]);
		return EXIT_FAILURE;
	}
	// run the scheduling algorithms defined
	if(strcmp(argv[2],FCFS) == 0) {
		//Run FCFS
		success = first_come_first_serve(ready_queue , result);
	}
	else if(strcmp(argv[2],P) == 0) {
		//Run priority
		success = priority(ready_queue , result);
	}

	else if(strcmp(argv[2],RR) == 0) {
		//Run round robin
		success = round_robin(ready_queue , result , 2);
	}

	else if(strcmp(argv[2],SJF) == 0) {
		//Run shortest job first
		success = shortest_job_first(ready_queue , result);
	}

	else if(strcmp(argv[2],SRT) == 0) {
		//Run shortest remaining time first
		success = shortest_remaining_time_first(ready_queue , result);
		
	}
	else{
		printf("%s <pcb file> <schedule algorithm> [quantum]\n", argv[0]);
		free(result);
		free(ready_queue);
		return EXIT_FAILURE;
	}
	free(ready_queue);
	if(success){
		//Print
		printf("%s average waiting time: %f\n", argv[2],result->average_waiting_time);
		printf("%s average turn around time: %f\n",argv[2],result->average_turnaround_time);
		printf("%s total run time: %ld\n",argv[2],result->total_run_time);

		// read from an input file (pcb.bin)
		int fd = open("../README.md", O_WRONLY | O_APPEND);
		if(fd == -1){
			free(result);
			perror("open failed");	
			return EXIT_FAILURE;
		}
		// formatted string to buffer
		char buffer[256];
		int len = snprintf(buffer, sizeof(buffer), 
			"%s average waiting time: %f\n%s average turn around time: %f\n%s total run time: %ld\n", 
			argv[2],result->average_waiting_time, 
			argv[2],result->average_turnaround_time, 
			argv[2],result->total_run_time);
		// write to the file
		int bytes_written = write(fd,buffer,len);
		// if error in writing, returns negative, otherwise it returns number of bytes written
		if (bytes_written < 0) {
			close(fd);
			free(result);	
			return EXIT_FAILURE;
		}
		//Free resources
		free(result);
	} else{
		perror("error running scheduling algorithm");
		free(result);
		return EXIT_FAILURE;
	}


	return EXIT_SUCCESS;
}

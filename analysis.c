#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

	if(strcmp(argv[2],FCFS) == 0) {
		//Load data
		dyn_array_t *ready_queue = load_process_control_blocks(argv[1]);
		//Create an empty ScheduleResult_t
		ScheduleResult_t *result = malloc(sizeof(ScheduleResult_t));
		if(result==NULL){
			perror("allocation failed");
			return EXIT_FAILURE;
		}
		//Run FCFS
		first_come_first_serve(ready_queue , result);
		//Print
		printf("FCFS average waiting time: %f\n", result->average_waiting_time);
		printf("FCFS average turn around time: %f\n",result->average_turnaround_time);
		printf("FCFS total run time: %ld\n",result->total_run_time);
		//Free resources
		free(ready_queue);
		free(result);
	}

	return EXIT_SUCCESS;
}

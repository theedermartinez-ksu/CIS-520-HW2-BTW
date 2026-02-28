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

	if(strcmp(argv[2],P) == 0) {
		//Load data
		dyn_array_t *ready_queue = load_process_control_blocks(argv[1]);
		//Create an empty ScheduleResult_t
		ScheduleResult_t *result = malloc(sizeof(ScheduleResult_t));
		if(result==NULL){
			perror("allocation failed");
			return EXIT_FAILURE;
		}
		//Run FCFS
		priority(ready_queue , result);
		//Print
		printf("P average waiting time: %f\n", result->average_waiting_time);
		printf("P average turn around time: %f\n",result->average_turnaround_time);
		printf("P total run time: %ld\n",result->total_run_time);
		//Free resources
		free(ready_queue);
		free(result);
	}

	if(strcmp(argv[2],RR) == 0) {
		//Load data
		dyn_array_t *ready_queue = load_process_control_blocks(argv[1]);
		//Create an empty ScheduleResult_t
		ScheduleResult_t *result = malloc(sizeof(ScheduleResult_t));
		if(result==NULL){
			perror("allocation failed");
			return EXIT_FAILURE;
		}
		//Run FCFS
		round_robin(ready_queue , result , 2);
		//Print
		printf("RR average waiting time: %f\n", result->average_waiting_time);
		printf("RR average turn around time: %f\n",result->average_turnaround_time);
		printf("RR total run time: %ld\n",result->total_run_time);
		//Free resources
		free(ready_queue);
		free(result);
	}

	if(strcmp(argv[2],SJF) == 0) {
		//Load data
		dyn_array_t *ready_queue = load_process_control_blocks(argv[1]);
		//Create an empty ScheduleResult_t
		ScheduleResult_t *result = malloc(sizeof(ScheduleResult_t));
		if(result==NULL){
			perror("allocation failed");
			return EXIT_FAILURE;
		}
		//Run FCFS
		shortest_job_first(ready_queue , result , 2);
		//Print
		printf("SJF average waiting time: %f\n", result->average_waiting_time);
		printf("SJF average turn around time: %f\n",result->average_turnaround_time);
		printf("SJF total run time: %ld\n",result->total_run_time);
		//Free resources
		free(ready_queue);
		free(result);
	}

	if(strcmp(argv[2],SRT) == 0) {
		//Load data
		dyn_array_t *ready_queue = load_process_control_blocks(argv[1]);
		//Create an empty ScheduleResult_t
		ScheduleResult_t *result = malloc(sizeof(ScheduleResult_t));
		if(result==NULL){
			perror("allocation failed");
			return EXIT_FAILURE;
		}
		//Run FCFS
		shortest_remaining_time_first(ready_queue , result);
		//Print
		printf("SRT average waiting time: %f\n", result->average_waiting_time);
		printf("SRT average turn around time: %f\n",result->average_turnaround_time);
		printf("SRT total run time: %ld\n",result->total_run_time);
		//Free resources
		free(ready_queue);
		free(result);
	}

	//write to i/0 or whatever
	//e




	return EXIT_SUCCESS;
}
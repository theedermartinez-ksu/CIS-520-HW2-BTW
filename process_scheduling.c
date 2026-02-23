#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "dyn_array.h"
#include "processing_scheduling.h"


// You might find this handy.  I put it around unused parameters, but you should
// remove it before you submit. Just allows things to compile initially.
#define UNUSED(x) (void)(x)

// private function
void virtual_cpu(ProcessControlBlock_t *process_control_block) 
{
	// decrement the burst time of the pcb
	--process_control_block->remaining_burst_time;
}

// Compares the arrival time of two process control blocks
// \param x is the first integer to compare
// \param y is the second integer to compare
// \return 0 if x == y return -1 if x < y and return 1 if x > y
int arrivalcompare(const void *x, const void *y)
{
	const ProcessControlBlock_t *thing1 = (const ProcessControlBlock_t *)x;
	const ProcessControlBlock_t *thing2 = (const ProcessControlBlock_t *)y;
	if(thing1->arrival == thing2->arrival){
		return 0;
	}
	if(thing1->arrival < thing2->arrival) {
		return -1;	
	}
	return 1;
}

bool first_come_first_serve(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
	//Error Checking	
	if (ready_queue == NULL || result == NULL || dyn_array_at(ready_queue,0) == NULL) {
		return false;
	}
	//Sort the dyn array based on arrival times
	dyn_array_sort(ready_queue, arrivalcompare);

	//Counter for loop initialized to zero
	int i = 0;
	//Sum of all wait times. Initialized to zero
	int wsum = 0;
	//Sum of all turn around times. Initialized to zero
	int tatsum = 0;
	//result total_run_time set to zero
	result->total_run_time = 0;
	//Iterate through dyn_array
	while(dyn_array_at(ready_queue, i ) != NULL) {
		//Sums result run time from previous run time (Sum of BT)
		result->total_run_time = result->total_run_time + ((ProcessControlBlock_t *)dyn_array_at(ready_queue,i))->remaining_burst_time;
		//Calculates and then sums turn around time (TATSUM = TATSUM + CT - AT).
		tatsum = tatsum + (result->total_run_time - ((ProcessControlBlock_t *)dyn_array_at(ready_queue,i))->arrival);
		//Calculates and then sums wait times (WSUM = WSUM + (CT - AT) - BT).
		wsum = wsum + ((result->total_run_time - ((ProcessControlBlock_t *)dyn_array_at(ready_queue,i))->arrival) - ((ProcessControlBlock_t *)dyn_array_at(ready_queue,i))->remaining_burst_time);
		i++;
	}
	//Calculates averages based on number of times looped.
	result->average_waiting_time = wsum / (float)(i);
	result->average_turnaround_time = tatsum / (float)(i);
	return true;
}

bool shortest_job_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
	UNUSED(ready_queue);
	UNUSED(result);
	return false;
}

bool priority(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
	UNUSED(ready_queue);
	UNUSED(result);
	return false;
}

bool round_robin(dyn_array_t *ready_queue, ScheduleResult_t *result, size_t quantum) 
{
	UNUSED(ready_queue);
	UNUSED(result);
	UNUSED(quantum);
	return false;
}

// Reads the PCB values from the binary file into ProcessControlBlock_t
// for N number of PCB entries stored in the file
// \param input_file the file containing the PCB burst times
// \return a populated dyn_array of ProcessControlBlocks if function ran successful else NULL for an error
dyn_array_t *load_process_control_blocks(const char *input_file) 
{

	// read from an input file (pcb.bin)
	int fd = open(input_file, O_RDONLY);
	if(fd == -1){
		perror("open failed");
		return NULL;
	}
	// read the binary file into data, first line gives the number of PCB blocks while the rest are the values
	uint32_t count;
	ssize_t first = read(fd, &count, sizeof(uint32_t));
	// 0 for EOF, -1 for error, else returns number of bytes read
	if(first <=0){
		perror("read failed");
		close(fd);
		return NULL;
	}
	ProcessControlBlock_t *array = malloc(count*sizeof(ProcessControlBlock_t));
	if(array==NULL){
		perror("array allocation failed");
		close(fd);
		return NULL;
	}
	for(uint32_t i =0;i<count;i++){
		ProcessControlBlock_t data;
		data.started = false;
		if(read(fd, &data.remaining_burst_time, sizeof(uint32_t))!= sizeof(uint32_t)
		|| read(fd, &data.priority, sizeof(uint32_t))!= sizeof(uint32_t)
		|| read(fd, &data.arrival, sizeof(uint32_t))!= sizeof(uint32_t)){
			close(fd);
			free(array);
			perror("read failed for pcb block");
			return NULL;
		}
		
		array[i] = data;
	}
	close(fd);
	// error in reading file
	dyn_array_t *populated = dyn_array_import(array,count,sizeof(ProcessControlBlock_t),NULL);
	free(array);
	return populated;

}

bool shortest_remaining_time_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
	UNUSED(ready_queue);
	UNUSED(result);
	return false;
}

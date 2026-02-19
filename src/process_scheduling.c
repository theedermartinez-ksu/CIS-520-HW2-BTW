#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "dyn_array.h"
#include "processing_scheduling.h"


// You might find this handy.  I put it around unused parameters, but you should
// remove it before you submit. Just allows things to compile initially.
#define UNUSED(x) (void)(x)
#define DYN_MAX_CAPACITY (((size_t) 1) << ((sizeof(size_t) << 3) - 8))

// private function
void virtual_cpu(ProcessControlBlock_t *process_control_block) 
{
	// decrement the burst time of the pcb
	--process_control_block->remaining_burst_time;
}

bool first_come_first_serve(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
	UNUSED(ready_queue);
	UNUSED(result);
	return false;
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
		return NULL;
	}
	// read the binary file into data
	ssize_t bytes_read;
	// = read(fd, &data, sizeof(ProcessControlBlock_t));
	ProcessControlBlock_t data;
    ProcessControlBlock_t array[DYN_MAX_CAPACITY];
	int count = 0;
	while(bytes_read = read(fd, &data, sizeof(ProcessControlBlock_t)) > 0){
		array[count] = data;
		count++;
		// delete later, printing results of PCB
		printf("remaining burst time: %d",data.remaining_burst_time);
		printf("priority: %d",data.priority);
		printf("arrival: %d",data.arrival);
		// error in reading files
		if(bytes_read == -1|| bytes_read != sizeof(ProcessControlBlock_t)){
			return NULL;
		}
	}
	close(fd);
	// error in reading file
	dyn_array_t *populated = dyn_array_import(&data,count,sizeof(ProcessControlBlock_t),NULL);
	free(&data);
	free(&array);
	return populated;
	// dyn_array_create(capacity, data_type_size, destruct func)
	//dyn_array_import(data,);
	//UNUSED(input_file);

}

bool shortest_remaining_time_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
	UNUSED(ready_queue);
	UNUSED(result);
	return false;
}

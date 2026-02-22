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
	char cwd[PATH_MAX];
	
	if(getcwd(cwd,sizeof(cwd)) != NULL){
		printf("Current working directory: %s\n",cwd);
	}else{
		perror("getcwd failed");
	}
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
		// delete later, printing results of PCB
		printf("remaining burst time: %d",data.remaining_burst_time);
		printf("priority: %d",data.priority);
		printf("arrival: %d",data.arrival);
		
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

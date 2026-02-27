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

// Compares the arrival time and priority time of two process control blocks
// \param x is the first integer to compare
// \param y is the second integer to compare
// \return 0 if x == y return -1 if x < y and return 1 if x > y
int priority_arrival_compare(const void *x, const void *y)
{
	const ProcessControlBlock_t *thing1 = (const ProcessControlBlock_t *)x;
	const ProcessControlBlock_t *thing2 = (const ProcessControlBlock_t *)y;
	if(thing1->arrival == thing2->arrival){
		if(thing1->priority == thing2-> priority){
			return 0;
		}
		if(thing1-> priority < thing2->priority){
			return -1;
		}
		return 1;
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
	//Number of PCB blocks in ready queue
	int i = 0;
	//Sum of all wait times. Initialized to zero
	int wsum = 0;
	//Sum of all turn around times. Initialized to zero
	int tatsum = 0;
	//result total_run_time set to zero
	result->total_run_time = 0;
	//Iterate through dyn_array
	while(dyn_array_at(ready_queue, 0 ) != NULL) {
		//Sums result run time from previous run time (Sum of BT)
		result->total_run_time = result->total_run_time + ((ProcessControlBlock_t *)dyn_array_at(ready_queue,0))->remaining_burst_time;
		//Calculates and then sums turn around time (TATSUM = TATSUM + CT - AT).
		tatsum = tatsum + (result->total_run_time - ((ProcessControlBlock_t *)dyn_array_at(ready_queue,0))->arrival);
		//Calculates and then sums wait times (WSUM = WSUM + (CT - AT) - BT).
		wsum = wsum + ((result->total_run_time - ((ProcessControlBlock_t *)dyn_array_at(ready_queue,0))->arrival) - ((ProcessControlBlock_t *)dyn_array_at(ready_queue,0))->remaining_burst_time);
		i++;
		dyn_array_pop_front(ready_queue);
	}
	//Calculates averages based on number of times looped.
	result->average_waiting_time = wsum / (float)(i);
	result->average_turnaround_time = tatsum / (float)(i);
	return true;
}

//shortest job first is NOT PREEMPTIVE
// no reevaluation
bool shortest_job_first(dyn_array_t *ready_queue, ScheduleResult_t *result)
{
	//Error Checking
	if (ready_queue == NULL || result == NULL || dyn_array_at(ready_queue,0) == NULL)
	{
		return false;
	}
	//sSort the dyn array based on arrival times
	// select the process that has minimum arrivaal time and m9inimum burst time
	// sort once
	dyn_array_sort(ready_queue, arrivalcompare);//sorted now we only check burst time

	// we need three things.
	/*
	*	EXPECT_FLOAT_EQ(arraySorted.average_waiting_time, 1.0f);
		EXPECT_FLOAT_EQ(arraySorted.average_turnaround_time, 5.666667f);
		EXPECT_EQ(arraySorted.total_run_time, 14UL);
	 */
	size_t current_time=0;
	int waitTimes = 0;
	int turnTimes = 0;
	int countItem = dyn_array_size(ready_queue);
	result->total_run_time = 0;

	while (dyn_array_size(ready_queue) > 0 ) // keep scheduling until done
	{
		int min_index = -1;
		size_t min_burstTime = 1000000; // large number
		for (size_t i = 0; i<dyn_array_size(ready_queue); i++)//loop and find small
		{
			ProcessControlBlock_t* pcb = (ProcessControlBlock_t*) dyn_array_at(ready_queue,i);//current evaluation

			if ((pcb->arrival) <= current_time )// if it fits
			{
				if (pcb->remaining_burst_time < min_burstTime)
				{
					min_index = i;
					min_burstTime = pcb->remaining_burst_time;
				}
			}
		}

		// at this point we have the index then we pass that along
		if (min_index == -1)// not dice
		{
			//??? doo ntohing ??
			//set to zero (first item)
			ProcessControlBlock_t* pcb0 = (ProcessControlBlock_t*) dyn_array_at(ready_queue, 0);
			current_time = pcb0->arrival;
			continue;
		}
		else //run it and add it to the stats
		{
			// increase coount of totals.
			ProcessControlBlock_t* currentPCB = (ProcessControlBlock_t*) dyn_array_at(ready_queue, min_index);
			//copmute the correct times
			size_t compTime = current_time + currentPCB ->remaining_burst_time;
			size_t tTime2 = compTime - currentPCB->arrival;
			size_t wTime2 = tTime2 - currentPCB->remaining_burst_time;

			turnTimes = turnTimes + tTime2;
			waitTimes = waitTimes + wTime2;

			current_time = compTime;
			dyn_array_erase(ready_queue,min_index);		// delete selected process

		}
	}

	// ipdate result at the end
	result->total_run_time = current_time;
	result->average_waiting_time = waitTimes / (float)countItem;
	result->average_turnaround_time = turnTimes / (float)countItem;

	return true;
}


// Runs the non-preemptive Priority algorithm over the incoming ready_queue
// \param ready queue a dyn_array of type ProcessControlBlock_t that contain be up to N elements
// \param result used for shortest job first stat tracking \ref ScheduleResult_t
// \return true if function ran successful else false for an error
// There is no guarantee that the passed dyn_array_t will be the result of your implementation of load_process_control_blocks
bool priority(dyn_array_t *ready_queue, ScheduleResult_t *result)
{
	//Error Checking
	if (ready_queue == NULL || result == NULL || dyn_array_at(ready_queue,0) == NULL) {
		return false;
	}
	dyn_array_sort(ready_queue,prioritycompare);
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
	// allocate a temporary array to hold process control blocks before importing to dynarray
	ProcessControlBlock_t *array = malloc(count*sizeof(ProcessControlBlock_t));
	if(array==NULL){
		perror("array allocation failed");
		close(fd);
		return NULL;
	}
	// for each PCB block, read in the binary to the variables
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
	dyn_array_t *populated = dyn_array_import(array,count,sizeof(ProcessControlBlock_t),NULL);
	// free temporary
	free(array);
	return populated;

}

// preemtive (interruption good)
// The process that has the least amoount of time runs until
// it finishes or a new process with a shorter remaining time arrive.
// This ensures the fastest finishing process always gets priority
bool shortest_remaining_time_first(dyn_array_t *ready_queue, ScheduleResult_t *result)
{
	//Error Checking
	if (ready_queue == NULL || result == NULL || dyn_array_at(ready_queue,0) == NULL)
	{
		return false;
	}
	//fix arrival time
	dyn_array_sort(ready_queue,arrivalcompare); // compare by arrival

	size_t countItem = dyn_array_size(ready_queue);
	size_t completed = 0;
	int current_time = 0;


	result->total_waiting_time = 0;
	result->total_turnaround_time = 0;
	result->total_run_time = 0;

	while (completed < countItem)
	{
		//---for loop
		int smallestIndex = -1;
		int smallest_time = 1000000;
		//find the items with the smallest remaining
		for (size_t i = 0 ; i < countItem; i++)
		{
			//get current count (
			ProcessControlBlock_t* pcb = (ProcessControlBlock_t*) dyn_array_at(ready_queue,i);//current evaluation
			int cur_smallest = pcb->remaining_burst_time;
			if (cur_smallest < smallest_time && cur_smallest > 0 && pcb->arrival_time <= current_time)
			{
				smallest_time = cur_smallest;
				smallestIndex = i;
			}

		}
		//idling
		if (smallestIndex == -1)
		{
			current_time ++;
			continue;
		}
		//process it
		ProcessControlBlock_t* currentPCB = (ProcessControlBlock_t*) dyn_array_at(ready_queue,smallestIndex);//current evaluation
		currentPCB->remaining_burst_time = currentPCB->remaining_burst_time  - 1;
		current_time++;

		//check if the curent pcb is done
		if (currentPCB->remaining_burst_time == 0)
		{
			completed++;
			int turnaround = current_time - (currentPCB ->arrival_time);
			int waiting = turnaround - (currentPCB ->burst_time);

			result->total_turnaround_time += turnaround;
			result->total_waiting_time += waiting;
		}

	}

	result->total_run_time = current_time;
	//Calculates averages based on number of times looped.
	result->average_turnaround_time = (float) result->total_turnaround_time / countItem;
	result->average_waiting_time = (float)result->total_waiting_time / countItem;


	return true;
}
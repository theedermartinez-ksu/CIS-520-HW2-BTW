#include <fcntl.h>
#include <stdio.h>
#include <pthread.h>
#include "gtest/gtest.h"
#include "../include/processing_scheduling.h"

// tests 2 one good one bad 
// for each one 

// Using a C library requires extern "C" to prevent function mangling
extern "C"
{
#include <dyn_array.h>
}

#define NUM_PCB 30
#define QUANTUM 5 // Used for Robin Round for process as the run time limit

/*
unsigned int score;
unsigned int total;

class GradeEnvironment : public testing::Environment
{
	public:
		virtual void SetUp()
		{
			score = 0;
			total = 210;
		}

		virtual void TearDown()
		{
			::testing::Test::RecordProperty("points_given", score);
			::testing::Test::RecordProperty("points_total", total);
			std::cout << "SCORE: " << score << '/' << total << std::endl;
		}
};
*/

// Reads the PCB values from the binary file into ProcessControlBlock_t
// for N number of PCB entries stored in the file
// \param input_file the file containing the PCB burst times
// \return a populated dyn_array of ProcessControlBlocks if function ran successful else NULL for an error
// dyn_array_t *load_process_control_blocks(const char *input_file);
TEST(load_process_blocks, DoesNotPassReturnsNULL)
{
	EXPECT_EQ(load_process_control_blocks("./beans.bi"), nullptr );
	EXPECT_EQ(load_process_control_blocks(""), nullptr);

}

TEST(load_process_blocks, PassesALL)
{
	dyn_array_t* result = load_process_control_blocks("pcb.bin");
	EXPECT_NE(result, nullptr);

}
// set up two dyn arrays. One of unorganized values, one iwth the expected values
// check that the unoganized one is empty and the other one is equal to the one that is ordered
// bool first_come_first_serve(dyn_array_t *ready_queue, ScheduleResult_t *result);

TEST(PCBGOOD,DoesPass)
{
	ProcessControlBlock_t pcb1 =
	{
		.remaining_burst_time = 10,
		.priority = 10,
		.arrival = 5,
		.started = false
	};

	ProcessControlBlock_t pcb2 =
	{
		.remaining_burst_time = 1,
		.priority = 1,
		.arrival = 3,
		.started = false
	};

	ProcessControlBlock_t pcb3 =
	{
		.remaining_burst_time = 3,
		.priority = 3,
		.arrival = 2,
		.started = false
	};

	dyn_array_t *arrayUnsorted = dyn_array_create(3, sizeof
		(ProcessControlBlock_t), NULL);

	dyn_array_push_back(arrayUnsorted, &pcb1);
	dyn_array_push_back(arrayUnsorted, &pcb2);
	dyn_array_push_back(arrayUnsorted, &pcb3);
	//----- sorted--//
	ScheduleResult_t  arraySorted{};
	EXPECT_TRUE(first_come_first_serve(arrayUnsorted, &arraySorted));

	EXPECT_FLOAT_EQ(arraySorted.average_waiting_time, 1.0f);
	EXPECT_FLOAT_EQ(arraySorted.average_turnaround_time, 5.666667f);
	EXPECT_EQ(arraySorted.total_run_time, 14UL);

}

TEST(PCBBAD,NOTPASS)
{
	dyn_array_t *arrayUnsorted = dyn_array_create(0, sizeof
		(ProcessControlBlock_t), NULL);

	ScheduleResult_t arraySorted{};
	EXPECT_FALSE(first_come_first_serve(arrayUnsorted, &arraySorted));

}

TEST(ROUND_ROB,PASS)
{
	ProcessControlBlock_t pcb1 =
	{
		.remaining_burst_time = 10,
		.priority = 10,
		.arrival = 5,
		.started = false
	};

	ProcessControlBlock_t pcb2 =
	{
		.remaining_burst_time = 1,
		.priority = 1,
		.arrival = 3,
		.started = false
	};

	ProcessControlBlock_t pcb3 =
	{
		.remaining_burst_time = 3,
		.priority = 3,
		.arrival = 2,
		.started = false
	};

	dyn_array_t *arrayUnsorted = dyn_array_create(3, sizeof
		(ProcessControlBlock_t), NULL);

	dyn_array_push_back(arrayUnsorted, &pcb1);
	dyn_array_push_back(arrayUnsorted, &pcb2);
	dyn_array_push_back(arrayUnsorted, &pcb3);

	ScheduleResult_t result = {0};
	size_t quant = 2;
	EXPECT_TRUE(round_robin(arrayUnsorted,result,quant));

	EXPECT_EQ(result.total_run_time, 14UL);
	EXPECT_FLOAT_EQ(result.average_turnaround_time, 4.666667f);
	EXPECT_FLOAT_EQ(result.average_waiting_time, 2.0f);

}

TEST(ROUND_ROB,FAIL)
{
	dyn_array_t *arrayUnsorted = dyn_array_create(0, sizeof
		(ProcessControlBlock_t), NULL);

	ScheduleResult_t arraySorted{};
	EXPECT_FALSE(round_robin(arrayUnsorted, &arraySorted));
}

TEST(shortest_job_first,Valid){
	// pass in good inputs, expecting function to return true and have the expected result values filled
	ScheduleResult_t result = {0};
	ProcessControlBlock_t pcb1 =
	{
		.remaining_burst_time = 10,
		.priority = 10,
		.arrival = 5,
		.started = false
	};

	ProcessControlBlock_t pcb2 =
	{
		.remaining_burst_time = 1,
		.priority = 1,
		.arrival = 3,
		.started = false
	};

	ProcessControlBlock_t pcb3 =
	{
		.remaining_burst_time = 3,
		.priority = 3,
		.arrival = 2,
		.started = false
	};
	dyn_array_t *arrayUnsorted = dyn_array_create(3, sizeof(ProcessControlBlock_t), NULL);
	dyn_array_push_back(arrayUnsorted, &pcb1);
	dyn_array_push_back(arrayUnsorted, &pcb2);
	dyn_array_push_back(arrayUnsorted, &pcb3);

	EXPECT_TRUE(shortest_job_first(arrayUnsorted,result));
	EXPECT_EQ(result.total_run_time, 14UL);
	EXPECT_FLOAT_EQ(result.average_turnaround_time, (float)(17/3));
	EXPECT_FLOAT_EQ(result.average_waiting_time, 1.0f);

	free(arrayUnsorted);
}
TEST(shortest_job_first,NullInputs){
	// invalid ready_queue input, valid result, expected false
	ScheduleResult_t result = {0};
	EXPECT_FALSE(shortest_job_first(NULL,result));
	// invalid result input, valid ready_queue, expected false
	ProcessControlBlock_t pcb1 =
	{
		.remaining_burst_time = 10,
		.priority = 10,
		.arrival = 5,
		.started = false
	};

	ProcessControlBlock_t pcb2 =
	{
		.remaining_burst_time = 1,
		.priority = 1,
		.arrival = 3,
		.started = false
	};

	ProcessControlBlock_t pcb3 =
	{
		.remaining_burst_time = 3,
		.priority = 3,
		.arrival = 2,
		.started = false
	};
	dyn_array_t *arrayUnsorted = dyn_array_create(3, sizeof(ProcessControlBlock_t), NULL);

	dyn_array_push_back(arrayUnsorted, &pcb1);
	dyn_array_push_back(arrayUnsorted, &pcb2);
	dyn_array_push_back(arrayUnsorted, &pcb3);
	EXPECT_FALSE(shortest_job_first(arraySorted,NULL));

	free(arrayUnsorted);

}
TEST(shortest_job_first, BadInputs){
	// dyn array containing non-pcb block elements, expect false
	int a = 0;
	int b = 2;
	int c = 8;
	dyn_array_t *arrayUnsorted = dyn_array_create(3, sizeof(int), NULL);
	dyn_array_push_back(arrayUnsorted, &a);
	dyn_array_push_back(arrayUnsorted, &b);
	dyn_array_push_back(arrayUnsorted, &c);
	ScheduleResult_t result = {0};
	EXPECT_FALSE(shortest_job_first(arraySorted,result));

}

TEST(shortest_remaining_time_first, BadInputs){
	// dyn array containing non-pcb block elements, expect false
	int a = 0;
	int b = 2;
	int c = 8;
	dyn_array_t *arrayUnsorted = dyn_array_create(3, sizeof(int), NULL);
	dyn_array_push_back(arrayUnsorted, &a);
	dyn_array_push_back(arrayUnsorted, &b);
	dyn_array_push_back(arrayUnsorted, &c);
	ScheduleResult_t result = {0};
	EXPECT_FALSE(shortest_remaining_time_first(arraySorted,result));

	free(arrayUnsorted);

}

TEST(shortest_remaining_time_first,NullInputs){
	// invalid ready_queue input, expected false
	ScheduleResult_t result = {0};
	EXPECT_FALSE(shortest_remaining_time_first(NULL,result));
	// invalid result input, expected false
	ProcessControlBlock_t pcb1 =
	{
		.remaining_burst_time = 10,
		.priority = 10,
		.arrival = 5,
		.started = false
	};

	ProcessControlBlock_t pcb2 =
	{
		.remaining_burst_time = 1,
		.priority = 1,
		.arrival = 3,
		.started = false
	};

	ProcessControlBlock_t pcb3 =
	{
		.remaining_burst_time = 3,
		.priority = 3,
		.arrival = 2,
		.started = false
	};
	dyn_array_t *arrayUnsorted = dyn_array_create(3, sizeof(ProcessControlBlock_t), NULL);

	dyn_array_push_back(arrayUnsorted, &pcb1);
	dyn_array_push_back(arrayUnsorted, &pcb2);
	dyn_array_push_back(arrayUnsorted, &pcb3);
	EXPECT_FALSE(shortest_remaining_time_first(arraySorted,NULL));

	free(arrayUnsorted);

}
TEST(shortest_remaining_time_first,Valid){
	// pass in good inputs, expecting function to return true and have the expected result values filled
	ScheduleResult_t result = {0};
	ProcessControlBlock_t pcb1 =
	{
		.remaining_burst_time = 10,
		.priority = 10,
		.arrival = 5,
		.started = false
	};

	ProcessControlBlock_t pcb2 =
	{
		.remaining_burst_time = 1,
		.priority = 1,
		.arrival = 3,
		.started = false
	};

	ProcessControlBlock_t pcb3 =
	{
		.remaining_burst_time = 3,
		.priority = 3,
		.arrival = 2,
		.started = false
	};
	dyn_array_t *arrayUnsorted = dyn_array_create(3, sizeof(ProcessControlBlock_t), NULL);
	dyn_array_push_back(arrayUnsorted, &pcb1);
	dyn_array_push_back(arrayUnsorted, &pcb2);
	dyn_array_push_back(arrayUnsorted, &pcb3);

	EXPECT_TRUE(shortest_remaining_time_first(arrayUnsorted,result));
	// 3+1+10
	EXPECT_EQ(result.total_run_time, 14UL);
	// (11+1+4)/3
	EXPECT_FLOAT_EQ(result.average_turnaround_time, (float)(16/3));
	// (1+0+1)/3
	EXPECT_FLOAT_EQ(result.average_waiting_time, (float)(2/3));

	free(arrayUnsorted);
}
//PRIORITY TESTS
TEST(PRIORITY_TEST,PASS)
{
	ProcessControlBlock_t pcb1 =
	{
		.remaining_burst_time = 4,
		.priority = 2,
		.arrival = 0,
		.started = false
	};

	ProcessControlBlock_t pcb2 =
	{
		.remaining_burst_time = 2,
		.priority = 1,
		.arrival = 1,
		.started = false
	};

	ProcessControlBlock_t pcb3 =
	{
		.remaining_burst_time = 6,
		.priority = 3,
		.arrival = 2,
		.started = false
	};

	dyn_array_t *arrayUnsorted = dyn_array_create(3, sizeof
		(ProcessControlBlock_t), NULL);

	dyn_array_push_back(arrayUnsorted, &pcb1);
	dyn_array_push_back(arrayUnsorted, &pcb2);
	dyn_array_push_back(arrayUnsorted, &pcb3);

	ScheduleResult_t result = {0};
	size_t quant = 2;
	EXPECT_TRUE(priority(arrayUnsorted,result,quant));

	EXPECT_EQ(result.total_run_time, 12UL);
	EXPECT_FLOAT_EQ(result.average_turnaround_time, 6.3333f);
	EXPECT_FLOAT_EQ(result.average_waiting_time, 2.33f);

}

TEST(PRIORITY_TEST,FAIL)
{
	dyn_array_t *arrayUnsorted = dyn_array_create(0, sizeof
		(ProcessControlBlock_t), NULL);

	ScheduleResult_t arraySorted{};
	EXPECT_FALSE(priority(arrayUnsorted, &arraySorted));
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	// ::testing::AddGlobalTestEnvironment(new GradeEnvironment);
	return RUN_ALL_TESTS();
}
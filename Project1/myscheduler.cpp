//myschedule.cpp
/*Define all the functions in 'myschedule.h' here.*/
//Aiyu Cui, Rex Li, Weiming Shan

#include "myscheduler.h"

void MyScheduler::CreateThread(int arriving_time, int remaining_time, int priority, int tid) //Thread ID not Process ID
{
	//Function to Create Thread(s) and insert them in the student
	//defined data structure
	ThreadDescriptorBlock thread1;
	
	thread1.priority = priority; 
	thread1.arriving_time = arriving_time;
	thread1.remaining_time = remaining_time;
	thread1.tid = tid;
	buffer.push(thread1); //add to our priorty queue
	
}

bool MyScheduler::Dispatch()
{
	//Todo: Check and remove finished threads
	//Todo: Check if all the threads are finished; if so, return false
	switch(policy)
	{
		case FCFS:
		{//First Come First Serve
			//can only fill one CPU per clk cycle 
			int counter = 0; //variable to check if all CPUs are done executing
			int clk = timer; 
			bool flag = false; //to check if arrival time <=timer 
			ThreadDescriptorBlock temp;
			while (!buffer.empty())
			{
				temp = buffer.top();
				if (temp.arriving_time <= clk)
					flag = true; 
				
				for (unsigned int i = 0; i < num_cpu; i++) //find an open CPU
				{
					if (CPUs[i] == NULL && flag == true ) {
						CPUs[i] = new ThreadDescriptorBlock;
						buffer.pop();
						*CPUs[i] = temp;
						flag = false; 
					}

				}
				return true; //return control to scheduler.h and run threads

			}
			for (unsigned int i = 0; i < num_cpu; i++)
			{
				counter = 0;
				if (CPUs[i] == NULL) {
					counter++;
				}
				if (counter == num_cpu) {
					return false;
				}
			}

		}
			break;
		case STRFwoP:	//Shortest Time Remaining First, without preemption
		{
			int counter = 0; //variable to check if all CPUs are done executing
			int tempSize = 0; //remaining threads after cpu are initalliy filled 
			int counter1 = 0; //to count the number of cpus unused 
			ThreadDescriptorBlock temp;
			while (!buffer.empty() && counter1 < num_cpu) //FCFS until all CPUs are filled, then push remaining threads into bufferRT
			{
				for (unsigned int i = 0; i < num_cpu; i++)
				{
					if (CPUs[i] == NULL) {
						CPUs[i] = new ThreadDescriptorBlock;
						temp = buffer.top();
						buffer.pop();
						*CPUs[i] = temp;
						counter1++;
					}
				}
			}
			tempSize = buffer.size();
			for (int i = 0; i < tempSize; i++) //add remaining threads to bufferRT
			{
				temp = buffer.top();
				buffer.pop();
				bufferRT.push(temp);
			}
			while (!bufferRT.empty()) //FCFS for bufferRT, sorted by remaining time
			{
				for (unsigned int i = 0; i < num_cpu; i++)
				{
					if (CPUs[i] == NULL) {
						CPUs[i] = new ThreadDescriptorBlock;
						temp = buffer.top();
						buffer.pop();
						*CPUs[i] = temp;

					}

				}
				return true; //return control to scheduler.h and run threads for one clk cycle

			}
			for (unsigned int i = 0; i < num_cpu; i++) //check if CPUs are done
			{
				counter = 0; //temp variable to check if all CPUs are done executing
				if (CPUs[i] == NULL) {
					counter++;
				}
				if (counter == num_cpu) {
					return false;
				}
			}
		}
			break;
		case STRFwP:	//Shortest Time Remaining First, with preemption
		{

		}
			break;
		case PBS:		//Priority Based Scheduling, with preemption
		{

		}
			break;
		default: 
		{
			cout << "Invalid policy!";
			throw 0; 
		}
	}
	return true;
}
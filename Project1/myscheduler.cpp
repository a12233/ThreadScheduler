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
	cout << "----TIME: " << timer<<"-----------"<<endl;
	cout << buffer.size() << " threads still there" << endl;
	if (buffer.empty()) {
		cout << "FINISH!!! NO more thread" << endl;
		return true;
	}
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
			for (unsigned int i = 0; i < num_cpu; i++) //only executes when buffer is empty
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
			int counterCPU = 0; 
			int clk = timer;
			bool flag = false; //to check if arrival time <=timer 
			bool flag1 = false; //to check if CPUs are filled 
			ThreadDescriptorBlock temp;
			
			while (!buffer.empty() && threadControl == false) //FCFS until all CPUs are filled, then push remaining threads into bufferRT
			{
				temp = buffer.top();
				if (temp.arriving_time <= clk)
					flag = true;
				for (unsigned int i = 0; i < num_cpu; i++)
				{
					if (CPUs[i] == NULL && flag == true) {
						CPUs[i] = new ThreadDescriptorBlock;
						temp = buffer.top();
						buffer.pop();
						*CPUs[i] = temp;
						counter1++;
						flag = false; 
					}
				}
				for (unsigned int i = 0; i < num_cpu && threadControl == false; i++) { //determined if all CPU are initially filled
					if (CPUs[i] != NULL)
						counterCPU++;
				}
				if (counterCPU == num_cpu)
					threadControl = true;
				return true; //give scheduler.h control, increament timer by 1
			}
			

			if (threadControl == true) 	//if all cpu are filled, push remaining threads into bufferRT, then FCFS on that buffer

			{
				tempSize = buffer.size();
				for (int i = 0; i < tempSize; i++) //add remaining threads to bufferRT
				{
					temp = buffer.top();
					buffer.pop();
					bufferRT.push(temp);
				}
				while (!bufferRT.empty()) //FCFS for bufferRT, sorted by remaining time
				{
					temp = bufferRT.top();
					if (temp.arriving_time <= clk) //only process threads that have "arrived"
						flag = true;
					for (unsigned int i = 0; i < num_cpu; i++)
					{
						if (CPUs[i] == NULL && flag == true) {
							CPUs[i] = new ThreadDescriptorBlock;
							temp = bufferRT.top();
							bufferRT.pop();
							*CPUs[i] = temp;
							flag = false; 
						}

					}
					return true; //return control to scheduler.h and run threads for one clk cycle

				}
				for (unsigned int i = 0; i < num_cpu; i++) //check if CPUs are done, only executes when bufferRT is empty
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
		}
			break;
		case STRFwP:	//Shortest Time Remaining First, with preemption
		{


		}
			break;
		case PBS:		//Priority Based Scheduling, with preemption - AC
		{
			cout << "----PBS----" << endl;
			
			ThreadDescriptorBlock temp;

			// Get Thread with highest priority among threads arrived
			ThreadDescriptorBlock *tmpPtr;
			tmpPtr = getHighestPriorityThread();
			
			if (tmpPtr == nullptr) {
				cout << "No thread incoming" << endl;
				return true;
			}
			temp  = *tmpPtr;

			cout << "    Incoming thread: #" << temp.tid << endl;
			cout << "    Curr Priority: " << temp.priority << endl;

			// Set directly, if any CPU free
			int nextCPU = findNextAvailableCPU();
			if (nextCPU != -1) {
				CPUs[nextCPU] = &temp;
				cout << "    CPU free #" << nextCPU << ": Thread " << temp.tid << endl;
				return true;
			}

			// IF CPU not Free, check
			cout << "  No CPU Free\n";

			// Find CPU with lowest priority thread
			int idx = getCPUThreadLowestPriority();
			int lowest = CPUs[idx]->priority;
			cout << "Lowest CPU Priority: " << lowest << endl;

			// Compare thread with lowest priority in CPU and the incoming thread
			if (lowest < temp.priority) {
				ThreadDescriptorBlock useless = *CPUs[idx];
				buffer.push(useless);
				CPUs[idx] = &temp;
				cout << "PBS: CPU #" << idx << ": Thread " << temp.tid << endl;
			}
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

int MyScheduler::findNextAvailableCPU()
{
	for (int i = 0; i < num_cpu; i++) {
		if (CPUs[i] == NULL) {return i;}
	}
	// If no CPU available, return -1
	return -1;
}

ThreadDescriptorBlock *MyScheduler::getHighestPriorityThread() {
	
	// Pop out and return the thread arrived with highest priority
	
	if (buffer.empty())
		return nullptr;
	
	//Get all thread arrived
	vector<ThreadDescriptorBlock> tmp;
	cout << "Arrived: " << endl;
	while (!buffer.empty()) {
		cout << "Th " << buffer.top().tid << " arrived at " << buffer.top().arriving_time << endl;
		tmp.push_back(buffer.top());
		buffer.pop();
	}
	
	//Return the thread with the highest priority

	if (tmp.empty()) { return nullptr; }


	ThreadDescriptorBlock;
	int highest = -1;
	ThreadDescriptorBlock *result = nullptr;

	while(!tmp.empty()) {
		ThreadDescriptorBlock curr = tmp.back();
		tmp.pop_back();

		if (curr.arriving_time > timer) {
			buffer.push(curr);
		}
		else {
			if (curr.priority > highest) {
				if (result != nullptr) {
					buffer.push(*result);
				}
				result = &curr;
			}
			else {
				buffer.push(curr);
			}
		}


	}
	cout << "!!";
	return (result);
}

int MyScheduler::getCPUThreadLowestPriority() {
	
	int index = -1;
	int lowest = 100;

	for (int i = 0; i < num_cpu; i++) {
		if (CPUs[i]->priority < lowest) {
			lowest = CPUs[i]->priority;
			index = i;
		}
	}
	return (index);
		
}
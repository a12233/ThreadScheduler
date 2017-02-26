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
	//bufferPR.push(thread1);
	buffer.push(thread1); //add to our priorty queue
	//bufferPR.push(thread1);
	//bufferRT.push(thread1);
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
			
			ThreadDescriptorBlock curr;

			// Get Thread with highest priority among threads arrived
			ThreadDescriptorBlock *tmpPtr;
			tmpPtr = getHighestPriorityThread();
			
			if (tmpPtr == nullptr) {
				cout << "No thread incoming" << endl;
				return true;
			}

			curr  = *tmpPtr;
			cout << "    Incoming thread: #" << curr.tid << endl;
			cout << "    Curr Priority: " << curr.priority << endl;

			// Set directly, if any CPU free
			int nextCPU = findNextAvailableCPU();
			if (nextCPU != -1) {
				CPUs[nextCPU] = new ThreadDescriptorBlock;
				*CPUs[nextCPU] = curr;
				cout << "    CPU free #" << nextCPU << ": Thread " << curr.tid << endl;
				return true;
			}


			// IF CPU not Free, check
			cout << "  No CPU Free\n";

			// Find CPU with lowest priority thread
			int idx = getCPUThreadLowestPriority();
			int lowest = CPUs[idx]->priority;
			cout << "Lowest CPU Priority: " << lowest << endl;

			// Compare thread with lowest priority in CPU and the incoming thread
			if (lowest < curr.priority) {
				ThreadDescriptorBlock useless = *CPUs[idx];
				buffer.push(useless);
				CPUs[idx] = new ThreadDescriptorBlock;
				*CPUs[idx] = curr;
				cout << "PBS: CPU #" << idx << ": Thread " << curr.tid << endl;
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

	if (buffer.empty()) {
		cout << "Buffer empty!" << endl;
		return (nullptr);
	}
	cout << "Threads arrived at time " << timer << ": " << endl;
	vector<ThreadDescriptorBlock> threads_tmp;
	ThreadDescriptorBlock curr;
	ThreadDescriptorBlock result;
	bool isFound = false;

	// 
	while (buffer.top().arriving_time <= timer) {
		
		curr = buffer.top();
		buffer.pop();
		threads_tmp.push_back(curr);
		cout << "thread #" << curr.tid << endl;
	}

	if (threads_tmp.empty()) {
		return nullptr;
	}

	// re-push back the thread checked but not taken to the bufferPR
	int prior = -1;
	int idx = 0;

	for(int i = 0;i<threads_tmp.size();i++){
		if (threads_tmp[i].priority > prior) {
			prior = threads_tmp[i].priority;
			idx = i;
		}
	}
	result = threads_tmp[idx];
	threads_tmp.erase(threads_tmp.begin() + idx);

	for (int i = 0; i < threads_tmp.size(); i++) {
		buffer.push(threads_tmp[i]);
	}

	return &result;
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
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

	//Check if all the threads are finished; if so, return false
	if (buffer.empty() && isFreeAllCPU()) {
		return false;
	}

	// Switching Policy
	switch (policy)
	{
	case FCFS:
	{//First Come First Serve
		while (true) {
			if (buffer.empty()) return true;

			ThreadDescriptorBlock curr = buffer.top();

			if (curr.arriving_time > timer) return true;

			buffer.pop();


			int nextCPU = findNextAvailableCPU();
			if (nextCPU == -1) {
				buffer.push(curr);
				return true;
			}
			CPUs[nextCPU] = new ThreadDescriptorBlock;
			*CPUs[nextCPU] = curr;
		}
	
	}
	break;

	case STRFwoP:	//Shortest Time Remaining First, without preemption
	{
		
		while (true) {
			ThreadDescriptorBlock curr;

			// Get Thread with SR among threads arrived
			ThreadDescriptorBlock *tmpPtr;
			tmpPtr = getThreadSRT();
			if (tmpPtr == nullptr) {
				//cout << "No thread incoming" << endl;
				return true;
			}
			curr = *tmpPtr;
			//cout << "    Incoming thread: #" << curr.tid << endl;

			// Set directly, if any CPU free
			int nextCPU = findNextAvailableCPU();
			if (nextCPU == -1) {
				buffer.push(curr);
				return true;
			}
			if (nextCPU != -1) {
				CPUs[nextCPU] = new ThreadDescriptorBlock;
				*CPUs[nextCPU] = curr;
			}
		}

	}
	break;
	case STRFwP:	//Shortest Time Remaining First, with preemption
	{
		while (true) {
		//cout << "STRFwP!!!~~~~\n";
		ThreadDescriptorBlock curr;
		
		// Get Thread with SR among threads arrived
		ThreadDescriptorBlock *tmpPtr;
		tmpPtr = getThreadSRT();
		if (tmpPtr == nullptr) {
			//cout << "No thread incoming" << endl;
			return true;
		}
		curr = *tmpPtr;
		//cout << "    Incoming thread: #" << curr.tid << endl;

		// Set directly, if any CPU free
		int nextCPU = findNextAvailableCPU();
		while (nextCPU != -1) {
			CPUs[nextCPU] = new ThreadDescriptorBlock;
			*CPUs[nextCPU] = curr;
			nextCPU = findNextAvailableCPU();
			if (nextCPU != -1) {
				tmpPtr = getThreadSRT();
				if (tmpPtr != nullptr) {
					curr = *tmpPtr;
					continue;
				}
				else
					return true;
			}
			else
				return true;
		}



		// IF CPU not Free, check
		//cout << "  -->No CPU Free\n";

		// Find CPU with longest RT thread
		
		
		
			int idx = getCPUThreadLRT();
			int longest = CPUs[idx]->remaining_time;
			// Compare thread with Longest RT  in CPU and the incoming thread
			if (longest > curr.remaining_time) {
				ThreadDescriptorBlock useless = *CPUs[idx];
				buffer.push(useless);
				CPUs[idx] = new ThreadDescriptorBlock;
				*CPUs[idx] = curr;
				//cout << "    STRFwP: CPU #" << idx << " switched to Thread " << curr.tid << endl;
			}
			else {
				buffer.push(curr);
				break;
			}
		}
	}
	break;
		
	case PBS:		//Priority Based Scheduling, with preemption - AC
		{
			while (true) {
				//cout << "PBS\n";
				ThreadDescriptorBlock curr;

				// Get Thread with highest priority among threads arrived
				ThreadDescriptorBlock *tmpPtr;
				tmpPtr = getHighestPriorityThread();
				if (tmpPtr == nullptr) {
					//cout << "No thread incoming" << endl;
					return true;
				}
				curr = *tmpPtr;
				//cout << "Incoming thread: #" << curr.tid << endl;

				// Set directly, if any CPU free
				int nextCPU = findNextAvailableCPU();
				while (nextCPU != -1) {
					CPUs[nextCPU] = new ThreadDescriptorBlock;
					*CPUs[nextCPU] = curr;
					nextCPU = findNextAvailableCPU();
					if (nextCPU != -1) {
						tmpPtr = getThreadSRT();
						if (tmpPtr != nullptr) {
							curr = *tmpPtr;
							continue;
						}
						else
							return true;
					}
					else
						return true;
				}

				// IF CPU not Free, check
				//cout << "-->No CPU Free\n";

				// Find CPU with lowest priority thread
				int idx = getCPUThreadLowestPriority();
				int lowest = CPUs[idx]->priority;

				// Compare thread with lowest priority in CPU and the incoming thread
				if (lowest > curr.priority) {
					ThreadDescriptorBlock useless = *CPUs[idx];
					buffer.push(useless);
					CPUs[idx] = new ThreadDescriptorBlock;
					*CPUs[idx] = curr;
					//cout << "    PBS: CPU #" << idx << " switched to Thread " << curr.tid << endl;
				}
				else {
					buffer.push(curr);
					break;
				}
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


//----------------------------------------------------------------------------------
// Function:	findNextAvailableCPU()
// Objective:	To find free CPU if any
// Input:		No
// Output:		return index of CPU if found, -1 otherwise
//----------------------------------------------------------------------------------
int MyScheduler::findNextAvailableCPU()
{
	for (unsigned int i = 0; i < num_cpu; i++) {
		if (CPUs[i] == NULL) {return i;}
	}
	// If no CPU available, return -1
	return -1;
}

//----------------------------------------------------------------------------------
// Function:	getHighestPriorityThread()
// Objective:	To find the thread with highest priority in the buffer
// Input:		No
// Output:		return nullptr if not found; return pointer to the thread if found
//----------------------------------------------------------------------------------
ThreadDescriptorBlock *MyScheduler::getHighestPriorityThread() {
	
	if (buffer.empty()) {
		return (nullptr);
	}
	//cout << "Threads arrived at time " << timer << ": " << endl;
	vector<ThreadDescriptorBlock> threads_tmp;
	ThreadDescriptorBlock curr;
	ThreadDescriptorBlock result;
	//bool isFound = false;

	// Get all threads arrived before timer
	while (!buffer.empty()) {
		curr = buffer.top();
		if (curr.arriving_time > timer) break;
		buffer.pop();
		threads_tmp.push_back(curr);
		//cout << "thread #" << curr.tid << endl;
	}
	
	if (threads_tmp.empty()) {
		return nullptr;
	}

	// Find the thread with the highest priority
	int prior = 10000000;
	int idx = 0;

	for(int i = 0;i<threads_tmp.size();i++){
		if (threads_tmp[i].priority < prior) {
			prior = threads_tmp[i].priority;
			idx = i;
		}
	}
	result = threads_tmp[idx];
	threads_tmp.erase(threads_tmp.begin() + idx);

	// Push other threads back to buffer
	for (int i = 0; i < threads_tmp.size(); i++) {
		buffer.push(threads_tmp[i]);
	}
	//cout << 4;

	// return if find
	return &result;
}

//----------------------------------------------------------------------------------
// Function:	getThreadSRT()
// Objective:	To find the thread with SRT in the buffer
// Input:		No
// Output:		return nullptr if not found; return pointer to the thread if found
//----------------------------------------------------------------------------------
ThreadDescriptorBlock *MyScheduler::getThreadSRT(){

	if (buffer.empty()) {
		return (nullptr);
	}
	//cout << "Threads arrived at time " << timer << ": " << endl;
	vector<ThreadDescriptorBlock> threads_tmp;
	ThreadDescriptorBlock curr;
	ThreadDescriptorBlock result;
	//bool isFound = false;

	// Get all threads arrived before timer
	while (!buffer.empty()) {
		curr = buffer.top();
		if (curr.arriving_time > timer) break;
		buffer.pop();
		threads_tmp.push_back(curr);
		//cout << "thread #" << curr.tid << endl;
	}

	if (threads_tmp.empty()) {
		return nullptr;
	}

	// Find the thread with the SRT
	int shortest = 100000;
	int idx = 0;

	for (int i = 0; i<threads_tmp.size(); i++) {
		if (threads_tmp[i].remaining_time< shortest) {
			shortest = threads_tmp[i].remaining_time;
			idx = i;
		}
	}
	result = threads_tmp[idx];
	threads_tmp.erase(threads_tmp.begin() + idx);

	// Push other threads back to buffer
	for (int i = 0; i < threads_tmp.size(); i++) {
		buffer.push(threads_tmp[i]);
	}
	//cout << 4;

	// return if find
	return &result;
}

//----------------------------------------------------------------------------------
// Function:	getCPUThreadLowestPriority()
// Objective:	To find the threads with lowest priority in CPU
// Input:		No
// Output:		return index of CPU
//----------------------------------------------------------------------------------
int MyScheduler::getCPUThreadLowestPriority() {
	
	int index = -1;
	int lowest = -1;

	for (int i = 0; i < num_cpu; i++) {
		if (CPUs[i]->priority > lowest) {
			lowest = CPUs[i]->priority;
			index = i;
		}
	}
	return (index);
		
}

//----------------------------------------------------------------------------------
// Function:	getCPUThreadLowestLRT()
// Objective:	To find the threads with longest remaining time in CPU
// Input:		No
// Output:		return index of CPU
//----------------------------------------------------------------------------------
int MyScheduler::getCPUThreadLRT() {
	int index = -1;
	int longest = -1;

	for (int i = 0; i < num_cpu; i++) {
		if (CPUs[i]->remaining_time > longest) {
			longest = CPUs[i]->remaining_time;
			index = i;
		}
	}
	return (index);
}

//----------------------------------------------------------------------------------
// Function:	isFreeAllCPU() 
// Objective:	To check whether all CPU finifshing working
// Input:		No
// Output:		return true if so
//----------------------------------------------------------------------------------
bool MyScheduler::isFreeAllCPU() {
	//To check whether all CPUs finish working
	for (int i = 0; i < num_cpu; i++) {
		if (CPUs[i] != NULL) {
			return false;
		}
	}
	return true;
}
// Project1.cpp : Defines the entry point for the console application.
//Aiyu Cui, Rex Li, Weiming Shan


#include "myscheduler.h"

int main(int argc, char* argv[])
{
	cout << "FCFS Policy, 2 CPUs\n";
	MyScheduler ms1 = MyScheduler(Policy::FCFS, 2);
	ms1.CreateThread(0, 12, 0, 0);
	ms1.CreateThread(5, 2, 2, 1);
	ms1.CreateThread(3, 7, 1, 2);
	ms1.CreateThread(9, 1, 10, 3);
	ms1.CreateThread(1, 4, 3, 4);
	ms1.CreateThread(5, 2, 3, 5);
	ms1.CreateThread(1, 4, 3, 6);

	ms1.Go();

	/* My results, assuming my project works...
	Thread ID : 4 Finishing Time : 5 CPU No. : 1
	Thread ID : 6 Finishing Time : 9 CPU No. : 1
	Thread ID : 0 Finishing Time : 12 CPU No. : 0
	Thread ID : 1 Finishing Time : 14 CPU No. : 0
	Thread ID : 5 Finishing Time : 16 CPU No. : 0
	Thread ID : 2 Finishing Time : 16 CPU No. : 1
	Thread ID : 3 Finishing Time : 17 CPU No. : 0
	*/

	cout << "STRFwoP Policy, 3 CPUs\n";
	MyScheduler ms2 = MyScheduler(Policy::STRFwoP, 3);
	ms2.CreateThread(0, 12, 0, 0);
	ms2.CreateThread(5, 2, 2, 1);
	ms2.CreateThread(3, 7, 1, 2);
	ms2.CreateThread(9, 1, 10, 3);
	ms2.CreateThread(1, 4, 3, 4);
	ms2.CreateThread(5, 2, 3, 5);
	ms2.CreateThread(1, 4, 3, 6);

	ms2.Go();

	/*
	Thread ID : 4 Finishing Time : 5 CPU No. : 1
	Thread ID : 6 Finishing Time : 5 CPU No. : 2
	Thread ID : 1 Finishing Time : 7 CPU No. : 1
	Thread ID : 5 Finishing Time : 7 CPU No. : 2
	Thread ID : 3 Finishing Time : 10 CPU No. : 2
	Thread ID : 0 Finishing Time : 12 CPU No. : 0
	Thread ID : 2 Finishing Time : 14 CPU No. : 1
	All the Threads have been executed !! .. Exiting Scheduler ..
	*/
	cout << "STRFwP Policy, 3 CPUs\n";
	MyScheduler ms3 = MyScheduler(Policy::STRFwP, 3);
	ms3.CreateThread(0, 12, 0, 0);
	ms3.CreateThread(5, 2, 2, 1);
	ms3.CreateThread(3, 7, 1, 2);
	ms3.CreateThread(9, 1, 10, 3);
	ms3.CreateThread(1, 4, 3, 4);
	ms3.CreateThread(5, 2, 3, 5);
	ms3.CreateThread(1, 4, 3, 6);

	ms3.Go();

	/*
	Thread ID : 6 Finishing Time : 5 CPU No. : 0
	Thread ID : 4 Finishing Time : 5 CPU No. : 1
	Thread ID : 5 Finishing Time : 7 CPU No. : 0
	Thread ID : 1 Finishing Time : 7 CPU No. : 1
	Thread ID : 2 Finishing Time : 10 CPU No. : 0
	Thread ID : 3 Finishing Time : 10 CPU No. : 1
	Thread ID : 0 Finishing Time : 16 CPU No. : 0
	All the Threads have been executed !! .. Exiting Scheduler ..
	*/

	cout << "PBS Policy, 3 CPUs\n";
	MyScheduler ms4 = MyScheduler(Policy::PBS, 3);
	ms4.CreateThread(0, 12, 5, 0);
	ms4.CreateThread(5, 2, 2, 1);
	ms4.CreateThread(3, 7, 4, 2);
	ms4.CreateThread(9, 1, 10, 3);
	ms4.CreateThread(1, 4, 3, 4);
	ms4.CreateThread(5, 2, 3, 5);
	ms4.CreateThread(1, 4, 3, 6);

	ms4.Go();

	/*
	Thread ID : 6 Finishing Time : 5 CPU No. : 0
	Thread ID : 4 Finishing Time : 5 CPU No. : 1
	Thread ID : 1 Finishing Time : 7 CPU No. : 0
	Thread ID : 5 Finishing Time : 7 CPU No. : 1
	Thread ID : 2 Finishing Time : 10 CPU No. : 0
	Thread ID : 3 Finishing Time : 10 CPU No. : 2
	Thread ID : 0 Finishing Time : 16 CPU No. : 0
	All the Threads have been executed !! .. Exiting Scheduler ..
	*/



	//Our TB
/*
#include "myscheduler.h"
#include <iostream>
int main(int argc, char* argv[])
{
	int flag = 0;
	//srand(time(NULL));
	//flag = rand() % 100 + 1; //random number between 1 - 100 

	int arrivingTime[5] = { 1, 3, 4, 2, 5 };
	int remainingTime[5] = { 4, 8, 3, 5, 7 };
	int priority[5] = { 1, 8, 4, 5, 1 };

	MyScheduler Scheduler1(FCFS, 2);
	MyScheduler Scheduler2(STRFwoP, 2);
	MyScheduler Scheduler3(PBS, 2);
	MyScheduler Scheduler4(STRFwP, 2);
	
	for (int i = 0; i < 5; i++) //5 threads
	{
		//Scheduler1.CreateThread(arrivingTime[i], remainingTime[i], priority[i], i); 
		//Scheduler2.CreateThread(arrivingTime[i], remainingTime[i], priority[i], i); 
		
		//Scheduler3.CreateThread(arrivingTime[i], remainingTime[i], priority[i], i);
		Scheduler4.CreateThread(arrivingTime[i], remainingTime[i], priority[i], i);

	}
	
	//Scheduler1.Go();
	//Scheduler2.Go();
	//Scheduler3.Go();
	Scheduler4.Go();

	return 0;
	*/
}


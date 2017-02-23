// Project1.cpp : Defines the entry point for the console application.
//Aiyu Cui, Rex Li, Weiming Shan

#include "myscheduler.h"
#include <iostream>
int main(int argc, char* argv[])
{
	int flag = 0;
	//srand(time(NULL));
	//flag = rand() % 100 + 1; //random number between 1 - 100 

	int arrivingTime[5] = { 5, 7, 8, 1, 3 };
	int remainingTime[5] = { 10, 15, 3, 8, 13 };
	int priority[5] = { 4, 3, 3, 2, 1 };

	MyScheduler Scheduler1(FCFS, 2);
	for (int i = 0; i < 5; i++) //5 threads
	{
		Scheduler1.CreateThread(arrivingTime[i], remainingTime[i], priority[i], i); 
	}
	Scheduler1.Go();
	return 0;
	
}


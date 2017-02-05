// PriorityQueue.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "BinaryHeap.h"
#include <iostream>
#include <random>
#include <ctime>


using namespace std;


int main()
{
	/*
	int min = 1, max = 10;

	std::mt19937 rng(time(NULL));
	std::uniform_int_distribution<int> gen(min, max); // uniform, unbiased

	//generate random numbers in range 1 to 10 and add them to heap
	
	BinaryHeap<Job<int>> h = BinaryHeap<Job<int>>();
	
	while (h.Count() < 20) {
		
		Job<int> job;
		job.priority = gen(rng);
		job.data = gen(rng);
		h.Enqueue( job );
	}
	

	cout << "Heap is as below" << endl;
	cout << h.Print();

	unsigned int count = 0;
	h.Count(9,0,count);

	cout << "Count of nodes of priority 9 is" << count << endl;

	
	unsigned int heapSize = h.Count();
	for (unsigned int i = 0; i < heapSize; i++) {
		if (h.Count() > 0) {
			Job<int> maxPriority = h.Dequeue();
			cout << "Max Priority Element is [" << maxPriority.priority << "]" << maxPriority.data << endl;
			cout << "New Heap is as below" << endl;
			cout << h.Print();
		}
	}
	*/
	

	//Unit tests
	UnitTests unit;	
	unit.TestPriorityQueue();


	getchar();
    return 0;
}


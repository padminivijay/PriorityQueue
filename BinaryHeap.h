#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <utility>
#include <sstream>
#include <ostream>
#include "windows.h"

//generic class to hold items of UINT priority to added to priority queue
template <class T>
class Job {

public:
	unsigned int priority;
	T data;
};

//Binary Heap class to implement priority queue. Each node on the heap is an object of BHNode class. BHNode class maintains node priority and it contains 
// a FIFO queue with items of similar priority
//The BinaryHeap class has the node with the highest priority at the root i.e. this heap class implements MaxHeap


template <class T>
class BinaryHeap {

public:
	BinaryHeap() : qSize(0) {
		InitializeCriticalSection(&lpCriticalSection);
	}

	~BinaryHeap() {
		DeleteCriticalSection(&lpCriticalSection);
	}

	void Enqueue( T item) {

		//Using critical section for thread safety
		EnterCriticalSection(&lpCriticalSection);

		priorityQue.push_back(item);
		qSize++;
		unsigned int index = qSize - 1;
		while (index > 0 && priorityQue[index].priority  > priorityQue[ParentIndex(index)].priority) {
			priorityQue[index] = priorityQue[ParentIndex(index)];
			priorityQue[ParentIndex(index)] = item;
			index = ParentIndex(index);
		}

		LeaveCriticalSection(&lpCriticalSection);
	}

	unsigned int ParentIndex(unsigned int index) {

		if (index <= 0 || index >= qSize)
			return -1;
		return (index - 1) / 2;

	}



	unsigned int LeftChildIndex(unsigned int index) {

		unsigned int left = 2 * index + 1;
		if (left >= qSize)
			return -1;

		return left;

	}

	unsigned int RightChildIndex(unsigned int index) {

		unsigned int right = 2 * index + 2;
		if (right >= qSize)
			return -1;

		return right;
	}

	//Returns the item with highest priority, removes it from heap and
	//restores head order
	//the function removes the the first element from the heap which has the highest priority,
	//and find the element in the back of the heap and put it in position occupied by first element.
	//and restores heap order based on priority
	T Dequeue() {

		//Using critical section for thread safety
		EnterCriticalSection(&lpCriticalSection);
		//if (qSize == 0)
		//	return;
		//The element with highest priority will be the first element in the heap.
		
		T maxPriorityJob = priorityQue[0];

		//place the element from the back of the queue to front of queue
		priorityQue[0] = priorityQue[qSize - 1];

		//delete the last element 		
		priorityQue.erase(priorityQue.begin() + qSize - 1);
		qSize--;

		//precolate down to restore heap order
		PercolateDown(0);
		LeaveCriticalSection(&lpCriticalSection);

		return maxPriorityJob;
	}

	//Restores heap order by iterating through the heap in top-down fashion
	void PercolateDown(unsigned int index) {


		unsigned int left, right, max;

		left = LeftChildIndex(index);
		right = RightChildIndex(index);

		if (left != -1 && priorityQue[left].priority >= priorityQue[index].priority)
			max = left;
		else
			max = index;

		if (right != -1 && priorityQue[right].priority >= priorityQue[max].priority)
			max = right;


		if (max != index) {
			//Swap index and max elements
			T temp = priorityQue[index];
			priorityQue[index] = priorityQue[max];
			priorityQue[max] = temp;
		}
		else
			return;

		PercolateDown(max);
	}


	std::string Print(unsigned int index = 0) {

		std::ostringstream actual;
		unsigned int level = 0;
		for (unsigned int i = 0; i < qSize; ) {

			unsigned int numnodes = pow(2, level);
			for (unsigned int j = 0; j < numnodes && i < qSize; j++) {
				T item = priorityQue[i++];
				actual << "[" << item.priority << "]" << item.data << "\t";
			}
			actual << endl;
			level++;
		}
		return actual.str();
	}

	//Returns the counts of elements by priority
	void Count(unsigned int priority, unsigned int index, unsigned int &count) {

		unsigned int left, right, max;
		
		
		if (index  > qSize)
			return;

		left = LeftChildIndex(index);
		right = RightChildIndex(index);

		
		if (left != -1 && priorityQue[left].priority == priority) {
			index = left;
			count++;
			Count(priority, index,count);			
		}

		if (right != -1 && priorityQue[right].priority == priority) {
			index = right;
			count++;
			Count(priority, index, count);
		}

		if (left != -1 && priorityQue[left].priority > priority)
			max = left;
		else
			max = index;
		

		if (right != -1 && priorityQue[right].priority > priority)
			max = right;

		if( index != max)
			Count(priority, max, count);		
	}


	unsigned int Count() { return priorityQue.size(); }

private:
	//vector to hold pair of elements. The first element in the pair is the priority for the element 
	//and second is templated class of T for data.
	std::vector< T > priorityQue;
	unsigned int qSize;
	CRITICAL_SECTION lpCriticalSection;
};



class UnitTests {

public:
	UnitTests() {
	}
	
	bool TestPriorityQueue() {

		//input order of jobs with their priority
		unsigned int parr_in[] = { 9, 1, 3, 5, 9, 3 };
		int data_in[] = { 12.0, 18.0, 12.0, 20.0, 34.0, 36.0 };

		//expected order of job with their priority
		unsigned int parr_exp[] = { 9, 9, 5, 3, 3, 1 };
		int data_exp[] = { 12.0, 34.0, 20.0, 12.0, 36.0, 18.0 };

		for (int i = 0; i < 6; i++) {
			Job<int> job;
			job.priority = parr_in[i];
			job.data = data_in[i];
			heap.Enqueue(job);
		}
		
		//Test the count of nodes by priority
		unsigned int count;
		heap.Count(9, 0, count);

		if (count == 2)
			std::cout << "Correctly counted the number of items with priority 9" << std::endl;
		else
			std::cout << "Failed to correctly count the number of nodes with priority 9" << std::endl;

		for (int i = 0; i < 6; i++) {

			Job<int> job = heap.Dequeue();

			if (job.priority == parr_exp[i] && job.data == data_exp[i])
				std::cout << "Success: Retrieved Max Priority job with priority " << job.priority << " and data " << job.data << std::endl;
			else
				std::cout << "Failed to return next highest priority job" << std::endl;
		}		

		return true;		
	}


	BinaryHeap<Job<int>> heap;
	
};
	

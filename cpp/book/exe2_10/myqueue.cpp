#include <iostream>
#include "myqueue.h"

using namespace std;

void q_type::init()
{
	head = tail = 0;
}

void q_type::q(int num)
{
	if(tail + 1 == head){
		cout << "Queue is full" << endl;
		return ;
	}

	tail++;
	if(tail == SIZE) tail = 0;//从队列头重新开始;
	queue[tail] = num;
}

int q_type::deq()
{
	if(head == tail){
		cout << "Queue is empty" << endl;
		return 0;
	}

	head++;
	if(head == SIZE) head = 0;
	return queue[head];
}



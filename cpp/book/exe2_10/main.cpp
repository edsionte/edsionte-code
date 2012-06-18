#include <iostream>
#include "myqueue.h"

using namespace std;

int main()
{
	q_type q1, q2;
	int i;

	q1.init();
	q2.init();

	for(i = 1; i <= 10; i++){
		q1.q(i);
		q2.q(i * i);
	}

	for(i = 1; i <= 10; i++){
		cout << "Dequeue 1: " << q1.deq() << endl;
		cout << "Dequeue 2: "  << q2.deq() << endl;
	}

	return 0;
}

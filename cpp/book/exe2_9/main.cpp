#include <iostream>
#include "time.h"

using namespace std;

int main()
{
	//当使用Time t1;这样的声明时，使用的是无参数的构造函数；
	Time t1;
	t1.print();
	t1.setTime(7, 45, 34);
	t1.print();

	Time t2(8, 30, 23);
	t2.print();
	t2.setTime(t2.getHour() + 1, t2.getMinute() + 10, 45);
	t2.print();

	return 0;
}

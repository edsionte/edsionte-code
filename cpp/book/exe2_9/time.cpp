#include <iostream>
#include "time.h"

using namespace std;

Time::Time()
{
	hour = 0;
	minute = 0;
	second = 0;
}
Time::Time(int h, int m, int s)
{
	hour = h;
	minute = m;
	second = s;
}

void Time::print()
{
	cout << "The Time is:  " << hour << ":" << minute << ":" << second << endl;
}

int Time::getHour()
{
	return hour;
}

int Time::getMinute()
{
	return minute;
}

int Time::getSecond()
{
	return second;
}

void Time::setTime(int h, int m, int s)
{
	hour = h;
	minute = m;
	second = s;
}

#include <iostream>
#include <string.h>
#include "student.h"

using namespace std;

//静态变量必须再次声明;

int student::count;
float student::sum;

student::student(int number1, const char *name1, float score1)
{
	number = number1;
	name = new char[strlen(name1)+1];
	strcpy(name, name1);
	score = score1;
	count++;
	sum = sum + score;
}

student::~student()
{
	delete [] name;
	count--;
}

void student::print()
{
	cout << "Student's info:" << endl;
	cout << "numbner:" << number << endl;
	cout << "name:" << name << endl;
	cout << "score:" << score << endl;
	cout << "count:" << count << endl;
}

float student::average()
{
	cout << "sum is " << sum << ", count is " << count << endl;
	cout << "average is " << sum/count << endl;
	return sum/count;
}
/*
int main()
{
	student::init();
	student stu1(1, "edsionte", 89);
	stu1.print();

	student stu2(3, "Zhang yun", 99);
	stu2.print();

	student stu3(4, "Wang sen", 86);
	stu3.print();

	student::average();

	return 0;
}
*/

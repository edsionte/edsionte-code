#include <iostream>
#include <string.h>
#include "student.h"

using namespace std;

//class student

student::student(int anumber, char *aname, float x, float y, float z) : myscore(x, y, z)
{
	number = anumber;
	name = new char[strlen(aname)+1];
	strcpy(name, aname);
}

student::~student()
{
	delete [] name;
}

void student::print()
{
	cout << "Student's info:" << endl;
	cout << "numbner:" << number << endl;
	cout << "name:" << name << endl;
	cout << "score:" << endl;
	myscore.print();
}

//class score

score::score()
{
	computer = 0;
	english = 0;
	math = 0;
}

score::score(float x, float y, float z)
{
	computer = x;
	english = y;
	math = z;
}

void score::print()
{
	cout << "computer:" << computer << ",english:" << english << ",math:" << math << endl;
}

void score::modify(float x, float y, float z)
{
	computer = x;
	english = y;
	math = z;
}

int main()
{
	int numberi;
	char namei[15];
	float computeri, englishi, mathi;

	cout << "Enter number:" << endl;
	cin >> numberi;
	cout << "Enter name:" << endl;
	cin >> namei;
	cout << "Enter computer's score:" << endl;
	cin >> computeri;
	cout << "Enter english's score:" << endl;
	cin >> englishi;
	cout << "Enter math's score:" << endl;
	cin >> mathi;
	student stu(numberi, namei, computeri, englishi, mathi);
	stu.print();

	return 0;

}

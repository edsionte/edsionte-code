#include <iostream>
#include <string.h>
#include "student.h"

using namespace std;

student::student(int number1, char *name1, float score1)
{
	number = number1;
	name = new char[strlen(name1)+1];
	strcpy(name, name1);
	score = score1;
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
	cout << "score:" << score << endl;
}

int main()
{
	int numberi;
	char namei[15];
	float scorei;

	cout << "Enter number:" << endl;
	cin >> numberi;
	cout << "Enter name:" << endl;
	cin >> namei;
	cout << "Enter score:" << endl;
	cin >> scorei;
	student stu1(numberi, namei, scorei);
	
	cout << "Enter number:" << endl;
	cin >> numberi;
	cout << "Enter name:" << endl;
	cin >> namei;
	student stu2(numberi, namei, scorei);
	stu2.modify(97);

	stu1.print();
	stu2.print();

	return 0;

}

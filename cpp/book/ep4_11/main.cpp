#include <iostream>
#include <cstring>
#include "student.h"

using namespace std;

int main()
{
	student stu1(1, "edsionte", 89);
	stu1.print();

	student stu2(3, "Zhang yun", 99);
	stu2.print();

	student stu3(4, "Wang sen", 86);
	stu3.print();

	student::average();

	return 0;
}

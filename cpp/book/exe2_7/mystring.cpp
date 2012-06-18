#include <iostream>
#include <cstring>
#include "mystring.h"

using namespace std;

mystring::mystring(const char *astring)
{
	contents = new char[strlen(astring) + 1];
	strcpy(contents, astring);
}

void mystring::print()
{
	cout << "The string is:" << contents << endl;
}

int mystring::getLength()
{
	return strlen(contents);
}

char *mystring::getString()
{
	return contents;
}

char *mystring::append(char *appendStr)
{
	char *tmp = contents;
	contents = new char[strlen(tmp) + strlen(appendStr) +1];
	strcpy(contents, tmp);
	strcat(contents, appendStr);
	delete [] tmp;
	return contents;
}

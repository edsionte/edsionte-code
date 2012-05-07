#include <iostream>
using namespace std;

class myclass
{
	private:
         	int a, b;
	public:
		void set(int i, int j)
		{
			a = i;
			b = j;
		}
		void show()
		{
			cout << a << ' ' << b << endl;
		}
};

class yourclass
{
	private:
         	int a, b;
	public:
		void set(int i, int j)
		{
			a = i;
			b = j;
		}
		void show()
		{
			cout << a << ' ' << b << endl;
		}

};

int main()
{
	myclass obj1, obj2;
	yourclass obj3;

	obj1.set(11, 19);
	obj1.show();

	obj2 = obj1;
	obj2.show();

	/*
	   error equality

	obj3 = obj2;

	*/

	return 0;
}


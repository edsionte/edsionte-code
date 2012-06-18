#include <iostream>

using namespace std;

class A;
void fun(A *p);

class A
{
	public:
		void display()
		{
			cout << "call function A" << endl;
		}

		int x;
};

class B : public A
{
	public:
		void display()
		{
			cout << "call function B" << endl;
		}

		int y;
};

class C : public B
{
	public:
		void display()
		{
			cout << "call function C" << endl;
		}
};

int  main()
{
	A a;
	B b;
	C c;

	c.x = 1;
	c.y = 2;

	a = c;
	b = c;
	cout << "a.x=" << a.x << "  a.y=" << b.y << endl;
	cout << "b.x=" << a.x << "  b.y=" << b.y << endl;

	b.x = 10;
	b.y = 20;
	a = b;
	cout << "a.x=" << a.x << "  b.y=" << b.y << endl;
	
	A *ap;
	ap = &a;
	fun(ap);
	
	ap = &b;
	fun(ap);
	
	ap = &c;
	fun(ap);

	return 0;
}

void fun(A *p)
{
	p->display();
}

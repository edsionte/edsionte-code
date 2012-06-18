#include <iostream>
using namespace std;

class A
{
	private:
		int a;
	public:
		A(int ai)
		{
			cout << "constructing class A" << endl;
			a = ai;
		}
		~A()
		{
			cout << "destructing class A" << endl;
		}

		void showa()
		{
			cout << "A:" << a << endl;
		}
};

class C
{
	private:
		int c;
	public:
		C(int ci)
		{
			cout << "constructing class C" << endl;
			c = ci;
		}
		~C()
		{
			cout << "destructing class C" << endl;
		}
		void showc()
		{
			cout << "C:" << c << endl;
		}
};

class B : public A
{
	private:
		C myc;
		int b;
	public:
		B(int bi, int ai, int ci) : A(ai), myc(ci) 
	        {
			cout << "constructing class B" << endl;
			b = bi;
		}
		~B()
		{
			cout << "destructing class B" << endl;
		}
		void showb()
		{
			cout << "B:" << b << endl;
			myc.showc();
		}
};

int main()
{
	B b(5, 6, 7);
	b.showa();
	b.showb();
	return 0;
}

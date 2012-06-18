#include <iostream>
using namespace std;

class Square
{
	public:
		void set(int x)
		{
			side = x;
		}
		void print()
		{
			cout << side << endl;
		}
		void f_value(Square x);
	        void f_reference(Square &x);
		void f_pointer(Square *x);
		/*
		void f(Square x);
		void f(Square &x);
		void f(Square *x);
		*/
	
	private:
		int side;

};

void Square::f_value(Square x)
{
	cout << "In function:" << endl;
	x.print();
	x.set(100);
	x.print();
}

void Square::f_reference(Square &x)
{
	cout << "In function:" << endl;
	x.print();
	x.set(100);
	x.print();
}

void Square::f_pointer(Square *x)
{
	cout << "In function:" << endl;
	x->print();
	x->set(100);
	x->print();
}

/*

void f(Square x)
{
	cout << "In function:" << endl;
	x.print();
	x.set(100);
	x.print();
}

void f(Square &x)
{
	cout << "In function:" << endl;
	x.print();
	x.set(100);
	x.print();
}

void f(Square *x)
{
	cout << "In function:" << endl;
	x->print();
	x->set(100);
	x->print();
}

 */

int main()
{
	Square s1, s2, s3;

	cout << "S1" << endl;
	s1.set(10);
	s1.f_value(s1);
	cout << "In main():" << endl;
	s1.print();

	s2.set(10);
	s2.f_reference(s2);
	cout << "In main():" << endl;
	s2.print();
	
	s3.set(10);
	s3.f_pointer(&s3);
	cout << "In main():" << endl;
	s3.print();

	return 0;
}

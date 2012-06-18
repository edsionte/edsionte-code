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
	
	private:
		int side;

};

void f_value(Square x)
{
	cout << "In function:" << endl;
	x.print();
	x.set(100);
	x.print();
}

void f_reference(Square &x)
{
	cout << "In function:" << endl;
	x.print();
	x.set(100);
	x.print();
}

void f_pointer(Square *x)
{
	cout << "In function:" << endl;
	x->print();
	x->set(100);
	x->print();
}

int main()
{
	Square s1, s2, s3;

	cout << "S1" << endl;
	s1.set(10);
	f_value(s1);
	cout << "In main():" << endl;
	s1.print();

	s2.set(10);
	f_reference(s2);
	cout << "In main():" << endl;
	s2.print();
	
	s3.set(10);
	f_pointer(&s3);
	cout << "In main():" << endl;
	s3.print();

	return 0;
}

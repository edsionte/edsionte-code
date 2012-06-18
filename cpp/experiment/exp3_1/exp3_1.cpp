#include <iostream>
using namespace std;

class counter
{
	private:
	int value;
	
	public:
	counter(int number);
	void increment();
	void decrement();
	int getvalue();
	void print();
};

counter::counter(int number)
{
	value = number;
}

void counter::increment()
{
	value++;
}

void counter::decrement()
{
	value--;
}

int counter::getvalue()
{
	return value;
}

void counter::print()
{
	cout << "The value is " << value <<endl;
}

int main()
{
	counter a(10);
	a.print();

	counter *b = new counter(100);
	b->decrement();
	b->print();
	cout << "value = " << b->getvalue() << endl;

	return 0;
}

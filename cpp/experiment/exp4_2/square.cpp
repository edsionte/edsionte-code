#include <iostream>

using namespace std;

class samp
{
	private:
		int i;
	public:
		samp(int n)
		{
			i  = n;
		}
		void set_i(int n)
		{
			i = n;
		}
		int get_i()
		{
			return i;
		}
};

void sqr_it(samp &o)
{
	o.set_i(o.get_i() * o.get_i());
	cout << "Copy of a has i value of " << o.get_i() << endl;
}

int main()
{
	samp a(10);
	sqr_it(a);
	cout << "Copy of a has i value of " << a.get_i() << endl;

	return 0;
}

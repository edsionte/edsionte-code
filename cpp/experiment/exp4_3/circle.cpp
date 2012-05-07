#include <iostream>

using namespace std;

class circle
{
	public:
		circle(float r)
		{
			radius = r;
			count++;
		}
		~circle()
		{
			--count;
		}
		int num()
		{
			return count;
		}

	private:
		float radius;
		static int count;
};

int circle::count = 0;

int main()
{
	circle c1(13.5);
	circle c2(3);
	circle c3(5);

	cout << "The number of circle object is " << c3.num() << endl;

	return 0;
}

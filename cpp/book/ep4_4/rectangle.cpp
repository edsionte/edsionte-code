#include <iostream>
using namespace std;

//本例用来演示对象之间的赋值；赋值以后某个对象对其成员变量的修改不映像其他成员;
class Rectangle 
{
	public:
		Rectangle(int i, int j)
		{
			L = i;
			W = j;
		}
		void showRect();
		void setRect(int length, int width);

	private:
		int L;
		int W;
};

void Rectangle::showRect()
{
	cout << "Length is " << L << endl;
	cout << "Width is " << W << endl;
}

void Rectangle::setRect(int length, int width)
{
	L = length;
	W = width;
}

int main()
{
	Rectangle rect1(10, 20), rect2(0, 0);

	cout << "rect1 before assignment:" << endl;
	rect1.showRect();
	cout << "rect2 before assignment:" << endl;
        rect2.showRect();

	rect2 = rect1;
	
	cout << "After assignment..." << endl;
	cout << "rect1 before assignment:" << endl;
	rect1.showRect();
	cout << "rect2 before assignment:" << endl;
        rect2.showRect();

	cout << "After rect1's Modified..." << endl;
	rect1.setRect(11, 19);
	cout << "rect1 before assignment:" << endl;
	rect1.showRect();
	cout << "rect2 before assignment:" << endl;
        rect2.showRect();

	cout << "After rect2's Modified..." << endl;
	rect2.setRect(19, 87);
	cout << "rect1 before assignment:" << endl;
	rect1.showRect();
	cout << "rect2 before assignment:" << endl;
        rect2.showRect();
	return 0;
}

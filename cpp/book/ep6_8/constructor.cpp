#include <iostream>
#include <cstring>

using namespace std;

class stud
{
	protected:
		int num;
		char name[20];
		char sex;
	public:
		stud(int n, const char nam[], char s)
		{
			num = n;
			strcpy(name, nam);
			sex = s;
		}
		~stud() { }
};

class student : public stud
{
	private:
		int age;
		char addr[30];
	public:
		student(int n, const char nam[], char s, int a, const char ad[])
			: stud(n, nam, s)
		{
			age = a;
			strcpy(addr, ad);
		}

		void show()
		{
			cout << "num:" << num << endl;
			cout << "name:" << name << endl;
			cout << "sex:" << sex << endl;
			cout << "age:" << age << endl;
			cout << "address:" << addr << endl;
		}
		~student() { }
};

int main()
{
	student a(1000, "edsionte wu", 'f', 19, "xiyou");
	student b(1001, "Wang sen", 'f', 20, "xiyou");

	a.show();
	b.show();

	return 0;
}

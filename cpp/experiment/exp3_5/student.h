class score
{
	private:
		float computer;
		float english;
		float math;

	public:
		score(float x, float y, float z);
		score();
		void print();
		void modify(float x, float y, float z);
};

class student
{
	private:
		int number;
		char *name;
		score myscore;
		

	public:
		student(int anumber, char *aname, float x, float y, float z);
		~student();
		void print();
};


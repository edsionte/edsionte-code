class student
{
	private:
		int number;
		char *name;
		float score;
		static int count;
		static float sum;

	public:
		student(int number1, const char *name1, float score1);
		~student();
		void modify(float score1)
		{
			score = score1;
		}
		void print();
		static float average();
};



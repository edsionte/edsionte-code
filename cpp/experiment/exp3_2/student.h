class student
{
	private:
		int number;
		char *name;
		float score;

	public:
		student(int number1, char *name1, float score1);
		~student();
		void modify(float score1)
		{
			score = score1;
		}
		void print();
};



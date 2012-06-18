class Time
{
	public:
		Time();
		Time(int h, int m, int s);
		void setTime(int h, int m, int s);
		void print();
		int getHour();
		int getMinute();
		int getSecond();

	private:
		int hour;
		int minute;
		int second;
};

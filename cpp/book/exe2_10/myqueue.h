#define SIZE 100

class q_type
{
	public:
		void init();
		void q(int num);
		int deq();
	private:
        	int queue[SIZE];
         	int head, tail;
};



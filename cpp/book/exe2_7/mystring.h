class mystring
{
	public:
		mystring(const char *astring);
		void print();
		char *getString();
		int getLength();
		char *append(char *appendStr);

	private:
		char *contents;
		int length;
};

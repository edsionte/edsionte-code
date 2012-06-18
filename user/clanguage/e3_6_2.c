#include <stdio.h>
#include <stdlib.h>

int main()
{
	int i,sum=0;
	int a,b,c,d,e;
	int flag=0;

	for(a=1;a<6;a++)
	{
		for(b=1;b<6;b++)
		{
			for(c=1;c<6;c++)
			{
				for(d=1;d<6;d++)
				{
					for(e=1;e<6;e++)
					{

						sum=0;
						sum=(b==1||a==3)+(b==2||e==4)+(c==1||d==2)+(c==5||d==3)+(e==4||a==1);
						if(sum==5)
						{
							if((a+b+c+d+e)==15)
							{
								if(a!=b&&a!=c&&a!=d&&a!=e&&b!=c&&b!=d&&b!=e&&c!=d&&c!=e&&d!=e)
								{
									printf("a=%d,b=%d,c=%d,d=%d,e=%d\n",a,b,c,d,e);
								}
							}
						}
					}

				}
			}
		}
	}

	return 0;
}

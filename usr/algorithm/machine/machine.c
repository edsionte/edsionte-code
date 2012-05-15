/*
 *Author: edsionte Wu
 *Time  : 2011-6-16
 *Email : edsionte@gmail.com
 *Blog  : edsionte.com
 */

#include <stdio.h>
#include <stdlib.h>

#define MAXNUM 10

void machine_init();
void machine_backtrace(int i);
void machine_output();

int m;//m个供应商
int n;//n个部件
int cost;//不超过该价格
int c[MAXNUM][MAXNUM];//存储第i个供应商提供的第j个部件的价格
int w[MAXNUM][MAXNUM];//存储第i个供应商提供的第j个部件的重量
int cw;//当前部件的重量
int cc;//当前部件的价格
int bestw;//最终的重量
int bestx[MAXNUM];//不超过规定重量的搜索路径
int savex[MAXNUM];//最好的搜索路径


int main()
{
	machine_init();
	machine_backtrace(0);
	machine_output();

	return 0;
	

}

void machine_init()
{
	FILE *fd;
	int i, j;

	//读input.txt文件，初始化相应的值
	if ((fd = fopen("./input.txt", "r")) < 0) {
		printf("read input.txt error\n");
		exit(1);
	}

	//读入m,n,cost
	fscanf(fd, "%d %d %d\n", &m, &n, &cost);
	//读入c数组
	for (i = 0; i < m; i++)
		for (j = 0; j < n; j++) 
			fscanf(fd, "%d ", &c[i][j]);
	//读入w数组
	for (i = 0; i < m; i++)
		for (j = 0; j < n; j++) 
			fscanf(fd, "%d ", &w[i][j]);

	for (i = 0; i < m; i++)
		bestx[i] = -1;
	
	bestw = -1;

	close(fd);
}

void machine_backtrace(int i)
{
        //此时已经到达叶子节点
	if(i >= n) {
	        //有更小的价格或第一次得到满足条件的价格，则更新bestw以及搜索路径
		if(cw <= bestw || bestw == -1) {
			bestw = cw; 
			int j;
                        for(j = 0; j < n; j++)
				//把当前搜过的路径记下来
				savex[j] = bestx[j];
		}
		return;
	}

	//继续向下搜索
	int j;
	for(j = 0 ; j < m; j++) {
		//依次递归尝试每个供应商
		if(cc + c[i][j] <= cost) {
			cc += c[i][j];
                        cw += w[i][j];
                        bestx[i] = j;
                        machine_backtrace(i + 1);
			//减去底层一个节点的路径、价格和重量，继续深度搜索
                        bestx[i] = -1;
                        cc -= c[i][j];
                        cw -= w[i][j];
		}
	}//for
}

void machine_output()
{
	int ccc = 0;
	int k;

	for(k=0; k<n; k++) {
		printf("Part %d from Supply %d..\n", k+1, savex[k] + 1);
		ccc += c[k][savex[k]];
	}
	printf("The lowest machine price is:%d\n", ccc);

}

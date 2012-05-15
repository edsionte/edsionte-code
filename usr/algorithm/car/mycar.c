#include<stdio.h>
#include<stdlib.h>

#define MAXNUM 100

int main()
{
    int N, K, A, B, C;
    int work[MAXNUM][MAXNUM][MAXNUM], map[MAXNUM][MAXNUM];
    int i, j, p, q, r, min;
    int s[4][3];
    FILE *fp, *fw;

    //打开输入文件，并从其中读入相关数据
    if((fp = fopen("input.txt", "r")) < 0) {
    	printf("open error\n");
	exit(1);
    }

    fscanf(fp,"%d%d%d%d%d",&N, &K, &A, &B, &C);
    
    for(i = 0; i < N; i++)
        for(j = 0; j < N; j++)
            fscanf(fp, "%d", &map[i][j]);

    for(i = 0; i < N; i++)
        for(j = 0; j < N; j++)
            for(p = 0; p <= K + 1; p++)
                work[i][j][p] = 1000000;

    //work[i][j][p]指汽车从(i, j)走p段路所花费的代价
    //从起始点开始的K段路程不花费任何代价
    for(i = 0; i <= K; i++)
        work[0][0][i] = 0;

    //初始化四个方向的收费
    s[0][0] = -1;
    s[0][1] = 0;
    s[0][2] = 0;
    s[1][0] = 0;
    s[1][1] = -1;
    s[1][2] = 0;
    s[2][0] = 1;
    s[2][1] = 0;
    s[2][2] = B;
    s[3][0] = 0;
    s[3][1] = 1;
    s[3][2] = B;

        for(i = 0; i < N; i++) {//for_N1
            for(j = 0; j < N; j++) {//for_N2
                if(i != 0 || j != 0) {//if
                   
		    for(p = 0; p <= K; p++) {//for_p
                        min = 1000000;
                        for(q = 0; q < 4; q++) {
			    //依次遍历四个方向
                            if(i == 0 && q == 0)
                                continue;
                            if(j == 0 && q == 1)
                                continue;
                            if(i == N-1 && q == 2)
                                continue;
                            if(j == N - 1 && q == 3)
                                continue;
			    //判断四个方向的代价是否小于当前最小代价min
                            if(work[ i + s[q][0] ][ j + s[q][1] ][ p + 1 ] + s[q][2] < min) {
                                min = work[ i + s[q][0] ][ j + s[q][1] ][ p + 1 ] + s[q][2];
                            }
                        }

                        work[i][j][p] = min;
 
			//如果当前坐标有加油站
			if(map[i][j] == 1) {
			    //加上加油费
                            work[i][j][p] += A;
			    //加油后从该点起始的K段路内都不需要额外费用
                            for(q = 1; q <= K; q++)
                                work[i][j][q] = work[i][j][0];
                            break;

                        } else if(work[i][j][p] == 1000000) {
                            //说明此处没有加油站需要建立加油站并且加油
			    work[i][j][p] = work[i][j][0] + C + A;
                            for(q = p + 1; q <= K; q++)  {
                                work[i][j][q] = work[i][j][p];
                            }
                            break;
                        }
                    }//for_p

                }//if
            }//for_N2
        }//for_N1

    fw = fopen("output.txt", "w");
    printf("%d\n", work[N-1][N-1][0]);
    fprintf(fw,"%d\n", work[N-1][N-1][0]);
    return 0;
}

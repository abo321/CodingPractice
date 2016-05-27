/************************************************************************
 * 该算法实现求N个数中M个数的组合(N>=M>0)
 ************************************************************************/

#include <stdlib.h>
#include <STDIO.H>

#define MAXNUMBER 20

void main()
{
	int a[MAXNUMBER+1]={0};	//存放N个原始数据
	int b[MAXNUMBER+1]={0};	//存放M个数据的下标
	int n,m;				//求组合数C(n,m)
	int i;
	int count=0;

	printf("please input n (n<=15) : ");
	scanf("%d",&n);

	printf("please input %d numbers : ",n);
	for(i=1;i<=n;i++)
		scanf("%d",a+i);	//数据从下标为1的位置开始存放

	printf("please input m (m<=n)  : ");
	scanf("%d",&m);

	printf("combinations of m numbers of n are:\n");

	//i作为数组的下标,数据存放在数组中,取其组合实际上就是取不同的下标
	//相当于有n位数像表的各个针一样在变化
	/* 例如:5个数中取3个
	 * 1 2 3
	 * 1 2 4
	 * 1 2 5
	 * 1 2 3 —— 1 3 3 —— 1 3 4 : 要求后面的下标值比前面的大
	 * 最后一个总是从m到n,但到n后恢复原值m,且其前的下标值进1,
	 */

	//下标值初始化
	//最开始下标值为1,2,3,...,m	
	for(i=1;i<=m;i++)
		b[i]=i;

	int j;
	
	//然后下标从最后一个开始转,并逐个向前进1,直到第1个下标值循环结束
	while(1)
	{
		j=m;

		//第j个下标从m到n值循环
		for(i=b[j];i<=n;i++)
		{
			//输出该组合
			for(int k=1;k<=m;k++)
				printf("%6d",a[b[k]]);
			printf("\n");

			count++;

			b[j]++;
		}

		//第j个下标值循环到n后结束,向前进1,即第j-1个下标值加1
		//第j个下标值必须满足b[j]<=n-m+j
		while(b[j]>n-m+j)
		{
			j--;
			b[j]++;
		}

		if(j==0)
			break;

		//且第j个下标值及其后的所有下标值复位
		for(int k=j+1;k<=m;k++)
			b[k]=b[k-1]+1;
	}

	printf("count = %d\n",count);
}
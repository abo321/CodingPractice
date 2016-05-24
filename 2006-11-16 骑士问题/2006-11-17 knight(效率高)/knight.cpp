/************************************************************************
 *  骑士遍历任意两点,求其最短路径
 ************************************************************************/

#include <STDIO.H>
#include <STDLIB.H>
#include <CONIO.H>
#include <string.h>

#define MAXLENGTH 8			//棋盘边长
#define MAXSTEP 8			//最大步数
#define MAXTEST 30			//测试文件的示例个数
#define FILENAMELENGTH 100	//文件名长度
#define MAXMINSTEP 6		//最短步长最大数

//定义棋盘点(i,j),i 为行,j为列
typedef struct 
{
	int i;		//棋盘上点行下标
	int j;		//棋盘上点列下标
}ChessPoint;

//定义测试文件的输入数据结构
typedef struct 
{
	char srcpoint[3];	//源点,例如 a2
	char destpoint[3];	//目的点,例如 g6
	int srci,srcj;		//源点,例如 a2 对应下标(1,2)
	int desti,destj;	//目的点,例如 g6 对应下标(7,6)
}TestCase;

int chess[MAXLENGTH+1][MAXLENGTH+1]={0};	//棋盘,行列下标均从1开始
int offseti[9]={0,-2,-1,+1,+2,+2,+1,-1,-2};	//i下标增量数组
int offsetj[9]={0,+1,+2,+2,+1,-1,-2,-2,-1};	//j下标增量数组

/************************************************************************
 * 方向数组,存放已经走过的每一步的走向
 *     该数组初始化为0,第0号单元不用,第i个单元存放第i步的走向
 *     当前活动单元索引号为step
 ************************************************************************/
int direction[MAXSTEP]={0};

/************************************************************************
 * 路径数组,存放已经走过的每一步走向到达的点
 *     该数组初始化为0,第0号单元存放源点(srci,srcj)
 *     第i个单元存放第i-1个单元的点沿direction[i]方向走一步后的新点
 *     当前活动单元索引号为step
 ************************************************************************/
ChessPoint path[MAXSTEP];

//定义测试文件的输入数据
TestCase testcase[MAXTEST];

/************************************************************************
 * 函数说明: 
 *     knight 在点(i,j) 沿着 k 方向走一步,到达新点 (newi,newj)
 * 参数说明:
 *     i    : 走一步前的行下标 (i>=1 and i<=8)
 *     j    : 走一步前的列下标 (j>=1 and j<=8)
 *     k    : 方向 (k>=1 and k<=8)
 *     newi : 走一步后的行下标 (newi>=1 and newi<=8)
 *     newj : 走一步后的列下标 (newj>=1 and newj<=8)
 ************************************************************************/
void new_point_at_direction(int i,int j,int k,int &newi,int &newj)
{
	newi=i+offseti[k];
	newj=j+offsetj[k];
}

/************************************************************************
 * 函数说明: 
 *     判断 knight 到达的新点 (newi,newj) 是否在棋盘上
 * 参数说明:
 *     newi : 新点行下标 (newi>=1 and newi<=8)
 *     newj : 新点列下标 (newj>=1 and newj<=8)
 * 返回值说明:
 *     true : 新点在棋盘上
 *     false: 新点不在棋盘上
 ************************************************************************/
bool new_point_is_on_chessboard(int newi,int newj)
{
	if((newi>=1 && newi<=MAXLENGTH) && (newj>=1 && newj<=MAXLENGTH))
		return true;
	return false;
}

//求解前初始化各种参数初始化
void initialize()
{
	for(int i=0;i<MAXLENGTH+1;i++)
		for(int j=0;j<MAXLENGTH+1;j++)
			chess[i][j]=0;

	for(int k=0;k<MAXSTEP;k++)
		path[k].i=path[k].j=0;
}

/************************************************************************
 * 函数说明: 
 *     骑士knight 从源点(srci,srcj) 走到目的点(desti,destj)
 * 参数说明:
 *     srci  : 源点行下标 (srci>=1 and srci<=8)
 *     srcj  : 源点列下标 (srcj>=1 and srcj<=8)
 *     desti : 目的行下标 (desti>=1 and desti<=8)
 *     destj : 目的列下标 (destj>=1 and destj<=8)
 * 返回值说明
 *     minstep : 所求源点到目的点最短路径
 ************************************************************************/
int knight_walk(int srci,int srcj,int desti,int destj)
{
	//初始化棋盘和其他参数
	initialize();

	if(srci==desti && srcj==destj)
		return 0;

	int k=0;	//初始化方向,每个点都从第1个方向开始搜索
	int i,j,newi,newj,step;
	int minstep=MAXMINSTEP;	//当前最短路径

	chess[srci][srcj]=1;	
	path[0].i=srci;
	path[0].j=srcj;

	step=1;	//先走第一步
	i=srci;
	j=srcj;

	while(step>0)
	{
		k++;	//搜索下一个方向

		while(k==9)
		{
			//回溯并记录上一步当前出发点
			step--;
			if(step==0)
				break;

			//记录上一步的方向
			k=direction[step];

			//取消上一步到达的点的赋值
			chess[path[step].i][path[step].j]=0;

			i=path[step-1].i;
			j=path[step-1].j;

			//准备搜索下一个方向
			k++;
		}

		if(step==0)
			break;

		//得到新点
		new_point_at_direction(i,j,k,newi,newj);
		//找到一个走后的新点仍在棋盘上的方向
		while(new_point_is_on_chessboard(newi,newj)==false)
		{
			k++;	//搜索下一个方向
			if(k==9)
				break;
			new_point_at_direction(i,j,k,newi,newj);
		}

		//8个方向都已经搜索
		if(k==9)
		{
			k=8;
			continue;
		}

		//若该点已经访问过
		if(chess[newi][newj]!=0)
			continue;

		//若到达目的点,记录该路径,结束本次搜索,启动下一次搜索
		if(newi==desti && newj==destj)
		{
			//记录该步
			direction[step]=k;
			path[step].i=newi;
			path[step].j=newj;
			chess[newi][newj]=step+1;

			//////////////////////////////////////////////////////////////////////////
			//求当前最短路径
			if(minstep>step)
				minstep=step;

			//////////////////////////////////////////////////////////////////////////
			//第1种回溯,回2步
			//该步已经到达,其他的不需要再搜索,只需回溯
			k=direction[step-1];

			//取消上一步到达的点的赋值
			chess[path[step].i][path[step].j]=0;
			chess[path[step-1].i][path[step-1].j]=0;

			//回溯并记录上一步当前出发点
			step--;
			i=path[step-1].i;
			j=path[step-1].j;
			//////////////////////////////////////////////////////////////////////////
			
			continue;
		}

		//////////////////////////////////////////////////////////////////////////
		//最快的一般不会超过6步,故若在第6步已经试探了所有8个方向还没有到达
		//则回溯,搜索下一个方向,在第6步相当于广度优先搜索
		//即在第6步还有其他方向没有搜索,则继续
		//若求出当前最短路径了,再求出的路径大于此值的路径多可以忽略
		//if(step==MAXMINSTEP)
		if(step==minstep)
			continue;
		//////////////////////////////////////////////////////////////////////////

		//记录该步
		direction[step]=k;
		path[step].i=newi;
		path[step].j=newj;

		//记录第step步到达的位置
		i=newi;
		j=newj;
		chess[i][j]=step+1;

		//开始下一步搜索
		step++;
		k=0;
	}

	return minstep;
}

/************************************************************************
 * 函数说明
 *     读测试文件
 * 参数说明
 *     filename : 文件名
 * 返回值
 *     testCount: 测试文件中的测试示例个数
 ************************************************************************/
int read_test_file(char *filename)
{
	FILE *fp;
	char line[10];	//存放每一行的示例,如 a1 b2
	int testCount=0;

	fp=fopen(filename,"r");
	if(fp==NULL)
	{
		printf("can not open the test file!\n");
		exit(0);
	}

	//读取每一行测试数据并存入测试数据数组中
	while(fgets(line,10,fp))
	{
		TestCase test;
		test.srcpoint[0]=line[0];
		test.srcpoint[1]=line[1];
		test.srcpoint[2]=0;

		test.destpoint[0]=line[3];
		test.destpoint[1]=line[4];
		test.destpoint[2]=0;

		test.srcj=test.srcpoint[0]-96;		//字符a,b,c,d,e,f,g,h转换为1,2,3,4,5,6,7,8
		test.srci=test.srcpoint[1]-48;		//字符1,2,3,4,5,6,7,8转换为1,2,3,4,5,6,7,8

		test.destj=test.destpoint[0]-96;	//字符a,b,c,d,e,f,g,h转换为1,2,3,4,5,6,7,8
		test.desti=test.destpoint[1]-48;	//字符1,2,3,4,5,6,7,8转换为1,2,3,4,5,6,7,8

		testcase[testCount++]=test;
	}

	fclose(fp);
	return testCount;
}

//显示菜单
void show_menu()
{
	printf("---------------------------------------------\n");
	printf("input command to test the program\n");
	printf("     f or F : test data from a file \n");
	printf("     p or P : test data by input point\n");
	printf("     q or Q : quit\n");
	printf("---------------------------------------------\n");
	printf("$ input command >");
}

/************************************************************************
 * 函数说明
 *     输出测试结果文件
 * 参数说明
 *     filename : 测试示例文件名
 *     nMinStep : 测试示例最短路径数组
 *     nCaseCount : 测试示例个数
 ************************************************************************/
void output_test_file(char *filename,int nMinStep[],int nCaseCount)
{
	char fileresult[FILENAMELENGTH];

	//求得结果文件的名字
	strcpy(fileresult,filename);
	int len=strlen(fileresult);
	fileresult[len-4]=0;
	strcat(fileresult,"_result.txt");

	//打开结果文件
	FILE *fp;
	fp=fopen(fileresult,"w");
	if(fp==NULL)
	{
		printf("can not open file : %s\n",fileresult);
		exit(0);
	}

	printf("\n");
	for(int i=0;i<nCaseCount;i++)
	{
		printf("To get from %s to %s takes %d knight moves.\n",testcase[i].srcpoint,testcase[i].destpoint,nMinStep[i]);
		fprintf(fp,"To get from %s to %s takes %d knight moves.\n",testcase[i].srcpoint,testcase[i].destpoint,nMinStep[i]);
	}

	printf("\n");
	fclose(fp);
}

//处理测试文件
void process_test_file(char *filename)
{
	//打开文件并返回文件中的测试示例个数
	int nCaseCount=read_test_file(filename);
	int nMinStep[MAXTEST];

	//求解
	for(int i=0;i<nCaseCount;i++)
		nMinStep[i]=knight_walk(testcase[i].srci,testcase[i].srcj,testcase[i].desti,testcase[i].destj);

	//输出
	output_test_file(filename,nMinStep,nCaseCount);
}

main()
{
	int srcrow,srccol,destrow,destcol;
	char filename[FILENAMELENGTH];
	char sinput[10];

	show_menu();

	scanf("%s",sinput);
	while(stricmp(sinput,"q")!=0)
	{
		if(stricmp(sinput,"p")==0)
		{
			printf("$ input src point :  col(a~h) >");
			scanf("%s",sinput);				
			srccol=sinput[0]-96;

			printf("$ input src point :  row(1~8) >");
			scanf("%d",&srcrow);

			printf("$ input dest point : col(a~h) >");
			scanf("%s",sinput);
			destcol=sinput[0]-96;

			printf("$ input dest point : row(1~8) >");
			scanf("%d",&destrow);

			//求解
			int nMinStep=knight_walk(srcrow,srccol,destrow,destcol);
			//int nMinStep=min_step();
			
			printf("\nTo get from %c%d to %c%d takes %d knight moves.\n\n",srccol+96,srcrow,destcol+96,destrow,nMinStep);
		}
		else if(stricmp(sinput,"f")==0)
		{
			printf("$ input test file name >");
			scanf("%s",filename);

			//处理测试文件
			process_test_file(filename);
		}

		//输入命令
		printf("$ input command >");
		scanf("%s",sinput);
	}

	return 0;
}
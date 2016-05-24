/************************************************************************
 *  骑士周游问题,从任意一点出发,沿汉密尔顿路回到源点
 ************************************************************************/

#include <STDIO.H>
#include <STDLIB.H>
#include <CONIO.H>
#include <string.h>

#define MAXLENGTH 6			//棋盘边长
#define MAXSTEP 70			//最大步数
#define MAXPATH 10000000	//所有路径最多个数
#define FILENAMELENGTH 100	//文件名长度
#define MAXMINSTEP 6		//最短步长最大数

//定义棋盘点(i,j),i 为行,j为列
typedef struct 
{
	int i;		//棋盘上点行下标
	int j;		//棋盘上点列下标
}ChessPoint;

//定义路径数据结构
typedef struct
{
	ChessPoint curpath[MAXSTEP];	//该条路径的路径数组
	int curstep;					//该条路径的步数
}ALLPATH;

//定义出口
typedef struct
{
	int k;		//方向
	int newi;	//当前点沿该方向走后的新点行下标
	int newj;	//当前点沿该方向走后的新点列下标
	int count;	//该方向出口个数
}WAYOUT;

int chess[MAXLENGTH+1][MAXLENGTH+1]={0};	//棋盘,行列下标均从1开始
int offseti[9]={0,-2,-1,+1,+2,+2,+1,-1,-2};	//i下标增量数组
int offsetj[9]={0,+1,+2,+2,+1,-1,-2,-2,-1};	//j下标增量数组
int pathcount=0;			//路径的个数
int pathstep[MAXPATH]={0};	//每一条路径步数数组
int maxpathcount=0;			//路径个数最大值
int maxminstep=0;			//最短路径个数最大值

/************************************************************************
 * 方向数组,存放已经走过的每一步的走向
 *     该数组初始化为0,第0号单元不用,第i个单元存放第i步的走向
 *     当前活动单元索引号为step
 ************************************************************************/
int direction[MAXSTEP]={0};
int directionindex[MAXSTEP]={0};

/************************************************************************
 * 路径数组,存放已经走过的每一步走向到达的点
 *     该数组初始化为0,第0号单元存放源点(srci,srcj)
 *     第i个单元存放第i-1个单元的点沿direction[i]方向走一步后的新点
 *     当前活动单元索引号为step
 ************************************************************************/
ChessPoint path[MAXSTEP];

/************************************************************************
 * 任意两点间所有路径数组,存放两点间的每一条路径
 *     该两点间的路径总数存于pathcount变量中
 ************************************************************************/
ALLPATH allpath[MAXPATH];

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
	pathcount=0;

	for(int i=0;i<MAXLENGTH+1;i++)
		for(int j=0;j<MAXLENGTH+1;j++)
			chess[i][j]=0;

	for(int k=0;k<MAXPATH;k++)
		pathstep[k]=0;

	for(k=0;k<MAXSTEP;k++)
		path[k].i=path[k].j=0;
}

/************************************************************************
 * 函数说明: 计算点(i,j)的出口个数
 * 参数说明:
 *     i   : 当前点横下标
 *     j   : 当前点纵下标
 * 返回值说明:
 *     count : 当前点(i,j)的出口个数
 *      -1 : 该点不在棋盘上,或者已经走过
 *       0 : 该点在棋盘上,但没有出口
 *      >0 : 该点的出口个数
 ************************************************************************/
int wayout_number(int i,int j)
{
	int count=0;
	int newi,newj;

	//若该点不在棋盘上,或者已经走过,返回-1
	if(i<1 || i>MAXLENGTH || j<1 || j>MAXLENGTH || chess[i][j]>0)
		return -1;

	for(int k=1;k<=8;k++)
	{
		newi=i+offseti[k];
		newj=j+offsetj[k];
		
		//该点不在棋盘上,继续试探
		if(newi<1 || newi>MAXLENGTH || newj<1 || newj>MAXLENGTH)
			continue;

		//否则若该点没有被走过,则该点可为出口
		if(chess[newi][newj]==0)
			count++;
	}

	return count;
}

/************************************************************************
 * 函数说明: 
 *     将当前点的出口按从少到多排序
 * 参数说明:
 *     curi  : 当前点行下标 (curi>=1 and curi<=8)
 *     curj  : 当前点下标 (curj>=1 and curj<=8)
 *     way   : 出口数组
 *     n     : 数组大小
 * 返回值:
 *     kindex: 出口最少的方向在出口数组中的索引
 ************************************************************************/
int sort_wayout(int curi,int curj,WAYOUT way[],int n)
{
	for(int k=1;k<=8;k++)
	{
		//k方向的新点
		way[k].k=k;
		way[k].newi=curi+offseti[k];
		way[k].newj=curj+offsetj[k];

		//计算该点的出口个数
		way[k].count=wayout_number(way[k].newi,way[k].newj);
	}

	int t;
	WAYOUT temp;
	//按出口从少到多排序
	for(int i=1;i<=8;i++)
    {
		t=i;		
        for(int j=i+1;j<=8;j++)
            if(way[j].count<way[t].count)
				t=j;
        if(t>i)
        {
            temp=way[i];
            way[i]=way[t];
            way[t]=temp;
        }
	}

	//找到第一个可用出口
	for(i=1;way[i].count<=0 && i<=8;i++);

	return i;
}

/************************************************************************
 * 函数说明: 
 *     骑士knight 从源点(srci,srcj) 经汉密尔顿路后回到源点
 * 参数说明:
 *     srci  : 源点行下标 (srci>=1 and srci<=8)
 *     srcj  : 源点列下标 (srcj>=1 and srcj<=8)
 *     minstep : 从源点到目的点的最短路径步数
 ************************************************************************/
int knight_tour(int srci,int srcj)
{
	WAYOUT wayout[9];

	//初始化棋盘和其他参数
	initialize();

	//初始化方向索引,每个点都从出口最少的方向开始搜索
	//每个点的出口都按出口从少到多存放在数组中
	int kindex=0;

	int i,j,newi,newj,step;

	pathcount=0;
	chess[srci][srcj]=1;
	
	path[0].i=srci;
	path[0].j=srcj;

	step=1;	//先走第一步
	i=srci;
	j=srcj;

	while(step>0)
	{
		//计算当前点出口并排序
		kindex=sort_wayout(i,j,wayout,9)-1;

		kindex++;	//搜索下一个方向

		while(kindex==9)
		{
			//回溯并记录上一步当前出发点
			step--;
			if(step==0)
				break;

			//记录上一步的方向
			//kindex=direction[step];
			kindex=directionindex[step];

			//取消上一步到达的点的赋值
			chess[path[step].i][path[step].j]=0;

			i=path[step-1].i;
			j=path[step-1].j;

			//准备搜索下一个方向
			kindex++;
		}

		if(step==0)
			break;

		//8个方向都已经搜索
		if(kindex==9)
		{
			kindex=8;
			continue;
		}

		//得到新点
		newi=wayout[kindex].newi;
		newj=wayout[kindex].newj;

		//若已经找到一条,结束本次搜索,启动下一次搜索
		if(step==64)
		{
			//记录该步
			direction[step]=wayout[kindex].k;
			directionindex[step]=kindex;
			path[step].i=newi;
			path[step].j=newj;

			//记录第pathcount条路径走step步
			//pathstep[pathcount++]=step;					
			chess[newi][newj]=step+1;

			printf("pathcount = %d\n",++pathcount);

			//////////////////////////////////////////////////////////////////////////			
			//将当前路径存于所有路径数组中
/*
			allpath[pathcount-1].curstep=step;
			for(int m=0;m<=step;m++)
				allpath[pathcount-1].curpath[m]=path[m];
*/
			//////////////////////////////////////////////////////////////////////////
						
			//////////////////////////////////////////////////////////////////////////
			//第1种回溯,回2步
			//该步已经到达,其他的不需要再搜索,只需回溯
			//kindex=direction[step-1];
			kindex=directionindex[step-1];

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

		//记录该步
		direction[step]=wayout[kindex].k;
		directionindex[step]=kindex;
		path[step].i=newi;
		path[step].j=newj;

		//记录第step步到达的位置
		i=newi;
		j=newj;
		chess[i][j]=step+1;

		//开始下一步搜索
		step++;
//		kindex=0;
	}

	return pathcount;
}

//显示菜单
void show_menu()
{
	printf("---------------------------------------------\n");
	printf("input command to test the program\n");
	printf("   g or G : get hamilton path count\n");
	printf("   w or W : get hamilton path and write\n");
	printf("   p or P : get hamilton path by input\n");
	printf("   q or Q : quit\n");
	printf("---------------------------------------------\n");
	printf("$ input command >");
}

/************************************************************************
 * 函数说明: 
 *     将每一个点到其他64个点路径个数写入文件
 * 参数说明:
 *     srci  : 源点行下标 (srci>=1 and srci<=8)
 *     srcj  : 源点列下标 (srcj>=1 and srcj<=8)
 *     a     : 源点(srci,srcj) 到其他64 个点的路径个数数组
 ************************************************************************/
void write_path_count(int srci,int srcj,int a[][MAXLENGTH+1])
{
	FILE *fp;
	char filename[FILENAMELENGTH];
	sprintf(filename,"path number from (%d,%d) to others.txt",srci,srcj);

	fp=fopen(filename,"w");
	if(fp==NULL)
	{
		printf("can not wirte file!");
		exit(0);
	}

	for(int i=1;i<=MAXLENGTH;i++)
	{
		for(int j=1;j<=MAXLENGTH;j++)
			fprintf(fp,"%-5d",a[i][j]);
		fprintf(fp,"\n");
	}
	
	fclose(fp);
}

/************************************************************************
 * 函数说明: 
 *     将每一个点到其他64个点最短路径及其个数写入文件
 * 参数说明:
 *     srci  : 源点行下标 (srci>=1 and srci<=8)
 *     srcj  : 源点列下标 (srcj>=1 and srcj<=8)
 *     aMinStep : 源点(srci,srcj) 到其他64 个点的最短路径数组
 *     aMinStepCount : 源点(srci,srcj) 到其他64 个点的最短路径个数数组
 ************************************************************************/
void write_min_step_count(int srci,int srcj,int aMinStep[][MAXLENGTH+1],
	int aMinStepCount[][MAXLENGTH+1])
{
	FILE *fp;
	char filename[FILENAMELENGTH];
	sprintf(filename,"min step from (%d,%d) to others.txt",srci,srcj);

	fp=fopen(filename,"w");
	if(fp==NULL)
	{
		printf("can not wirte file!");
		exit(0);
	}

	//写入提示,(a - b),a 为最短路径,b为最短路径的个数
	fprintf(fp,"(a - b), a is the min step, b is the number of min path\n\n");

	for(int i=1;i<=MAXLENGTH;i++)
	{
		for(int j=1;j<=MAXLENGTH;j++)
			fprintf(fp,"(%d - %-3d)  ",aMinStep[i][j],aMinStepCount[i][j]);
		fprintf(fp,"\n");
	}
	
	fclose(fp);
}

//得到任意一点的汉密尔顿路
void get_hamilton_path()
{
	int i,j;
	for(i=1;i<=MAXLENGTH;i++)
	{
		for(j=1;j<=MAXLENGTH;j++)
		{
			//求(srci,srcj) 到(desti,destj) 最短路径
			knight_tour(i,j);


		}
		printf(".");
	}
	printf("\ncalculation finished!\n");
}

//得到任意一点的汉密尔顿路并写入文件
void get_hamilton_path_and_write_to_file()
{
	int i,j;

	//所有路径个数
	int aPathCount[MAXLENGTH+1][MAXLENGTH+1]={0};

	//最短路径步数
	int aMinStep[MAXLENGTH+1][MAXLENGTH+1]={0};

	for(i=1;i<=MAXLENGTH;i++)
	{
		for(j=1;j<=MAXLENGTH;j++)
		{
			knight_tour(i,j);

			//将计算结果写入文件
//			write_path_count(i,j,aPathCount);
//			write_min_step_count(i,j,aMinStep,aMinStepCount);

			printf(".");
		}
	}
	printf("\ncalculation finished!\n");
}

//初始化棋盘
void initialize_chess()
{
	for(int i=0;i<MAXLENGTH+1;i++)
		for(int j=0;j<MAXLENGTH+1;j++)
			chess[i][j]=0;
}

/************************************************************************
 * 函数说明: 
 *     输出棋盘上的路径
 * 参数说明:
 *     pathnumber : 路径编号
 *     step       : 第 pathcount 条路径的步数
 ************************************************************************/
void output_chess(int pathnumber,int step)
{
	printf("path no. = %d, step = %d\n",pathnumber,step);
	for(int i=1;i<MAXLENGTH+1;i++)
	{
		for(int j=1;j<MAXLENGTH+1;j++)
			printf("%-3d",chess[i][j]);
		printf("\n");
	}
	printf("\n");
}

/************************************************************************
 * 函数说明: 
 *     输出棋盘上的路径
 * 参数说明:
 *     pathnumber : 路径编号
 *     step       : 第 pathcount 条路径的步数
 *     fp         : 文件指针
 ************************************************************************/
void output_chess(int pathnumber,int step,FILE *fp)
{
	fprintf(fp,"path no. = %d, step = %d\n",pathnumber,step);
	for(int i=1;i<MAXLENGTH+1;i++)
	{
		for(int j=1;j<MAXLENGTH+1;j++)
			fprintf(fp,"%-3d",chess[i][j]);
		fprintf(fp,"\n");
	}
	fprintf(fp,"\n");
}

/************************************************************************
 * 函数说明: 
 *     输出knight 从(srci,srcj) 到(desti,destj) 的所有长度<=6的路径
 * 参数说明:
 *     srci  : 源点行下标 (srci>=1 and srci<=8)
 *     srcj  : 源点列下标 (srcj>=1 and srcj<=8)
 *     desti : 目的行下标 (desti>=1 and desti<=8)
 *     destj : 目的列下标 (destj>=1 and destj<=8)
 *     pathcount : 求源点到目的点间最短路径搜索到的路径个数
 *     nMinStep  : 源点到目的点间最短路径
 *     nMinStepCount : 源点到目的点间最短路径的个数
 ************************************************************************/
void output_all_path(int srci,int srcj,int desti,int destj,int pathcount,int nMinStep,int nMinStepCount)
{
	int step,i,j,curi,curj;
	char filename[FILENAMELENGTH];
	
	//构造文件名
	sprintf(filename,"(%d,%d) to (%d,%d) - all path.txt",srci,srcj,desti,destj);
	FILE *fp=fopen(filename,"w");
	if(fp==NULL)
	{
		printf("can not wirte file!");
		exit(0);
	}

	//写入要搜索的路径个数
	fprintf(fp,"(%d,%d) to (%d,%d)\n\tpath number searched =%d\n",srci,srcj,desti,destj,pathcount);

	//写入最短路径及其个数
	fprintf(fp,"\tmin step = %d\n",nMinStep);
	fprintf(fp,"\tmin path number = %d\n\n",nMinStepCount);

	for(curi=0;curi<pathcount;curi++)
	{
		//初始化棋盘
		initialize_chess();
		
		step=allpath[curi].curstep;
		for(curj=0;curj<=step;curj++)
		{
			//第curi条路径的第curj步的行、列下标
			i=allpath[curi].curpath[curj].i;
			j=allpath[curi].curpath[curj].j;
			//给棋盘赋值
			chess[i][j]=curj+1;
		}

		//输出棋盘到屏幕
		output_chess(curi+1,step);

		//输出棋盘到文件
		output_chess(curi+1,step,fp);
	}

	fclose(fp);
}

/************************************************************************
 * 函数说明: 
 *     输出knight 从(srci,srcj) 到(desti,destj) 的最短路径
 * 参数说明:
 *     srci  : 源点行下标 (srci>=1 and srci<=8)
 *     srcj  : 源点列下标 (srcj>=1 and srcj<=8)
 *     desti : 目的行下标 (desti>=1 and desti<=8)
 *     destj : 目的列下标 (destj>=1 and destj<=8)
 *     pathcount : 求源点到目的点间最短路径搜索到的路径个数
 *     nMinStep  : 源点到目的点间最短路径
 *     nMinStepCount : 源点到目的点间最短路径的个数
 ************************************************************************/
void output_min_path(int srci,int srcj,int desti,int destj,int pathcount,int nMinStep,int nMinStepCount)
{
	int step,i,j,curi,curj;
	char filename[FILENAMELENGTH];
	
	//构造文件名
	sprintf(filename,"(%d,%d) to (%d,%d) - min path.txt",srci,srcj,desti,destj);
	FILE *fp=fopen(filename,"w");
	if(fp==NULL)
	{
		printf("can not wirte file!");
		exit(0);
	}

	//写入最短路径及其个数
	fprintf(fp,"(%d,%d) to (%d,%d)\n\tmin step = %d\n",srci,srcj,desti,destj,nMinStep);
	fprintf(fp,"\tmin path number = %d\n\n",nMinStepCount);

	for(curi=0;curi<pathcount;curi++)
	{
		step=allpath[curi].curstep;

		if(step!=nMinStep)
			continue;

		//初始化棋盘
		initialize_chess();

		for(curj=0;curj<=step;curj++)
		{
			//第curi条路径的第curj步的行、列下标
			i=allpath[curi].curpath[curj].i;
			j=allpath[curi].curpath[curj].j;
			//给棋盘赋值
			chess[i][j]=curj+1;
		}

		//输出棋盘到屏幕
		output_chess(curi+1,step);

		//输出棋盘到文件
		output_chess(curi+1,step,fp);
	}

	fclose(fp);
}

main()
{
	int srcrow,srccol;
	char sinput[10];

	show_menu();

	scanf("%s",sinput);
	while(stricmp(sinput,"q")!=0)
	{
		if(stricmp(sinput,"p")==0)
		{
			printf("$ input the point :  col(a~h) >");
			scanf("%s",sinput);				
			srccol=sinput[0]-96;

			printf("$ input the point :  row(1~8) >");
			scanf("%d",&srcrow);

			printf("\n");

			//求解
			int nPathCount=knight_tour(srcrow,srccol);
			//int nMinStepCount=min_step_count(nMinStep);

			//显示要搜索的路径个数及其最短路径个数
			printf("\npath number =%d\n\n",nPathCount);

			printf("$ display All path or Return (A,R)? >");
			scanf("%s",sinput);
			while(stricmp(sinput,"r")!=0)
			{
				if(stricmp(sinput,"a")==0)		//显示所有路径
				{
					//输出所有路径
					//output_all_path(srcrow,srccol,pathcount,nMinStep,nMinStepCount);

					//输入命令
					printf("$ display All path or Return (A,R)? >");
					scanf("%s",sinput);
				}
				else if(stricmp(sinput,"r")==0)
					break;
			}
		}
		else if(stricmp(sinput,"g")==0)
		{
			//得到任意一点的汉密尔回路
			get_hamilton_path();

			printf("max path number searched of all two points: %d\n",maxpathcount);
			printf("max path number of all min step count : %d\n\n",maxminstep);
		}
		else if(stricmp(sinput,"w")==0)
		{
			//得到任意一点的汉密尔回路个数并写入文件
			get_hamilton_path_and_write_to_file();

			printf("max path number searched of all two points: %d\n",maxpathcount);
			printf("max path number of all min step count : %d\n\n",maxminstep);
		}

		//输入命令
		printf("$ input command >");
		scanf("%s",sinput);
	}

	return 0;
}
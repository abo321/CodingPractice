/************************************************************************
 *  ��ʿ��������,������һ�����,�غ��ܶ���·�ص�Դ��
 ************************************************************************/

#include <STDIO.H>
#include <STDLIB.H>
#include <CONIO.H>
#include <string.h>

#define MAXLENGTH 6			//���̱߳�
#define MAXSTEP 70			//�����
#define MAXPATH 10000000	//����·��������
#define FILENAMELENGTH 100	//�ļ�������
#define MAXMINSTEP 6		//��̲��������

//�������̵�(i,j),i Ϊ��,jΪ��
typedef struct 
{
	int i;		//�����ϵ����±�
	int j;		//�����ϵ����±�
}ChessPoint;

//����·�����ݽṹ
typedef struct
{
	ChessPoint curpath[MAXSTEP];	//����·����·������
	int curstep;					//����·���Ĳ���
}ALLPATH;

//�������
typedef struct
{
	int k;		//����
	int newi;	//��ǰ���ظ÷����ߺ���µ����±�
	int newj;	//��ǰ���ظ÷����ߺ���µ����±�
	int count;	//�÷�����ڸ���
}WAYOUT;

int chess[MAXLENGTH+1][MAXLENGTH+1]={0};	//����,�����±����1��ʼ
int offseti[9]={0,-2,-1,+1,+2,+2,+1,-1,-2};	//i�±���������
int offsetj[9]={0,+1,+2,+2,+1,-1,-2,-2,-1};	//j�±���������
int pathcount=0;			//·���ĸ���
int pathstep[MAXPATH]={0};	//ÿһ��·����������
int maxpathcount=0;			//·���������ֵ
int maxminstep=0;			//���·���������ֵ

/************************************************************************
 * ��������,����Ѿ��߹���ÿһ��������
 *     �������ʼ��Ϊ0,��0�ŵ�Ԫ����,��i����Ԫ��ŵ�i��������
 *     ��ǰ���Ԫ������Ϊstep
 ************************************************************************/
int direction[MAXSTEP]={0};
int directionindex[MAXSTEP]={0};

/************************************************************************
 * ·������,����Ѿ��߹���ÿһ�����򵽴�ĵ�
 *     �������ʼ��Ϊ0,��0�ŵ�Ԫ���Դ��(srci,srcj)
 *     ��i����Ԫ��ŵ�i-1����Ԫ�ĵ���direction[i]������һ������µ�
 *     ��ǰ���Ԫ������Ϊstep
 ************************************************************************/
ChessPoint path[MAXSTEP];

/************************************************************************
 * �������������·������,���������ÿһ��·��
 *     ��������·����������pathcount������
 ************************************************************************/
ALLPATH allpath[MAXPATH];

/************************************************************************
 * ����˵��: 
 *     knight �ڵ�(i,j) ���� k ������һ��,�����µ� (newi,newj)
 * ����˵��:
 *     i    : ��һ��ǰ�����±� (i>=1 and i<=8)
 *     j    : ��һ��ǰ�����±� (j>=1 and j<=8)
 *     k    : ���� (k>=1 and k<=8)
 *     newi : ��һ��������±� (newi>=1 and newi<=8)
 *     newj : ��һ��������±� (newj>=1 and newj<=8)
 ************************************************************************/
void new_point_at_direction(int i,int j,int k,int &newi,int &newj)
{
	newi=i+offseti[k];
	newj=j+offsetj[k];
}

/************************************************************************
 * ����˵��: 
 *     �ж� knight ������µ� (newi,newj) �Ƿ���������
 * ����˵��:
 *     newi : �µ����±� (newi>=1 and newi<=8)
 *     newj : �µ����±� (newj>=1 and newj<=8)
 * ����ֵ˵��:
 *     true : �µ���������
 *     false: �µ㲻��������
 ************************************************************************/
bool new_point_is_on_chessboard(int newi,int newj)
{
	if((newi>=1 && newi<=MAXLENGTH) && (newj>=1 && newj<=MAXLENGTH))
		return true;
	return false;
}

//���ǰ��ʼ�����ֲ�����ʼ��
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
 * ����˵��: �����(i,j)�ĳ��ڸ���
 * ����˵��:
 *     i   : ��ǰ����±�
 *     j   : ��ǰ�����±�
 * ����ֵ˵��:
 *     count : ��ǰ��(i,j)�ĳ��ڸ���
 *      -1 : �õ㲻��������,�����Ѿ��߹�
 *       0 : �õ���������,��û�г���
 *      >0 : �õ�ĳ��ڸ���
 ************************************************************************/
int wayout_number(int i,int j)
{
	int count=0;
	int newi,newj;

	//���õ㲻��������,�����Ѿ��߹�,����-1
	if(i<1 || i>MAXLENGTH || j<1 || j>MAXLENGTH || chess[i][j]>0)
		return -1;

	for(int k=1;k<=8;k++)
	{
		newi=i+offseti[k];
		newj=j+offsetj[k];
		
		//�õ㲻��������,������̽
		if(newi<1 || newi>MAXLENGTH || newj<1 || newj>MAXLENGTH)
			continue;

		//�������õ�û�б��߹�,��õ��Ϊ����
		if(chess[newi][newj]==0)
			count++;
	}

	return count;
}

/************************************************************************
 * ����˵��: 
 *     ����ǰ��ĳ��ڰ����ٵ�������
 * ����˵��:
 *     curi  : ��ǰ�����±� (curi>=1 and curi<=8)
 *     curj  : ��ǰ���±� (curj>=1 and curj<=8)
 *     way   : ��������
 *     n     : �����С
 * ����ֵ:
 *     kindex: �������ٵķ����ڳ��������е�����
 ************************************************************************/
int sort_wayout(int curi,int curj,WAYOUT way[],int n)
{
	for(int k=1;k<=8;k++)
	{
		//k������µ�
		way[k].k=k;
		way[k].newi=curi+offseti[k];
		way[k].newj=curj+offsetj[k];

		//����õ�ĳ��ڸ���
		way[k].count=wayout_number(way[k].newi,way[k].newj);
	}

	int t;
	WAYOUT temp;
	//�����ڴ��ٵ�������
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

	//�ҵ���һ�����ó���
	for(i=1;way[i].count<=0 && i<=8;i++);

	return i;
}

/************************************************************************
 * ����˵��: 
 *     ��ʿknight ��Դ��(srci,srcj) �����ܶ���·��ص�Դ��
 * ����˵��:
 *     srci  : Դ�����±� (srci>=1 and srci<=8)
 *     srcj  : Դ�����±� (srcj>=1 and srcj<=8)
 *     minstep : ��Դ�㵽Ŀ�ĵ�����·������
 ************************************************************************/
int knight_tour(int srci,int srcj)
{
	WAYOUT wayout[9];

	//��ʼ�����̺���������
	initialize();

	//��ʼ����������,ÿ���㶼�ӳ������ٵķ���ʼ����
	//ÿ����ĳ��ڶ������ڴ��ٵ�������������
	int kindex=0;

	int i,j,newi,newj,step;

	pathcount=0;
	chess[srci][srcj]=1;
	
	path[0].i=srci;
	path[0].j=srcj;

	step=1;	//���ߵ�һ��
	i=srci;
	j=srcj;

	while(step>0)
	{
		//���㵱ǰ����ڲ�����
		kindex=sort_wayout(i,j,wayout,9)-1;

		kindex++;	//������һ������

		while(kindex==9)
		{
			//���ݲ���¼��һ����ǰ������
			step--;
			if(step==0)
				break;

			//��¼��һ���ķ���
			//kindex=direction[step];
			kindex=directionindex[step];

			//ȡ����һ������ĵ�ĸ�ֵ
			chess[path[step].i][path[step].j]=0;

			i=path[step-1].i;
			j=path[step-1].j;

			//׼��������һ������
			kindex++;
		}

		if(step==0)
			break;

		//8�������Ѿ�����
		if(kindex==9)
		{
			kindex=8;
			continue;
		}

		//�õ��µ�
		newi=wayout[kindex].newi;
		newj=wayout[kindex].newj;

		//���Ѿ��ҵ�һ��,������������,������һ������
		if(step==64)
		{
			//��¼�ò�
			direction[step]=wayout[kindex].k;
			directionindex[step]=kindex;
			path[step].i=newi;
			path[step].j=newj;

			//��¼��pathcount��·����step��
			//pathstep[pathcount++]=step;					
			chess[newi][newj]=step+1;

			printf("pathcount = %d\n",++pathcount);

			//////////////////////////////////////////////////////////////////////////			
			//����ǰ·����������·��������
/*
			allpath[pathcount-1].curstep=step;
			for(int m=0;m<=step;m++)
				allpath[pathcount-1].curpath[m]=path[m];
*/
			//////////////////////////////////////////////////////////////////////////
						
			//////////////////////////////////////////////////////////////////////////
			//��1�ֻ���,��2��
			//�ò��Ѿ�����,�����Ĳ���Ҫ������,ֻ�����
			//kindex=direction[step-1];
			kindex=directionindex[step-1];

			//ȡ����һ������ĵ�ĸ�ֵ
			chess[path[step].i][path[step].j]=0;
			chess[path[step-1].i][path[step-1].j]=0;

			//���ݲ���¼��һ����ǰ������
			step--;
			i=path[step-1].i;
			j=path[step-1].j;
			//////////////////////////////////////////////////////////////////////////
			
			continue;
		}

		//��¼�ò�
		direction[step]=wayout[kindex].k;
		directionindex[step]=kindex;
		path[step].i=newi;
		path[step].j=newj;

		//��¼��step�������λ��
		i=newi;
		j=newj;
		chess[i][j]=step+1;

		//��ʼ��һ������
		step++;
//		kindex=0;
	}

	return pathcount;
}

//��ʾ�˵�
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
 * ����˵��: 
 *     ��ÿһ���㵽����64����·������д���ļ�
 * ����˵��:
 *     srci  : Դ�����±� (srci>=1 and srci<=8)
 *     srcj  : Դ�����±� (srcj>=1 and srcj<=8)
 *     a     : Դ��(srci,srcj) ������64 �����·����������
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
 * ����˵��: 
 *     ��ÿһ���㵽����64�������·���������д���ļ�
 * ����˵��:
 *     srci  : Դ�����±� (srci>=1 and srci<=8)
 *     srcj  : Դ�����±� (srcj>=1 and srcj<=8)
 *     aMinStep : Դ��(srci,srcj) ������64 ��������·������
 *     aMinStepCount : Դ��(srci,srcj) ������64 ��������·����������
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

	//д����ʾ,(a - b),a Ϊ���·��,bΪ���·���ĸ���
	fprintf(fp,"(a - b), a is the min step, b is the number of min path\n\n");

	for(int i=1;i<=MAXLENGTH;i++)
	{
		for(int j=1;j<=MAXLENGTH;j++)
			fprintf(fp,"(%d - %-3d)  ",aMinStep[i][j],aMinStepCount[i][j]);
		fprintf(fp,"\n");
	}
	
	fclose(fp);
}

//�õ�����һ��ĺ��ܶ���·
void get_hamilton_path()
{
	int i,j;
	for(i=1;i<=MAXLENGTH;i++)
	{
		for(j=1;j<=MAXLENGTH;j++)
		{
			//��(srci,srcj) ��(desti,destj) ���·��
			knight_tour(i,j);


		}
		printf(".");
	}
	printf("\ncalculation finished!\n");
}

//�õ�����һ��ĺ��ܶ���·��д���ļ�
void get_hamilton_path_and_write_to_file()
{
	int i,j;

	//����·������
	int aPathCount[MAXLENGTH+1][MAXLENGTH+1]={0};

	//���·������
	int aMinStep[MAXLENGTH+1][MAXLENGTH+1]={0};

	for(i=1;i<=MAXLENGTH;i++)
	{
		for(j=1;j<=MAXLENGTH;j++)
		{
			knight_tour(i,j);

			//��������д���ļ�
//			write_path_count(i,j,aPathCount);
//			write_min_step_count(i,j,aMinStep,aMinStepCount);

			printf(".");
		}
	}
	printf("\ncalculation finished!\n");
}

//��ʼ������
void initialize_chess()
{
	for(int i=0;i<MAXLENGTH+1;i++)
		for(int j=0;j<MAXLENGTH+1;j++)
			chess[i][j]=0;
}

/************************************************************************
 * ����˵��: 
 *     ��������ϵ�·��
 * ����˵��:
 *     pathnumber : ·�����
 *     step       : �� pathcount ��·���Ĳ���
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
 * ����˵��: 
 *     ��������ϵ�·��
 * ����˵��:
 *     pathnumber : ·�����
 *     step       : �� pathcount ��·���Ĳ���
 *     fp         : �ļ�ָ��
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
 * ����˵��: 
 *     ���knight ��(srci,srcj) ��(desti,destj) �����г���<=6��·��
 * ����˵��:
 *     srci  : Դ�����±� (srci>=1 and srci<=8)
 *     srcj  : Դ�����±� (srcj>=1 and srcj<=8)
 *     desti : Ŀ�����±� (desti>=1 and desti<=8)
 *     destj : Ŀ�����±� (destj>=1 and destj<=8)
 *     pathcount : ��Դ�㵽Ŀ�ĵ�����·����������·������
 *     nMinStep  : Դ�㵽Ŀ�ĵ�����·��
 *     nMinStepCount : Դ�㵽Ŀ�ĵ�����·���ĸ���
 ************************************************************************/
void output_all_path(int srci,int srcj,int desti,int destj,int pathcount,int nMinStep,int nMinStepCount)
{
	int step,i,j,curi,curj;
	char filename[FILENAMELENGTH];
	
	//�����ļ���
	sprintf(filename,"(%d,%d) to (%d,%d) - all path.txt",srci,srcj,desti,destj);
	FILE *fp=fopen(filename,"w");
	if(fp==NULL)
	{
		printf("can not wirte file!");
		exit(0);
	}

	//д��Ҫ������·������
	fprintf(fp,"(%d,%d) to (%d,%d)\n\tpath number searched =%d\n",srci,srcj,desti,destj,pathcount);

	//д�����·���������
	fprintf(fp,"\tmin step = %d\n",nMinStep);
	fprintf(fp,"\tmin path number = %d\n\n",nMinStepCount);

	for(curi=0;curi<pathcount;curi++)
	{
		//��ʼ������
		initialize_chess();
		
		step=allpath[curi].curstep;
		for(curj=0;curj<=step;curj++)
		{
			//��curi��·���ĵ�curj�����С����±�
			i=allpath[curi].curpath[curj].i;
			j=allpath[curi].curpath[curj].j;
			//�����̸�ֵ
			chess[i][j]=curj+1;
		}

		//������̵���Ļ
		output_chess(curi+1,step);

		//������̵��ļ�
		output_chess(curi+1,step,fp);
	}

	fclose(fp);
}

/************************************************************************
 * ����˵��: 
 *     ���knight ��(srci,srcj) ��(desti,destj) �����·��
 * ����˵��:
 *     srci  : Դ�����±� (srci>=1 and srci<=8)
 *     srcj  : Դ�����±� (srcj>=1 and srcj<=8)
 *     desti : Ŀ�����±� (desti>=1 and desti<=8)
 *     destj : Ŀ�����±� (destj>=1 and destj<=8)
 *     pathcount : ��Դ�㵽Ŀ�ĵ�����·����������·������
 *     nMinStep  : Դ�㵽Ŀ�ĵ�����·��
 *     nMinStepCount : Դ�㵽Ŀ�ĵ�����·���ĸ���
 ************************************************************************/
void output_min_path(int srci,int srcj,int desti,int destj,int pathcount,int nMinStep,int nMinStepCount)
{
	int step,i,j,curi,curj;
	char filename[FILENAMELENGTH];
	
	//�����ļ���
	sprintf(filename,"(%d,%d) to (%d,%d) - min path.txt",srci,srcj,desti,destj);
	FILE *fp=fopen(filename,"w");
	if(fp==NULL)
	{
		printf("can not wirte file!");
		exit(0);
	}

	//д�����·���������
	fprintf(fp,"(%d,%d) to (%d,%d)\n\tmin step = %d\n",srci,srcj,desti,destj,nMinStep);
	fprintf(fp,"\tmin path number = %d\n\n",nMinStepCount);

	for(curi=0;curi<pathcount;curi++)
	{
		step=allpath[curi].curstep;

		if(step!=nMinStep)
			continue;

		//��ʼ������
		initialize_chess();

		for(curj=0;curj<=step;curj++)
		{
			//��curi��·���ĵ�curj�����С����±�
			i=allpath[curi].curpath[curj].i;
			j=allpath[curi].curpath[curj].j;
			//�����̸�ֵ
			chess[i][j]=curj+1;
		}

		//������̵���Ļ
		output_chess(curi+1,step);

		//������̵��ļ�
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

			//���
			int nPathCount=knight_tour(srcrow,srccol);
			//int nMinStepCount=min_step_count(nMinStep);

			//��ʾҪ������·�������������·������
			printf("\npath number =%d\n\n",nPathCount);

			printf("$ display All path or Return (A,R)? >");
			scanf("%s",sinput);
			while(stricmp(sinput,"r")!=0)
			{
				if(stricmp(sinput,"a")==0)		//��ʾ����·��
				{
					//�������·��
					//output_all_path(srcrow,srccol,pathcount,nMinStep,nMinStepCount);

					//��������
					printf("$ display All path or Return (A,R)? >");
					scanf("%s",sinput);
				}
				else if(stricmp(sinput,"r")==0)
					break;
			}
		}
		else if(stricmp(sinput,"g")==0)
		{
			//�õ�����һ��ĺ��ܶ���·
			get_hamilton_path();

			printf("max path number searched of all two points: %d\n",maxpathcount);
			printf("max path number of all min step count : %d\n\n",maxminstep);
		}
		else if(stricmp(sinput,"w")==0)
		{
			//�õ�����һ��ĺ��ܶ���·������д���ļ�
			get_hamilton_path_and_write_to_file();

			printf("max path number searched of all two points: %d\n",maxpathcount);
			printf("max path number of all min step count : %d\n\n",maxminstep);
		}

		//��������
		printf("$ input command >");
		scanf("%s",sinput);
	}

	return 0;
}
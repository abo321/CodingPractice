/************************************************************************
 *  ��ʿ������������,���䳤��С�ڵ���6��·������,���·���������
 ************************************************************************/

#include <STDIO.H>
#include <STDLIB.H>
#include <CONIO.H>
#include <string.h>

#define MAXLENGTH 8			//���̱߳�
#define MAXSTEP 10			//�����
#define MAXPATH 1000		//����·��������
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
 * ����˵��: 
 *     ��ʿknight ��Դ��(srci,srcj) �ߵ�Ŀ�ĵ�(desti,destj)
 * ����˵��:
 *     srci  : Դ�����±� (srci>=1 and srci<=8)
 *     srcj  : Դ�����±� (srcj>=1 and srcj<=8)
 *     desti : Ŀ�����±� (desti>=1 and desti<=8)
 *     destj : Ŀ�����±� (destj>=1 and destj<=8)
 *     minstep : ��Դ�㵽Ŀ�ĵ�����·������
 ************************************************************************/
int knight_walk(int srci,int srcj,int desti,int destj)
{
	//��ʼ�����̺���������
	initialize();

	if(srci==desti && srcj==destj)
		return 0;

	int k=0;	//��ʼ������,ÿ���㶼�ӵ�1������ʼ����
	int i,j,newi,newj,step;
	int minstep=MAXMINSTEP;	//��ǰ���·��

	pathcount=0;
	chess[srci][srcj]=1;
	
	path[0].i=srci;
	path[0].j=srcj;

	step=1;	//���ߵ�һ��
	i=srci;
	j=srcj;

	while(step>0)
	{
		k++;	//������һ������

		while(k==9)
		{
			//���ݲ���¼��һ����ǰ������
			step--;
			if(step==0)
				break;

			//��¼��һ���ķ���
			k=direction[step];

			//ȡ����һ������ĵ�ĸ�ֵ
			chess[path[step].i][path[step].j]=0;

			i=path[step-1].i;
			j=path[step-1].j;

			//׼��������һ������
			k++;
		}

		if(step==0)
			break;

		//�õ��µ�
		new_point_at_direction(i,j,k,newi,newj);
		//�ҵ�һ���ߺ���µ����������ϵķ���
		while(new_point_is_on_chessboard(newi,newj)==false)
		{
			k++;	//������һ������
			if(k==9)
				break;
			new_point_at_direction(i,j,k,newi,newj);
		}

		//8�������Ѿ�����
		if(k==9)
		{
			k=8;
			continue;
		}

		//���õ��Ѿ����ʹ�
		if(chess[newi][newj]!=0)
			continue;

		//������Ŀ�ĵ�,��¼��·��,������������,������һ������
		if(newi==desti && newj==destj)
		{
			//��¼�ò�
			direction[step]=k;
			path[step].i=newi;
			path[step].j=newj;

			//��¼��pathcount��·����step��
			pathstep[pathcount++]=step;					
			chess[newi][newj]=step+1;
			
			//////////////////////////////////////////////////////////////////////////			
			//����ǰ·����������·��������
			allpath[pathcount-1].curstep=step;
			for(int m=0;m<=step;m++)
				allpath[pathcount-1].curpath[m]=path[m];
			//////////////////////////////////////////////////////////////////////////

			//////////////////////////////////////////////////////////////////////////
			//��ǰ���·��
			if(minstep>step)
				minstep=step;
						
			//////////////////////////////////////////////////////////////////////////
			//��1�ֻ���,��2��
			//�ò��Ѿ�����,�����Ĳ���Ҫ������,ֻ�����
			k=direction[step-1];

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

		//////////////////////////////////////////////////////////////////////////
		//����һ�㲻�ᳬ��6��,�����ڵ�6���Ѿ���̽������8������û�е���
		//�����,������һ������,�ڵ�6���൱�ڹ����������
		//���ڵ�6��������������û������,�����
		//�������ǰ���·����,�������·�����ڴ�ֵ��·������Ժ���
		//if(step==MAXMINSTEP)
		if(step==minstep)
			continue;
		//////////////////////////////////////////////////////////////////////////

		//��¼�ò�
		direction[step]=k;
		path[step].i=newi;
		path[step].j=newj;

		//��¼��step�������λ��
		i=newi;
		j=newj;
		chess[i][j]=step+1;

		//��ʼ��һ������
		step++;
		k=0;
	}

	return minstep;
}

/************************************************************************
 * ����˵��: 
 *     �����·���ĸ���
 * ����˵��:
 *     nMinStep : ���·��
 * ����ֵ˵��:
 *     nMinStepCount : ���·���ĸ���
 ************************************************************************/
int min_step_count(int nMinStep)
{
	int nMinStepCount=0;

	for(int i=0;i<pathcount;i++)
	{
		if(pathstep[i]==nMinStep)
			nMinStepCount++;
	}
	return nMinStepCount;
}

//��ʾ�˵�
void show_menu()
{
	printf("---------------------------------------------\n");
	printf("input command to test the program\n");
	printf("   g or G : get path count (step<=6)\n");
	printf("   w or W : get path count and wirte in files\n");
	printf("   p or P : test data by input\n");
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

//����Ҫ������·������,������·������
void get_path_count()
{
	int srci,srcj,desti,destj;
	int minstep;		//���·��
	int minstepcount;	//���·������

	for(srci=1;srci<=MAXLENGTH;srci++)
	{
		for(srcj=1;srcj<=MAXLENGTH;srcj++)
		{
			for(desti=1;desti<=MAXLENGTH;desti++)
			{
				for(destj=1;destj<=MAXLENGTH;destj++)
				{
					//��(srci,srcj) ��(desti,destj) ���·��
					minstep=knight_walk(srci,srcj,desti,destj);

					//��(srci,srcj) ��(desti,destj) ���·���ĸ���
					minstepcount=min_step_count(minstep);

					//��Ҫ������·���������ֵ
					if(maxpathcount<pathcount)
						maxpathcount=pathcount;

					//�����·�����������ֵ
					if(maxminstep<minstepcount)
						maxminstep=minstepcount;
				}
			}
			printf(".");
		}
	}
	printf("\ncalculation finished!\n");
}

//����Ҫ������·������,������·��������д���ļ�
void get_and_write_path_count()
{
	int srci,srcj,desti,destj;
	int minstep;		//���·��
	int minstepcount;	//���·������

	//����·������
	int aPathCount[MAXLENGTH+1][MAXLENGTH+1]={0};

	//���·������
	int aMinStep[MAXLENGTH+1][MAXLENGTH+1]={0};

	//���·������
	int aMinStepCount[MAXLENGTH+1][MAXLENGTH+1]={0};

	for(srci=1;srci<=MAXLENGTH;srci++)
	{
		for(srcj=1;srcj<=MAXLENGTH;srcj++)
		{
			for(desti=1;desti<=MAXLENGTH;desti++)
			{
				for(destj=1;destj<=MAXLENGTH;destj++)
				{
					//��(srci,srcj) ��(desti,destj) ���·��
					minstep=knight_walk(srci,srcj,desti,destj);
					aPathCount[desti][destj]=pathcount;					
					aMinStep[desti][destj]=minstep;

					//��(srci,srcj) ��(desti,destj) ���·���ĸ���
					minstepcount=min_step_count(minstep);
					aMinStepCount[desti][destj]=minstepcount;

					//��Ҫ������·���������ֵ
					if(maxpathcount<pathcount)
						maxpathcount=pathcount;

					//�����·�����������ֵ
					if(maxminstep<minstepcount)
						maxminstep=minstepcount;
				}
			}
			//��������д���ļ�
			write_path_count(srci,srcj,aPathCount);
			write_min_step_count(srci,srcj,aMinStep,aMinStepCount);

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
	int srcrow,srccol,destrow,destcol;
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

			//���
			int nMinStep=knight_walk(srcrow,srccol,destrow,destcol);
			int nMinStepCount=min_step_count(nMinStep);

			//��ʾ���·��
			printf("\n%c%d to %c%d : min step = %d\n",srccol+96,srcrow,destcol+96,destrow,nMinStep);
			
			//��ʾҪ������·�������������·������
			printf("path number searched =%d, min path number = %d\n\n",pathcount,nMinStepCount);			

			printf("$ display All searched path,Min path or Return (A,M,R)? >");
			scanf("%s",sinput);
			while(stricmp(sinput,"r")!=0)
			{
				if(stricmp(sinput,"a")==0)		//��ʾ����·��
				{
					//�������·��
					output_all_path(srcrow,srccol,destrow,destcol,pathcount,nMinStep,nMinStepCount);

					//��������
					printf("$ display All searched path,Min path or Return (A,M,R)? >");
					scanf("%s",sinput);
				}
				else if(stricmp(sinput,"m")==0)	//��ʾ���·��
				{
					//������·��
					output_min_path(srcrow,srccol,destrow,destcol,pathcount,nMinStep,nMinStepCount);

					//��������
					printf("$ display All searched path,Min path or Return (A,M,R)? >");
					scanf("%s",sinput);
				}
				else if(stricmp(sinput,"r")==0)
					break;
			}
		}
		else if(stricmp(sinput,"g")==0)
		{
			//�õ��������������·������
			get_path_count();

			printf("max path number searched of all two points: %d\n",maxpathcount);
			printf("max path number of all min step count : %d\n\n",maxminstep);
		}
		else if(stricmp(sinput,"w")==0)
		{
			//�õ��������������·��������д���ļ�
			get_and_write_path_count();

			printf("max path number searched of all two points: %d\n",maxpathcount);
			printf("max path number of all min step count : %d\n\n",maxminstep);
		}

		//��������
		printf("$ input command >");
		scanf("%s",sinput);
	}

	return 0;
}
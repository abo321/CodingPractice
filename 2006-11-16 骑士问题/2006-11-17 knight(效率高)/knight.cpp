/************************************************************************
 *  ��ʿ������������,�������·��
 ************************************************************************/

#include <STDIO.H>
#include <STDLIB.H>
#include <CONIO.H>
#include <string.h>

#define MAXLENGTH 8			//���̱߳�
#define MAXSTEP 8			//�����
#define MAXTEST 30			//�����ļ���ʾ������
#define FILENAMELENGTH 100	//�ļ�������
#define MAXMINSTEP 6		//��̲��������

//�������̵�(i,j),i Ϊ��,jΪ��
typedef struct 
{
	int i;		//�����ϵ����±�
	int j;		//�����ϵ����±�
}ChessPoint;

//��������ļ����������ݽṹ
typedef struct 
{
	char srcpoint[3];	//Դ��,���� a2
	char destpoint[3];	//Ŀ�ĵ�,���� g6
	int srci,srcj;		//Դ��,���� a2 ��Ӧ�±�(1,2)
	int desti,destj;	//Ŀ�ĵ�,���� g6 ��Ӧ�±�(7,6)
}TestCase;

int chess[MAXLENGTH+1][MAXLENGTH+1]={0};	//����,�����±����1��ʼ
int offseti[9]={0,-2,-1,+1,+2,+2,+1,-1,-2};	//i�±���������
int offsetj[9]={0,+1,+2,+2,+1,-1,-2,-2,-1};	//j�±���������

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

//��������ļ�����������
TestCase testcase[MAXTEST];

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
	for(int i=0;i<MAXLENGTH+1;i++)
		for(int j=0;j<MAXLENGTH+1;j++)
			chess[i][j]=0;

	for(int k=0;k<MAXSTEP;k++)
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
 * ����ֵ˵��
 *     minstep : ����Դ�㵽Ŀ�ĵ����·��
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
			chess[newi][newj]=step+1;

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
 * ����˵��
 *     �������ļ�
 * ����˵��
 *     filename : �ļ���
 * ����ֵ
 *     testCount: �����ļ��еĲ���ʾ������
 ************************************************************************/
int read_test_file(char *filename)
{
	FILE *fp;
	char line[10];	//���ÿһ�е�ʾ��,�� a1 b2
	int testCount=0;

	fp=fopen(filename,"r");
	if(fp==NULL)
	{
		printf("can not open the test file!\n");
		exit(0);
	}

	//��ȡÿһ�в������ݲ������������������
	while(fgets(line,10,fp))
	{
		TestCase test;
		test.srcpoint[0]=line[0];
		test.srcpoint[1]=line[1];
		test.srcpoint[2]=0;

		test.destpoint[0]=line[3];
		test.destpoint[1]=line[4];
		test.destpoint[2]=0;

		test.srcj=test.srcpoint[0]-96;		//�ַ�a,b,c,d,e,f,g,hת��Ϊ1,2,3,4,5,6,7,8
		test.srci=test.srcpoint[1]-48;		//�ַ�1,2,3,4,5,6,7,8ת��Ϊ1,2,3,4,5,6,7,8

		test.destj=test.destpoint[0]-96;	//�ַ�a,b,c,d,e,f,g,hת��Ϊ1,2,3,4,5,6,7,8
		test.desti=test.destpoint[1]-48;	//�ַ�1,2,3,4,5,6,7,8ת��Ϊ1,2,3,4,5,6,7,8

		testcase[testCount++]=test;
	}

	fclose(fp);
	return testCount;
}

//��ʾ�˵�
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
 * ����˵��
 *     ������Խ���ļ�
 * ����˵��
 *     filename : ����ʾ���ļ���
 *     nMinStep : ����ʾ�����·������
 *     nCaseCount : ����ʾ������
 ************************************************************************/
void output_test_file(char *filename,int nMinStep[],int nCaseCount)
{
	char fileresult[FILENAMELENGTH];

	//��ý���ļ�������
	strcpy(fileresult,filename);
	int len=strlen(fileresult);
	fileresult[len-4]=0;
	strcat(fileresult,"_result.txt");

	//�򿪽���ļ�
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

//��������ļ�
void process_test_file(char *filename)
{
	//���ļ��������ļ��еĲ���ʾ������
	int nCaseCount=read_test_file(filename);
	int nMinStep[MAXTEST];

	//���
	for(int i=0;i<nCaseCount;i++)
		nMinStep[i]=knight_walk(testcase[i].srci,testcase[i].srcj,testcase[i].desti,testcase[i].destj);

	//���
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

			//���
			int nMinStep=knight_walk(srcrow,srccol,destrow,destcol);
			//int nMinStep=min_step();
			
			printf("\nTo get from %c%d to %c%d takes %d knight moves.\n\n",srccol+96,srcrow,destcol+96,destrow,nMinStep);
		}
		else if(stricmp(sinput,"f")==0)
		{
			printf("$ input test file name >");
			scanf("%s",filename);

			//��������ļ�
			process_test_file(filename);
		}

		//��������
		printf("$ input command >");
		scanf("%s",sinput);
	}

	return 0;
}
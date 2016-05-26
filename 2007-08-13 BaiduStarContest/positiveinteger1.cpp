/************************************************************************
 * һ���������п��ܿ��Ա���ʾΪn(n>=2)������������֮�ͣ��磺
 * 15=1+2+3+4+5
 * 15=4+5+6
 * 15=7+8
 * ���д���򣬸���������κ�һ�����������ҳ���������Ҫ���������������������
 ************************************************************************/

#include <stdio.h>
#include <VECTOR>

using namespace std;

class PositiveInteger
{
public:
	vector <int> m_vecBegin;//the begin integer of the sequence
	vector <int> m_vecEnd;	//the end integer of the sequence

public:
	PositiveInteger()
	{
		m_vecBegin.clear();
		m_vecEnd.clear();
	}

	~PositiveInteger(){}

	void GetIntegerSequence(int n);
	void display(int n);
};

//����Ȼ���������������ӷ�����
//x1+x2+...+xi=n, ����: i>=2,x1<=n/2
//x2=x1+1
//x3=x1+2
//xi=x1+i-1
//i*x1+i*(i-1)/2=n  =>  x1=(n-i*(i-1)/2 )/i=n/i-(i-1)/2
void PositiveInteger::GetIntegerSequence(int n)
{
	int i,sum,begin;

	i=2;
	while(1)
	{
		begin=n/i-(i-1)/2;	//���㿪ʼֵ
		if(begin<1)
			break;

		//��begin��ʼ������i�����������Ƿ�Ϊn
		sum=i*begin+i*(i-1)/2;
		if(sum==n)	//��ͬ,����������Ҫ���,����������
		{
			m_vecBegin.push_back(begin);
			m_vecEnd.push_back(begin+i-1);
		}

		i++;
	}
}

void PositiveInteger::display(int n)
{
	int size=m_vecBegin.size();
	if(size==0)
	{
		printf("\n  NONE\n\n");
	}
	else
	{
		for(int i=size-1;i>=0;i--)
		{
			printf("\n  %d=%d",n,m_vecBegin.at(i));
			for(int j=m_vecBegin.at(i)+1;j<=m_vecEnd.at(i);j++)
				printf("+%d",j);
		}
		printf("\n\n");
	}
}

//��ʾ�˵�
void show_menu()
{
	printf("---------------------------------------------\n");
	printf("input command to test the program\n");
	printf("   i or I : input n to test\n");
	printf("   q or Q : quit\n");	
	printf("---------------------------------------------\n");
	printf("$ input command >");
}

void main()
{
	char sinput[10];
	int n;

	show_menu();

	scanf("%s",sinput);
	while(stricmp(sinput,"q")!=0)
	{
		if(stricmp(sinput,"i")==0)
		{
			printf("  please input an integer:");
			scanf("%d",&n);

			PositiveInteger obj;
			obj.GetIntegerSequence(n);
			obj.display(n);
		}

		//��������
		printf("$ input command >");
		scanf("%s",sinput);
	}
}


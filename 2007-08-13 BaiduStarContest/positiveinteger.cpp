/************************************************************************
 * 一个正整数有可能可以被表示为n(n>=2)个连续正整数之和，如：
 * 15=1+2+3+4+5
 * 15=4+5+6
 * 15=7+8
 * 请编写程序，根据输入的任何一个正整数，找出符合这种要求的所有连续正整数序列
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

void PositiveInteger::GetIntegerSequence(int n)
{
	int i,sum,begin;

	i=begin=1;
	sum=0;

	while(begin<=n/2)
	{
		sum+=i++;
		if(sum==n)
		{
			m_vecBegin.push_back(begin);
			m_vecEnd.push_back(i-1);
			i=++begin;
			sum=0;
		}
		else if(sum>n)
		{
			i=++begin;
			sum=0;
		}
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
		for(int i=0;i<size;i++)
		{
			printf("\n  %d=%d",n,m_vecBegin.at(i));
			for(int j=m_vecBegin.at(i)+1;j<=m_vecEnd.at(i);j++)
				printf("+%d",j);
		}
		printf("\n\n");
	}
}

//显示菜单
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

		//输入命令
		printf("$ input command >");
		scanf("%s",sinput);
	}
}


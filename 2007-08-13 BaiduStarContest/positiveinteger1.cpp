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

//求自然数的连续正整数加法序列
//x1+x2+...+xi=n, 条件: i>=2,x1<=n/2
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
		begin=n/i-(i-1)/2;	//计算开始值
		if(begin<1)
			break;

		//从begin开始的连续i个正整数和是否为n
		sum=i*begin+i*(i-1)/2;
		if(sum==n)	//相同,则是我们所要求的,否则继续求解
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


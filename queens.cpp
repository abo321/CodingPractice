#include <stdlib.h>
#include <iostream>

#define MAX_NUM 20

//Check current solution is valid
bool place_queen(int x[], int k)
{
    for(int i = 1; i < k; i++)
    {
        if((x[i] == x[k]) || (abs(x[i] - x[k]) == abs(i - k)))
            return false;
    }
    return true;
}

void output_queens(int number, int x[], int n)
{
    std::cout << "solution " << number << std::endl;
    for (int i = 1; i <= n; i++)
        std::cout << x[i] << "  ";
    std::cout << std::endl;
}

void output_queens(int x[], int n)
{
    std::cout << "queens:" << std::endl;
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            if (j == x[i])
                std::cout << "1  ";
            else
                std::cout << "0  ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

/*
 *  Resolution for n queens
 *  input  : n, the number of queens
 *  output : the vector of solution, X
 */
int n_queens(int n, int x[])
{
    int nCount = 0;  //the number of resolutions
    int k = 1;       //firstly to process the 1st queen
    x[1] = 0;

    while(k>0)
    {
        x[k] = x[k] + 1;  //search from the next column
        while(x[k] <= n && !place_queen(x, k))
            x[k]=x[k]+1;  //continue to search if not meet the condition

        if (x[k] <= n)    //there exists one column meeting the condition
        {
            if (k == n)   //if the last queen, it will be a resolution
            {
                //break;  //if break, there will be only 1 resolution
                nCount++;
                output_queens(nCount, x, n);
                output_queens(x, n);
            }
            else          //process the next queen
            {
                k++;
                x[k] = 0;
            }
        }
        else              //backtrack if there is no such column
        {
            x[k] = 0;     //reset the kth queen
            k--;          //backtracking to previous queen
        }
    }

    return nCount;
}

int main()
{
    int n = 8;
    int x[MAX_NUM] = {0};

    int nCount = n_queens(n, x);
    std::cout << "Totally " << nCount << " solutions" << std::endl;

    return 0;
}

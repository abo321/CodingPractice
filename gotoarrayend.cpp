#include <iomanip>
#include <iostream>
using namespace std;

bool goto_end(int a[], int n)
{
    if (a[0] == 0)
        return false;

    int i = 0;
    int ipos = 0, jpos = 0;
    int farpos = 0;
    int maxpos = 0;

    while (i < n)
    {
        //check the current value is 0 or not, e.g., 0, 1, 1, 1 
        if (a[i] == 0)
        {
            std::cout << "array[" << i << "] = " << a[i] << ", cannot go forward" << std::endl;
            return false;
        }

        ipos = i + a[i];
        std::cout << "ipos = " << ipos << std::endl;
        if (ipos >= n - 1)
        {
            std::cout << "array[" << i << "] = " << a[i] << ", step out" << std::endl;
            return true;
        }

        if (ipos > farpos)
            farpos = ipos;

        //check the current value is 0 or not, e.g., 0, 1, 1, 1 
        if (i + 1 == farpos && a[farpos] == 0)
        {
            std::cout << "array[" << farpos << "] = " << a[farpos] << ", cannot go forward" << std::endl;
            return false;
        }

        maxpos = farpos;
        std::cout << "farpos = " << farpos << std::endl;
        for (int j = i + 1; j <= farpos; j++)
        {
            jpos = j + a[j]; 
            if (jpos >= n - 1)
            {
                std::cout << "array[" << j << "] = " << a[j] << ", step out" << std::endl;
                return true;
            }

            if (jpos > maxpos)
                maxpos = jpos;
        }

        //adjust the check/search scope
        //i = ipos + 1;
        //i = farpos + 1;
        i = farpos;
        farpos = maxpos;
        std::cout << "maxpos = " << maxpos << ", ipos = " << ipos << ", i = " << i << std::endl;
    }

    return true;
}

void dump(int a[], int n)
{
    std::cout << "Array: ";
    for (int i = 0; i < n; i++)
        std::cout << a[i] << " ";
    std::cout << std::endl;
}

void test1()
{
    int a[] = {1, 0, 0, 1};
    dump(a, 4);
    goto_end(a, 4);
}

void test2()
{
    int a[] = {5, 0, 0, 1};
    dump(a, 4);
    goto_end(a, 4);
}

void test3()
{
    int a[] = {5, 2, 1, 4, 7, 5, 1, 1, 1, 1, 1, 0, 2};
    dump(a, 13);
    goto_end(a, 13);
}

void test4()
{
    int a[] = {5, 2, 1, 4, 7, 5, 7, 0, 0, 0, 0, 0, 0};
    dump(a, 13);
    goto_end(a, 13);
}

void test5()
{
    int a[] = {5, 2, 1, 4, 4, 5, 1, 1, 1, 1, 2, 0, 0};
    dump(a, 13);
    goto_end(a, 13);
}

void test6()
{
    int a[] = {5, 2, 1, 4, 4, 5, 1, 1, 1, 1, 1, 1, 0};
    dump(a, 13);
    goto_end(a, 13);
}

void test7()
{
    int a[] = {5, 2, 1, 4, 4, 5, 1, 1, 1, 1, 1, 1, 0, 0};
    dump(a, 14);
    goto_end(a, 14);
}

int main(int argc, char** argv)
{
    test1();
    std::cout << std::endl;
    test2();
    std::cout << std::endl;
    test3();
    std::cout << std::endl;
    test4();
    std::cout << std::endl;
    test5();
    std::cout << std::endl;
    test6();
    std::cout << std::endl;
    test7();
    return 0;
}

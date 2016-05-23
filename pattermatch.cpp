#include <iostream>
using namespace std;

bool isMatch(const char *s, const char *p)
{
    if (!s)
    {
        std::cout << "invalid source string" << std::endl;
        return false;
    }

    if (!p)
    {
        std::cout << "invalid pattern" << std::endl;
        return false;
    }

    if (*p == '\0')
        return (*s == '\0'); 
    
    //if next char is not "*": must match current character
    if (*(p + 1) != '*')
    {
        return ((*p == *s) || (*p == '.' && *s != '\0')) && isMatch(s + 1, p + 1);
    }

    // if next char is "*"
    while ((*p == *s) || (*p == '.' && *s != '\0'))
    {
        if (isMatch(s, p + 2))
            return true;
        s++;
    }

    return isMatch(s, p + 2);
}

void test_match(const char *s, const char *p)
{
    std::cout << "source : " << s << std::endl;
    std::cout << "pattern: " << p << std::endl;
    bool match = isMatch(s, p);
    if (match)
        std::cout << "result : matched" << std::endl;
    else
        std::cout << "result : not matched" << std::endl;
}

void test1()
{
    const char *s = "ab";
    const char *p = "a*b";
    test_match(s, p);
}

void test2()
{
    const char *s = "ab";
    const char *p = "a*ab";
    test_match(s, p);
}

void test3()
{
    const char *s = "ab";
    const char *p = "a*a*ab";
    test_match(s, p);
}

void test4()
{
    const char *s = "ab";
    const char *p = "a*a*b";
    test_match(s, p);
}

void test5()
{
    const char *s = "ab";
    const char *p = "a*aa*b";
    test_match(s, p);
}

void test6()
{
    const char *s = "ab";
    const char *p = "b*a*b";
    test_match(s, p);
}

void test7()
{
    const char *s = "ab";
    const char *p = "b*aa*b";
    test_match(s, p);
}

void test8()
{
    const char *s = "ab";
    const char *p = "b*a*b";
    test_match(s, p);
}

void test9()
{
    const char *s = "ab";
    const char *p = "b*a*ab";
    test_match(s, p);
}

void test10()
{
    const char *s = "ab";
    const char *p = "ab*a*ab";
    test_match(s, p);
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
    std::cout << std::endl;
    test8();
    std::cout << std::endl;
    test9();
    std::cout << std::endl;
    test10();
    return 0;
}

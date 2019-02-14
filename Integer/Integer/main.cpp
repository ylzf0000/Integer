//main.cpp
#include "Integer.h"
using namespace std;
#define PRINT_VAR(x) cout << (#x) << ": " << (x) << endl;
int main()
{
    //Integer x = 123;
    //while (true)
    //{
    //    PRINT_VAR(x);
    //    x *= 10;
    //}
    //puts("");
    Integer a = "12312346352341284693258623496512432940129801201023012603712455189023642394968126384994345734";
    Integer b = "-218346327859823498439583498589345862387462342839423462342374982374892374912374929734972394";
    Integer c = 123ULL;
    Integer d = 'j';
    Integer z1 = 12312310000000000;
    Integer z2 = 0;
    //Integer c = a * b;
    PRINT_VAR(a);
    PRINT_VAR(b);
    PRINT_VAR(a + b);
    PRINT_VAR(a - b);
    PRINT_VAR(a * b);
    PRINT_VAR(a / b);
    PRINT_VAR(c);
    PRINT_VAR(d);
    PRINT_VAR(c + d);
    PRINT_VAR(c - d);
    PRINT_VAR(c * d);
    PRINT_VAR(c / d);
    PRINT_VAR(d / c);
    //PRINT_VAR(z1/0);
    //while (true)
    //{
    //    PRINT_VAR(b);
    //    ++b;
    //}
    //long long x = 1000000000;
    //Integer i = x;
    //cout << x << endl;
    //cout << i << endl;
    system("pause");
}
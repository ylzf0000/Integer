//main.cpp
#include "Integer.h"
using namespace std;
#define PRINT_VAR(x) cout << #x << ": " << x << endl;
int main()
{
    //unsigned long long x = 123;
    //for (int i = 0; i < 66; ++i)
    //{
    //    Integer val = x;
    //    cout << val << endl;
    //    x *= 10;
    //}
    //puts("");
    Integer a = 2321;
    Integer b = 13210000000000;
    Integer c = a * b;
    PRINT_VAR(a);
    PRINT_VAR(b);
    PRINT_VAR(c);
    //long long x = 1000000000;
    //Integer i = x;
    //cout << x << endl;
    //cout << i << endl;
    system("pause");
}
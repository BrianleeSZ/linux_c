#include <iostream>
using namespace std;
int main()
{
    int nNum = 0x12345678;
    char *p =(char*)&nNum;
    if (*p == 0x78)
    {
        cout <<"This machine is small endian."<<endl;
    }
    else
    {
        cout <<"This machine is big endian."<<endl;
    }
    
    return 0;
}
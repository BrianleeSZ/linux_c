     #include <iostream>
     #include <thread>
     using namespace std;
    
     void thfunc(int n,int m,int *pk,char s[])               //线程函数
     {
         cout << "in thfunc:n=" <<n<<",m="<<m<<",k="<<* pk <<"\nstr="<<s<<endl;
         *pk = 5000;                                         //修改* pk
     }
    
     int main(int argc, char *argv[])
     {
         int n = 110,m=200,k=5;
         char str[] = "hello world";
    
         thread t(thfunc, n,m,&k,str);                       //定义线程对象t，并传入多个参数
         t.join();                   //等待线程对象t结束
         cout << "k=" << k << endl;  //此时打印应该是5000
    
         return 0;
     }
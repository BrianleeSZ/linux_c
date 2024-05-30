     #include <iostream>
     #include <thread>
     using namespace std;
    
     void thfunc(int n)                           //线程函数
     {
        cout << "thfunc: " << n << "\n";   //这里的n是1
     }
    
     int main(int argc, char *argv[])
     {
        thread t(thfunc,1);               //定义线程对象t，并传入线程函数指针和线程函数参数
        t.join();                         //等待线程对象t结束
    
        return 0;
     }   
     #include <iostream>
     #include <thread>
     #include <unistd.h>                  //sleep
     using namespace std;                 //使用命名空间std
    
     void thfunc()                        //子线程的线程函数
     {
          cout << "i am c++11 thread func" << endl;
     }
    
     int main(int argc, char *argv[])
     {
         thread t(thfunc);                //定义线程对象，并把线程函数指针传入
         sleep(1);                        //主线程挂起1秒钟，为了让子线程有机会执行
         t.join();
         return 0;
     }
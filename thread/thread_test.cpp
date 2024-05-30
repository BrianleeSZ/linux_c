     #include <stdio.h>
     #include <stdlib.h>
    
     #include <chrono>           //std::chrono::seconds
     #include <iostream>         //std::cout
     #include <thread>           //std::thread, std::this_thread::sleep_for
     using namespace std;
     void thfunc(int n)          //线程函数
     {
         std::cout << "thfunc:" << n  <<  endl;
     }
    
     int main(int argc, const char *argv[])
     {
         std::thread threads[5];               //批量定义5个thread对象，但此时并不会执行线程
         std::cout << "create 5 threads...\n";
         for (int i = 0; i < 5; i++)
             threads[i] = std::thread(thfunc, i + 1); //这里开始执行线程函数thfunc
    
         for (auto& t : threads)                              //等待每个线程结束
             t.join();
    
         std::cout << "All threads joined.\n";
    
         return EXIT_SUCCESS;
     }
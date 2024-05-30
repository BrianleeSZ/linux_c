     #include <iostream>                 //std::cout
     #include <thread>                   //std::thread
     #include <mutex>                    //std::mutex
    
     volatile int counter(0);            //定义一个全局变量，当作计数器用于累加
     std::mutex mtx;                     //用于保护计数器的互斥锁
    
     void thrfunc()
     {
        for (int i = 0; i < 10000; ++i)
        {
            mtx.lock();                  //互斥锁上锁
            ++counter;                   //计数器累加
            mtx.unlock();                //互斥锁解锁
        }
     }
    
     int main(int argc, const char* argv[])
     {
         std::thread threads[10];
    
         for (int i = 0; i < 10; ++i)
             threads[i] = std::thread(thrfunc);  //启动10个线程
    
         for (auto& th : threads) th.join();             //等待10个线程结束
         std::cout <<"count to "<< counter << " successfully \n";
    
         return 0;
     }
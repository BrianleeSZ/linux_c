#include <iostream>
#include <thread>
#include <atomic>
using namespace std;

atomic<bool> ready(false);      //定义全局变量

void thfunc(int id)
{
    while (!ready)                      //一直等待，直到主线程中重置全局变量ready
        this_thread::yield();           //让出自己的CPU时间片
    
    for(volatile int i = 0; i< 1000000; ++i)
    {}
    cout << id <<",";                   //累加后，打印本线程的序号，最终输出的是排名，先完成先打印
}

int main()
{
    thread threads[10];
    cout <<"raceof 10 threads that count to 1 million:\n";
    for(int i = 0; i< 10; ++i)
        threads[i] = thread(thfunc, i); 

    ready = true;
    for(auto& th : threads) th.join();
        cout <<'\n';

    return 0;
}

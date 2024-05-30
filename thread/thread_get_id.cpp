#include <iostream>
#include <thread>
using namespace std;
thread::id main_thread_id = this_thread::get_id();      //获取主线程ID

void is_main_thread()
{
    if (main_thread_id == this_thread::get_id())        //判断是否和主线程ID相同
        std::cout << "This is the main thread.\n";
    else
        std::cout << "This is not the main thread.\n";    
}

int main(){
    is_main_thread();                                       //is_main_thread 作为主线程的普通函数调用
    thread th(is_main_thread);                              //is_main_thread作为线程函数使用
    th.join();                                              //等待th结束
    return 0;
}
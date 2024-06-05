#include <iostream>
#include <time.h>
#include <functional>
#include <chrono>
#include <set>
#include <sys/epoll.h>
#include <memory>
#include <unistd.h>
#include <gtest/gtest.h>
using namespace std;
/**
 * @brief 面试现场手撕定时器
 * 简单处理
*/
struct TimerBaseNode{
    time_t expire;
    int64_t id;
};
struct TimerNode : public TimerBaseNode
{
   using Callback = std::function<void (const TimerNode &node)>;
    Callback func;
    TimerNode(int64_t id,time_t expire, Callback func) :func(std::move(func))
    {
        this->id = id;
        this->expire = expire;
    }
};

bool operator < (const TimerBaseNode &lhd, const TimerBaseNode &rhd){
    if(lhd.expire < rhd.expire)
        return true;
    else if (lhd.expire > rhd.expire)
        return false;
    return lhd.id < rhd.id;
}

class Timer{
public:
    static inline time_t GetTick(){
        uint64_t t;
        struct timespec ti;
        clock_gettime(CLOCK_MONOTONIC, &ti);
        t = ti.tv_sec * 1000 + ti.tv_nsec / 1000000;
        return t;
    }
    // 使用std::chrono库获取单调时间
    static inline time_t GetTick2() {
        auto now = std::chrono::steady_clock::now();
        auto duration = now.time_since_epoch();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);

        // 将std::chrono::milliseconds转换为time_t类型
        return static_cast<time_t>(ms.count());
    }
     TimerBaseNode AddTimerOld(time_t msec,TimerNode::Callback func){
        time_t expire = GetTick() + msec;
        //cout << "AddTimer:" << GetTick() << endl;
        
        auto pairs = timeouts.emplace(GenID(),expire,std::move(func));
        return static_cast<TimerBaseNode>(*pairs.first);
    }
    TimerBaseNode AddTimer(time_t msec,TimerNode::Callback func){
        time_t expire = GetTick() + msec;
        //cout << "AddTimer:" << GetTick() << endl;
        if(timeouts.empty() || expire < timeouts.crbegin()->expire){
            auto pairs = timeouts.emplace(GenID(),expire,std::move(func));
            return static_cast<TimerBaseNode>(*pairs.first);
        }
        //O(1) 插入的节点 大于 最大值
        //cout <<"优化点......"<<endl;
        auto ele = timeouts.emplace_hint(timeouts.crbegin().base(),GenID(),expire,std::move(func));
        return static_cast<TimerBaseNode>(*ele);
    }
    bool DelTimer(TimerBaseNode &node){
       auto iter = timeouts.find(node);    //c++14 等价key的特性
       if (iter != timeouts.end())
       {
            timeouts.erase(iter);
            return true;
       }
       return false;
    }

    void HandleTimer(time_t now){
        auto iter = timeouts.begin();
        while (iter != timeouts.end() && iter->expire <= now)
        {
            iter->func(*iter);
            iter = timeouts.erase(iter);
            continue;
        }
        
    }

    int TimeToSleep(){
        auto iter = timeouts.begin();
        if (iter == timeouts.end())
        {
            return -1;
        }
        else
        {
            int diss = iter->expire - GetTick();
            return diss > 0 ? diss : 0;
        }
    }

private:
    static inline int64_t GenID(){
        return gid++;
    }
    static int64_t gid;
    set<TimerNode,std::less<>> timeouts;
};

int64_t Timer::gid = 0;
// int main(){
//     int epfd = epoll_create(1);;
//     unique_ptr<Timer> timer = make_unique<Timer>();

//     epoll_event ev[256]={0};

//     int i = 0;
//     timer->AddTimer(1000, [&](const TimerNode &node)
//     {   
//         i++;
//         cout << "node id:" << node.id << " hello brian i= " << i << "expire = " << node.expire << endl;
//     });
//     timer->AddTimer(1000, [&](const TimerNode &node)
//     {   
//         i++;
//         cout << "node id:" << node.id << " hello brian i= " << i << "expire = " << node.expire << endl;
//     });
//     while (true)
//     {
//         int n = epoll_wait(epfd,ev,256, timer->TimeToSleep());
//         time_t now = Timer::GetTick();
//         for (int i = 0; i < n; i++)
//         {
//             //处理网络事件
//         }
//         //处理定时事件
//         timer->HandleTimer(now);
//     }
    
//     close(epfd);
    
//     return 0;
// }

TEST(Timer,AddTimerOld){
    unique_ptr<Timer> timer = make_unique<Timer>();
    int t = 0;
    for (int i = 0; i < 10000; i++)
    {
       timer->AddTimerOld(3000, [&](const TimerNode &node)
       {   
            t++;
            //cout << "node id:" << node.id << " hello brian i= " << i << "expire = " << node.expire << endl;
       });
    }
    
}

TEST(Timer, AddTimer){
    unique_ptr<Timer> timer = make_unique<Timer>();
    int t = 0;
    for (int i = 0; i < 10000; i++)
    {
       timer->AddTimer(3000, [&](const TimerNode &node)
       {   
            t++;
            //cout << "node id:" << node.id << " hello brian i= " << i << "expire = " << node.expire << endl;
       });
    }
}
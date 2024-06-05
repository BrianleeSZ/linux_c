#include <iostream>
#include <time.h>
#include <functional>
#include <chrono>
#include <set>
using namespace std;

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
    TimerBaseNode AddTimer(time_t msec,TimerNode::Callback func){
        time_t expire = GetTick() + msec;
        auto pairs = timeouts.emplace(GenID(),expire,std::move(func));
        return static_cast<TimerBaseNode>(*pairs.first);
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
private:
    static inline int64_t GenID(){
        return gid++;
    }
    static int64_t gid;
    set<TimerNode,std::less<>> timeouts;
};

int main(){

    return 0;
}
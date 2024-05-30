#include "thread_poll.h"
#include <cstdio>
#include <stdlib.h>
#include <unistd.h>

class CMyTask : public CTask
{
public:
    CMyTask() = default;
    
    int Run()
    {
        printf("%s\n", (char*)m_ptrData);
        int x = rand() % 4 + 1;
        sleep(x);
        return 0;
    }
    ~CMyTask(){}
};

int main(){
    CMyTask taskObj;
    char szTmp[]="hello!";
    taskObj.setData((char*)szTmp);
    CThreadPool threadPool(5);       //线程池大小为5

    for(int i=0;i<10;i++)
        threadPool.AddTask(&taskObj);
    
    while (1)
    {
        printf("There are still %d tasks need to handle\n", threadPool.getTaskSize());
        
        //任务队列已经没有任务了
        if (threadPool.getTaskSize() == 0)
        {
            //清除线程池
            if (threadPool.StopAll()==-1)
            {
                printf("Thread pool clear, exit.\n");
                exit(0);
            }  
        }
        sleep(2);
        printf("2 seconds later ... \n");
    }
    return 0;    
}

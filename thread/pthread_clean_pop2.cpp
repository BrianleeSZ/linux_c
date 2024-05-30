#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void mycleanfunc(void *arg)
{
    printf("mycleanfunc:%d\n",*((int*)arg));
}
void *thfunc(void *arg)
{
    int i =1;
    printf("thread start ------\n");
    pthread_cleanup_push(mycleanfunc, &i);
    while (1)
    {
        i++;
        printf("i=%d\n", i);
    }
    printf("this line will not run\n");
    pthread_cleanup_pop(0);

    return (void*)0;
}
int main()
{
    void *ret = NULL;
    int iret = 0;
    pthread_t tid;
    pthread_create(&tid, NULL, thfunc, NULL);
    sleep(1);

    pthread_cancel(tid);                        //发送取消线程的请求
    pthread_join(tid, &ret);                    //等待线程结束
    if (ret == PTHREAD_CANCELED)                //判断是否成功取消线程
    {
        printf("thread has stopped,and exit code : %p\n", ret);     //打印返回值
    }
    else
    {
        printf("some error occured");
    }
    
    return 0;
}
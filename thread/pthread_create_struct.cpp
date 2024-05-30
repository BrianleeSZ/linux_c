#include <pthread.h>
#include <stdio.h>
#include <string>

typedef struct
{
    int n;
    const char *str;
}MYSTRYCT;
void *thfunc(void *arg)
{
    MYSTRYCT *p =(MYSTRYCT*)arg;
    printf("in thfunc:n=%d,str=%s\n", p->n,p->str);
    return (void*)0;
}
int main(int argc,char*argv[])
{
    pthread_t tidp;
    int ret;
    MYSTRYCT mystruct;
    mystruct.n = 110;
    std::string str = "Brian";
    mystruct.str=str.c_str();
    
    ret = pthread_create(&tidp, NULL, thfunc, (void*)&mystruct);
    if (ret)
    {
        printf("pthread_create failed:%d\n", ret);
        return -1;
    }
    pthread_join(tidp, NULL);       //等待子线程结束
    printf("in main:thread is created\n");
    
    return 0;
}
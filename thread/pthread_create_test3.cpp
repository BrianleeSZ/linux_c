     #include <pthread.h>
     #include <stdio.h>
    
     void *thfunc(void *arg)
     {
        char *str;
        str = (char *)arg;                             //得到传进来的字符串
        printf("in thfunc:str=%s\n", str);             //打印字符串
        return (void *)0;
     }
     int main(int argc, char *argv [])
     {
        pthread_t tidp;
        int ret;
        const char *str = "hello world";
    
        ret = pthread_create(&tidp, NULL, thfunc, (void *)str);//创建线程并传递str
        if (ret)
        {
            printf("pthread_create failed:%d\n", ret);
            return -1;
        }
        pthread_join(tidp, NULL);                                         //等待子线程结束
        printf("in main:thread is created\n");
    
        return 0;
     }
     #include <stdio.h>
     #include <unistd.h>
     #include <pthread.h>
     #include <sys/time.h>
     #include <string.h>
     #include <cstdlib>
    
     int gcn = 0;                                  //定义一个全局变量，用于累加
    
     void *thread_1(void *arg) {                   //第一个线程
        int j;
        for (j = 0; j < 10000000; j++) {           //开始累加
            gcn++;
        }
        pthread_exit((void *)0);
     }
    
     void *thread_2(void *arg) {                   //第二个线程
        int j;
        for (j = 0; j < 10000000; j++) {           //开始累加
            gcn++;
        }
        pthread_exit((void *)0);
     }
     int main(void)
     {
        int j,err;
        pthread_t th1, th2;
    
        for (j = 0; j < 10; j++)                   //做10次
        {
            err = pthread_create(&th1, NULL, thread_1, (void *)0); //创建第一个线程
            if (err != 0) {
                printf("create new thread error:%s\n", strerror(err));
                exit(0);
            }
            err = pthread_create(&th2, NULL, thread_2, (void *)0); //创建第二个线程
            if (err != 0) {
                printf("create new thread error:%s\n", strerror(err));
                exit(0);
            }
    
            err = pthread_join(th1, NULL);              //等待第一个线程结束
            if (err != 0) {
                printf("wait thread done error:%s\n", strerror(err));
                exit(1);
            }
            err = pthread_join(th2, NULL);              //等待第二个线程结束
            if (err != 0) {
                printf("wait thread done error:%s\n", strerror(err));
                exit(1);
            }
            printf("gcn=%d\n", gcn);
            gcn = 0;
        }
    
        return 0;
     }
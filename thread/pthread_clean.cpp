     #include <stdio.h>
     #include <stdlib.h>
     #include <pthread.h>
     #include <string.h>                                    //strerror
    
     void mycleanfunc(void *arg)                            //清理函数
     {
        printf("-----mycleanfunc:%d\n", *((int *)arg)); //打印传进来的不同参数
     }
     void *thfrunc1(void *arg)
     {
        int m=1;
        printf("..thfrunc1 comes \n");
        pthread_cleanup_push(mycleanfunc, &m);              //把清理函数压栈
        return (void *)0;                                   //退出线程
        pthread_cleanup_pop(0);    //把清理函数出栈，这句不会执行，但必须有，否则编译不过
     }
    
     void *thfrunc2(void *arg)
     {
        int m = 2;
        printf("....thfrunc2 comes \n");
        pthread_cleanup_push(mycleanfunc, &m);              //把清理函数压栈
        pthread_exit(0);                                    //退出线程
        pthread_cleanup_pop(0); //把清理函数出栈，这句不会执行，但必须有，否则编译不过
     }
    
     int main(void)
     {
        pthread_t pid1,pid2;
        int res;
        res = pthread_create(&pid1, NULL, thfrunc1, NULL);  //创建线程1
        if (res)
        {
            printf("pthread_create failed: %s\n", strerror(res));
            exit(1);
        }
        pthread_join(pid1, NULL);                                     //等待线程1结束
    
        res = pthread_create(&pid2, NULL, thfrunc2, NULL);  //创建线程2
        if (res)
        {
            printf("pthread_create failed: %s\n", strerror(res));
            exit(1);
        }
        pthread_join(pid2, NULL);                                     //等待线程2结束
    
        printf("main over\n");
        return 0;
     }
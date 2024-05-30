     #include <stdio.h>
     #include <stdlib.h>
     #include <pthread.h>
     #include <string.h> //strerror
    
     void mycleanfunc(void *arg)                    //清理函数
     {
        printf("mycleanfunc:%d\n", *((int *)arg));
     }
     void *thfrunc1(void *arg)                               //线程函数
     {
         int m=1,n=2;
         printf("thfrunc1 comes \n");
         pthread_cleanup_push(mycleanfunc, &m);              //把清理函数压栈
         pthread_cleanup_push(mycleanfunc, &n);              //再压一个清理函数压栈
         pthread_cleanup_pop(1);                             //出栈清理函数，并执行
         pthread_exit(0);                                    //退出线程
         pthread_cleanup_pop(0);                             //不会执行，仅仅为了成对
     }
    
     int main(void)
     {
         pthread_t pid1 ;
         int res;
         res = pthread_create(&pid1, NULL, thfrunc1, NULL);                //创建线程
         if (res)
         {
             printf("pthread_create failed: %s\n", strerror(res));
             exit(1);
         }
         pthread_join(pid1, NULL);                                         //等待线程结束
    
         printf("main over\n");
         return 0;
     }
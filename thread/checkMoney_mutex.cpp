     #include <stdio.h>
     #include <unistd.h>
     #include <pthread.h>
     int a = 200;                              //代表价值200元的货物
     int b = 100;                              //代表现在有100元现金
    
    pthread_mutex_t lock;                       //定义一个全局的互斥锁

     void* ThreadA(void*)                      //模拟伙计卖货收钱
     {
         while (1)
         {
            pthread_mutex_lock(&lock);
             a -= 50;                          //卖出价值50元的货物
             b += 50;                          //收回50元钱
            pthread_mutex_unlock(&lock);
         }
     }
    
     void* ThreadB(void*)                      //模拟老板对账
     {
         while (1)
         {
            pthread_mutex_lock(&lock);
            printf("%d\n", a + b);            //打印当前货物和现金的总和
            pthread_mutex_unlock(&lock);
            sleep(1);                         //隔一秒
         }
     }
    
     int main()
     {
         pthread_t tida, tidb;
         pthread_mutex_init(&lock, NULL);                         //初始化互斥锁
         pthread_create(&tida, NULL, ThreadA, NULL);              //创建伙计卖货线程
         pthread_create(&tidb, NULL, ThreadB, NULL);              //创建老板对账线程
         pthread_join(tida, NULL);                                //等待线程结束
         pthread_join(tidb, NULL);                                //等待线程结束

         pthread_mutex_destroy(&lock);                            //销毁互斥锁
         return 1;
     }
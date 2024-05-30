     #include <stdio.h>
     #include <unistd.h>
     #include <pthread.h>
     int a = 200;                              //代表价值200元的货物
     int b = 100;                              //代表现在有100元现金
    
     void* ThreadA(void*)                      //模拟伙计卖货收钱
     {
         while (1)
         {
             a -= 50;                          //卖出价值50元的货物
             b += 50;                          //收回50元钱
         }
     }
    
     void* ThreadB(void*)                      //模拟老板对账
     {
         while (1)
         {
             printf("%d\n", a + b);            //打印当前货物和现金的总和
             sleep(1);                         //隔一秒
         }
     }
    
     int main()
     {
         pthread_t tida, tidb;
    
         pthread_create(&tida, NULL, ThreadA, NULL);              //创建伙计卖货线程
         pthread_create(&tidb, NULL, ThreadB, NULL);              //创建老板对账线程
         pthread_join(tida, NULL);                                //等待线程结束
         pthread_join(tidb, NULL);                                //等待线程结束
         return 1;
     }
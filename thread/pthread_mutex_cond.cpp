#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *thread1(void *);
void *thread2(void *);

int i = 1;
int main(void)
{
    pthread_t t_a;
    pthread_t t_b;

    pthread_create(&t_a, NULL, thread2, (void *)NULL);
    pthread_create(&t_b, NULL, thread1, (void *)NULL);
    pthread_join(t_b, NULL);                                //等待线程t_b结束
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    exit(0);
}

void *thread1(void *junk)
{
    for (i = 0; i <=20; i++)
    {
        pthread_mutex_lock(&mutex);
        if(i % 3 ==0)
            pthread_cond_signal(&cond);                 //唤醒等待条件变量cond的线程
        else
            printf("thread1:%d\n", i);
        pthread_mutex_unlock(&mutex);

        sleep(1);
    }
    return (void*)0;
}

void *thread2(void *junk)
{
    while (i<20)
    {
        pthread_mutex_lock(&mutex);
        if(i % 3 !=0)
            pthread_cond_wait(&cond, &mutex);           //等待条件变量
        printf("----------------thread2:%d\n", i);      //打印能整除3的i
        pthread_mutex_unlock(&mutex);

        sleep(1);
        i++;
    }
    return (void*)0;

}
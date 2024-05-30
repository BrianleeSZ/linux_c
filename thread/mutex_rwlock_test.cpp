#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <string.h>
#include <cstdlib>

int gcn = 0;
pthread_mutex_t mutex;
pthread_rwlock_t rwlock;

void *thread_1(void *arg){
    int j;
    volatile int a;
    for (j = 0; j < 10000000; j++)
    {
        pthread_mutex_lock(&mutex);
        a = gcn;
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit((void *)0);  
}


void *thread_2(void *arg){
    int j;
    volatile int b;
    for (j = 0; j < 10000000; j++)
    {
        pthread_mutex_lock(&mutex);
        b = gcn;
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit((void *)0);  
}

void *thread_3(void *arg){
    int j;
    volatile int a;
    for (j = 0; j < 10000000; j++)
    {
        pthread_rwlock_rdlock(&rwlock);
        a = gcn;
        pthread_rwlock_rdlock(&rwlock);
    }
    pthread_exit((void *)0);  
}


void *thread_4(void *arg){
    int j;
    volatile int b;
    for (j = 0; j < 10000000; j++)
    {
        pthread_rwlock_rdlock(&rwlock);
        b = gcn;
        pthread_rwlock_rdlock(&rwlock);
    }
    pthread_exit((void *)0);  
}

int mutexVer(void)
{
    int j,err;
    pthread_t th1,th2;
    struct timeval start;
    clock_t t1,t2;
    struct timeval end;

    pthread_mutex_init(&mutex, NULL);           //初始化互斥锁

    gettimeofday(&start, NULL);
    err = pthread_create(&th1, NULL, thread_1, (void*)0);
    if (err!=0)
    {
        printf("create new thread error:%s\n", strerror(err));
        exit(0);
    }

    err = pthread_create(&th2, NULL, thread_2, (void*)0);
    if (err!=0)
    {
        printf("create new thread error:%s\n", strerror(err));
        exit(0);
    }

    err = pthread_join(th1, NULL);
    if (err!=0)
    {
        printf("wait thread done error:%s\n", strerror(err));
        exit(1);
    }

    err = pthread_join(th2, NULL);
    if (err!=0)
    {
        printf("wait thread done error:%s\n", strerror(err));
        exit(1);
    }
    
    gettimeofday(&end, NULL);
    pthread_mutex_destroy(&mutex);  //销毁互斥锁
    
    long long total_time = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
    total_time /=1000;
    printf("total mutex time is %lld ms\n", total_time);

    return 0;
}


int rdlockVer(void)
{
    int j,err;
    pthread_t th1,th2;

    struct timeval start;
    clock_t t1,t2;
    struct timeval end;

    pthread_rwlock_init(&rwlock, NULL);                 //初始化读写锁
    gettimeofday(&start, NULL);
    err = pthread_create(&th1, NULL, thread_3, (void *)0);
    if (err !=0)
    {
        printf("create new thread error:%s\n", strerror(err));
        exit(0);
    }

    err = pthread_create(&th2, NULL, thread_4, (void *)0);
    if (err !=0)
    {
        printf("create new thread error:%s\n", strerror(err));
        exit(0);
    }

    err = pthread_join(th1, NULL);
    if (err!=0)
    {
        printf("wait thread done error:%s\n",strerror(err));
        exit(1);
    }
    err = pthread_join(th2, NULL);
    if (err!=0)
    {
        printf("wait thread done error:%s\n",strerror(err));
        exit(1);
    }
    
    gettimeofday(&end, NULL);
    pthread_rwlock_destroy(&rwlock);                //销毁读写锁

    long long total_time = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
    total_time /=1000;
    printf("total rwlock time is %lld ms\n", total_time);
    
    return 0;
}

int main()
{
    mutexVer();
    rdlockVer();
    return 0;
}
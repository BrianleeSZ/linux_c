#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#if 0
int memleak_enable = 0;
void *_malloc(size_t size, const char *filename, int line){
    void *ptr = malloc(size);
    if (memleak_enable)
    {
         char buff[64]={0};
         snprintf(buff, 64, "./mem/%p", ptr); //创建日志文件
         /*
         将指针ptr的地址以十六进制形式附加到文件路径"./mem/"之后，
         形成一个新的文件路径，
         并将其存储到缓冲区buff中，
         同时确保不会因为缓冲区空间不足而导致数据损坏或安全漏洞
         */
        //写入日志信息
        FILE *fp = fopen(buff, "w");
        fprintf(fp,"[+] mem_addr: %p, filenam: %s, line: %d\n",ptr,filename,line);
        fflush(fp);
        fclose(fp);
    }
    return ptr;
}
void _free(void *ptr, const char *filename, int line){
    if (memleak_enable)
    {
        //printf("[-] mem_addr: %p, filenam: %s, line: %d\n",ptr,filename,line);
         char buff[64]={0};
         snprintf(buff, 64, "./mem/%p", ptr);

         if (unlink(buff) <0)       //删除日志文件
         {
            printf("double free\n");
            return;
         }
         
    }
    return free(ptr);
}

#define malloc(size) _malloc(size, __FILE__, __LINE__)
#define free(ptr) _free(ptr, __FILE__, __LINE__)
#else
//多个文件的内存泄漏
typedef void* (*malloc_t)(size_t size);
malloc_t malloc_f = NULL;
typedef void (*free_t)(void *ptr);
free_t free_f = NULL;
int memleak_enable = 0;
int enable_malloc_hook = 1;
int enable_free_hook = 1;

void *malloc(size_t size){
    void *ptr = NULL;

    if(enable_malloc_hook){
        enable_malloc_hook = 0;

        ptr = malloc_f(size);
        void *caller = __builtin_return_address(0); //返回当前函数的调用地址

         char buff[128]={0};
         snprintf(buff, 128,"./mem/%p.mem", ptr); //拼装日志文件名称
        
         //写入日志信息
        FILE *fp = fopen(buff, "w");
        fprintf(fp,"[+] caller: %p, addr: %p, size: %ld\n",caller, ptr,size);//ChangetoELF
        fflush(fp);

        enable_malloc_hook = 1;
    }
    else{
        ptr = malloc_f(size);
    }
    return ptr;

}

void free(void *ptr){
    if (enable_free_hook)
    {
        enable_free_hook = 0;

        char buff[128]={0};
        snprintf(buff, 128, "./mem/%p.mem", ptr); //拼装日志文件名称

        if (unlink(buff) <0)       //删除日志文件
        {
            printf("double free\n");
            return;
        }

        free_f(ptr);
        enable_free_hook = 1;
    }
    else
    {
        free_f(ptr);
    }
    
}


void init_hook(void){
    if(!malloc_f)
    {
        malloc_f=dlsym(RTLD_NEXT, "malloc");
    }
    if (!free_f)
    {
        free_f = dlsym(RTLD_NEXT, "free");
    }
    
}

#endif
int main(int argc,char **argv){
     init_hook();
    if (argc>1)
    {
        memleak_enable = 1;
        //enable_malloc_hook = 1;
        //enable_free_hook = 1;
    }
   
    void *p1 = malloc(5);
    void *p2 = malloc(10);
    void *p3 = malloc(15);
    free(p1);
    //free(p2);
    free(p3);

    return 0;
}
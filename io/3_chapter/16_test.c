
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

static char buf[4096];
//static char buf[4096] __attribute((aligned (4096)));

int main(void)
{
    int fd; 
    int ret;

    fd = open("./test1.txt",O_WRONLY | O_CREAT | O_EXCL,0666);
    if(-1 == fd) 
    {   
        perror("open error");
        return 1;
    }   

  //  lseek(fd,4096,SEEK_SET);

    for(int i = 0;i<4906;i++)
    {   
        ret = write(fd,buf,sizeof(buf));
        if(-1==ret)
        {
            perror("write error");
            close(fd);
            return 1;
        }
    }   

    close(fd);
    return 0;

}

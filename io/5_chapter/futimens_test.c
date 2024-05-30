#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>

/**
  futimes()函数测试
  该函数主要是用来设置文件的访问时间和修改时间
#include <fcntl.h> // Definition of AT_* constants 
       #include <sys/stat.h>

       int utimensat(int dirfd, const char *pathname,
                     const struct timespec times[2], int flags);

       int futimens(int fd, const struct timespec times[2]);

times数组可以可以同时控制文件访问时间、修改时间

 **/
int main(void)
{
	int fd;
	struct timespec tmsp_arr[2];
	int ret;

	fd = open("./test.txt",O_RDWR);
	if(-1 == fd)
	{
		perror("open error");
		return 1;
	}

#if 0
	ret = futimens(fd,NULL);//同时设置为当前时间	
#endif
#if 1
	tmsp_arr[0].tv_nsec = UTIME_OMIT;//访问时间保持不变
	tmsp_arr[1].tv_nsec = UTIME_NOW;//内容修改时间设置为当前时间
	ret = futimens(fd,tmsp_arr);
#endif

#if 0
	tmsp_arr[0].tv_nsec = UTIME_NOW;//访问时间设置为当前时间
	tmsp_arr[1].tv_nsec = UTIME_OMIT;//内容修改时间保持不变
	ret = futimens(fd,tmsp_arr);
#endif

	if(ret ==-1)
	{
		perror("futimes error");
		close(fd);
		return 1;
	}

	close(fd);
	return 0;
}

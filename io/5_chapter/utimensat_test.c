#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>

/**
#include <sys/stat.h>

       int utimensat(int fd, const char *path, const struct timespec times[2],
           int flag);
 **/
int main(void)
{
	struct timespec tmsp_arr[2];
	int ret;


#if 0
	ret = utimensat(Ad_FDCWD,"./test.txt",NULL,0);//同时设置为当前时间	
#endif
#if 0
	tmsp_arr[0].tv_nsec = UTIME_OMIT;//访问时间保持不变
	tmsp_arr[1].tv_nsec = UTIME_NOW;//内容修改时间设置为当前时间
	ret = utimensat(AT_FDCWD,"./test.txt",tmsp_arr,0);
#endif

#if 1
	tmsp_arr[0].tv_nsec = UTIME_NOW;//访问时间设置为当前时间
	tmsp_arr[1].tv_nsec = UTIME_OMIT;//内容修改时间保持不变
	
	ret = utimensat(AT_FDCWD,"./test.txt",tmsp_arr,0);
#endif

	if(ret ==-1)
	{
		perror("utimensat error");
		return 1;
	}

	return 0;
}

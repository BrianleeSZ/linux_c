#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <utime.h>
/**
  utime()函数改变文件的最后访问时间和最后修改时间

 **/
int main(void)
{
	int ret;
	struct utimbuf times = {0};
	time_t cur_sec;

	//获取当前时间
	time(&cur_sec);
	times.actime = cur_sec;
	times.modtime = cur_sec;

	ret = utime("./test.txt",&times);
	if(-1==ret)
	{
		perror("utime error");
		return 1;
	}

	return 0;
}

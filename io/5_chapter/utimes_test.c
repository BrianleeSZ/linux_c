#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <utime.h>
#include <sys/time.h>


/**
  utimes()函数与utime()函数类似，他们最大区别是细节上稍有不同，
  前者可以以微妙级的精度来指定时间值。
  改变文件的最后访问时间和最后修改时间

int utimes(const char *filename, const struct timeval times[2]);

struct utimbuf {
               time_t actime;      // /* access time 
               time_t modtime;      ///* modification time 
           };
 **/
int main(void)
{
	int ret;
	struct timeval tmval_arr[2];
	time_t cur_sec;

	//获取当前时间
	time(&cur_sec);
	for(int i = 0;i<2;i++)
	{
		tmval_arr[i].tv_sec = cur_sec;
		tmval_arr[i].tv_usec = 0;
	}

	ret = utimes("./test.txt",tmval_arr);
	if(-1==ret)
	{
		perror("utimes error");
		return 1;
	}

	return 0;
}

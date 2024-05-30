#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static char buf[4096];

int main(void)
{
	int fd;
	time_t start,end;
	double seconds;

	fd = open("./test.txt",O_WRONLY | O_CREAT | O_EXCL,0666);
	if(-1 == fd)
	{
		perror("open error");
		return -1;
	}
	time(&start);

	for(int i = 0;i<4906;i++)
		write(fd,buf,sizeof(buf));

//	fsync(fd);
	close(fd);

	time(&end);
	seconds = difftime(end,start);
	printf("cost time: %f\n",seconds);

	return 0;
}

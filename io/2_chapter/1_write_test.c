#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


int main(void){
	int fd;
	int ret;
	fd = open("./test.txt",O_WRONLY | O_CREAT | O_EXCL,0644);
	if(fd <0)
	{
		printf("open error\n");
		return 1;
	}
	printf("open OK!\n");

	ret = write(fd,"Hello World",11);
	if(ret <0)
	{
		printf("write error\n");
		close(fd);
		return 1;
	}
	printf("write %d bytes OK!\n",ret);
		
	return 0;
}

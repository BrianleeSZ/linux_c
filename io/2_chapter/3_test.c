#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
/**
给定一个文件，计算它得大小并打印出来
*/
int main(int argc,char **argv)
{
	int fd;
	long int sizeN;
	

	if(argc<2)
	{
		printf("the number of files are wrong!\n");
		return -1;
	}
	fd = open(argv[1],O_RDONLY);
	if(fd<0)
	{
		printf("open file error\n");
		close(fd);
		return -1;
	}
	
	sizeN = lseek(fd,0,SEEK_END);
	if(sizeN != -1){
		printf("seek success and file is %ld bytes\n",sizeN);
		
	}
	else
	{
		printf("seek error\n");
		close(fd);
		return -1;
	}

	lseek(fd,0,SEEK_SET);
	close(fd);
	return 0;
}

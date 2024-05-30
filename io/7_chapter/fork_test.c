#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>


int main(void)
{
	int fd;

	fd = open("./test.txt",O_WRONLY | O_TRUNC);
	if(fd == -1)
	{
		perror("open error");
		return 1;
	}

	switch(fork()){
		case -1:
			perror("fork error");
			close(fd);
			return 1;
		case 0:
			printf("I am child process\n");
			write(fd,"Hello World",11);
			close(fd);
			return 0;
		default:
			printf("I am parent process\n");
			write(fd,"666 888",7);
			close(fd);
			return 0;
	}

	return 0;
}

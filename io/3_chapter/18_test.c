#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	int fd,ret;
	FILE* file;
	fd = open("./test.txt",O_WRONLY | O_CREAT | O_TRUNC,0666);
	if(fd == -1)
	{
		perror("open error");
		return 1;
	}

	file = fdopen(fd,"w");
	if(NULL == file){
		perror("fdopen error");
		close(1);
		return 1;
	}

	ret = fwrite("Hello World",1,11,file);
	if(11>ret)
	{
		printf("fwrite error or end-of-file\n");
		fclose(file);
		return 1;
	}

	fclose(file);
	return 0;

}

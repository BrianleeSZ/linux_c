#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc,char **argv)
{
	if(argc != 3)
	{
		printf("the number of command line is wrong!\n");
		return -1;
	}

	int sfd,tfd;

	sfd =open(argv[1],O_RDONLY);
	if(sfd ==-1)
	{
		printf("read source file error\n");
		return -1;
	}
	tfd=open(argv[2],O_WRONLY | O_CREAT | O_EXCL,0644);
	if(tfd == -1)
	{
		printf("read target file error\n");
		close(sfd);
		return -1;
	}

	char buffer[4096];
	while(read(sfd,buffer,1)>0)
	{
		write(tfd,buffer,1);
	}
	
	close(sfd);
	close(tfd);

	printf("copy success\n");
	return 0;

}

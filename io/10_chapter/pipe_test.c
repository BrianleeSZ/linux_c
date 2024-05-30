#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int main(void)
{
	int fds[2] = {0};

	if(-1 == pipe(fds)){
		perror("pipe error");
		return -1;
	}
	
	switch(fork())
	{
		case -1:
			perror("fork error");
			break;
		case 0:{	//子进程读管道
			char rdbuf[64] = {0};
			printf("I am child, pid =%d\n",getpid());
			close(fds[1]);
			while(1)
			{
				read(fds[0],rdbuf,sizeof(rdbuf));
				printf("%s\n",rdbuf);
				memset(rdbuf,0x0,sizeof(rdbuf));
				sleep(1);
			}
			_exit(0);

		}
		default:{
			close(fds[0]);

			printf("I am parent, pid =%d\n",getpid());
			while(1)
			{
				write(fds[1],"Hello World",11);
				sleep(1);
			}

			break;
		}

	}



	return 0;
}

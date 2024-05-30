#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

int main(void)
{
	pid_t pid;
	
	/*1.创建子进程、终止父进程*/
	pid = fork();
	if(-1==pid)
	{
		perror("fork error");
		exit(0);
	}
	else if(0<pid)
	{
		exit(0);
	}
	else if(0==pid)
	{
		/*2.子进程调用setsid创建新的会话*/
		setsid();

		/*3.将工作目录改为"/"根目录*/
		if(-1 == chdir("/")){
			perror("chdir error");
			_exit(0);
		}

		/*4.重新设置文件权限掩码*/
		umask(0);

		/*5.关闭打开的文件描述符*/
		for(int i =0; i< sysconf(_SC_OPEN_MAX); i++)
			close(i);

		/*6.忽略SIGCHLD信号*/
		if(SIG_ERR==signal(SIGCHLD,SIG_IGN)){
			perror("signal error");
			exit(1);
		}
		
		while(1)
			sleep(1);

	}

	return 0;
}

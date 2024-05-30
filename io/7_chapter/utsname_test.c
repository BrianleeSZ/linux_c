#include <stdio.h>
#include <stdlib.h>
#include <sys/utsname.h>


int main(void)
{
	int ret;
	struct utsname info ={0};

	ret =uname(&info);
	if(-1==ret)
	{
		perror("uname error");
		return -1;
	}
	
	printf("os: %s\n",info.sysname);
	printf("host name : %s\n",info.nodename);
	printf("kernel version: %s\n",info.release);
	printf("release version: %s\n",info.version);
	printf("arc: %s\n",info.machine);

	return 0;
}

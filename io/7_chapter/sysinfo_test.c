#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>

int main(void)
{
	int ret;
	struct sysinfo info ={0};

	ret = sysinfo(&info);
	if(-1==ret)
	{
		perror("sysinfo error");
		return -1;
	}

	printf("uptime: %ld\n",info.uptime);
	printf("totalram: %lu\n",info.totalram);
	printf("freeram: %lu\n",info.freeram);
	printf("procs: %u\n",info.procs);
	
	return 0;
}

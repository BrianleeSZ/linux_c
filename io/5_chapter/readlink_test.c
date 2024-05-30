#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(void)
{
	int ret;
	char buf[128]={0};

	ret = readlink("./sym_file",buf,128);
	if(-1==ret)
	{
		perror("readlink error");
		return -1;
	}

	printf("readlink: %d\n",ret);
	printf("%s \n",buf);
	
	return 0;
}




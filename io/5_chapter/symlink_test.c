#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(void)
{
	int ret;

	ret = symlink("./test.txt","./sym_file");
	if(-1==ret)
	{
		perror("symlink error");
		return -1;
	}

	printf("create soft link file successfully\n");
	return 0;
}




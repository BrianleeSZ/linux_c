#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(void)
{
	int ret;

	ret = link("./test.txt","hard_test");
	if(-1==ret)
	{
		perror("link error");
		return -1;
	}

	printf("create hard file successfully\n");
	return 0;
}




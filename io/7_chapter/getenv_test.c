#include <stdio.h>
#include <stdlib.h>

int main(int argc,char *argv[])
{
	const char *env = NULL;

	if(argc <2)
		return -1;

	env = getenv(argv[1]);
	if(NULL == env)
	{
		printf("getenv error");
		return -1;
	}
	
	printf("%s\n",env);
	return 0;

}

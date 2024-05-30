#include <stdio.h>
#include <stdlib.h>

extern char **environ;

int main(int argc,char *argv[],char *env[])
{
	for(int i =0; NULL != env[i];i++)
	{
		printf("%s\n",env[i]);
	}

	return 0;

}

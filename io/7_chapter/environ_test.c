#include <stdio.h>
#include <stdlib.h>

extern char **environ;

int main(void)
{
	for(int i =0; NULL != environ[i];i++)
	{
		printf("%s\n",environ[i]);
	}

	return 0;

}

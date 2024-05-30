#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <errno.h>


int main(void)
{
	DIR *dir = NULL;
	int ret;
	struct dirent *rent;

	dir = opendir("./");
	if(NULL ==dir)
	{
		perror("opendir error");
		return -1;
	}

	while(rent = readdir(dir))
		printf("%s: %ld\n",rent->d_name,rent->d_ino);

	if(NULL == rent)
	{
		if(0!= errno)
			perror("readdir error");
		else
			printf("end of dir \n");
	}
	closedir(dir);

	return 0;
}

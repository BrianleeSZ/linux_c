#include <stdio.h>
#include <time.h>
#include <unistd.h>

int main(void)
{
	time_t start,end;
	double seconds;

	time(&start);

	sleep(10);

	time(&end);
	seconds = difftime(end,start);
	printf("total time: %f",seconds);

	return 0;
}

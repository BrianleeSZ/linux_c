#include "log.h"

int main(int argc, char *argv[])
{
	char *pName = argv[0];
	pName +=2;
	printf("PName is %s\n", pName);
	msgInit("log_demo2");
	msglog(MSG_INFO, "Begin run program....");
	sleep(2);
	msglog(MSG_BOTH, "Begin to game over...%ld", time(NULL));
	msgLogClose();
	return 0;
}

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>

/**
  #include <unistd.h>
  int access(const char *pathname, int mode);
  access函数测试，它只要是判断一个文件是否含有对象的查询、读取、写入、执行的权限。
 **/
int main(void)
{

	int ret;

	ret = access("./test.txt",F_OK);
	if(ret == -1)
	{
		perror("access error");
		return 1;
	}
	
	printf("the result is %d\n",ret);   
	return 0;
}

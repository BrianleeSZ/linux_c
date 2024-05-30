//测试当前系统支持的libevent方法和当前使用方法
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <event2/event.h>

int main()
{
	int i=0;
	//获取当前系统支持的方法
	const char **p = event_get_supported_methods();
	while(p[i]!=NULL)
	{
		printf("%s \t",p[i++]);
	}
	printf("\n");

	//获取地基节点
	struct event_base *base = event_base_new();
	if(base ==NULL)
	{
		printf("event_base_new error\n");
		return -1;
	}
	
	//获取当前系统使用的方法
	const char *pp = event_base_get_method(base);
	printf("%s\n",pp);

	//释放地基节点
	event_base_free(base);

	return  0;
}


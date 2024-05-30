//多线程版本的高并发服务器
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ctype.h>
#include "wrap.h"
#include <pthread.h>

//子线程回调函数
void *thread_work(void *arg)
{
	int cfd =*(int*)arg;

	int n;
	char buf[1024];
	int i;

	while(1)
	{
		//read data
		memset(buf, 0x00, sizeof(buf));
		n = Read(cfd, buf, sizeof(buf));
		if(n<=0)
		{
			printf("read error or client closed,n==[%d]\n",n);
			break;
		}
		printf("n==[%d], buf==[%s]\n", n, buf);

		for(i=0; i<n;i++)
		{
			buf[i]=toupper(buf[i]);
		}

		//Write data to Client
		Write(cfd, buf, n);
	}

	//关闭通信文件描述符
	close(cfd);
	pthread_exit(NULL);

}

int main()
{
	//创建socket
	int lfd = Socket(AF_INET,SOCK_STREAM,0);
	
	//设置端口复用
	int opt = 1;
	setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));
	
	//绑定
	struct sockaddr_in serv;
	bzero(&serv,sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_port = htons(8888);
	serv.sin_addr.s_addr = htonl(INADDR_ANY);
	Bind(lfd,(struct sockaddr *)&serv,sizeof(serv));

	//设置监听
	Listen(lfd,128);

	int cfd;
	pthread_t threadID;

	while(1)
	{
		//接受新的连接
		cfd = Accept(lfd, NULL, NULL);

		//创建子线程
		pthread_create(&threadID, NULL, thread_work, &cfd);


		//设置子线程为分离属性
		pthread_detach(threadID);



	}
	//关闭监听文件描述符
	close(lfd);

	return 0;
}

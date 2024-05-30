//多进程版本得网络服务器
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ctype.h>
#include "wrap.h"

int main()
{
	//创建socket
	int lfd = Socket(AF_INET,SOCK_STREAM,0);

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
	pid_t pid;
	char sIP[16];
	socklen_t len;
	struct sockaddr_in client;
	while(1)
	{
		//接受新得连接
		len = sizeof(client);
		memset(sIP, 0x00, sizeof(sIP));
		cfd = Accept(lfd,(struct sockaddr *)&client, &len);
		printf("client:[%s] [%d]\n",inet_ntop(AF_INET, &client.sin_addr.s_addr,sIP, sizeof(sIP)),ntohs(client.sin_port));
		
		//接受一个新得连接，创建一个新得子进程，让子进程进行数据得收发工作
		pid = fork();
		if(pid<0)
		{
			perror("fork error");
			exit(-1);
		}
		else if(pid >0)
		{
			//父进程
			//关闭通信文件描述符cfd
			close(cfd);

		}
		else if(pid==0)
		{
			//子进程
			//关闭监听文件描述符
			close(lfd);

			int n;
			char buf[1024];
			int i;

			while(1){
				//读数据
				n = Read(cfd,buf,sizeof(buf));
				if(n<=0)
				{
					printf("read error or client closed, n==[%d]\n",n);
					break;
				}
				printf("[%d]---->:n==[%d],buf==[%s]\n",ntohs(client.sin_port) ,n, buf);
				//printf("client:[%s] [%d]\n",inet_ntop(AF_INET, &client.sin_addr.s_addr,sIP, sizeof(sIP)),ntohs(client.sin_port));

				//将小写转换为大写
				for(i = 0;i<n;i++)
				{
					buf[i]=toupper(buf[i]);
				}
				
				//发送数据
				Write(cfd, buf, n);
			}
			
			close(cfd);
			exit(0); //防止子进程继续创建子进程
		}

	}

	//关闭监听文件描述符
	close(lfd);

	return 0;
}

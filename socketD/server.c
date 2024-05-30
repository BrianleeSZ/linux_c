#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <ctype.h>

int main()
{
	//创建socket
	int lfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(lfd<0)
	{
		perror("socket error");
		return -1;
	}

	//删除serv.sock文件
	unlink("serv.sock");

	//bind
	struct sockaddr_un serv;
	serv.sun_family = AF_UNIX;
	strcpy(serv.sun_path, "./serv.sock");
	int ret =bind(lfd, (struct sockaddr *)&serv, sizeof(serv));
	if(ret<0)
	{
		perror("bind error");
		return -1;
	}
	//监听
	listen(lfd,128);

	//接收新的客户竿
	struct sockaddr_un client;
	socklen_t len =sizeof(client);
	int cfd =accept(lfd, (struct sockaddr *)&client, &len);
	if(cfd<0)
	{
		perror("accept error");
		return -1;
	}

	int n;
	int i;
	char buf[1024];
	while(1)
	{
		//读数捿
		memset(buf, 0x00, sizeof(buf));
		n = read(cfd, buf, sizeof(buf));
		if(n<0)
		{
			perror("read error or client closed");
			break;
		}

		for(i=0;i<n;i++)
		{
			buf[i]=toupper(buf[i]);
		}

		//发送数捿
		write(cfd, buf, n);
	}

	//关闭文件描述笿
	close(lfd);
	close(cfd);

	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <ctype.h>

int main(){
	//创建socket 用来与服务端进行通信
	int cfd = socket(AF_INET,SOCK_STREAM,0);
	if(cfd <0)
	{
		perror("socket error");
		return -1;
	}
	
	//连接服务器
	//int connect(int sockfd, const struct sockaddr *addr,socklen_t addrlen);
	struct sockaddr_in serv;
	serv.sin_family = AF_INET;
	serv.sin_port = htons(8888);
	inet_pton(AF_INET,"127.0.0.1",&serv.sin_addr.s_addr);
	printf("[%x]\n",serv.sin_addr.s_addr);
	int ret = connect(cfd,(struct sockaddr *)&serv,sizeof(serv));
	if(ret<0)
	{
		perror("connect error");
		return -1;
	}

	int n = 0;
	char buf[256];
	while(1)
	{
		//读标准输入的数据
		memset(buf,0x00,sizeof(buf));
		n = read(STDIN_FILENO,buf,sizeof(buf));
		if(n<=0)
		{
			printf("read error or server closed, n==[%d]\n",n);
			break;
		}
		//发送数据
		write(cfd,buf,n);

		//读服务端发来的数据

		memset(buf,0x00,sizeof(buf));
		n = read(cfd,buf,sizeof(buf));
		printf("n==[%d], buf ==[%s]\n",n,buf);
	}

	//关闭监听文件描述符、通信文件描述符
	close(cfd);

	return 0;
}

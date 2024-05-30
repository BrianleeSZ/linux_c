#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/select.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/wait.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include "wrap.h"
//epoll模型demo程序
int main()
{
    int i;
    int n;
	int k;
	int sockfd;
    int lfd;
    int cfd;
    int ret;
    int maxfd;
    int nready;
	int epfd;
    char buf[1024];
    socklen_t socklen;
    int maxi;//有效的文件描述符最大值
    struct sockaddr_in svraddr,cliaddr;
    struct epoll_event ev;
	struct epoll_event events[1024];

    //创建socket
    lfd = Socket(AF_INET, SOCK_STREAM, 0);

    //设置端口复用
    int opt = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));

    //绑定-bind
    svraddr.sin_family = AF_INET;
    svraddr.sin_port = htons(8888);
    svraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    Bind(lfd, (struct sockaddr *)&svraddr, sizeof(svraddr));

    //监听-listen
    ret = Listen(lfd, 128);
    if (ret<0)
    {
        perror("listen error");
        return -1;
    }
   
	//创建一棵epoll树
	epfd = epoll_create(1024);
	if(epfd<0)
	{
		perror("create epoll error");
		return -1;
	}
	
	//将lfd上epoll树
	ev.data.fd = lfd;
	ev.events = EPOLLIN;
	epoll_ctl(epfd,EPOLL_CTL_ADD, lfd, &ev);

	while(1)
	{
		nready = epoll_wait(epfd, events, 1024, -1);
		if(nready<0)
		{
			
			perror("epoll_wait error");
			if(errno == EINTR){
				continue;
			}
			break;
		}
		
		for(i=0;i<nready;i++)
		{
			//有客户端连接请求
			sockfd = events[i].data.fd;
			if(sockfd == lfd)
			{
				cfd = Accept(lfd, NULL, NULL);
				//将新的cfd上epoll树
				ev.data.fd = cfd;
				ev.events = EPOLLIN;
				epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev);
				continue;
			}

			//有客户端发送数据过来
			memset(buf, 0x00, sizeof(buf));
			//n = Read(sockfd, buf, sizeof(buf));
			n = recv(sockfd, buf, sizeof(buf), 0);
			if(n<=0)
			{
				close(sockfd);
				//将sockfd对应的事件节点从epoll树上删除
				epoll_ctl(epfd, EPOLL_CTL_DEL, sockfd, NULL);
			}
			else
			{
				printf("n==[%d], buf==[%s]\n", n, buf);
				for(k = 0; k< n;k++)
				{
					buf[k]=toupper(buf[k]);
				}
			//	Write(sockfd, buf, n);
				send(sockfd, buf, n, 0);
			}


		}


	}

    close(epfd);
	close(lfd);
	return 0;
}

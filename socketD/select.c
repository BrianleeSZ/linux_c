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
#include "wrap.h"




int main()
{
    int i;
    int n;
    int lfd;
    int cfd;
    int ret;
    int maxfd;
    int nready;
    char buf[FD_SETSIZE];
    socklen_t len;
    int maxi;//有效的文件描述符最大值
    int connfd[FD_SETSIZE];//有效的文件描述符数组
    fd_set tmpfds,readfds;//要监控的文件描述符集
    struct sockaddr_in svraddr,cliaddr;
    
    

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
    
    //清空readfds和tmpfds集合
    FD_ZERO(&readfds);
    FD_ZERO(&tmpfds);

    //将lfd加入到readfds中，委托内核监控
    FD_SET(lfd,&readfds);

    //初始化有效的文件描述符集，为-1表示可用，该数组不保存lfd
    for (i = 0; i < FD_SETSIZE; i++)
    {
        connfd[i]=-1;
    }
    
    maxfd = lfd;
    len = sizeof(struct sockaddr_in);


    while (1)
    {
        //select为阻塞函数,若没有变化的文件描述符,就一直阻塞,若有事件发生则解除阻塞,函数返回
		//select的第二个参数tmpfds为输入输出参数,调用select完毕后这个集合中保留的是发生变化的文件描述符
        tmpfds = readfds;
        //tmpfds是输入输出参数
        //输入：告诉内核要检测哪些文件描述符
        //输出：内核告诉应用程序有哪些文件描述符发生变化
        nready = select(maxfd +1 ,&tmpfds, NULL, NULL, NULL);
        if (nready<0)
        {
            if (errno == EINTR)  //被信号中断
            {
                continue;
            }
            break;
        }
        
        //发生变化的文件描述符有两类, 一类是监听的, 一类是用于数据通信的
		//监听文件描述符有变化, 有新的连接到来, 则accept新的连接


        //有客户端连接请求到来
        if (FD_ISSET(lfd,&tmpfds))
        {
            cfd = Accept(lfd, (struct sockaddr *)&cliaddr, &len);        
            if(cfd<0)
			{
				if(errno==ECONNABORTED || errno==EINTR)
				{
				    continue;
				}
				break;
			}

            //先找位置, 然后将新的连接的文件描述符保存到connfd数组中
				for(i=0; i<FD_SETSIZE; i++)
				{
					if(connfd[i]==-1)
					{
						connfd[i] = cfd;
						break;
					}
				}

            //若连接总数达到了最大值,则关闭该连接
				if(i==FD_SETSIZE)
				{	
					close(cfd);
					printf("too many clients, i==[%d]\n", i);
					//exit(1);
					continue;
				}
            //确保connfd中maxi保存的是最后一个文件描述符的下标
				if(i>maxi)
				{
					maxi = i;
				}

            //打印客户端的IP和PORT
				char sIP[16];
				memset(sIP, 0x00, sizeof(sIP));
				printf("receive from client--->IP[%s],PORT:[%d]\n", inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, sIP, sizeof(sIP)), htons(cliaddr.sin_port));


            //将新的文件 描述符加入到select监控的文件描述符集合中
            FD_SET(cfd, &readfds);

            //修改内核的监控范围
            if (maxfd<cfd)
            {
                maxfd = cfd;
            }

            //若没有变化的文件描述符,则无需执行后续代码
            if (--nready==0)
            {
                continue;
            }
        }
        
        //下面是通信的文件描述符有变化的情况
		//只需循环connfd数组中有效的文件描述符即可, 这样可以减少循环的次数
        //有数据发来的情况
        for (i = 0; i <=maxi; i++)
        {
            int sockfd = connfd[i];
            //数组内的文件描述符如果被释放有可能变成-1
				if(sockfd==-1)
				{
					continue;
				}


            //判断sockfd文件描述符是否有变化
            if (FD_ISSET(sockfd, &tmpfds))
            {
                //读数据
                memset(buf, 0x00,sizeof(buf));
                n = Read(sockfd, buf,sizeof(buf));
                if (n<=0)
                {
                    perror("read over");
                    //读取失败，关闭连接
                    close(sockfd);

                    //将sockfd从监控的文件描述符集合中拿掉
                    FD_CLR(sockfd, &readfds);
                    connfd[i]=-1;//将connfd[i]置为-1，表示该位置可用
                }
                else if(n==0)
				{
						printf("client is closed\n");	
						close(sockfd);
						FD_CLR(sockfd, &readfds);
						connfd[i] = -1; //将connfd[i]置为-1,表示该位置可用
				}
                else 
                {
                   printf("[%d]:[%s]\n", n, buf);
				   write(sockfd, buf, n);

                }

                //提高效率，nready是select函数返回的检测到可读、可写、错误文件描述符的数量总和；
                //--nready==0,表示目前只有一个文件有变化，不需要多次轮询，直接跳出；
                if (--nready==0)
                {
                    break;
                }
            }
            
        }
        

    }
    
    //关闭监听文件描述符
    close(lfd);
    return 0;
}

//开发libevent 服务器
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <event2/event.h>

struct event *connev =  NULL;
//typedef void (*event_callback_fn)(evutil_socket_t fd, short events, void *arg);
void readcb(evutil_socket_t fd, short events, void *arg)
{
	int n;
	char buf[1024];
		memset(buf, 0x00, sizeof(buf));
		n = read(fd, buf, sizeof(buf));
		if(n<=0)
		{
			close(fd);
			//将通信文件描述符对应的事件从base地基上删除
			event_del(connev);
		}
		write(fd, buf, n);
}
void conncb(evutil_socket_t fd, short events, void *arg)
{
	struct event_base *base = (struct event_base*)arg;

	//接受新的客户端连接
	int cfd = accept(fd, NULL, NULL);
	if(cfd>0)
	{
		//创建通信文件描述符对应的事件并设置回调函数为readcb
		
		connev = event_new(base, cfd, EV_READ|EV_PERSIST, readcb, NULL);
		if(connev==NULL)
		{
			event_base_loopexit(base, NULL);//退出循环
		}

		//将通信文件描述符对应的事件上event_base地基
		event_add(connev, NULL);
	}
}

int main()
{
    //创建socket
    int lfd = socket(AF_INET, SOCK_STREAM, 0);

    //设置端口复用
    int opt = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));

    //绑定-bind
	struct sockaddr_in svraddr;
	bzero(&svraddr, sizeof(svraddr));
    svraddr.sin_family = AF_INET;
    svraddr.sin_port = htons(8888);
    svraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(lfd, (struct sockaddr *)&svraddr, sizeof(svraddr));

    //监听-listen
    int ret = listen(lfd, 128);
    if (ret<0)
    {
        perror("listen error");
        return -1;
    }
   
	//创建地基
	struct event_base *base = event_base_new();
	if(base==NULL)
	{
		printf("event_base_new error\n");
		return -1;
	}

	//创建监听文件描述符对应的事件
	//struct event *event_new(struct event_base *base, evutil_socket_t fd, short events, event_callback_fn cb, void *arg);
	struct event* ev = event_new(base, lfd, EV_READ|EV_PERSIST, conncb, base);
	if(ev==NULL)
	{
		printf("event_new error\n");
		return -1;
	}
	//将新的事件节点上base地基
	event_add(ev, NULL);

	//进入事件循环等待
	event_base_dispatch(base);

	//释放资源
	event_base_free(base);
	event_free(ev);

	close(lfd);
	return 0;
}

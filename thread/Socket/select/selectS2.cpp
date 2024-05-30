#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MYPORT 8888 // 连接时使用的端口
#define MAXCLINE 5   // 连接队列中的个数
#define BUF_SIZE 200

int fd[MAXCLINE]; // 连接的fd
int conn_amount;   // 当前的连接数

// 优化：使用结构体来存储客户端信息
typedef struct ClientInfo {
    int socket_fd;
    socklen_t addr_len;
    struct sockaddr_in addr;
} ClientInfo;

ClientInfo clients[MAXCLINE];

// 新增：显示客户端信息的函数
void showClients()
{
    int i;
    printf("client amount: %d\n", conn_amount);
    for (i = 0; i < conn_amount; i++)
    {
        printf("[%d]: %s:%d ", i, inet_ntoa(clients[i].addr.sin_addr), ntohs(clients[i].addr.sin_port));
    }
    printf("\n\n");
}

int main(void)
{
    int listen_sock_fd, new_sock_fd; // 监听套接字，连接套接字
    struct sockaddr_in server_addr;  // 服务器的地址信息
    int yes = 1;
    char buf[BUF_SIZE];
    int ret;
    int i;

    // 建立listen_sock_fd套接字
    if ((listen_sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("setsockopt");
        exit(1);
    }

    // 设置套接口的选项 SO_REUSEADDR 允许在同一个端口启动服务器的多个实例
    if (setsockopt(listen_sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
        perror("setsockopt error \n");
        exit(1);
    }

    server_addr.sin_family = AF_INET; // 主机字节序
    server_addr.sin_port = htons(MYPORT);
    server_addr.sin_addr.s_addr = INADDR_ANY; // 通配IP
    memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));

    if (bind(listen_sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("bind error!\n");
        getchar();
        exit(1);
    }
    if (listen(listen_sock_fd, MAXCLINE) == -1)
    {
        perror("listen error!\n");
        exit(1);
    }
    printf("listen port %d\n", MYPORT);

    fd_set fdsr; // 文件描述符集的定义
    int max_sock;
    struct timeval tv;
    conn_amount = 0;

    while (1)
    {
        FD_ZERO(&fdsr); // 清除描述符集
        FD_SET(listen_sock_fd, &fdsr); // 把listen_sock_fd加入描述符集

        // 添加活动的连接
        for (i = 0; i < conn_amount; i++)
        {
            FD_SET(clients[i].socket_fd, &fdsr);
        }

        // 超时的设定
        tv.tv_sec = 30;
        tv.tv_usec = 0;

        // 如果文件描述符中有连接请求，会做相应的处理，实现I/O的复用，多用户的连接通讯
        ret = select(MAXCLINE + 1, &fdsr, NULL, NULL, &tv);
        if (ret < 0) // 没有找到有效的连接，失败
        {
            perror("select error!\n");
            break;
        }
        else if (ret == 0) // 指定的时间到
        {
            printf("timeout \n");
            continue;
        }

        // 循环判断有效的连接是否有数据到达
        for (i = 0; i < conn_amount; i++)
        {
            if (FD_ISSET(clients[i].socket_fd, &fdsr))
            {
                ret = recv(clients[i].socket_fd, buf, sizeof(buf), 0);
                if (ret <= 0) // 客户端连接关闭，清除文件描述符集中的相应位
                {
                    printf("client[%d] closed\n", i);
                    close(clients[i].socket_fd);
                    clients[i].socket_fd = 0;
                    conn_amount--;
                }
                else // 否则有相应的数据发送过来，进行相应的处理
                {
                    if (ret < BUF_SIZE)
                        memset(&buf[ret], '\0', 1);
                    printf("client[%d] sent: %s\n", i, buf);
                    send(clients[i].socket_fd, buf, sizeof(buf), 0); // 反射回去
                }
            }
        }

        if (FD_ISSET(listen_sock_fd, &fdsr))
        {
            clients[conn_amount].addr_len = sizeof(clients[conn_amount].addr); // 初始化客户端地址长度
            new_sock_fd = accept(listen_sock_fd, (struct sockaddr *)&clients[conn_amount].addr, &clients[conn_amount].addr_len);
            if (new_sock_fd <= 0)
            {
                perror("accept error\n");
                continue;
            }

            // 添加新的fd到数组中，判断有效的连接数是否小于最大的连接数，如果小于的话，就把新的连接套接字加入集合
            if (conn_amount < MAXCLINE)
            {
                clients[conn_amount].socket_fd = new_sock_fd;
                conn_amount++;
                printf("new connection client[%d] %s:%d\n", conn_amount,
                       inet_ntoa(clients[conn_amount - 1].addr.sin_addr),
                       ntohs(clients[conn_amount - 1].addr.sin_port));
            }
            else
            {
                printf("max connections reached, exit\n");
                send(new_sock_fd, "bye", 4, 0);
                close(new_sock_fd);
                continue;
            }
        }

        showClients();
    }

    // 关闭所有客户端连接
    for (i = 0; i < conn_amount; i++)
    {
        if (clients[i].socket_fd != 0)
        {
            close(clients[i].socket_fd);
        }
    }

    return 0;
}
#include <cstdio>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>

int main()
{
    int sfp;
    struct sockaddr_in s_add;
    unsigned short portnum = 10051;
    struct sockaddr_in serv = {0};
    int on = 1;
    int serv_len = sizeof(serv);
    int err;
    sfp = socket(AF_INET, SOCK_STREAM, 0);
    if (-1==sfp)
    {
        printf("socket error:%s\n", strerror(errno));
        return -1;
    }
    printf("socket ok\n");
    //to print ip and port before bind
    printf("ip=%s,port=%d\n",inet_ntoa(serv.sin_addr),ntohs(serv.sin_port));
    //allow to reuse ip and port
    if (-1==setsockopt(sfp, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)))
    {
        printf("setsockopt error:%s\n", strerror(errno));
        return -1;
    }
    memset(&s_add, 0, sizeof(struct sockaddr_in));
    s_add.sin_family = AF_INET;
    s_add.sin_addr.s_addr = inet_addr("192.168.234.128");       //this ip must be the local ip address.
    s_add.sin_port = htons(portnum);
    if (-1==bind(sfp, (struct sockaddr*)(&s_add), sizeof(struct sockaddr)))
    {
        printf("bind error:%s\n", strerror(errno));
        return -1;
    }
    printf("bind success\n");
    //get the local ip address
    getsockname(sfp, (struct sockaddr*)&serv, (socklen_t*)&serv_len);
    printf("ip:%s,port=%d\n", inet_ntoa(serv.sin_addr),ntohs(serv.sin_port));
    return 0;
}
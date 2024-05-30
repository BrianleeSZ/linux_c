#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

// 定义端口和IP为常量，提高代码的可读性和可维护性
const unsigned short PORTNUM = 10051;
const char* IP_ADDRESS = "192.168.234.128";

// 封装错误处理
void handleError(const char* message) {
    fprintf(stderr, "%s: %s\n", message, strerror(errno));
    exit(EXIT_FAILURE); // 使用exit确保程序安全退出
}

// 封装套接字创建和绑定逻辑
int createAndBindSocket() {
    int sfp = socket(AF_INET, SOCK_STREAM, 0);
    if (sfp == -1) {
        handleError("socket error");
    }

    // 允许地址重用
    int on = 1;
    if (setsockopt(sfp, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1) {
        handleError("setsockopt error");
    }

    struct sockaddr_in s_add = {0};
    s_add.sin_family = AF_INET;
    s_add.sin_addr.s_addr = inet_addr(IP_ADDRESS); // 使用定义的常量
    s_add.sin_port = htons(PORTNUM);

    if (bind(sfp, (struct sockaddr*)(&s_add), sizeof(struct sockaddr)) == -1) {
        handleError("bind error");
    }

    return sfp;
}

int main() {
    int sfp = createAndBindSocket();

    // 获取本地IP地址和端口
    struct sockaddr_in serv = {0};
    socklen_t serv_len = sizeof(serv);
    if (getsockname(sfp, (struct sockaddr*)&serv, &serv_len) == -1) {
        handleError("getsockname error");
    }

    printf("ip:%s,port=%d\n", inet_ntoa(serv.sin_addr), ntohs(serv.sin_port));

    // 关闭套接字
    close(sfp);

    return 0;
}
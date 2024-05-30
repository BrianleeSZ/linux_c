#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(struct sockaddr_in);

    // 创建服务器套接字
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return -1;
    }

    // 设置服务器地址
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(12345);

    // 绑定服务器套接字到地址和端口
    if (bind(server_fd, (struct sockaddr*)&server_addr, addr_len) < 0) {
        std::cerr << "Error binding socket" << std::endl;
        return -1;
    }

    // 开始监听
    listen(server_fd, 5);

    while (true) {
        // 接受客户端连接
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
        if (client_fd < 0) {
            std::cerr << "Error accepting connection" << std::endl;
            continue;
        }

        char buffer[1024];
        ssize_t bytes_received;

        // 读取客户端发送的数据
        bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            std::cerr << "Error receiving data" << std::endl;
            continue;
        }

        // 输出接收到的数据
        std::cout << "Received from client(" << inet_ntoa(client_addr.sin_addr) << ")" << std::string(buffer, bytes_received) << std::endl;

        // 发送响应给客户端
        const std::string response = "Hello, Client!";
        send(client_fd, response.c_str(), response.size(), 0);

        // 关闭客户端连接
        close(client_fd);
    }

    close(server_fd);
    return 0;
}
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

int main() {
    int client_fd;
    struct sockaddr_in server_addr;

    // 创建客户端套接字
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return -1;
    }

    // 设置服务器地址
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("192.168.234.128"); // 服务器IP
    server_addr.sin_port = htons(12345); // 服务器端口号

    // 连接到服务器
    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Error connecting to server" << std::endl;
        return -1;
    }

    // 发送数据到服务器
    const std::string message = "Hello, Server!";
    send(client_fd, message.c_str(), message.size(), 0);

    // 从服务器接收数据
    char buffer[1024];
    ssize_t bytes_received;

    bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
    if (bytes_received <= 0) {
        std::cerr << "Error receiving data" << std::endl;
    } else {
        std::cout << "Received from server: " << std::string(buffer, bytes_received) << std::endl;
    }

    // 关闭客户端连接
    close(client_fd);
    return 0;
}
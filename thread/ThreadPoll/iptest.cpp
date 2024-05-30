#include <stdio.h>
#include <arpa/inet.h>
#include <string.h> // 用于使用memcpy

int main() {
    struct in_addr ia;

    in_addr_t dwIP = inet_addr("192.168.1.1");
    if (dwIP == INADDR_NONE) {
        fprintf(stderr, "Invalid IP address\n");
        return 1; // 无效地址时，返回错误码
    }
    ia.s_addr = dwIP;

    // 使用%lu来正确打印无符号长整型
    printf("ia.s_addr=0x%lu\n", ia.s_addr);

    // inet_ntoa返回的是一个静态字符数组，直接使用无需额外安全检查
    // 但是考虑到代码的整洁和可读性，使用const char*接收返回值是更好的做法
    const char* ipStr = inet_ntoa(ia);
    if (ipStr == NULL) {
        fprintf(stderr, "Error converting IP address to string\n");
        return 1; // 转换失败时，返回错误码
    }
    printf("%s\n", ipStr);

    return 0;
}
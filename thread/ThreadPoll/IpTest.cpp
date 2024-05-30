#include <cstdio>
#include <arpa/inet.h>
int main(){
    struct in_addr ia;

    in_addr_t dwIP = inet_addr("192.168.1.1");
    ia.s_addr = dwIP;
    printf("ia.s_addr=0x%u\n", ia.s_addr);
    printf("%s\n", inet_ntoa(ia));
    return 0;
}
#include "unp.h"

extern void str_cli(FILE *, int);

int
main(int argc, char **argv)
{
    int                 sockfd[5];
    struct sockaddr_in  srv_addr;

    if (argc != 2)
        err_quit("usage: %s <IPaddress>", argv[0]);

    for (int i = 0; i < 5; i++) {
        sockfd[i] = Socket(AF_INET, SOCK_STREAM, 0);

        bzero(&srv_addr, sizeof(srv_addr));
        srv_addr.sin_family = AF_INET;
        Inet_pton(AF_INET, argv[1], &srv_addr.sin_addr);
        srv_addr.sin_port = htons(SERV_PORT);

        Connect(sockfd[i], (SA *)&srv_addr, sizeof(srv_addr));
    }

    // 处理与服务器的交互
    // 调用str_cli时仅仅使用第一个连接
    // 建立多个连接的目的是从并发服务器上派生多个子进程
    str_cli(stdin, sockfd[0]);

    exit(0);
}

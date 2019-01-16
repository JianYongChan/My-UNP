#include "unp.h"

extern void str_cli(FILE *, int);

int
main(int argc, char **argv)
{
    int                 sockfd;
    struct sockaddr_in  srv_addr;

    if (argc != 2)
        err_quit("usage: %s <IPaddress>", argv[0]);

    sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&srv_addr, sizeof(srv_addr));
    srv_addr.sin_family = AF_INET;
    Inet_pton(AF_INET, argv[1], &srv_addr.sin_addr);
    srv_addr.sin_port = htons(SERV_PORT);

    Connect(sockfd, (SA *)&srv_addr, sizeof(srv_addr));

    // 处理与服务器的交互
    str_cli(stdin, sockfd);

    exit(0);
}

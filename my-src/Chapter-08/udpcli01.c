#include "unp.h"

int
main(int argc, char **argv)
{
    int     sockfd;
    struct sockaddr_in srv_addr;

    if (argc != 2)
        err_quit("usage: %s <IPaddress>", argv[0]);

    sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&srv_addr, sizeof(srv_addr));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    srv_addr.sin_port = htons(SERV_PORT);

    // 处理与服务器之间的数据传输
    dg_cli(stdin, sockfd, (SA *)&srv_addr, sizeof(srv_addr));

    exit(0);
}

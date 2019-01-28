#include "unp.h"

int
main(int argc, char **argv)
{
    int     sockfd;
    struct sockaddr_in  srvaddr;

    if (argc != 2)
        err_quit("usage: %s <IPaddress>", argv[0]);

    sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&srvaddr, sizeof(srvaddr));
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_port = htons(19);   // port that chargen server listens
    Inet_pton(AF_INET, argv[1], &srvaddr.sin_addr);

    Connect(sockfd, (SA *)&srvaddr, sizeof(srvaddr));

    pause();

    exit(0);
}

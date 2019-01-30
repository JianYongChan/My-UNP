#include "unp.h"

void
dg_echo(int sockfd, SA *pcli_addr, socklen_t clilen)
{
    int     n;
    socklen_t len;
    char    msg[MAXLINE];

    for ( ; ; ) {
        len = clilen;
        // 从套接字读取
        n = Recvfrom(sockfd, msg, MAXLINE, 0, pcli_addr, &len);

        // 从套接字回显至客户端
        Sendto(sockfd, msg, n, 0, pcli_addr, len);
    }
}

#include "unp.h"

void
str_echo(int sockfd)
{
    ssize_t     n;
    char        buf[MAXLINE];

again:
    // 将从客户端读取的数据再回射至客户端
    while (( n = read(sockfd, buf, sizeof(buf))) > 0)
        Writen(sockfd, buf, n);

    if (n < 0 && errno == EINTR)
        goto again;
    else if (n < 0)
        err_sys("str_echo: read error");
}

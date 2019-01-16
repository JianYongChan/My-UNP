#include "unp.h"
#include <time.h>

int
main(int argc, char **argv)
{
    int                 listenfd, connfd;
    socklen_t           len;
    struct sockaddr_in  srv_addr, cli_addr;
    char                buff[MAXLINE];
    time_t              ticks;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&srv_addr, sizeof(srv_addr ));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = htons(INADDR_ANY);
    srv_addr.sin_port = htons(13);

    Bind(listenfd, (SA *)&srv_addr, sizeof(srv_addr));

    Listen(listenfd, LISTENQ);

    for ( ; ; ) {
        len = sizeof(cli_addr);
        connfd = Accept(listenfd, (SA *)&cli_addr, &len);
        printf("connection from %s port %d\n",
                inet_ntop(AF_INET, &cli_addr.sin_addr, buff, sizeof(buff)),
                ntohs(cli_addr.sin_port));
        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s", ctime(&ticks));
        Write(connfd, buff, strlen(buff));

        Close(connfd);
    }
}

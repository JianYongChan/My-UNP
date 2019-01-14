#include "unp.h"
#include <time.h>

int
main(int argc, char **argv)
{
    int                 listenfd, connfd, len, i;
    struct sockaddr_in  server_addr;
    char                buff[MAXLINE];
    time_t              ticks;

    // socket, bind, listen三步骤准备监听描述符
    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(9999);

    Bind(listenfd, (SA *)&server_addr, sizeof(server_addr));

    Listen(listenfd, LISTENQ);

    for ( ; ; ) {
        // accept返回一个已连接描述符
        connfd = Accept(listenfd, (SA *)NULL, NULL);

        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        len = strlen(buff);
        for (i = 0; i < len; i++)
            Write(connfd, buff+i, 1);

        Close(connfd);
    }
}

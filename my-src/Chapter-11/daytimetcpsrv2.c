#include "unp.h"
#include <time.h>

/*
 * INFO: 使用tcp_listen的协议无关时间获取服务器程序
 *
 * 在命令行指定一个可选的host以及一个必须的service
 *
 *
 * 通过在命令行在命令行中指定IP地址或者主机名来强制服务器使用某个给定的协议(IPv4 or IPv6)
 */
int
main(int argc, char **argv)
{
    int     listenfd, connfd;
    socklen_t len;
    char    buff[MAXLINE];
    time_t  ticks;
    struct  sockaddr_storage cli_addr;

    if (argc == 2)
        listenfd = Tcp_listen(NULL, argv[1], NULL);
    else if (argc == 3)
        listenfd = Tcp_listen(argv[1], argv[2], NULL);
    else
        err_quit("usage: %s [host] <service or port>", argv[0]);

    for ( ; ; ) {
        len = sizeof(cli_addr);
        connfd = Accept(listenfd, (SA *)&cli_addr, &len);
        printf("connection from %s\n", Sock_ntop((SA *)&cli_addr, len));

        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        Write(connfd, buff, strlen(buff));

        Close(connfd);
    }
}

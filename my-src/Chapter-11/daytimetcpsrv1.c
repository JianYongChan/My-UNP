#include "unp.h"

/*
 * INFO: 用tcp_listen重新编写的时间获取服务器程序
 *
 * 在命令行中提供service(或者port number)即可
 *
 *
 * FIXME: 这个例子存在一个问题
 * 由于tcp_listen调用的第一个参数为空指针，而在该函数内部指定的地址族为AF_UNSPEC
 * 两者结合可能导致getaddrinfo返回的不是期望的地址族的套接字结构
 * 比如在双栈主机上返回的第一个套接字地址结构将是IPv6的，但是我们可能期望该服务器仅仅处理IPv4
 * 这个问题将在daytimetcpsrv2.c中解决
 */
int
main(int argc, char **argv)
{
    int     listenfd, connfd;
    socklen_t len;
    char    buff[MAXLINE];
    time_t  ticks;
    struct sockaddr_storage cli_addr;

    if (argc != 2)
        err_quit("usage: %s <service or port#>", argv[0]);

    listenfd = Tcp_listen(NULL, argv[1], NULL);

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

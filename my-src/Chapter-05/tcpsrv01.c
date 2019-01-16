#include "unp.h"

extern void str_echo(int sockfd);

int
main(int argc, char **argv)
{
    int                 listenfd, connfd;
    pid_t               child_pid;
    socklen_t           cli_len;
    struct sockaddr_in  cli_addr, srv_addr;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&srv_addr, sizeof(srv_addr));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    srv_addr.sin_port = htons(SERV_PORT);   // SERV_PORT in unp.h: 9877

    Bind(listenfd, (SA *)&srv_addr, sizeof(srv_addr));

    Listen(listenfd, LISTENQ);

    for ( ; ; ) {
        cli_len = sizeof(cli_addr);
        connfd = accept(listenfd, (SA *)&cli_addr, &cli_len);
        if ((child_pid = Fork()) == 0) {
            Close(listenfd);
            // 处理与客户端是交互
            str_echo(connfd);
            exit(0);
        }
        Close(connfd);
    }
}

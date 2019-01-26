#include "unp.h"

extern void str_echo(int sockfd);

extern void sig_chld(int signo);

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

    // 等待子进程结束，防止产生僵死进程
    Signal(SIGCHLD, sig_chld);

    for ( ; ; ) {
        cli_len = sizeof(cli_addr);
        // NOTE: 当阻塞于某个慢速系统调用的一个进程捕获某个信号且从该信号的处理函数中返回时
        // 该系统调用可能返回一个EINTR错误，所以我们必须有所准备
        if ((connfd = accept(listenfd, (SA *)&cli_addr, &cli_len)) < 0) {
            if (errno == EINTR)
                continue;
            else
                err_sys("accept error");
        }
        if ((child_pid = Fork()) == 0) {
            Close(listenfd);
            // 处理与客户端的交互
            str_echo(connfd);
            exit(0);
        }
        Close(connfd);
    }
}

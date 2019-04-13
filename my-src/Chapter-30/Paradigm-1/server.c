#include "unp.h"

int
main(int argc, char **argv)
{
    int               listenfd, connfd;
    pid_t             childpid;
    // NOTE: 函数还可以声明在函数内部？
    void              sig_chld(int), sig_int(int), web_child(int);
    socklen_t         clilen, addrlen;
    struct sockaddr  *cliaddr;

    if (argc == 2) {
        listenfd = Tcp_listen(NULL, argv[1], &addrlen);

    } else if (argc == 3) {
        listenfd = Tcp_listen(argv[1], argv[2], &addrlen);

    } else {
        err_quit("usage: %s [ <host> ] <port#>", argv[0]);
    }

    cliaddr = Malloc(addrlen);

    Signal(SIGCHLD, sig_chld);
    Signal(SIGINT, sig_int);

    for ( ;; ) {
        clilen = addrlen;
        if ((connfd = accept(listenfd, cliaddr, &clilen)) < 0) {
            if (errno == EINTR) {
                continue;   /* back to `for` */

            } else {
                err_sys("accept error");
            }
        }


        if ((childpid = Fork()) == 0) { /* child */
            Close(listenfd);
            web_child(connfd);  // 子进程处理客户的请求
            exit(0);
        }

        printf("server: connection arrived, and dispatch child %d to handle\n", childpid);

        Close(connfd);
    }
}

void
sig_int(int signo)
{
    void pr_cpu_time(void);

    pr_cpu_time();
    exit(0);    // NOTE: 不返回而直接终止进程，注意 exit 和 return 的区别
}

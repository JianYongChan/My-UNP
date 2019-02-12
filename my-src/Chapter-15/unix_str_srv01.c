#include "unp.h"

static void
sig_chld(int);

int
main(int argc, char **argv)
{
    int listenfd, connfd;
    socklen_t cli_len;
    pid_t child_pid;
    struct sockaddr_un cli_addr, srv_addr;


    listenfd = Socket(AF_LOCAL, SOCK_STREAM, 0);

    unlink(UNIXSTR_PATH);

    bzero(&srv_addr, sizeof(srv_addr));
    srv_addr.sun_family = AF_LOCAL;
    strncpy(srv_addr.sun_path, UNIXSTR_PATH, sizeof(srv_addr.sun_path)-1);

    // NOTE: 这里使用的是sizeof而不是SUN_LEN
    // 这两个长度都是有效的，因为路径名一定是以0字符结尾的
    Bind(listenfd, (SA *)&srv_addr, sizeof(srv_addr));

    Listen(listenfd, LISTENQ);

    Signal(SIGCHLD, sig_chld);

    for ( ; ;) {
        cli_len = sizeof(cli_addr);
        if ((connfd = accept(listenfd, (SA *)&cli_addr, &cli_len)) < 0) {
            // NOTE: 注意这里
            // 这里使用的是accept而不是Accept
            if (errno == EINTR)
                continue;
            else
                err_sys("accept error");
        }

        if ((child_pid = Fork()) == 0) { /* 子进程 */
            Close(listenfd);
            str_echo(connfd);
            exit(0);
        }

        // 父进程
        Close(connfd);
    }
}


void
sig_chld(int signo) {
    pid_t pid;
    int stat;

    pid = wait(&stat);
    printf("child %d terminated\n", pid);
    return;
}

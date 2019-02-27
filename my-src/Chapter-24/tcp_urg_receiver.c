#include "unp.h"

int     listenfd, connfd;

void sig_urg(int);

int
main(int argc, char **argv)
{
    int     n;
    char    buf[100];

    if (argc == 2)
        listenfd = Tcp_listen(NULL, argv[1], NULL);
    else if (argc == 3)
        listenfd = Tcp_listen(argv[1], argv[2], NULL);
    else
        err_quit("usage: %s [ <host> ] <port#>", argv[0]);

    connfd = Accept(listenfd, NULL, NULL);

    Signal(SIGURG, sig_urg);
    // 设置接收套接字的属主进程
    Fcntl(connfd, F_SETOWN, getpid());

    for ( ; ; ) {
        if ((n = Read(connfd, buf, sizeof(buf)-1)) == 0) {
            printf("received EOF\n");
            exit(0);
        }
        buf[n] = 0;
        printf("read %d bytes: %s\n", n, buf);
    }
}

void
sig_urg(int signo)
{
    int     n;
    char    buf[100];

    printf("SIGURG received\n");
    n = Recv(connfd, buf, sizeof(buf)-1, MSG_OOB);
    buf[n] = 0;
    printf("read %d OOB byte: %s\n", n, buf);
}

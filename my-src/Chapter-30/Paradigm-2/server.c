#include "unp.h"

int     nchildren;
pid_t  *pids;
long   *cptr;

int
main(int argc, char **argv)
{
    int         listenfd, i;
    socklen_t   addrlen;
    void        sig_int(int);
    pid_t       child_make(int, int, int);
    long       *meter(int); // meter the distribution of connection in children

    if (argc == 3) {
        listenfd = Tcp_listen(NULL, argv[1], &addrlen);

    } else if (argc == 4) {
        listenfd = Tcp_listen(argv[1], argv[2], &addrlen);

    } else {
        err_quit("usage: %s [ <host> ] <port#> <#children>", argv[0]);
    }

    // 指定预先要派生的子进程数目
    nchildren = atoi(argv[argc-1]);
    pids = Calloc(nchildren, sizeof(pid_t));
    cptr = meter(nchildren);

    for (i = 0; i < nchildren; i++) {
        pids[i] = child_make(i, listenfd, addrlen);
    }

    Signal(SIGINT, sig_int);

    for ( ;; ) {
        pause();
    }
}

void
sig_int(int signo)
{
    int     i;
    void    pr_cpu_time(void);

    for (i = 0; i < nchildren; i++) {
        kill(pids[i], SIGTERM);
    }

    while (wait(NULL) > 0) {
        /* do nothing */;
    }

    if (errno != ECHILD) {
        err_sys("wait error");
    }

    pr_cpu_time();

    for (int i = 0; i < nchildren; i++) {
        printf("conns on child-%d: %ld\n", i, cptr[i]);
    }

    exit(0);    // NOTE: 直接退出进程，并不返回
}

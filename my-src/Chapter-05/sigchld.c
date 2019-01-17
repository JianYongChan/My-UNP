#include "unp.h"

void
sig_chld(int signo)
{
    pid_t   pid;
    int     stat;

    // -1表示等待第一个终止的子进程
    // WNOHANG用来告知内核在有尚未终止的子进程时不要阻塞
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
        printf("child %d terminated\n", pid);
    return;
}

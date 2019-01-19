#include "unp.h"

void
str_echo(int sockfd)
{
    long arg1, arg2;
    ssize_t     n;
    char        line[MAXLINE];

    for ( ; ; ) {
        // 小于0的情况已经在包裹函数中处理了
        if ((n = Readline(sockfd, line, sizeof(line))) == 0)
            return;

        // sscanf将两个参数转换为长整数
        if (sscanf(line, "%ld %ld", &arg1, &arg2) == 2)
            snprintf(line, sizeof(line), "%ld\n", arg1+arg2);
        else
            snprintf(line, sizeof(line), "input error\n");

        n = strlen(line);
        Writen(sockfd, line, n);
    }
}

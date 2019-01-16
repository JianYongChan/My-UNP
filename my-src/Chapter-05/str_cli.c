#include "unp.h"

void
str_cli(FILE *fp, int sockfd)
{
    char    sendline[MAXLINE], recvline[MAXLINE];

    while (Fgets(sendline, MAXLINE, fp) != NULL) {
        // 将数据传送至服务器
        Writen(sockfd, sendline, strlen(sendline));

        // 从服务器读取其回射的数据
        if (Readline(sockfd, recvline, MAXLINE) == 0)
            err_quit("str_cli: server terminated prematurely");

        // 将从服务器回射来的数据显示在终端上
        Fputs(recvline, stdout);
    }
}

#include "unp.h"

void
dg_cli(FILE *fp, int sockfd, const SA *psrvaddr, socklen_t srvlen)
{
    int     n;
    char    sendline[MAXLINE], recvline[MAXLINE];

    while(Fgets(sendline, MAXLINE, fp) != NULL) {
        // 将从标准输入上获取的数据通过套接字发送给服务器
        Sendto(sockfd, sendline, strlen(sendline), 0, psrvaddr, srvlen);

        // 从服务器获取回显的数据
        n = Recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL);

        recvline[n] = 0;    // 以0字节结尾
        // 将回显的数据显示在标准输出上
        Fputs(recvline, stdout);
    }
}

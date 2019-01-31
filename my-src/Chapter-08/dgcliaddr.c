#include "unp.h"

void
dg_cli(FILE *fp, int sockfd, const SA *psrvaddr, socklen_t srvlen)
{
    int     n;
    char    sendline[MAXLINE], recvline[MAXLINE];
    socklen_t len;
    struct sockaddr *preply_addr;

    preply_addr = Malloc(srvlen);

    while (Fgets(sendline, MAXLINE, fp) != NULL) {
        // 将数据发送给服务器
        Sendto(sockfd, sendline, strlen(sendline), 0, psrvaddr, srvlen);

        len = srvlen;
        n = Recvfrom(sockfd, recvline, MAXLINE, 0, preply_addr, &len);
        // 验证的确是从服务器回显过来的数据，而不是其他来源
        if (len != srvlen || memcmp(psrvaddr, preply_addr, len) != 0) {
            printf("reply from %s (ignored)\n", Sock_ntop(preply_addr, len));
            continue;
        }

        // 将从服务器接收的数据显示在标准输出
        recvline[n] = 0;
        Fputs(recvline, stdout);
    }
}

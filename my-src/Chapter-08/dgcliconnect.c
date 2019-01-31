#include "unp.h"

void
dg_cli(FILE *fp, int sockfd, const SA *psrvaddr, socklen_t socklen)
{
    int     n;
    char    sendline[MAXLINE], recvline[MAXLINE];

    Connect(sockfd, psrvaddr, socklen);

    while (Fgets(sendline, MAXLINE, fp) != NULL) {
        Write(sockfd, sendline, strlen(sendline));

        n = Read(sockfd, recvline, MAXLINE);

        recvline[n] = 0;
        Fputs(recvline, stdout);
    }

}

#include "unp.h"

#define NDG     2000    /* datagrams to send */
#define DGLEN   1400    /* length of each datagram */

/*
 * 写固定数目的数据报到服务器
 */
void
dg_cli(FILE *fp, int sockfd, const SA *psrvaddr, socklen_t srvlen)
{
    int     i;
    char    sendline[DGLEN];

    for (i = 0; i < NDG; i++) {
        Sendto(sockfd, sendline, DGLEN, 0, psrvaddr, srvlen);
    }
}

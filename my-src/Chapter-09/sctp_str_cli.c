#include "unp_sctp.h"

void
sctp_str_cli(FILE *fp, int sockfd, SA *to, socklen_t tolen)
{
    struct sockaddr_in peer_addr;
    struct sctp_sndrcvinfo sri;
    char sendline[MAXLINE], recvline[MAXLINE];
    socklen_t len;
    int out_sz, rd_sz;
    int msg_flags;
    int n;

    bzero(&sri, sizeof(sri));
    while (fgets(sendline, MAXLINE, fp) != NULL) {
        if (sendline[0] != '[') {
            printf("Error: line must be of the form '[streamnum]text'\n");
            continue;
        }
        sri.sinfo_stream = strtol(&sendline[1], NULL, 0);
        out_sz = strlen(sendline);
        n = sctp_sendmsg(sockfd, sendline, out_sz,
                to, tolen, 0, 0, sri.sinfo_stream, 0, 0);
        if (n < 0) {
            perror("sctp_sendmsg error");
            return;
        }
        len = sizeof(peer_addr);
        rd_sz = sctp_recvmsg(sockfd, recvline, MAXLINE, (SA *)&peer_addr, &len, &sri, &msg_flags);
        if (rd_sz < 0) {
            perror("sctp_recvmsg error");
            return;
        }
        printf("From str:%d seq:%d (assoc:0x%x): %s",
                sri.sinfo_stream, sri.sinfo_ssn, (unsigned int)sri.sinfo_assoc_id, recvline);
    }
}

#include "unp_sctp.h"

void
sctp_str_cli_echoall(FILE *fp, int sockfd, SA *to, socklen_t tolen)
{
    struct sockaddr_in perr_addr;
    struct sctp_sndrcvinfo sri;
    char sendline[MAXLINE], recvline[MAXLINE];
    socklen_t len;
    int rd_sz, i, str_sz, n;
    int msg_flags;

    bzero(sendline, sizeof(sendline));
    bzero(&sri, sizeof(sri));
    while(fgets(sendline, MAXLINE, fp) != NULL) {
        str_sz = strlen(sendline);
        if (sendline[str_sz-1] == '\n') {
            sendline[str_sz-1] = '\0';
            str_sz--;
        }
        for (i = 0; i < SRV_MAX_SCTP_STRM; i++) {
            snprintf(sendline+str_sz, sizeof(sendline)-str_sz, ".msg.%d", i);
            n = sctp_sendmsg(sockfd, sendline, sizeof(sendline), to, tolen, 0, 0, i, 0, 0);
            if (n < 0) {
                perror("sctp_sendmsg error");
                return;
            }
        }
        for (i = 0; i < SRV_MAX_SCTP_STRM; i++) {
            len = sizeof(perr_addr);
            rd_sz = sctp_recvmsg(sockfd, recvmsg, MAXLINE, (SA *)&perr_addr, &len, &sri, &msg_flags);
            if (rd_sz < 0) {
                perror("sctp_recvmsg error");
                return;
            }
            printf("From str:%d seq:%d (assoc:0x%x):",
                    sri.sinfo_stream, sri.sinfo_ssn,
                    (unsigned int)sri.sinfo_assoc_id);
            printf("%.*s\n", rd_sz, recvline);
        }
    }
}


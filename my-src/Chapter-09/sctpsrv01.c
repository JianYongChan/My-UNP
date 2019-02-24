#include "unp_sctp.h"

#define BUFFSIZE 1024
#define LISTENQ 7

int
main(int argc, char **argv)
{
    int     sockfd, msg_flags;
    char    readbuf[BUFFSIZE];
    struct sockaddr_in srv_addr, cli_addr;
    struct sctp_sndrcvinfo sri;
    struct sctp_event_subscribe events;
    int stream_increment = 1;
    socklen_t len;
    size_t rd_sz;
    int n, strms;

    if (argc == 2)
        stream_increment = atoi(argv[1]);

    if ((sockfd = socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP)) < 0) {
        perror("socket error");
        return -1;
    }

    bzero(&srv_addr, sizeof(srv_addr));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    srv_addr.sin_port = htons(SRVPORT);

    if (bind(sockfd, (SA *)&srv_addr, sizeof(srv_addr)) < 0) {
        perror("bind error");
        return -1;
    }

    bzero(&events, sizeof(events));
    events.sctp_data_io_event = 1;
    if (setsockopt(sockfd, IPPROTO_SCTP, SCTP_EVENTS, &events, sizeof(events)) < 0) {
        perror("setsockopt error for SCTP_EVENTS");
        return -1;
    }

    if (listen(sockfd, LISTENQ) < 0) {
        perror("listen error");
        return -1;
    }
    for ( ; ; ) {
        len = sizeof(cli_addr);
        rd_sz = sctp_recvmsg(sockfd, readbuf, sizeof(readbuf),
                (struct sockaddr *)&cli_addr, &len, &sri, &msg_flags);
        if (rd_sz < 0) {
            perror("sctp_recvmsg error");
            return -1;
        }
        if (stream_increment) {
            sri.sinfo_stream++;
            strms = sctp_get_no_strms(sockfd, (SA *)&cli_addr, len);
            if (strms < 0) {
                perror("sctp_get_no_strms error");
                return -1;
            }
            if (sri.sinfo_stream >= strms)
                sri.sinfo_stream = 0;
        }

        n = sctp_sendmsg(sockfd, readbuf, rd_sz,
                (struct sockaddr *)&cli_addr, len,
                sri.sinfo_ppid, sri.sinfo_flags,
                sri.sinfo_stream, 0, 0);
        if (n < 0) {
            perror("sctp_sendmsg error");
            return -1;
        }
    }
}

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
    int rd_sz;
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
        len = sizeof(struct sockaddr_in);
        rd_sz = sctp_recvmsg(sockfd, readbuf, sizeof(readbuf),
                (struct sockaddr *)&cli_addr, &len, &sri, &msg_flags);
        if (rd_sz < 0) {
            perror("sctp_recvmsg errorrrrr");
            return -1;
        }
#if 0
        printf("msg_flags: %d\n", msg_flags);
        if (msg_flags & MSG_NOTIFICATION) {
            printf("flags: MSG_NOTIFICATION\n");
        }
        if (msg_flags & MSG_EOR) {
            printf("flags: MSG_EOR\n");
        }
        printf("assoc_id: %d\n", sri.sinfo_assoc_id);
        printf("context: %d\n", sri.sinfo_context);
        printf("cumtsn: %d\n", sri.sinfo_cumtsn);
        printf("flags: %d\n", sri.sinfo_flags);
        printf("ppid: %d\n", sri.sinfo_ppid);
        printf("ssn: %d\n", sri.sinfo_ssn);
        printf("stream: %d\n", sri.sinfo_stream);
        printf("timetolive: %d\n", sri.sinfo_timetolive);
        printf("tsn: %d\n", sri.sinfo_tsn);
        printf("rd_sz = %ld\n", rd_sz);
        printf("rd_sz: %d\n", rd_sz);
#endif
        printf("rd_sz < 0: %s\n", (rd_sz < 0) ? "yes" : "no");
        if ((int)rd_sz < 0) {
            printf("rd_sz < 0\n");
            fflush(stdout);
            return -1;
        }

        if (stream_increment) {
            sri.sinfo_stream++;
            strms = sctp_get_no_strms(sockfd, &sri);
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

#include "../unp_sctp.h"

#undef MAXLINE
#define MAXLINE 512

int
main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in srv_addr ,cli_addr;
    struct sctp_event_subscribe events;
    struct sctp_sndrcvinfo sri;
    char recvbuf[MAXLINE];
    socklen_t len;
    int ret, rd_sz;
    int msg_flags;

    if ((sockfd = socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP)) < 0) {
        perror("socket error");
        return -1;
    }

    bzero(&srv_addr, sizeof(srv_addr));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    srv_addr.sin_port = htons(SRVPORT);

    ret = bind(sockfd, (SA *)&srv_addr, sizeof(srv_addr));
    if (ret < 0) {
        perror("bind error");
        return -1;
    }

    bzero(&events, sizeof(events));
    events.sctp_data_io_event = 0;
    ret = setsockopt(sockfd, IPPROTO_SCTP, SCTP_EVENTS, &events, sizeof(events));
    if (ret < 0) {
        perror("setsockopt error for SCTP_EVENTS");
        return -1;
    }

    ret = listen(sockfd, 10);
    if (ret < 0) {
        perror("listen error");
        return -1;
    }
    for ( ; ; ) {
        len = sizeof(cli_addr);
        rd_sz = sctp_recvmsg(sockfd, recvbuf, sizeof(recvbuf),
                (SA *)&cli_addr, &len, &sri, &msg_flags);
        if (rd_sz < 0) {
            perror("sctp_recvmsg error");
        }

        printf("recv: %*s", rd_sz, recvbuf);
    }
}

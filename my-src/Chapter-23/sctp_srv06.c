#include "unp_sctp.h"

extern void print_notification(char *notify_buf);

int
main(int argc, char **argv)
{
    int sockfd;
    char recvline[MAXLINE];
    struct sockaddr_in srv_addr, cli_addr;
    struct sctp_sndrcvinfo sri;
    struct sctp_event_subscribe events;
    int n, rd_sz;
    int msg_flags = 0;
    socklen_t len;

    if ((sockfd = socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP)) < 0) {
        perror("socket error");
        return -1;
    }
    bzero(&srv_addr, sizeof(srv_addr));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    srv_addr.sin_port = htons(SRVPORT);

    n = bind(sockfd, (SA *)&srv_addr, sizeof(srv_addr));
    if (n < 0) {
        perror("bind error");
        return -1;
    }

    bzero(&events, sizeof(events));
    events.sctp_data_io_event = 1;
    events.sctp_association_event = 1;
    events.sctp_address_event = 1;
    events.sctp_send_failure_event = 1;
    events.sctp_peer_error_event = 1;
    events.sctp_shutdown_event = 1;
    events.sctp_partial_delivery_event = 1;
    events.sctp_adaptation_layer_event = 1;
    n = setsockopt(sockfd, IPPROTO_SCTP, SCTP_EVENTS, &events, sizeof(events));
    if (n < 0) {
        perror("setsockopt error for SCTP_EVENTS");
        return -1;
    }

    n = listen(sockfd, LISTENQ);
    if (n < 0) {
        perror("listen error");
        return -1;
    }
    for ( ; ; ) {
        len = sizeof(cli_addr);
        do {
            rd_sz = sctp_recvmsg(sockfd, recvline, sizeof(recvline), (SA *)&cli_addr, &len, &sri, &msg_flags);
        } while (rd_sz < 0 && errno == EAGAIN);
        if (rd_sz < 0) {
            perror("sctp_recvmsg error");
            return -1;
        }
        if (msg_flags & MSG_NOTIFICATION) {
            print_notification(recvline);
            continue;
        }

        /* TODO: handle data */
    }

    return 0;
}

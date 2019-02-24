#include "unp_sctp.h"


int
main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in srv_addr;
    struct sctp_event_subscribe events;
    int echo_to_all = 0;

    if (argc < 2) {
        perror("missing host argument - use './sctpcli01 host [echo]'\n");
        return -1;
    }
    if (argc > 2) {
        printf("echoing messages to all streams\n");
        echo_to_all = 1;
    }

    if ((sockfd = socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP)) < 0) {
        perror("socket error");
        return -1;
    }

    bzero(&srv_addr, sizeof(srv_addr));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    srv_addr.sin_port = htons(SRVPORT);
    if (inet_pton(AF_INET, argv[1], &srv_addr.sin_addr) != 1) {
        perror("inet_pton error");
        return -1;
    }

    bzero(&events, sizeof(events));
    events.sctp_data_io_event = 1;
    if (setsockopt(sockfd, IPPROTO_SCTP, SCTP_EVENTS, &events, sizeof(events)) < 0) {
        perror("setsockopt error for SCTP_EVENTS\n");
        return -1;
    }
    if (echo_to_all == 0)
        sctp_str_cli(stdin, sockfd, (SA *)&srv_addr, sizeof(srv_addr));
    else
        sctp_str_cli_echoall(stdin, sockfd, (SA *)&srv_addr, sizeof(srv_addr));

    if (close(sockfd) < 0) {
        perror("close error");
        return -1;
    }
    return 0;
}

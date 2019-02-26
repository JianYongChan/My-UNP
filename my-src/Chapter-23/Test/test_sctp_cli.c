#include "../unp_sctp.h"

int
main(int argc, char **argv)
{
    int sockfd;
    char sendbuf[MAXLINE];
    struct sockaddr_in srv_addr;
    struct sctp_event_subscribe events;
    int ret;
    socklen_t len;

    if (argc != 2) {
        perror("usage: ./testsrv host");
        return -1;
    }

    sockfd = socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);
    if (sockfd < 0) {
        perror("socket error");
        return -1;
    }

    bzero(&srv_addr, sizeof(srv_addr));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    srv_addr.sin_port = htons(SRVPORT);
    ret = inet_pton(AF_INET, argv[1], &srv_addr.sin_addr);
#if 0
    if (ret != 1) {
        perror("inet_pton error");
        return -1;
    }
#endif

    bzero(&events, sizeof(events));
    events.sctp_data_io_event = 1;
    ret = setsockopt(sockfd, IPPROTO_SCTP, SCTP_EVENTS, &events, sizeof(events));
    if (ret < 0) {
        perror("setsockopt error for SCTP_EVENTS");
        return -1;
    }

    while (fgets(sendbuf, sizeof(sendbuf), stdin) != NULL) {
        len = sizeof(srv_addr);
        ret = sctp_sendmsg(sockfd, sendbuf, strlen(sendbuf), (SA *)&srv_addr, len, 0, 0, 0, 0, 0);
        if (ret < 0) {
            perror("sctp_sendmsg error");
            return -1;
        }
    }

    exit(0);
}

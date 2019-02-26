#include "../unp_sctp.h"

int
sctp_bind_arg_list(int sockfd, char **argv, int argc)
{
    struct addrinfo *addr;
    char *bindbuf, *p, portbuf[10];
    int addrcnt = 0;
    int i;
    int ret;

    bindbuf = (char *)calloc(argc, sizeof(struct sockaddr_storage));
    if (bindbuf == 0) {
        perror("calloc error");
        return -1;
    }
    p = bindbuf;
    sprintf(portbuf, "%d", SRVPORT);
    for (i = 0; i < argc; i++) {
        addr = host_serv(argv[i], portbuf, AF_UNSPEC, SOCK_SEQPACKET);
        if (addr == NULL) {
            perror("host_serv error");
            goto done;
            return -1;
        }
        memcpy(p, addr->ai_addr, addr->ai_addrlen);
        freeaddrinfo(addr);
        addrcnt++;
        p += addr->ai_addrlen;
    }
    ret = sctp_bindx(sockfd, (SA *)bindbuf, addrcnt, SCTP_BINDX_ADD_ADDR);

    if (ret < 0) {
        perror("sctp_bindx error");
        goto done;
    }

done:
    free(bindbuf);
    return ret;
}

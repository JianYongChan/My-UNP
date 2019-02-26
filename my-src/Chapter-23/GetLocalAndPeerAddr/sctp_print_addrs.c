#include "../unp_sctp.h"

void
sctp_print_addrs(struct sockaddr_storage *addrs, int num)
{
    struct sockaddr_storage *ss;

    int i, salen;

    ss = addrs;
    for (i = 0; i < num; i++) {
        switch(ss->ss_family) {
            case AF_INET:
                salen = sizeof(struct sockaddr_in);
                break;
            case AF_INET6:
                salen = sizeof(struct sockaddr_in6);
                break;
            default:
                perror("sctp_print_addrs: unknown AF");
                return;
        }
        printf("%s\n", sock_ntop((SA *)ss, salen));
        ss = (struct sockaddr_storage *)((char *)ss + salen);
    }
}

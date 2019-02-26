#include "../unp_sctp.h"

extern void sctp_print_addrs(struct sockaddr_storage *addrs, int num);

void
check_notification(int sockfd, char *recvline, int rd_len)
{
    union sctp_notification *snp;
    struct sctp_assoc_change *sac;
    struct sockaddr_storage *sal, *sar;
    int num_rem, num_loc;

    snp = (union sctp_notification *)recvline;
    if (snp->sn_header.sn_type == SCTP_ASSOC_CHANGE) {
        sac = &snp->sn_assoc_change;
        if ((sac->sac_state == SCTP_COMM_UP) ||
            (sac->sac_state == SCTP_RESTART)) {
            num_rem = sctp_getpaddrs(sockfd, sac->sac_assoc_id, (SA **)&sar);
            printf("There are %d remote addresses and they are:\n", num_rem);
            sctp_print_addrs(sar, num_rem);
            sctp_freepaddrs((SA *)sar);

            num_loc = sctp_getladdrs(sockfd, sac->sac_assoc_id, (SA **)&sal);
            printf("There are %d local addresses and they are:\n", num_loc);
            sctp_print_addrs(sal, num_loc);
            sctp_freeladdrs((SA *)sal);
        }
    }
}

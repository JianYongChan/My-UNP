#include "unp_sctp.h"

int
sctp_get_no_strms(int sockfd, struct sockaddr *to, socklen_t tolen)
{
    socklen_t retsz;
    struct sctp_status status;

    retsz = sizeof(status);
    bzero(&status, sizeof(status));

    status.sstat_assoc_id = sctp_address_to_associd(sockfd, to, tolen);
    printf("assoc_id: 0x%x\n", status.sstat_assoc_id);
    if (getsockopt(sockfd, IPPROTO_SCTP, SCTP_STATUS, &status, &retsz) < 0) {
        perror("getsockopt error for SCTP_STATUS");
        return -1;
    }

    return (status.sstat_outstrms);
}

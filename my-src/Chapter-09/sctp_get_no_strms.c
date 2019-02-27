#include "unp_sctp.h"

int
sctp_get_no_strms(int sockfd, struct sctp_sndrcvinfo *srip)
{
    socklen_t retsz;
    struct sctp_status status;

    retsz = sizeof(status);
    bzero(&status, sizeof(status));

    status.sstat_assoc_id = srip->sinfo_assoc_id;
//    printf("assoc_id: 0x%x\n", status.sstat_assoc_id);
//    printf("sockfd = %d\n", sockfd);
    if (getsockopt(sockfd, IPPROTO_SCTP, SCTP_STATUS, &status, &retsz) < 0) {
        perror("getsockopt error for SCTP_STATUS");
    }
//    printf("outstrms: %d\n", status.sstat_outstrms);

    return (status.sstat_outstrms);
}

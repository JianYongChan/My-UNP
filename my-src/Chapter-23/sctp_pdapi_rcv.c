#include "unp_sctp.h"

static uint8_t *sctp_pdapi_readbuf = NULL;
static int sctp_pdapi_rdbuf_sz = 0;

uint8_t *
pdapi_recvmsg(int sockfd, int *rdlen, SA *from, socklen_t *fromlen,
        struct sctp_sndrcvinfo *srip, int *msg_flags)
{
    int rd_sz, left, at_in_buf;
    socklen_t frm_len = 0;

    if (sctp_pdapi_readbuf == NULL) {
        sctp_pdapi_readbuf = (uint8_t *)malloc(SCTP_PDAPI_INCR_SZ);
        if (sctp_pdapi_readbuf == NULL) {
            perror("malloc error");
            return NULL;
        }
        sctp_pdapi_rdbuf_sz = SCTP_PDAPI_INCR_SZ;
    }
    at_in_buf =
        sctp_recvmsg(sockfd, sctp_pdapi_readbuf, sctp_pdapi_rdbuf_sz,
                from, fromlen, srip, msg_flags);
    if (at_in_buf < 1) {
        perror("sctp_recvmsg error(1)");
        *rdlen = at_in_buf;
        return NULL;
    }

    // MSG_EOR:
    while ((*msg_flags & MSG_EOR) == 0) {
        left = sctp_pdapi_rdbuf_sz - at_in_buf;
        if (left < SCTP_PDAPI_NEED_MODE_THRESHOLD) {
            sctp_pdapi_readbuf = realloc(sctp_pdapi_readbuf, SCTP_PDAPI_INCR_SZ);
            if (sctp_pdapi_readbuf == NULL) {
                perror("realloc error");
                return NULL;
            }
            sctp_pdapi_rdbuf_sz += SCTP_PDAPI_INCR_SZ;
            left = sctp_pdapi_rdbuf_sz - at_in_buf;
        }
        rd_sz = sctp_recvmsg(sockfd, &sctp_pdapi_readbuf[at_in_buf],
                left, NULL, &frm_len, NULL, msg_flags);
        if (rd_sz < 0) {
            perror("sctp_recvmsg error(2)");
            return NULL;
        }
        at_in_buf += rd_sz;
    }
    *rdlen = at_in_buf;
    return sctp_pdapi_readbuf;
}


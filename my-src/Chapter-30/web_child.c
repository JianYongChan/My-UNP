#include "unp.h"

#define MAXN    16384


void
web_child(int sockfd)
{
    int      ntowrite;
    ssize_t  nread;
    char     line[MAXLINE], result[MAXN];

    for ( ;; ) {
        if ((nread = Readline(sockfd, line, MAXLINE)) == 0) {
            return; /* connection closed by the other end */
        }

        ntowrite = atol(line);
        if ((ntowrite <= 0) || (ntowrite >= MAXN)) {
            err_quit("client request for %d bytes\n", ntowrite);
        }

        Writen(sockfd, result, ntowrite);
    }
}

#include "unp.h"

extern pid_t *pids;
extern long  *cptr;

pid_t
child_make(int i, int listenfd, int addrlen)
{
    pid_t   pid;
    void    child_main(int, int, int);

    if ((pid = Fork()) > 0) {   /* parent */
        return pid;
    }

    child_main(i, listenfd, addrlen);
}

void
child_main(int i, int listenfd, int addrlen)
{
    int               connfd;
    void              web_child(int);
    socklen_t         clilen;
    struct sockaddr  *cliaddr;
    fd_set            rset;

    cliaddr = Malloc(addrlen);
    FD_ZERO(&rset);
    for ( ;; ) {
        FD_SET(listenfd, &rset);
        Select(listenfd+1, &rset, NULL, NULL, NULL);
        if (FD_ISSET(listenfd, &rset) == 0) {
            err_quit("listenfd readable");
        }

        clilen = addrlen;
        connfd = Accept(listenfd, cliaddr, &clilen);

        cptr[i]++;
        web_child(connfd);
        Close(connfd);
    }
}

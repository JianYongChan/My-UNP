#include "unp.h"

int
main(int argc, char **argv)
{
    int     listenfd, connfd, n;
    char    buf[100];
    fd_set  rset, xset;

    if (argc == 2)
        listenfd = Tcp_listen(NULL, argv[1], NULL);
    else if (argc == 3)
        listenfd = Tcp_listen(argv[1], argv[2], NULL);
    else
        err_quit("usage: %s [ <host> ] <port#>", argv[0]);

    connfd = Accept(listenfd, NULL, NULL);

    FD_ZERO(&rset);
    FD_ZERO(&xset);
    for ( ; ; ) {
        FD_SET(connfd, &rset);
        FD_SET(connfd, &xset);

        Select(connfd+1, &rset, NULL, &xset, NULL);

        if (FD_ISSET(connfd, &xset)) {
            n = Recv(connfd, buf, sizeof(buf)-1, MSG_OOB);
            buf[n] = 0;
            printf("read %d OOB byte: %s\n", n, buf);
        }

        if (FD_ISSET(connfd, &rset)) {
            if ((n = Read(connfd, buf, sizeof(buf)-1)) == 0) {
                printf("received EOF\n");
                exit(0);
            }
            buf[n] = 0;
            printf("read %d normal byte: %s\n", n, buf);
        }
    }
}

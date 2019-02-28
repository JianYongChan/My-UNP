#include "unp.h"

int
main(int argc, char **argv)
{
    int     listenfd, connfd, n, on = 1;
    char    buf[100];

    if (argc == 2)
        listenfd = Tcp_listen(NULL, argv[1], NULL);
    else if (argc == 3)
        listenfd = Tcp_listen(argv[1], argv[2], NULL);
    else
        err_quit("usage: %s [ <host> ] <port#>", argv[0]);

    Setsockopt(listenfd, SOL_SOCKET, SO_OOBINLINE, &on, sizeof(on));

    connfd = Accept(listenfd, NULL, NULL);
    sleep(5);

    for ( ; ; ) {
        if (Sockatmark(connfd))
            printf("at OOB mark\n");

        if ((n = Read(connfd, buf, sizeof(buf)-1)) == 0) {
            printf("received EOF\n");
            exit(0);
        }
        buf[n] = 0;
        printf("read %d byte: %s\n", n, buf);
    }
}

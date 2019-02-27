#include "unp.h"

/*
 * 书上前一个select例子出现了问题，书上说：
 *
 * 问题在于select一直指示一个异常条件，直到进程的读入越过带外数据。
 * 同一个带外数据不能读入多次，因为首次读入后，内核就清空这个单字节的缓冲区，
 * 再次指定MSG_OOB标志调用recv时，它将返回EINVAL
 *
 * 这里的解决方法是只在读入普通数据之后才select异常条件。
 */

int
main(int argc, char **argv)
{
    // justreadooob标志决定是否select异常条件
    int     listenfd, connfd, n, justreadoob = 0;
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
        if (justreadoob == 0)
            FD_SET(connfd, &xset);

        Select(connfd+1, &rset, NULL, &xset, NULL);

        if (FD_ISSET(connfd, &xset)) {
            n = Recv(connfd, buf, sizeof(buf)-1, MSG_OOB);
            buf[n] = 0;
            printf("read %d OOB byte: %s\n", n, buf);
            justreadoob = 1;
            FD_CLR(connfd, &xset);
        }

        if (FD_ISSET(connfd, &rset)) {
            if ((n = Read(connfd, buf, sizeof(buf)-1)) == 0) {
                printf("received EOF\n");
                exit(0);
            }
            buf[n] = 0;
            printf("read %d normal byte: %s\n", n, buf);
            justreadoob = 0;
        }
    }
}

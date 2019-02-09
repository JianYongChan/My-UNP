#include "unp.h"

int
main(int argc, char **argv)
{
    int    sockfd, n;
    char   recvline[MAXLINE+1];
    struct sockaddr_in srv_addr;
    struct in_addr **pptr;
    struct in_addr *inetaddrp[2];
    struct in_addr inetaddr;
    struct hostent *hp;
    struct servent *sp;

    if (argc != 3)
        err_quit("usage: %s <hostname> <service>", argv[0]);

    if ((hp = gethostbyname(argv[1])) == NULL) {
        // 如果gethostbyname失败
        // 则看看是否命令行就是用ASCII码指定的地址
        // 如果是，则手动构造一个地址列表
        if (inet_aton(argv[1], &inetaddr) == 0) {
            err_quit("hostname error for %s: %s", argv[1], hstrerror(h_errno));
        } else {
            inetaddrp[0] = &inetaddr;
            inetaddrp[1] = NULL;
            pptr = inetaddrp;
        }
    } else {
        // 使用gethostbyname返回的地址列表
        pptr = (struct in_addr **)hp->h_addr_list;
    }

    if ((sp = getservbyname(argv[2], "tcp")) == NULL)
        err_quit("getservbyname error for %s", argv[2]);

    for ( ; *pptr != NULL; pptr++) {
        sockfd = Socket(AF_INET, SOCK_STREAM, 0);

        bzero(&srv_addr, sizeof(srv_addr));
        srv_addr.sin_family = AF_INET;
        srv_addr.sin_port = sp->s_port;
        memcpy(&srv_addr.sin_addr, *pptr, sizeof(struct in_addr));
        printf("trying %s\n", Sock_ntop((SA *)&srv_addr, sizeof(srv_addr)));

        if (connect(sockfd, (SA *)&srv_addr, sizeof(srv_addr)) == 0)
                break;
        err_ret("connect error");

        // connect调用失败的fd必须关闭，不能再用
        close(sockfd);
    }

    // 一个地址都没有连上
    if (*pptr == NULL)
        err_quit("unable to connect");

    while ((n = Read(sockfd, recvline, MAXLINE)) > 0) {
        recvline[n] = 0;
        Fputs(recvline, stdout);
    }

    exit(0);
}

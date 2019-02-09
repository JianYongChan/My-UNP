#include "unp.h"

int
tcp_connect(const char *host, const char *serv)
{
    int     sockfd, n;
    struct addrinfo hints, *res, *ressave;

    bzero(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((n = getaddrinfo(host, serv, &hints, &res)) != 0)
        err_quit("tcp_connect error for %s, %s: %s",
                host, serv, gai_strerror(n));

    ressave = res;
    for ( ; res != NULL; res = res->ai_next) {
        // 这里不能调用socket的包裹函数Socket
        // 因为失败了一次还可以继续尝试下一个
        sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

        if (sockfd < 0)
            continue;
        // 成功连接
        if (connect(sockfd, res->ai_addr, res->ai_addrlen) == 0)
            break;

        Close(sockfd);
    }

    if (res == NULL)
        err_sys("tcp_connect error for %s, %s", host, serv);

    freeaddrinfo(ressave);

    return sockfd;
}

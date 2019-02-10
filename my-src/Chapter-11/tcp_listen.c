#include "unp.h"

int
tcp_listen(const char *host, const char *serv, socklen_t *addrlenp)
{
    int     listenfd, n;
    const int on = 1;
    struct addrinfo hints, *res, *ressave;

    bzero(&hints, sizeof(hints));
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((n = getaddrinfo(host, serv, &hints, &res)) != 0)
        err_quit("tcp_listen error for %s, %s: %s",
                host, serv, gai_strerror(n));
    ressave = res;

    for ( ; res != NULL; res = res->ai_next) {
        listenfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (listenfd < 0)
            continue;

        Setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
        // 这里不能!=0然后continue
        // 因为socket调用成功了，所以需要Close，而continue会跳过Close
        if (bind(listenfd, res->ai_addr, res->ai_addrlen) == 0)
            break;

        Close(listenfd);
    }

    if (res == NULL)
        err_ret("tcp_listen: error for %s, %s", host, serv);

    Listen(listenfd, LISTENQ);

    if (addrlenp)
        *addrlenp = res->ai_addrlen;

    freeaddrinfo(ressave);

    return listenfd;
}

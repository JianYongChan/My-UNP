#include "unp.h"

struct addrinfo *
host_serv(const char *host, const char *serv, int family, int socktype)
{
    int     n;
    struct addrinfo hints, *res;

    bzero(&hints, sizeof(hints));
    hints.ai_flags = AI_CANONNAME;  /* 总是返回canoncial name */
    hints.ai_family = family;       /* AF_INET, AF_INET6, AF_UNSPEC, ... */
    hints.ai_socktype = socktype;   /* 0, SOCK_STREAM, SOCK_DGRAM, ... */

    if ((n = getaddrinfo(host, serv, &hints, &res)) != 0)
        return NULL;

    return res;
}

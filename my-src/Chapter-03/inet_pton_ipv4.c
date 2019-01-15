#include "unp.h"

// inet_pton的IPv4的简化版本
int
inet_pton(int family, const char *strptr, void *addrptr)
{
    if (family == AF_INET) {
        struct in_addr in_val;

        if (inet_aton(strptr, &in_val)) {
            memcpy(addrptr, &in_val, sizeof(struct in_addr));
            return 0;
        }
    }

    errno = EAFNOSUPPORT;
    return -1;
}

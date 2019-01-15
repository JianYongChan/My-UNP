#include "unp.h"

// 往一个fd中读n个字节，返回实际读取的字节数
ssize_t
read_n(int fd, void *vptr, size_t n)
{
    size_t  nleft;
    ssize_t nread;
    char    *ptr;

    ptr = vptr;
    nleft = n;
    while (nleft > 0) {
        if ((nread = read(fd, ptr, nleft)) < 0) {
            if (errno == EINTR)
                nread = 0;
            else
                return -1;
        } else if (nread == 0) {
            break;
        }
        nleft -= nread;
        ptr += nread;
    }

    return n-nleft;
}

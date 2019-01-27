#include "unp.h"
#include <limits.h>     /* for OPEN_MAX */

int
main(int argc, char **argv)
{
    int     i, maxi, listenfd, connfd, sockfd;
    int     nready;
    ssize_t n;
    char    buf[MAXLINE];
    socklen_t clilen;
    struct pollfd client[OPEN_MAX];
    struct sockaddr_in cliaddr, srvaddr;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&srvaddr, sizeof(srvaddr));
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    srvaddr.sin_port = htons(SERV_PORT);

    Bind(listenfd, (SA *)&srvaddr, sizeof(srvaddr));

    Listen(listenfd, LISTENQ);

    // client用于存放监听套接字描述符
    client[0].fd = listenfd;
    client[0].events = POLLRDNORM;
    for (i = 1; i < OPEN_MAX; i++) {
        client[i].fd = -1;
    }
    maxi = 0;

    for ( ; ; ) {
        nready = Poll(client, maxi+1, INFTIM);

        // 检查监听套接字
        // 看看是否有新的连接到来
        if (client[0].revents & POLLRDNORM) {
            clilen = sizeof(cliaddr);
            connfd = Accept(listenfd, (SA *)&srvaddr, &clilen);

            for (i = 1; i < OPEN_MAX; i++) {
                // 找到第一个可存放客户的空位
                if (client[i].fd < 0) {
                    client[i].fd = connfd;
                    client[i].events = POLLRDNORM;  // 书上少了这一句
                    break;
                }
            }

            // 没有多余的空位用于接收客户端的连接
            if (i == OPEN_MAX)
                err_quit("too many clients");
            if (i > maxi)
                maxi = i;

            // 接下来就是检查已经建立了连接的客户了
            if (--nready <= 0)
                continue;
        }

        // 检查连接套接字
        // 看看客户端是否发送了数据或者断开了连接
        for (i = 1; i < maxi; i++) {
            if ((sockfd = client[i].fd) < 0)
                continue;
            // 这里将POLLRDNORM和POLLERR相或
            // 是因为有的实现在一个连接上接收到RST时返回的是POLLERR事件，而其他实现返回的是POLLRDNORM事件
            if (client[i].revents & (POLLRDNORM | POLLERR)) {
                if ((n = read(sockfd, buf, MAXLINE)) < 0) {
                    if (errno == ECONNRESET) {  // 接收到RST
                        Close(sockfd);
                        client[i].fd = -1;
                    } else {
                        err_sys("read error");
                    }
                } else if (n == 0) {    // 连接被客户端关闭
                    Close(sockfd);
                    client[i].fd = -1;
                } else {
                    // 将客户发送过来的数据回显回去
                    Writen(sockfd, buf, n);
                }

                // 看看是否还有其他客户发来了数据
                if (--nready <= 0)
                    break;
            }
        }
    }
}

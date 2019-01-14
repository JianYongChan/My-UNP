#include "unp.h"

int
main(int argc, char **argv)
{
    int                 sockfd, n;
    char                recvline[MAXLINE+1];
    struct sockaddr_in  server_addr;

    if (argc != 2)
        err_quit("usage: %s <IPaddress>", argv[0]);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        err_sys("socket error");

    // 使用bzero而不是memset更方便
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    // NOTE: htons: host to net short
    server_addr.sin_port = htons(13);

    if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr) <= 0)
        err_quit("inet_pton error for %s", argv[1]);

    // SA: struct sockaddr
    if (connect(sockfd, (SA *)&server_addr, sizeof(server_addr)) < 0)
        err_sys("connect error");

    // 在从TCP套接字读取数据时，我们总是需要把read编写在某个循环中
    // 当read返回0(对端关闭连接)或者负值(发送错误)时终止循环
    // 因为TCP是一个字节流协议
    while ((n = read(sockfd, recvline, MAXLINE)) > 0) {
        recvline[n] = 0;
        if (fputs(recvline, stdout) < 0)
            err_sys("fputs error");
    }
    // NOTE: 需要检查n是否小于0以判断是出错还是对端关闭了连接
    if (n < 0)
        err_sys("read error");

    // UNIX在一个进程终止时总是关闭该进程所有打开的描述符
    exit(0);
}

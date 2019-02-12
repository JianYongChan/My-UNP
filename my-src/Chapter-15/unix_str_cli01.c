#include "unp.h"

int
main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_un srv_addr;

    sockfd = Socket(AF_LOCAL, SOCK_STREAM, 0);

    bzero(&srv_addr, sizeof(srv_addr));
    srv_addr.sun_family = AF_LOCAL;
    strcpy(srv_addr.sun_path, UNIXSTR_PATH);

    Connect(sockfd, (SA *)&srv_addr, sizeof(srv_addr));

    str_cli(stdin, sockfd);

    exit(0);
}

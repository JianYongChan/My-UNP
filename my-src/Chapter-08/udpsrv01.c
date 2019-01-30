#include "unp.h"

int
main(int argc, char **argv)
{
    int     sockfd;
    struct sockaddr_in   srv_addr, cli_addr;

    sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&srv_addr, sizeof(srv_addr));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    srv_addr.sin_port = htons(SERV_PORT);

    Bind(sockfd, (SA *)&srv_addr, sizeof(srv_addr));

    dg_echo(sockfd, (SA *)&cli_addr, sizeof(cli_addr));
}

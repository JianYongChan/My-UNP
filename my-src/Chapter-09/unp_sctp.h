#ifndef UNP_SCTP_H__
#define UNP_SCTP_H__

#include <sys/types.h>  /* basic system data types */
#include <sys/socket.h> /* basic socket definations */
#include <sys/time.h>   /* timeval{} for select() */
#include <time.h>       /* timespece{} for pselect() */
#include <netinet/in.h> /* sockaddr_in{} and other Internet defns */
#include <arpa/inet.h>  /* inet(3) functions */
#include <netinet/sctp.h>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>    // bzero
#include <string.h>     // memcopy
#include <netinet/sctp.h>

typedef struct sockaddr SA;

#define SRVPORT 9877

#define MAXLINE 1024

#define BUFFSIZE 1024

#define SRV_MAX_SCTP_STRM 10

sctp_assoc_t sctp_address_to_associd(int sockfd, struct sockaddr *sa, socklen_t salen);

int sctp_get_no_strms(int sockfd, struct sockaddr *to, socklen_t tolen);

void sctp_str_cli(FILE *fp, int sockfd, SA *to, socklen_t tolen);
void sctp_str_cli_echoall(FILE *fp, int sockfd, SA *to, socklen_t tolen);
#endif /* UNP_SCTP_H__ */

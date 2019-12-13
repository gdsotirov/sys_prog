/* Example : pr7-2
 * Description: Client via TCP/IP socket.
 * Edited by  : George D. Sotirov, astronom@dir.bg
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 2121 /* communication port */

void fatal(char *err, int how);

int fd;
int sockfd;
int len;

int main(int argc, char *argv[]) {
    char buf[BUFSIZ]; /* transfer buffer */
    struct sockaddr_in addr_cl;
    struct hostent *hp;

    if ( argc != 4 ) {
        fprintf(stderr, "usage: %s host remote_file local_file\n", argv[0]);
        exit(1);
    }

    /* create client socket */
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
        fatal("Socket failed ", 0);

    /* get the address of the remote computer */
    if ( (hp = gethostbyname(argv[1])) == 0 )
        fatal("Gethostbyname failed ", 0);

    if ( hp->h_addrtype != AF_INET )
        fatal("Invalid address family ", 0);

    addr_cl.sin_family      = (sa_family_t)hp->h_addrtype;
    addr_cl.sin_port        = htons((uint16_t)PORT);
    addr_cl.sin_addr.s_addr = *(in_addr_t *)hp->h_addr;

    /* connect with the server */
    if ( connect(sockfd, (struct sockaddr *)&addr_cl, sizeof(addr_cl)) < 0 )
        fatal("Connect failed ", 0);

#define OPEN_FLAGS O_RDWR | O_CREAT | O_TRUNC
#define OPEN_MODE  S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH
    /* open/create local file */
    if ( (fd = open(argv[3], OPEN_FLAGS, OPEN_MODE)) < 0 )
        fatal("Open failed ", 1);
#undef OPEN_MODE
#undef OPEN_FLAGS

    /* remote file name length */
    len = strlen(argv[2]);

    /* send name to server */
    if ( send(sockfd, argv[2], len, 0) != len )
        fatal("Send failed ", 1);

    /* read remote file and write to local file */
    while ( (len = recv(sockfd, buf, sizeof(buf), 0)) != 0 ) {
        if ( len == -1 )
            fatal("Recv failed ", 1);
        if ( write(fd, buf, len) < 0 )
            fatal("Write failed ", 1);
    }

    shutdown(sockfd, 2); /* close connection */
    close(sockfd); /* destroy socket */
    close(fd); /* close local file */

    return 0;
}

/* print error message and/or client exit */
void fatal(char *err, int how) {
    perror(err);

    if ( how )
        shutdown(sockfd, 2);
    close(sockfd);

    exit(1);
}


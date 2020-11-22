/* Example    : pr7-1
 * Description: Server via TCP/IP socket.
 * Edited by  : Georgi D. Sotirov, gdsotirov@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/param.h>
#include <netinet/in.h>

#define NAMELEN 64 /* name length */
#define PORT 2121 /* communication port */

int sockfd; /* socket descriptors */
int sockfd_new;

void p_error(char error[], int errnum, int how);

int main() {
    int i;
    struct sockaddr_in addr;
    struct sockaddr_in addr_cl;
    socklen_t addrlen;
    struct hostent *hp = NULL;
    int flags; /* socket flags */
    char localhost[NAMELEN + 1]; /* local host name */

    /* server will work as a daemon */
    for ( i = 0; i < NOFILE; ++i )
        close(i); /* close open files */

    switch ( fork() ) { /* start new session */
        case -1 :   p_error("Fork failed", errno, 0);
                    break;
        default :   exit(0); /* close first parent */
        case  0 :   { /* first child, start new session */
                        if ( setsid() == -1 ) /* become new session leader */
                            p_error("Setsid failed", errno, 0);

                        switch ( fork() ) { /* second fork */
                            case -1 :   p_error("Fork failed", errno, 0);
                            case  0 :   /* second child is not session leader*/
                                        umask(0);
                                        break;
                            default :   exit(0); /* second parent exit */
                        }
                    }
    }

    signal(SIGCHLD, SIG_IGN); /* avoid zombies */

    /* create stram socket */
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
        p_error("Socket failed", errno, 0);

    if ( (gethostname(localhost, NAMELEN) == -1 ) || (hp = gethostbyname(localhost)) == 0 )
        p_error("Error getting hostname", errno, 1);

    addr.sin_family      = (sa_family_t)hp->h_addrtype; /* AF_INET */
    addr.sin_port        = htons((uint16_t)PORT);
    addr.sin_addr.s_addr = *(in_addr_t *)hp->h_addr;

    /* bind socket */
    if ( bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1 )
        p_error("Bind failed", errno, 1);

    /* prepare the socket for incoming queries */
    if ( listen(sockfd, 5) == -1 )
        p_error("Listen failed", errno, 1);

    if ( (flags = fcntl(sockfd, F_GETFL)) == -1 || fcntl(sockfd, F_SETFL, flags&~O_NDELAY) == -1 )
        p_error("Fcntl failed", errno, 1);

    /* receive and serve client requests */
    for ( ;; ) {
        addrlen = sizeof(addr_cl);

        if ( (sockfd_new = accept(sockfd, (struct sockaddr *)&addr_cl, &addrlen)) < 0 ) {
            p_error("Accept failed", errno, 2);
            continue;
        }

        switch ( fork() ) {
            case -1 :   p_error("Fork failed", errno, 3);
                        continue;
            case  0 :   { /* child for serving the client */
                            char addrcl[4], filename[256], buf[BUFSIZ];
                            int cnt, fd;

                            close(sockfd); /* close the old socket descriptor */

                            /* retrieve client IP address */
                            *addrcl = ntohl(addr_cl.sin_addr.s_addr);

                            /* read file name */
                            if ( (cnt = read(sockfd_new, &filename, 255)) == -1 )
                                p_error("Read failed", errno, 4);
                            filename[cnt] = '\0'; /* format file name string */

                            /* open file */
                            if ( (fd = open(filename, O_RDONLY)) == -1 )
                                p_error("Open failed", errno, 4);

                            /* read file, send to client */
                            while ( (cnt = read(fd, &buf, sizeof(buf))) != 0 ) {
                                if ( cnt == -1 )
                                    p_error("Read failed", errno, 4);
                                if ( write(sockfd_new, &buf, cnt) != cnt )
                                    p_error("Write failed", errno, 4);
                            }

                            /* child will exit */
                            shutdown(sockfd_new, 2);
                            close(sockfd_new);

                            exit(0);
                        }
        }

        /* parent: close socket descriptor */
        close(sockfd_new);
    }
}

/* Write messages to file and finish server work */
void p_error(char error[], int errnum, int how) {
    FILE *fp;

    if ( (fp = fopen("err.log", "a")) == 0 ) {
        fclose(fp);
        exit(1);
    }

    fprintf(fp, "%s error %d", error, errnum);
    fclose(fp);

    switch ( how ) {
        case 0 :    exit(2);
        case 1 :    close(sockfd);
                    exit(3);
        case 2 :    return;
        case 3 :    shutdown(sockfd_new, 2);
                    close(sockfd_new);
                    return;
        case 4 :    shutdown(sockfd_new, 2);
                    close(sockfd_new);
                    exit(4);
    }
}


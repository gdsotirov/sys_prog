/* Example    : pr3-13
 * Description: File locks example.
 * Edited by  : George D. Sotirov, astronom@dir.bg
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    struct flock fileck;
    int fd;

    if ( argc != 2 ) {
        printf("usage: %s file_name\n", argv[0]);
        return 0;
    }

    if ( (fd = open(argv[1], O_RDWR)) == -1 ) { /* open file in argument */
        perror("Error: Open failed ");
        return 1;
    }

    fileck.l_type   = F_WRLCK;  /* lock record */
    fileck.l_whence = SEEK_SET; /* from begining */
    fileck.l_start  = 0;        /* offset from whence is 0 */
    fileck.l_len    = 512;      /* length of locked record is 512 bytes */

    if ( fcntl(fd, F_SETLK, &fileck) == -1 ) { /* try to lock record */
        if ( fcntl(fd, F_GETLK, &fileck) == -1 ) { /* if file is locked ... */
            perror("Error: Fcntl failed ");
            return 2;
        }
        /* ... then print who locked it */
        printf("File %s is locked by process %d from offset %d for %d bytes.\n", argv[1], fileck.l_pid, (int)fileck.l_start, (int)fileck.l_len);
        printf("Lock type is %s.\n", ((fileck.l_type == F_RDLCK)?"Read":"Write"));
        return 3;
    }

    printf("Working...\n");
    sleep(5); /* simulate work with file */

    fileck.l_type   = F_UNLCK;  /* unlock record */
    fileck.l_whence = SEEK_SET; /* from begining */
    fileck.l_start  = 0;        /* offset from whence is 0 */
    fileck.l_len    = 512;      /* length of unlocked record is 512 bytes */

    if ( fcntl(fd, F_SETLKW, &fileck) == -1 ) {
        perror("Error: Fcntl failed ");
        return 4;
    }

    return 0;
}


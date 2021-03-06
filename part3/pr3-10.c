/* Example    : pr3-10
 * Description: Example for stat system call.
 * Edited by  : Georgi D. Sotirov, gdsotirov@gmail.com
 */

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
    /* Linux, Android, and other systems using GNU C library or HP-UX*/
#if defined(__linux__) || defined(__GLIBC__) || defined(hpux)
#include <sys/sysmacros.h>
    /* FreeBSD, OpenBSD, NetBSD, and DragonFlyBSD */ \
#elif defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__) || defined(__DragonFly__) || \
    /* Mac OS X */ \
    defined(__APPLE__) && defined(__MACH__) || \
    /* AIX, OSF/1, Tru64 Unix */ \
    defined(_AIX) || defined (__osf__)
#include <sys/types.h>
#elif defined(sun)
/* Solaris */
#include <sys/types.h>
#include <sys/mkdev.h>
#else
/* Unknown OS. Try the BSD approach. */
#include <sys/types.h>
#endif

struct stat buf;

int main(int argc, char **argv) {
    int i;

    if ( argc != 2 ) {
        printf("usage: %s file_name\n", argv[0]);
        return 1;
    }

    printf("Status of file %s\n", argv[1]);

    for ( i = 0; i < (strlen(argv[1]) + 16); i++ )
        printf("-");
    printf("\n");

    if ( stat(argv[1], &buf) ) { /* read file info */
        perror("Error: Stat failed ");
        return 2;
    }

    /* print file info on the screen */
    printf("File is on device  : %d%d\n", major(buf.st_dev), minor(buf.st_dev));
    printf("File owner         : %d\n", buf.st_uid);
    printf("File group         : %d\n", buf.st_gid);
    printf("File status        : %o\n", buf.st_mode);
    printf("File type          : ");

    switch ( buf.st_mode & S_IFMT ) { /* type of file */
        case S_IFREG :  printf("disk file\n");
                        break;
        case S_IFDIR :  printf("directory\n");
                        break;
        case S_IFCHR :  printf("byte device file\n");
                        break;
        case S_IFBLK :  printf("block device file\n");
                        break;
        case S_IFLNK :  printf("symbolic link\n");
                        break;
        case S_IFIFO :  printf("FIFO file\n");
                        break;
    }

    printf("File length        : %d bytes\n", (int)buf.st_size);
    printf("Owner access rights: "); /* owner rights */
    if ( buf.st_mode & S_IRUSR )
        printf("r");
    if ( buf.st_mode & S_IWUSR )
        printf("w");
    if ( buf.st_mode & S_IXUSR )
        printf("x");
    printf("\n\n");

    return 0;
}


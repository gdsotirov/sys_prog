/* Example    : pr3-14
 * Description: Directory listing example.
 * Edited by  : Georgi D. Sotirov, gdsotirov@gmail.com
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char **argv) {
    typedef struct dirent Dirent;
    Dirent *dirp;
    DIR *dirfd;
    int count = 0;

    if ( argc != 2 ) {
        printf("usage: %s path_name\n", argv[0]);
        return 0;
    }

    if ( !(dirfd = opendir(argv[1])) ) { /* open dir */
        perror("Error: Opendir failed ");
        if ( mkdir(argv[1], 0755) == -1 )
            perror("Error: Mkdir "); /* create dir */
        else printf("Directory '%s' created.\n", argv[1]);

        return 1;
    }

    printf("Directory %s content:\n", argv[1]);
    printf("-------------------\n");
    while ( (dirp = readdir(dirfd)) ) { /* read and print elements to screen */
        printf("%s\n",dirp->d_name);
        if ( strcmp(dirp->d_name, ".") && strcmp(dirp->d_name, "..") )
            count++;
    }

    /* id dir is empty - then remove it */
    if ( count == 0 ) {
        rmdir(argv[1]);
        printf("Directory '%s' removed.\n", argv[1]);
    }
    else closedir(dirfd); /* close dir */

    return 0;
}


/* Example    : pr3-4
 * Description: File descriptors duplication example.
 * Edited by  : George D. Sotirov, astronom@dir.bg
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    int dup_fd;
    char ch;

    if ( (dup_fd = dup(STDIN_FILENO)) == -1 ) { /* diplicate descriptor */
        perror("Error: Dup failed ");
        _exit(1);
    }

    close(STDIN_FILENO); /* close std input file */

    if ( open("test", O_RDONLY) == -1 ) {
        perror("Error: Open failed ");
        _exit(2);
    }

    while ( (ch = getchar()) != EOF )
        putchar(ch);

    return 0;
}


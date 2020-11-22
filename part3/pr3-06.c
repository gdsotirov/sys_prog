/* Example    : pr3-6
 * Description: Example for a 'ln' command.
 * Edited by  : Georgi D. Sotirov, gdsotirov@gmail.com
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    if ( (argc < 3 || argc > 4) || (argc == 4 && strcmp(argv[1], "-s")) ) {
        printf("usage: %s [-s] file sym_link\n", argv[0]);
        return 1;
    }

    if ( argc == 4 )
        if ( symlink(argv[2], argv[3]) == -1 ) {
            perror("Error: Symlink failed ");
            return 2;
        }

    if ( argc == 3 )
        if ( link(argv[1], argv[2]) == -1 ) {
            perror("Error: Link failed ");
            return 3;
        }

    return 0;
}


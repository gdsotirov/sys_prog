/* Example    : pr3-12
 * Description: Change file owner example.
 * Edited by  : George D. Sotirov, astronom@dir.bg
 */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <pwd.h>

int chowner(const char *username, const char *pathname) {
    struct passwd *pwd;

    if ( (pwd = getpwnam(username)) == NULL ) {
        printf("Invalid user name!\n");
        return 1;
    }

    if ( chown(pathname, pwd->pw_uid, -1) == -1 ) {
        perror("Error: Chown failed ");
        return 2;
    }

    return 0;
}

int main(int argc, char **argv) {
    if ( argc != 3 ) {
        printf("usage: %s new_owner file_path\n", argv[0]);
        return 0;
    }

    if ( chowner(argv[1], argv[2]) == 0 )
        printf("The file owner was changed to %s\n", argv[1]);
    else return 1;

    return 0;
}


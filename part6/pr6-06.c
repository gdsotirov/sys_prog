/* Example    : pr6-6
 * Description: Process client.
 * Edited by  : George D. Sotirov, astornom@dir.bg
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "msgbuf.h"


int main() {
    int qdes;
    struct msgbuf message;
    struct msgstruct *msg = (struct msgstruct *)message.mtext;

    printf("TimeClient starts with pid %d. TimeClient ready...\n", getpid());
    if ( (qdes = msgget(MSG_KEY, 0666)) == -1 ) {
        perror("Error: Msgget failed ");
        exit(1);
    }

    /* send queryes and receive answers */
    while ( 1 ) {
        printf("Please choose:\n");
        printf("1 - get local time\n");
        printf("2 - get universal time\n");
        printf("3 - delete message queue, client&server exit\n");
        printf("Ctrl + d - client exits\n\n");
        printf("Your choice: ");

        /* read query type */
        if ( scanf("%ld", &message.mtype) == EOF )
            break; /* client exit if Ctrl-d (EOF) is pressed */

        if ( message.mtype < 0 || message.mtype > 3 )
            continue;

        /* format message */
        msg->uid = getuid();
        msg->pid = getpid();

        // send message to server
        if ( msgsnd(qdes, &message, sizeof(message), 0) == -1 ) {
            perror("Error: Msgsnd failed ");
            continue;
        }

        /* receive answer message from server */
        if ( msgrcv(qdes, &message, sizeof(message), getuid(), 0) == -1 )
            perror("Error: Msgrcv failed ");
        else {
            printf("TimeClient (%d) receives:\n", getpid());
            printf("Type = %ld\n", message.mtype);
            printf("Text = %s\n", message.mtext);
        }

        if ( !strcmp(message.mtext, "TimeServer exits.") )
            break; /* server exited */
    }

    printf("TimeClient (%d) exits.\n", getpid());

    exit(0); /* client exits */
}


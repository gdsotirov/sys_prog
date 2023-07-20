/* Example    : pr6-5
 * Description: Process server.
 * Edited by  : Georgi D. Sotirov, gdsotirov@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "msgbuf.h"

int qdes; /* queue descriptor */

void on_sig(int); /* SIGTSTP handler */

int main() {
    int i;
    uid_t uid;
    pid_t pid;
    time_t time_buf;
    struct msgbuf message;
    struct msgstruct *msg = (struct msgstruct *)message.mtext;

    signal(SIGTSTP, on_sig); /* signal catch */

    /* process will work like a deamon in background */
    for ( i = 0; i < 20; i++ )
        signal(i, SIG_IGN); /* ignore some signals */

    setsid(); /* process become sesion leader */

    printf("TimeServer started. Server ready...\n");

    /* create queue with key 20 */
    if ( (qdes = msgget(MSG_KEY, IPC_CREAT | 0666)) == -1 ) {
        perror("Error: Msgget failed ");
        exit(1);
    }

    while ( 1 ) { /* wait client queryes */
        printf("TimeServer waits messages from clients...\n");

        /* read type MSG_TYPE messages */
        if ( msgrcv(qdes, &message, sizeof(message), -3, 0) == -1 ) {
            perror("Error: Msgrcv failed ");
            /* server destroy queue and exit */
            if ( msgctl(qdes, IPC_RMID, 0) == -1 )
                perror("Error: Msgctl failed ");
            exit(2);
        }

        uid = msg->uid;
        pid = msg->pid;

        switch ( message.mtype ) { /* sevice the client */
            case 1 :    (void)time(&time_buf); /* read time in seconds */
                        /* record local time in message text */
                        strcpy(message.mtext, ctime(&time_buf));
                        break;
            case 2 :    (void)time(&time_buf); /* read time in seconds */
                        /* record UTC time in message text */
                        strcpy(message.mtext, asctime(gmtime(&time_buf)));
                        break;
            case 3 :    /* server exit */
                        message.mtype = uid;
                        strcpy(message.mtext, "TimeServer exits.");

                        if ( msgsnd(qdes, &message, sizeof(message), 0) == -1 )
                            perror("Error: Msgsnd failed ");

                        printf("TimeServer deletes message queue.\n");
                        if ( msgctl(qdes, IPC_RMID, 0) == -1 )
                            perror("Error: Msgctl failed ");

                        printf("TimeServer exits on message receiving.\n");

                        return 0;
        }

        message.mtype = uid;

        printf("Serving client with pid = %d...\n", pid);
        if ( msgsnd(qdes, &message, sizeof(message), 0) == -1 )
            perror("Error: Msgsnd failed ");
    }

    return 0;
}

void on_sig(int sig) {
    if ( msgctl(qdes, IPC_RMID, 0) == -1 )
        perror("Error: Msgctl failed ");
    printf("TimeServer exits by Ctrl-z.\n");

    exit(0); // server exits
}


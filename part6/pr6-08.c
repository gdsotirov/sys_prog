/* Example    : pr6-8
 * Description: Reader-writer example with semaphores.
 * Edited by  : George D. Sotirov, astronom@dir.bg
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>

#include "semc.h"

#define FNAME "temp"

union semun arg;
int semdes;
int fdes;
int status;
pid_t pid;

void on_term(int sig) {
    printf("\nChild (consumer) caught a SIGTERM signal and exits.\n");

    exit(0);
}

int main() {
    key_t key; /* semaphor array key */
    char buffer[80]; /* data buffer */
    key = ftok("..", 2); /* generate key */
    struct sembuf p_sbuf = {1, -1, SEM_UNDO}; /* buffer for p-operation */
    struct sembuf v_sbuf = {0, 1, SEM_UNDO}; /* buffer for v-operation */
    struct semid_ds my_semds; /* control data */

    if ( (semdes = semget(key, 2, IPC_CREAT | IPC_EXCL | 0666)) == -1 ) {
        perror("Error: Semget failed ");
        exit(1);
    }

    arg.buf = &my_semds;
    if ( semctl(semdes, 0, IPC_STAT, arg) == -1 ) { /* semaphor array status */
        perror("Error: Semctl failed ");
        exit(2);
    }

    printf("Semaphors in the array = %lu\n", arg.buf->sem_nsems);
    printf("Semaphor creator is user with id %d\n", arg.buf->sem_perm.cuid);

    /* initialize operation buffer */
    arg.val = 0; /* semaphor with index 0 (full) */
    if ( semctl(semdes, 0, SETVAL, arg) == -1 ) {
        perror("Error: Semctl failed ");
        exit(3);
    }

    arg.val = 1; /* semaphor with index 1 (empty) */
    if ( semctl(semdes, 1, SETVAL, arg) == -1 ) {
        perror("Error: Semctl failed ");
        exit(4);
    }

    switch ( pid = fork() ) { /* create child process */
        case -1 :   perror("Error: Fork failed ");
                    exit(5);
        case  0 :   /* child process */
                    p_sbuf.sem_num = 0;
                    v_sbuf.sem_num = 1;

                    signal(SIGTERM, on_term);

                    /* create file */
                    fdes = open(FNAME, O_CREAT | O_RDWR, 0666);

                    while ( 1 ) {
                        /* p-operation on sem 0 */
                        if ( semop(semdes, &p_sbuf, 1) == -1 ) {
                            perror("Error: Semop failed ");
                            exit(6);
                        }

                        if ( read(fdes, buffer, sizeof(buffer)) == -1 )
                            perror("Error: Read failed ");

                        printf("Consumer reads: %s\n", buffer);

                        /* v-operation on sem 1 */
                        if ( semop(semdes, &v_sbuf, 1) == -1 ) {
                            perror("Error: Semop failed ");
                            exit(7);
                        }
                    }
        default :   /* parent process */
                    fdes = open(FNAME, O_CREAT | O_TRUNC | O_RDWR, 0666);

                    while ( 1 ) {
                        /* p-operation on sem 1 */
                        if ( semop(semdes, &p_sbuf, 1) == -1 ) {
                            perror("Error: Semop failed ");
                            exit(8);
                        }

                        printf("\nProducer. Insert string: ");

                        if ( scanf("%s", buffer) == EOF ) {
                            kill(pid, SIGTERM);
                            waitpid(pid, &status, 0);
                            break;
                        }

                        /* write buffer in file */
                        if ( write(fdes, buffer, sizeof(buffer)) == -1 ) {
                            perror("Error: Write failed ");
                            exit(9);
                        }

                        /* v-operation on sem 0 */
                        if ( semop(semdes, &v_sbuf, 1) == -1 ) {
                            perror("Error: Semop failed ");
                            exit(10);
                        }
                    }
    }

    printf("\nParent (producer) exits.\n");

    close(fdes);
    unlink(FNAME);

    /* destroy semaphores array */
    if ( semctl(semdes, 0, IPC_RMID, 0) == -1 ) {
        perror("Error: Semctl failed ");
        exit(11);
    }

    exit(0);
}


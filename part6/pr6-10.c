/* Example    : pr6-10
 * Description: Shared memory example.
 * Edited by  : George D. Sotirov, astronom@dir.bg
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>

#include "semc.h"

#define SHM_SIZE 1024

int semdes;
int shmdes;
int status;
pid_t pid; /* process id */
char *shmaddr; /* pointer to shared memory segment */

void on_sig(int sig); /* signal handler */
void on_term(int sig); /* signal SIGTERM handler */

int main() {
    key_t key = ftok("/tmp", 's'); /* key generation */
    struct sembuf p_sbuf = {1, -1, SEM_UNDO}; /* P operation buffer */
    struct sembuf v_sbuf = {0, 1, SEM_UNDO}; /* V operation buffer */
    struct shmid_ds my_shmds; /* used in shmctl */
    union semun arg; /* argument in semctl */
    int i;

    /* Create/open semaphores array */
    if ( (semdes = semget(key, 2, IPC_CREAT | IPC_EXCL | 0666)) == -1 ) {
        perror("Error: Semget failed ");
        exit(1);
    }

    /* Semaphores initialization (SETALL can be used too) */
    arg.val = 0;
    if ( semctl(semdes, 0, SETVAL, arg) == -1 ) { /* semaphore #0 ('full') */
        perror("Error: Semctl failed ");
        exit(2);
    }

    arg.val = 1;
    if ( semctl(semdes, 1, SETVAL, arg) == -1 ) { /* semaphore #1 ('empty') */
        perror("Error: Semctl failed ");
        exit(3);
    }

    /* Create/open shared memory segment */
    if ( (shmdes = shmget(key, SHM_SIZE, IPC_CREAT | IPC_EXCL | 0666)) == -1 ) {
        perror("Error: Shmget failed ");
        exit(4);
    }

    /* Read and set permissions to shared memory segment */
    if ( shmctl(shmdes, IPC_STAT, &my_shmds) == -1 ) { /* get the status */
        perror("Error: Shmctl failed ");
        exit(5);
    }
    printf("Old permissions were: %o\n", my_shmds.shm_perm.mode);

    my_shmds.shm_perm.mode = 0600;
    if ( shmctl(shmdes, IPC_SET, &my_shmds) == -1 ) { /* set new permissions */
        perror("Error: Shmctl failed ");
        exit(6);
    }
    printf("New permissions are: %o\n", my_shmds.shm_perm.mode);

    switch ( pid = fork() ) {
        case -1 :   perror("Error: Fork failed ");
                    exit(7);
        case  0 :   /* child process */
                    for ( i = 1; i < 20; i++ )
                        sigset(i, on_term); /* catch main signals */

                    p_sbuf.sem_num = 0; /* values for semaphore operations */
                    v_sbuf.sem_num = 1;

                    /* assign shared memory segment */
                    if ((shmaddr = (char *)shmat(shmdes, 0, 0)) == (char *)-1) {
                        perror("Error: Shmat failed ");
                        exit(8);
                    }

                    while ( 1 ) {
                        /* semaphore operation P(full) */
                        if ( semop(semdes, &p_sbuf, 1) == -1 )
                            perror("Error: Semop failed ");

                        printf("Shared memory contents: %s\n", shmaddr);

                        /* semaphore operation V(empty) */
                        if ( semop(semdes, &v_sbuf, 1) == -1 )
                            perror("Error: Semop failed ");
                    }
        default :   /* parent process */
                    for ( i = 1; i < 20; i++ )
                        signal(i, on_sig); /* catch main signals */

                    /* assign shared memory segment */
                    if ((shmaddr = (char *)shmat(shmdes, 0, 0)) == (char *)-1) {
                        perror("Error: Shmat failed ");
                        exit(9);
                    }

                    while ( 1 ) {
                        /* operation p('empty') */
                        if ( semop(semdes, &p_sbuf, 1) == -1 )
                            perror("Error: Semop failed ");

                        printf("Producer. Insert text: ");
                        /* insert text in the buffer */
                        fgets(shmaddr, SHM_SIZE, stdin);

                        /* operation v('full') */
                        if ( semop(semdes, &v_sbuf, 1) == -1 )
                            perror("Error: Semop failed ");
                    }
    }
}

void on_sig(int sig) {
    if ( sig != SIGCHLD )
        kill(pid, SIGTERM); /* kill the child */

    waitpid(pid, &status, 0); /* wait child to exit */

    if ( shmdt(shmaddr) == -1 ) /* detach shared memory */
        perror("Error: Shmdt failed ");

    /* destroy semaphors array and shared memory */
    if ( shmctl(shmdes, IPC_RMID, 0) == -1 )
        perror("Error: Shmctl failed ");

    if ( semctl(semdes, 0, IPC_RMID, 0) == -1 )
        perror("Error: Semctl failed ");

    printf("Parent exits.\n");

    exit(0);
}

void on_term(int sig) {
    if ( shmdt(shmaddr) == -1 )
        perror("Error: Shmdt failed "); /* detach shared memory */

    printf("Child exits.\n");

    exit(0);
}


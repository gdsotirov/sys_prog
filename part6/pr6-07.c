/* Example    : pr6-7
 * Description: Semaphores array example.
 * Edited by  : Georgi D. Sotirov, gdsotirov@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include "semc.h"

#define NSEMS 3

int main() {
    int semdes;
    struct sembuf sbuf[NSEMS] = {{0,  1, IPC_NOWAIT},
                                 {1,  0, 0},
                                 {2, -1, IPC_NOWAIT}};
    key_t key = ftok("/tmp", 's');

    /* open array of semaphores */
    if ( (semdes = semget(key, NSEMS, IPC_CREAT | IPC_EXCL | 0666)) == -1 ) {
        perror("Error: Semget failed ");
        exit(1);
    }
    printf("Semaphores got.\n");

    if ( semop(semdes, sbuf, 3) == -1 ) {
        perror("Error: Semop failed ");
        exit(2);
    }
    printf("Semaphore operations complete.\n");

    if ( semctl(semdes, 0, IPC_RMID, 0) == -1 ) {
        perror("Error: Semctl failed ");
        exit(3);
    }

    return 0;
}


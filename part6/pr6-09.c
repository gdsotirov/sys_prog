/* Example    : pr6-9
 * Description: Reader-writer example with semaphores.
 * Edited by  : George D. Sotirov, astronom@dir.bg
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main() {
    int shmdes; /* share memory descriptor */
    char *shmaddr; /* share memory start address */
    struct shmid_ds shmbuf;

    if ( (shmdes = shmget(10, 256, IPC_CREAT | IPC_EXCL | 0666)) == -1 ) {
        perror("Error: Shmget failed ");
        if ( errno == EEXIST )
            shmdes = shmget(10, 0, 0); /* open segment */
        else exit(1);
    }

    /* create segment */
    if ( (shmaddr = (char *)shmat(shmdes, 0, 0)) == (char*)-1 ) {
        perror("Error: Shmat failed ");
        exit(2);
    }

    /* write to the share memory segment */
    strcpy(shmaddr, "Hello from share memory:-)\0");

    printf("Shared memory address: %p\n", shmaddr);
    printf("Shared memory contents: %s\n", shmaddr);
    /* get share memory status */
    if ( shmctl(shmdes, IPC_STAT, &shmbuf) == -1 ) {
        perror("Error: Shmctl failed ");
        exit(3);
    }
    else {/* printout informatin about the shared memory segment */
        printf("\nShare memory %d\n", shmdes);
        printf("------------------\n");
        printf("Permissions>\n");
        printf(" Effective uid of owner      : %d\n", shmbuf.shm_perm.uid);
        printf(" Effective gid of owner      : %d\n", shmbuf.shm_perm.gid);
        printf(" Effective uid of creator    : %d\n", shmbuf.shm_perm.cuid);
        printf(" Effective gid of creator    : %d\n", shmbuf.shm_perm.cgid);
        printf(" Access mode for own,grp,oth : %d\n", shmbuf.shm_perm.mode);
        printf(" Sequence counter            : %d\n", shmbuf.shm_perm.__seq);
        printf("Sizes>\n");
        printf(" Segment size in bytes       : %d\n", shmbuf.shm_segsz);
        /*printf(" Size of segment in pages    : %d\n", shmbuf.__shm_npages);*/
        printf("Processes>\n");
        printf(" Process make last oper (pid): %d\n", shmbuf.shm_lpid);
        printf(" Process created sh mem (pid): %d\n", shmbuf.shm_cpid);
        printf(" Attached to segm proc count : %lu\n", shmbuf.shm_nattch);
#if defined(__sun__) || defined(__solaris__)
        printf(" Used by shm_info (cattch)   : %lu\n", shmbuf.shm_cnattch);
#endif
        printf("Timers>\n");
        printf(" Time of last attach (shmat) : %s", ctime(&shmbuf.shm_atime));
        printf(" Time of last disp   (shmdt) : %s", ctime(&shmbuf.shm_dtime));
        printf(" Time of last change (chmctl): %s", ctime(&shmbuf.shm_ctime));
#if defined(__sun__) || defined(__solaris__)
        printf("Others>\n");
        printf(" For exchange compatability  : %s\n", chmbuf.pad);
#endif
    }

    if ( shmdt(shmaddr) == -1 ) { /* detach segment from process memory */
        perror("Error: Shmdt failed ");
        exit(4);
    }

    if ( shmctl(shmdes, IPC_STAT, &shmbuf) == -1 ) {
        perror("Error: Shmctl failed ");
        exit(5);
    }
    else { /* printout informatin about the shared memory segment */
        printf("\nShare memory %d\n", shmdes);
        printf("------------------\n");
        printf("Permissions>\n");
        printf(" Effective uid of owner      : %d\n", shmbuf.shm_perm.uid);
        printf(" Effective gid of owner      : %d\n", shmbuf.shm_perm.gid);
        printf(" Effective uid of creator    : %d\n", shmbuf.shm_perm.cuid);
        printf(" Effective gid of creator    : %d\n", shmbuf.shm_perm.cgid);
        printf(" Access mode for own,grp,oth : %d\n", shmbuf.shm_perm.mode);
        printf(" Sequence counter            : %d\n", shmbuf.shm_perm.__seq);
        printf("Sizes>\n");
        printf(" Segment size in bytes       : %d\n", shmbuf.shm_segsz);
        /*printf(" Size of segment in pages    : %d\n", shmbuf.__shm_npages);*/
        printf("Processes>\n");
        printf(" Process make last oper (pid): %d\n", shmbuf.shm_lpid);
        printf(" Process created sh mem (pid): %d\n", shmbuf.shm_cpid);
        printf(" Attached to segm proc count : %lu\n", shmbuf.shm_nattch);
#if defined(__sun__) || defined(__solaris__)
        printf(" Used by shm_info (cattch)   : %lu\n", shmbuf.shm_cnattch);
#endif
        printf("Timers>\n");
        printf(" Time of last attach (shmat) : %s", ctime(&shmbuf.shm_atime));
        printf(" Time of last disp   (shmdt) : %s", ctime(&shmbuf.shm_dtime));
        printf(" Time of last change (chmctl): %s", ctime(&shmbuf.shm_ctime));
#if defined(__sun__) || defined(__solaris__)
        printf("Others>\n");
        printf(" For exchange compatability  : %s\n", chmbuf.pad);
#endif
    }

    if ( shmctl(shmdes, IPC_RMID, 0) == -1 ) {
        perror("Error: Shmctl failed");
        exit(6);
    }

    exit(0);
}


/*****************************************************************************
 *                          THREADS PACKAGE V10.52                           *
 *                      Author (c) Lilyan Nikolov, 2000                      *
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <ucontext.h>
#include <setjmp.h>
#include <sys/time.h>

/* Define JB_BP, JB_SP and JB_PC macros if not available as Glibc 2.5 removed
 * these from setjmp.h */
#ifndef JB_BP
#define JB_BP 3
#endif

#ifndef JB_SP
#define JB_SP 4
#endif

#ifndef JB_PC
#define JB_PC 5
#endif

#define NTHREADS 20 /* set maximum threads */

typedef sigjmp_buf context; /* define type 'context' */
typedef unsigned int sema_t; /* define 'semaphore' type */
typedef unsigned int thread_t; /* define 'straem' type */

enum thread_state {RUNNING, READY, BLOCKED, DELAYED, TERMINATED};

struct thread_TCB {             /* TCB - descriptor */
    context         thread_context;     /* thread context           */
    char            *thread_stack;      /* stack pointer            */
    enum            thread_state state; /* thread state             */
    unsigned int    *ptr_to_sem;        /* pointer to semaphore     */
    unsigned long   delay_time;         /* delay time, ms           */
} thread[NTHREADS]; /* Threads table */

extern unsigned int  thread_id; /* running thread id                */
extern unsigned int  next_id;   /* next thread id                   */
extern unsigned int  old_t_id;  /* old stream thread id             */
extern unsigned char sleepers;  /* count of sleeping threads        */
extern unsigned char start_fl;  /* start flag for multithreading    */

/* create thread */
int thr_create(void (*func)(void), size_t stack_size, thread_t *id);
int thr_delete(thread_t id);    /* destroy thread                   */
void thr_mexit();               /* exit multithreading mode         */
void thr_yield(void);           /* invoke dispatcher                */
void thr_delay(unsigned long ticks);    /* delay thread             */
void sema_pb(sema_t *sem);      /* operation on binary semaphore    */
void sema_vb(sema_t *sem);      /* V operation on binary semaphore  */
void sema_pc(sema_t *sem);      /* operation on common semaphore    */
void sema_vc(sema_t *sem);      /* V operation on common semaphore  */
void parbegin(void);            /* begin multithreading mode        */
void parend(void);              /* end multithreading mode          */


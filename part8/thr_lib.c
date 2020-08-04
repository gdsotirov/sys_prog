/*****************************************************************************
 *                         THREADS PACKAGE V10.52                            *
 *                      Author (c) Lilyan Nikolov, 2000                      *
 *****************************************************************************/

#include "thr_def.h"

#if __WORDSIZE == 64
#define JMPBUF_TYPE long
#else
#define JMPBUF_TYPE int
#endif

context start_context; /* start context */
struct itimerval val1; /* Timer (SVR4 virtual timer) */
struct sigaction act1; /* Signal handler (SIGVTALRM) */

unsigned int  thread_id; /* running thread id                */
unsigned int  next_id;   /* next thread id                   */
unsigned int  old_t_id;  /* old stream thread id             */
unsigned char sleepers;  /* count of sleeping threads        */
unsigned char start_fl;  /* start flag for multithreading    */

static void check_timeout(int signal);  /* signal handler           */
static int ready_threads(void);         /* ready threds count       */
static void free_space(void);           /* free stack spacees       */
static void check_sleepers(void);       /* sleeping threads count   */
static void error(unsigned int err);    /* print error message      */

/* Function: check_timeout
 * Descr   : Signals handler. Decrease by 1 delay time of delayed threads.
 *           Record their count into "sleepers".
 * Input   : Signal.
 */
static void check_timeout(int sig) {
    unsigned int i;

    sleepers = 0;
    for ( i = 0; i < NTHREADS; i++ ) {
        if ( thread[i].state == DELAYED ) {
            thread[i].delay_time--;
            if ( thread[i].delay_time == 0 )
                thread[i].state = READY;
            else sleepers++;
        }
    }
}

/* Function: ready_threads
 * Descr   : Count ready threads.
 * Return  : Count of ready threads.
 */
static int ready_threads(void) {
    int i;
    int numb = 0;

    for ( i = 0; i < NTHREADS; i++ )
        if ( thread[i].state == READY)
            numb++;

    return numb;
}

/* Function: free_space
 * Descr   : Free stack spaces on all threads.
 */
static void free_space(void) {
    unsigned int i;

    for ( i = 0; i < NTHREADS; i++ )
        if ( thread[i].thread_stack )
            free(thread[i].thread_stack);
}

/* Function: check_cleepers
 * Descr   : Count sleeping threads.
 */
static void check_sleepers(void) {
    unsigned int i;

    sleepers = 0;

    for ( i = 0; i < NTHREADS; i++ )
        if ( thread[i].state == DELAYED)
            sleepers++;
}

/* Function: error
 * Descr   : Print error message.
 */
static void error(unsigned int i) {
    switch ( i ) {
        case 1 :    puts("Error #1: No free entry in thread table.\n");
                    break;
        case 2 :    puts("Error #2: No free memory.\n");
                    break;
        case 3 :    puts("Error #3: The thread does not exist.\n");
                    break;
        case 4 :    puts("Error #4: The thread is not in READY or RUNNING state.\n");
                    break;
        case 5 :    puts("Warning #5: All threads are terminated or dead lock.\n");
                    puts("Multithreading ends.\n");
                    break;
        case 6 :    puts("User exit.\n");
                    break;
        case 7 :    puts("Warning #7: Only one thread is running - the dispatcher can't switch.\n");
                    break;
    }
}

/* Function: thr_create
 * Descr   : Create new thread.
 * Input   : function name, stack size.
 * Output  : Thread ID.
 */
int thr_create(void (*func)(void), size_t stack_size, thread_t *id) {
    /* Search empty element in the threads table */
    while ( thread[next_id].state != TERMINATED && next_id < NTHREADS )
        next_id++;

    if ( next_id >= NTHREADS ) { /* Error */
        error(1);
        _exit(1);
    }

    *id = next_id; /* Give ID to the new thread */

    /***** Allocate stack memory *****/
    thread[next_id].thread_stack = (char *)malloc(stack_size);

    if ( thread[next_id].thread_stack == NULL ) {
        error(2);
        _exit(1);
    }

    /* Set new thread context */
    sigsetjmp(thread[next_id].thread_context, 1);
#ifdef __linux__
    thread[next_id].thread_context->__jmpbuf[JB_SP] = (JMPBUF_TYPE)(thread[next_id].thread_stack + stack_size);
    thread[next_id].thread_context->__jmpbuf[JB_PC] = (JMPBUF_TYPE)func;
    thread[next_id].thread_context->__jmpbuf[JB_BP] = thread[next_id].thread_context->__jmpbuf[JB_SP];
#elif defined(__APPLE__)
    thread[next_id].thread_context[JB_SP] = (JMPBUF_TYPE)(thread[next_id].thread_stack + stack_size);
    thread[next_id].thread_context[JB_PC] = (JMPBUF_TYPE)func;
    thread[next_id].thread_context[JB_BP] = thread[next_id].thread_context[JB_SP];
#elif defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__) || defined(__DragonFly__)
    thread[next_id].thread_context->_sjb[JB_SP] = (JMPBUF_TYPE)(thread[next_id].thread_stack + stack_size);
    thread[next_id].thread_context->_sjb[JB_PC] = (JMPBUF_TYPE)func;
    thread[next_id].thread_context->_sjb[JB_BP] = thread[next_id].thread_context->__jmpbuf[JB_SP];
#endif
    thread[next_id].state                           = READY;

    return 0;
}

/* Function: thr_delete
 * Descr   : Destroy thread.
 * Input   : Thread ID.
 */
int thr_delete(thread_t id) {
    if ( thread[id].state == TERMINATED ) { /* Error */
        error(3);
        return 3;
    }

    thread[id].state = TERMINATED;
    free(thread[id].thread_stack);
    thread[id].thread_stack = NULL;

    if ( id == thread_id )  /* Current thread? */
        thr_yield();

    return 0;
}

/* Function: thr_mexit
 * Descr   : End multithreading and return to 'main'.
 */
void thr_mexit() {
    free_space(); /* free stack spaces */
    siglongjmp(start_context, 1);
}

/* Dispatcher - choose next thread for execution */
void thr_yield() {
    if ( start_fl ) { /* At start choose first thread */
        thread[thread_id].state = RUNNING;
        start_fl = 0;

        siglongjmp(thread[thread_id].thread_context, 1);
    }

    if (thread[thread_id].state == RUNNING )
        thread[thread_id].state = READY;

    check_sleepers(); /* how many are sleeping threads */

    /* wait for ready thread */
    while ( ready_threads() == 0 && sleepers > 0 )
        ;

    /* Dead lock or all thread are destroyed */
    if ( ready_threads() == 0 && sleepers == 0 ) {
        error(5);
        thr_mexit();
    }

    /* Search next ready thread */
    if ( 2 == sigsetjmp(thread[thread_id].thread_context, 1) )
        return;

    old_t_id = thread_id;   /* remember old id */

    thread_id++;
    thread_id %= NTHREADS;

    while ( thread[thread_id].state != READY ) {
        thread_id++;
        thread_id %= NTHREADS;
    }

    if ( old_t_id == thread_id ) /* Only one thread running? */
        error(7);

    thread[thread_id].state = RUNNING;
    siglongjmp(thread[thread_id].thread_context, 2);
}

/* Function: thr_delay
 * Descr   : Delay thread for an interval in ms.
 * Input   : Thread ID.
 */
void thr_delay(unsigned long ticks) {
    thread[thread_id].state = DELAYED;
    thread[thread_id].delay_time = ticks;
    sleepers = 1;
    thr_yield();
}

/* Function: sema_pb
 * Descr   : Execute P operation on binary semaphore.
 * Input   : Semaphore pointer.
 */
void sema_pb(sema_t *sem) {
    while ( *sem == 0 ) {
        thread[thread_id].state = BLOCKED;
        thread[thread_id].ptr_to_sem = sem;
        thr_yield();
    }

    *sem = 0;
}

/* Function: sema_vb
 * Descr   : Execute V operation on binary semaphore.
 * Input   : Semaphore pointer.
 */
void sema_vb(sema_t *sem) {
    int i;

    *sem = 1;
    for ( i = 0; i < NTHREADS; i++ )
        if ( thread[i].ptr_to_sem == sem ) {
            thread[i].ptr_to_sem = NULL;
            thread[i].state      = READY;
            break;
        }

    thr_yield();
}

/* Function: sema_pc
 * Descr   : Execute P operation on common (counter) semaphore.
 * Input   : Semaphore pointer.
 */
void sema_pc(sema_t *sc) {
    while ( *sc == 0 ) {
        thread[thread_id].state      = BLOCKED;
        thread[thread_id].ptr_to_sem = sc;
        thr_yield();
    }

    (*sc)--;
}

/* Function: sema_vc
 * Descr   : Execute V operation on common (counter) semaphore.
 * Input   : Semaphore pointer.
 */
void sema_vc(sema_t *sc) {
    int i;

    (*sc)++;

    for ( i = 0; i < NTHREADS; i++ )
        if ( thread[i].ptr_to_sem == sc ) {
            thread[i].ptr_to_sem = NULL;
            thread[i].state      = READY;
            break;
        }
    thr_yield();
}

/* Function: parbegin
 * Descr   : Initialize data structures.
 */
void parbegin(void) {
    int i;

    /* Initialize the table of TCB */
    for ( i = 0; i < NTHREADS; i++ ) {
        thread[i].state        = TERMINATED;
        thread[i].thread_stack = NULL;
        thread[i].ptr_to_sem   = NULL;
        thread[i].delay_time   = 0;
    }

    /*** Flags initialization ***/
    start_fl  = 1;
    thread_id = 0;
    sleepers  = 0;
    next_id   = 0;
}

/* Function: parend
 * Descr   : Start multithreading mode.
 */
void parend() {
    if ( ready_threads() == 0 ) { /* no ready threads? */
        puts("Create a thread!\n");
        _exit(1);
    }

    /* Set the timer - SVR4 virtual */
    val1.it_interval.tv_sec  = 0;
    val1.it_interval.tv_usec = 1000; /* 1 ms interval */
    val1.it_value.tv_sec     = 0;
    val1.it_value.tv_usec    = 1000;

    if ( setitimer(ITIMER_VIRTUAL, &val1, NULL) == -1 ) {
        perror("Error: Setitimer failed ");
        _exit(1);
    }

    /* Catch signal SIGVTALRM (from the timer) */
    sigemptyset(&act1.sa_mask);
    act1.sa_handler = check_timeout;
    act1.sa_flags   = SA_RESTART;

    if ( sigaction(SIGVTALRM, &act1, 0) == -1) {
        perror("Error: Sigaction failed ");
        _exit(6);
    }

    /* Dispatcher chooses first ready thread */
    if ( 0 == sigsetjmp(start_context, 1))
        thr_yield();

    /* Stop timer after multithreading ends */
    val1.it_value.tv_sec  = 0;
    val1.it_value.tv_usec = 0;

    if ( setitimer(ITIMER_VIRTUAL, &val1, NULL)== -1 )
        perror("Error: Setitimer failed ");
}


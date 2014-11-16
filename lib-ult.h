#define _XOPEN_SOURCE 600

#ifndef _UT_H_
#define _UT_H_

/**
 * ut Public API
 *
 */

#include "ut_queue.h"
#include <ucontext.h>

void system_init(int max_number_of_klt);
int uthread_create(void (*func)());
void uthread_yield();
void uthread_exit();

/* ut thread functions */
void ut_exit();
void ut_spawn_trampoline(void);
void ut_yield();

/* tcb functions */
ut_tcb_t ut_tcb_alloc();
void ut_tcb_free(ut_tcb_t );

/* scheduler functions */
int ut_scheduler_init(void);
void ut_scheduler_kill(void);
void ut_scheduler(void );


/* helper functions */
void print_rq();
void print_nq();
void dump_tcb(ut_tcb_t t);


#endif

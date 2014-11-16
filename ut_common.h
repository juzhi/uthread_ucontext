#define _XOPEN_SOURCE 600
#ifndef _UT_COMMON_H_
#define _UT_COMMON_H_

/**
 * ut Private API
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <ucontext.h>
#include "list.h"

#define MAX_TIME_SEC 99999
#define MAX_TIME_USEC 99999
#define UT_TCB_NAMELEN 100
#define STKSIZE (64*1024)

#define ut_tcb_set_name(tcb_name, name) strcpy(tcb_name, name)

#define UT_DEBUG

#ifndef UT_DEBUG
#define ut_debug(msg)
#else
#define ut_debug(msg) ut_debug_msg(msg)
#endif

static inline void ut_debug_msg(char *msg){
    printf("%s\n",msg);
}


typedef struct list_head queue_st;
typedef queue_st *queue_t;   // queue head
typedef queue_st *queue_ele; // queue element

typedef struct timeval ut_time_t;

typedef enum ut_state_en {
    UT_STATE_SCHEDULER = 0,
    UT_STATE_NEW,
    UT_STATE_READY,
    UT_STATE_DEAD
}ut_state_t;


/* user thread control block structure */
typedef struct ut_tcb_st{
    ut_state_t  state;
    char        name[UT_TCB_NAMELEN];

    /* queues */
    queue_st new_q; // new queue
    queue_st ready_q; // ready queue
    queue_st dead_q; // dead queue

    /* timing */
    ut_time_t spawned;
    ut_time_t lastran;
    ut_time_t running;

    /* ucontext */
    ucontext_t  uctx;
    char*       stack;
    void        (*start_func)(void );
    void        *start_arg;
} *ut_tcb_t;

#define __gettimeofday(t) gettimeofday(t, NULL)
#define UT_TIME_NOW  (ut_time_t *)(0)
#define ut_time_set(t1,t2) \
    do { \
        if ((t2) == UT_TIME_NOW) \
        __gettimeofday((t1)); \
        else { \
            (t1)->tv_sec  = (t2)->tv_sec; \
            (t1)->tv_usec = (t2)->tv_usec; \
        } \
    } while (0)

#define ut_time_sub(t1,t2) \
    (t1)->tv_sec  -= (t2)->tv_sec; \
(t1)->tv_usec -= (t2)->tv_usec; \
if ((t1)->tv_usec < 0) { \
    (t1)->tv_sec  -= 1; \
    (t1)->tv_usec += 1000000; \
}

/* t1 = t1 + t2 */
#define ut_time_add(t1,t2) \
    (t1)->tv_sec  += (t2)->tv_sec; \
    (t1)->tv_usec += (t2)->tv_usec; \
    if ((t1)->tv_usec > 1000000) { \
        (t1)->tv_sec  += 1; \
        (t1)->tv_usec -= 1000000; \
    }

/* time value constructor */
static inline ut_time_t ut_time(long sec, long usec)
{
    ut_time_t tv;

    tv.tv_sec  = sec;
    tv.tv_usec = usec;
    return tv;
}

/* convert a time structure into a double value */
static inline double ut_time_t2d(ut_time_t *t)
{
    double d;

    d = ((double)t->tv_sec*1000000 + (double)t->tv_usec) / 1000000;
    return d;
}


#endif

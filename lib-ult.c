#include "lib-ult.h"

ut_tcb_t main_tcb;
ut_tcb_t sched_tcb;
ut_tcb_t current_tcb;

queue_st ut_NQ;
queue_st ut_RQ;
queue_st ut_DQ;

int max_thread_size = 0;

void create_main_thread(){
    main_tcb = ut_tcb_alloc();
    ut_tcb_set_name(main_tcb->name, "main_thread");
    main_tcb->running = ut_time(MAX_TIME_SEC,MAX_TIME_USEC);
    main_tcb->state = UT_STATE_NEW;

    getcontext(&main_tcb->uctx);
    main_tcb->uctx.uc_stack.ss_sp = NULL;
    main_tcb->uctx.uc_stack.ss_size = 0;
}

void create_sched_thread(){
    sched_tcb = ut_tcb_alloc();
    ut_tcb_set_name(sched_tcb->name, "scheduler");
    sched_tcb->start_func = ut_scheduler;
    sched_tcb->state =  UT_STATE_SCHEDULER;

    getcontext(&(sched_tcb->uctx));

    sched_tcb->uctx.uc_stack.ss_sp = sched_tcb->stack;
    sched_tcb->uctx.uc_stack.ss_size = STKSIZE;
    sched_tcb->uctx.uc_stack.ss_flags = 0;

    makecontext(&(sched_tcb->uctx), sched_tcb->start_func, 0);
}

void system_init(int max_number_of_klt){

    /*  set max thread limit */
    max_thread_size = max_number_of_klt;

    /* init scheduler */
    ut_scheduler_init();

    /* create main thread */
    create_main_thread();

    /* put main thread to New Queue */
    ut_nq_enqueue(main_tcb, &ut_NQ);

    /* create scheduler thread */
    create_sched_thread();

    /* set current thread to sheduler before swap */
    current_tcb = sched_tcb;

    /* swap to scheduler thread, then scheduler will take over all */
    swapcontext(&(main_tcb->uctx), &(sched_tcb->uctx));

}

int uthread_create(void (*func)()){
    ut_tcb_t tcb;

    /* we can not create new thread as to exceed the max thread limit */
    size_t nq_size = 0;
    size_t rq_size = 0;
    ut_rq_size(&rq_size,&ut_RQ);
    ut_nq_size(&nq_size,&ut_NQ);

    if (nq_size  > max_thread_size){
        return -1;
    }

    /* create tcb */
    if(( tcb = ut_tcb_alloc()) == NULL){
        //printf("alloc tcb error!\n");
        return -1;
    }

    ut_tcb_set_name(tcb->name, "normal");
    tcb->start_func = func;

    getcontext(&(tcb->uctx));

    tcb->uctx.uc_stack.ss_sp = tcb->stack;
    tcb->uctx.uc_stack.ss_size = STKSIZE;
    tcb->uctx.uc_stack.ss_flags = 0;

    makecontext(&(tcb->uctx), ut_spawn_trampoline, 0);


    /* put tcb into ut_NQ */
    ut_nq_enqueue(tcb, &ut_NQ);

    return 0;
}

void uthread_yield(){
    ut_yield();
}

void uthread_exit(){
    ut_exit();
}

void ut_spawn_trampoline(){
    (current_tcb->start_func)();
    ut_exit();
}

void ut_kill(){

    free(main_tcb);
    free(sched_tcb);

}

void ut_yield(){
//    printf("Entering ut_yield()\n");
    swapcontext(&current_tcb->uctx, &sched_tcb->uctx);
}

void ut_exit(){
//    ut_debug("Entering ut_exit()\n");

    if( current_tcb == main_tcb){
        if( ut_queue_empty(&ut_NQ) && ut_queue_empty(&ut_RQ)){

            /* there is no threads except for the main thread, and now we could exit the program */
            ut_kill();

            exit(0);
        }else{
            ut_yield();
        }
    }else {
        current_tcb->state = UT_STATE_DEAD;

        //printf("Thread running time: %f\n", ut_time_t2d(&current_tcb->running));

        //print_rq();
        swapcontext(&current_tcb->uctx, &sched_tcb->uctx);
    }

}

/* scheduler functions */
int ut_scheduler_init(void){
    ut_queue_init(ut_NQ);
    ut_queue_init(ut_RQ);
    ut_queue_init(ut_DQ);

    current_tcb = NULL;
    sched_tcb = NULL;

    return 1;
}

void ut_scheduler_kill(void){

}


void ut_scheduler(void){

    ut_time_t  snapshot;
    ut_time_t  running;

    while(1){

        /* move all new_queue elements to ready_queue */
        ut_move_nq_to_rq(&ut_NQ, &ut_RQ);

        ut_rq_dequeue(current_tcb, &ut_RQ);

        if(current_tcb == NULL){
            //printf("no more threads.\n");
            abort();
        }

        ut_time_set(&(current_tcb->lastran), UT_TIME_NOW);

        swapcontext(&sched_tcb->uctx, &(current_tcb->uctx));

        ut_time_set(&snapshot, UT_TIME_NOW);

        ut_time_sub(&snapshot, &current_tcb->lastran);

        ut_time_set(&running, &snapshot);

        ut_time_add(&current_tcb->running, &running);

        if(current_tcb->state != UT_STATE_DEAD){
            ut_rq_enqueue(current_tcb, &ut_RQ);
        }else{
            /* The thread has been finished normally, free the tcb */
            ut_tcb_free(current_tcb);
        }

    }
}

/* tcb functions */
ut_tcb_t ut_tcb_alloc(){
    ut_tcb_t t;

    // malloc tcb
    if((  t = (ut_tcb_t)malloc(sizeof(struct ut_tcb_st))) == NULL){
        return NULL;
    }

    // malloc stack
    if(( t->stack = (char *)malloc(STKSIZE)) == NULL){
        return NULL;
    }

    return t;
}

void ut_tcb_free(ut_tcb_t t){

    if ( NULL == t)
        return;

    if ( t->stack != NULL)
        free(t->stack);

    free(t);
    t = NULL;

}

/* helper functions */

void print_line(){
    printf("\n-------------------------\n");
}
void print_rq(){

    print_line();
    printf("Ready queue is: \n");
    ut_rq_display(&ut_RQ);
    print_line();
}

void print_nq(){
    print_line();
    printf("New queue is: \n");
    ut_nq_display(&ut_NQ);
    print_line();
}

void dump_tcb(ut_tcb_t t){
    print_line();
    printf("Dump TCB:\n");
    printf("Name: %s\n", t->name);
    printf("State: %d\n", t->state);
    print_line();

}


#include "ut_queue.h"
#include <assert.h>

void print_rq(queue_t ready_q){
    printf("Ready queue is: \n");
    ut_rq_display(ready_q);
}

void print_nq(queue_t new_q){
    printf("New queue is: \n");
    ut_nq_display(new_q);
}

void test_queue_size(){
    ut_queue_init0(new_q);

    ut_tcb_t t1 = (ut_tcb_t)malloc(sizeof(struct ut_tcb_st));
    strcpy(t1->name, "john");
    t1->running = ut_time(2,1);

    ut_tcb_t t2 = (ut_tcb_t)malloc(sizeof(struct ut_tcb_st));
    strcpy(t2->name, "bob");
    t2->running = ut_time(0,1);

    ut_tcb_t t3 = (ut_tcb_t)malloc(sizeof(struct ut_tcb_st));
    strcpy(t3->name, "hen");
    t3->running = ut_time(0,3);

    ut_tcb_t t4 = (ut_tcb_t)malloc(sizeof(struct ut_tcb_st));
    strcpy(t4->name, "hen2");
    t4->running = ut_time(0,5);

    ut_nq_enqueue(t1, &new_q);
    ut_nq_enqueue(t2, &new_q);
    ut_nq_enqueue(t3, &new_q);
    ut_nq_enqueue(t4, &new_q);

    print_nq(&new_q);

    int size = 0;
//    ut_queue_size(&size, &new_q, newq);
    ut_nq_size(&size, &new_q);

    assert(4 == size);


}

void test_move_nq_to_rq(){
    ut_queue_init0(new_q);
    ut_queue_init0(ready_q);

    ut_tcb_t t1 = (ut_tcb_t)malloc(sizeof(struct ut_tcb_st));
    strcpy(t1->name, "john");
    t1->running = ut_time(2,1);

    ut_tcb_t t2 = (ut_tcb_t)malloc(sizeof(struct ut_tcb_st));
    strcpy(t2->name, "bob");
    t2->running = ut_time(0,1);

    ut_tcb_t t3 = (ut_tcb_t)malloc(sizeof(struct ut_tcb_st));
    strcpy(t3->name, "hen");
    t3->running = ut_time(0,3);

    ut_tcb_t t4 = (ut_tcb_t)malloc(sizeof(struct ut_tcb_st));
    strcpy(t4->name, "hen2");
    t4->running = ut_time(0,5);

    // add t1 to new_q
    ut_nq_enqueue(t1, &new_q);
    print_nq(&new_q);

    // add t2 to ready_q
    ut_rq_enqueue(t2, &ready_q);

    ut_move_nq_to_rq(&new_q, &ready_q);

    print_rq(&ready_q);

    free(t1);
    free(t2);
    free(t3);
    free(t4);

}

void test_rq_ops(){
    ut_queue_init0(new_q);
    ut_queue_init0(ready_q);

    ut_tcb_t t1 = (ut_tcb_t)malloc(sizeof(struct ut_tcb_st));
    strcpy(t1->name, "john");
    t1->running = ut_time(2,1);

    ut_tcb_t t2 = (ut_tcb_t)malloc(sizeof(struct ut_tcb_st));
    strcpy(t2->name, "bob");
    t2->running = ut_time(0,1);

    ut_tcb_t t3 = (ut_tcb_t)malloc(sizeof(struct ut_tcb_st));
    strcpy(t3->name, "hen");
    t3->running = ut_time(0,3);

    ut_tcb_t t4 = (ut_tcb_t)malloc(sizeof(struct ut_tcb_st));
    strcpy(t4->name, "hen2");
    t4->running = ut_time(0,5);


    ut_rq_enqueue(t1,&ready_q);
    ut_rq_enqueue(t2,&ready_q);
    ut_rq_enqueue(t3,&ready_q);
    ut_rq_enqueue(t4,&ready_q);
    ut_rq_display(&ready_q);


    printf("\nAfter ready queue dequeue: \n");
    ut_tcb_t tmp;
    ut_rq_dequeue(tmp, &ready_q);
    ut_rq_display(&ready_q);

    free(t1);
    free(t2);
    free(t3);
    free(t4);

}

int main(int argc, char *argv[]){
    test_rq_ops();
    test_move_nq_to_rq();
    test_queue_size();

    return 0;
}

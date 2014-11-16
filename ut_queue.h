#ifndef _QUEUE_H
#define _QUEUE_H

#include "ut_common.h"
//extern ut_tcb_t main_tcb;

#define ut_queue_init0(head)  LIST_HEAD(head)
#define ut_queue_init(head)  do{\
    INIT_LIST_HEAD(&head);\
}while(0)

/* new_queue ops START */
#define ut_nq_enqueue(new_tcb,head) list_add_tail(&((new_tcb)->new_q), head)
/* dequeue from tail */
#define ut_nq_dequeue(tcb, head)  do{\
    if(list_empty(head))\
    tcb = NULL;\
    else{\
        tcb = list_entry((head)->prev, struct ut_tcb_st, new_q);\
        list_del((head)->prev);\
    }\
}while(0)
/* new_queue ops END */

/* ready queue ops START */
/* dequeue from head */
#define ut_rq_dequeue(tcb, head) do{\
    if(list_empty(head))\
    tcb = NULL;\
    else{\
        tcb = list_entry((head)->next, struct ut_tcb_st, ready_q);\
        list_del(&(tcb->ready_q));\
    }\
}while(0)

#define ut_rq_enqueue(new_tcb, head) do{\
    queue_ele pos;\
    ut_tcb_t tcb;\
    int done = 0;\
    if(list_empty(head)){\
        list_add_tail(&(new_tcb->ready_q),head);\
        done = 1;\
    }else{\
        list_for_each_prev(pos, (head)){\
            tcb = list_entry(pos, struct ut_tcb_st, ready_q);\
            if(ut_time_t2d(&(new_tcb)->running) >= ut_time_t2d(&tcb->running)){\
                __list_add(&((new_tcb)->ready_q), pos, pos->next);\
                done = 1;\
                break;\
            }\
        }\
    }\
    if(!done){\
        list_add(&((new_tcb)->ready_q),head);\
    }\
}while(0)
/* ready queue ops END */

/* move all NewQueue elements to ReadyQueue */
#define ut_move_nq_to_rq(new_q_head, ready_q_head) do{\
    ut_tcb_t tcb;\
    while(!list_empty(new_q_head)){\
        ut_nq_dequeue(tcb, new_q_head);\
        list_add(&(tcb->ready_q), ready_q_head);\
    }\
}while(0)

/* display queue ops */
#define ut_nq_display(head) ut_queue_display(head, new_q)
#define ut_rq_display(head) ut_queue_display(head, ready_q)


#define ut_queue_display(head, member) do{\
    if(list_empty(head)){\
        printf("empty queue.\n");\
    }else{\
        ut_tcb_t tcb;\
        list_for_each_entry(tcb, head, member){\
            printf("running: %f    name: %s \n",ut_time_t2d(&tcb->running),tcb->name);\
        }\
    }\
}while(0)

#define ut_queue_empty(head) list_empty(head)

#define ut_nq_size(size,head) ut_queue_size(size,head,new_q)
#define ut_rq_size(size,head) ut_queue_size(size,head,ready_q)

#define ut_queue_size(size,head, member) do{\
    if(list_empty(head)){\
        *size = 0;\
    }else{\
        *size = 0;\
        queue_ele pos;\
        list_for_each(pos, head)\
            (*size)++;\
    }\
}while(0)

#endif

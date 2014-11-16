#include "ut.h"

void test_create_tcb(){

    ut_tcb_t tcb = ut_tcb_alloc();
    strcpy(tcb->name, "main thread");


    dump_tcb(tcb);
    ut_tcb_free(tcb);

}

int main(int argc, char *argv[]){
    test_create_tcb();
    return 0;
}

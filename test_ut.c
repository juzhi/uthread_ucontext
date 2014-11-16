#include "ut.h"

void bar(){

    for(int i = 0; i< 1000; i++){

        for(int j = 0; j < 1000000; ++j)
            sqrt(j*1.94);
    }

    printf("this is bar.\n");
}
int main(int argc, char* argv[]){

    system_init(10);

    uthread_create(bar);
    uthread_create(bar);
    uthread_create(bar);
    uthread_create(bar);
    uthread_create(bar);

    printf("enter main thread.\n");

    uthread_exit();

    return 0;
}

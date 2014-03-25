#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>

#define STACK_2M			2097152
#define PTHREAD_STACK_MIN	16384
void *thread_func(void *arg){
	sleep(1);
	return 0;
}

static int do_test()
{
    /* Warning: error checking removed to keep the example small */
    pthread_attr_t attr, cur_attr;
    pthread_t th;
    int ret;
    size_t stack_size;
    void *stack_addr;
    struct rlimit rlim;

    /*	Test 1 */
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, PTHREAD_STACK_MIN - 1);/* Set small value to see if VD eglibc overrides it */
   pthread_attr_getstacksize(&attr, &stack_size);
   if(stack_size != STACK_2M)
	return 1;
   pthread_attr_destroy(&attr);
   return 0;
}


#define TEST_FUNCTION do_test ()
#include "../test-skeleton.c"

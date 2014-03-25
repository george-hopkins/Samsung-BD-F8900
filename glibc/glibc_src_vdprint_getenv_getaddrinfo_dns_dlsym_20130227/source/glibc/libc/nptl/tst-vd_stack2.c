#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>

#define STACK_8M			8388608
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
	pthread_attr_init(&attr);
	pthread_attr_setstacksize(&attr, STACK_8M);
	pthread_create(&th, &attr, thread_func, NULL);
	ret = pthread_getattr_np(th, &cur_attr);
	if(ret){
		perror("pthread_getattr_np");
		return 1;
	}
	ret = pthread_attr_getstack(&cur_attr, &stack_addr, &stack_size);
	if(stack_size != STACK_2M)
		return 1;
	pthread_attr_destroy(&cur_attr);
	pthread_attr_destroy(&attr);
    return 0;
}


#define TEST_FUNCTION do_test ()
#include "../test-skeleton.c"

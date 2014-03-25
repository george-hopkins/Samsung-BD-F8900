#include <stdint.h>
# define TIMEVAL_TO_TIMESPEC(tv, ts) {                                   \
	        (ts)->tv_sec = (tv)->tv_sec;                                    \
	        (ts)->tv_nsec = (tv)->tv_usec * 1000;                           \
}
# define TIMESPEC_TO_TIMEVAL(tv, ts) {                                   \
	        (tv)->tv_sec = (ts)->tv_sec;                                    \
	        (tv)->tv_usec = (ts)->tv_nsec / 1000;                           \
}
# define TEMP_FAILURE_RETRY(expression) \
	  (__extension__							      \
	       ({ long int __result;						      \
			       do __result = (long int) (expression);				      \
			       while (__result == -1L && errno == EINTR);			      \
			       __result; }))

#define PTHREAD_RWLOCK_WRITER_NONRECURSIVE_INITIALIZER_NP \
	  { { 0, 0, 0, 0, 0, 0, PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP, \
			      0, 0, 0, 0 } }
#define PTHREAD_MUTEX_NORMAL 0

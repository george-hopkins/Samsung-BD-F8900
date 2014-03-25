#include <errno.h>
#include <sysdep.h>
#include <lowlevellock.h>
#include <internaltypes.h>
#include <semaphore.h>

#include <pthreadP.h>
#include <shlib-compat.h>


extern void __sem_wait_cleanup (void *arg) attribute_hidden;


int
sem_timedwait_relative (sem_t *sem, const struct timespec *reltime)
{
  struct new_sem *isem = (struct new_sem *) sem;
  int err;

  if (atomic_decrement_if_positive (&isem->value) > 0)
    return 0;

  if (reltime->tv_nsec < 0 || reltime->tv_nsec >= 1000000000)
    {
      __set_errno (EINVAL);
      return -1;
    }

  atomic_increment (&isem->nwaiters);

  pthread_cleanup_push (__sem_wait_cleanup, isem);

  while (1)
    {
      struct timeval tv;
      struct timespec rt;
      int sec, nsec;

      /* Compute relative timeout.  */
      sec = reltime->tv_sec;
      nsec = reltime->tv_nsec;
      if (nsec < 0)
	{
	  nsec += 1000000000;
	  --sec;
	}

      /* Already timed out?  */
      err = -ETIMEDOUT;
      if (sec < 0)
	{
	  __set_errno (ETIMEDOUT);
	  err = -1;
	  break;
	}

      /* Do wait.  */
      rt.tv_sec = sec;
      rt.tv_nsec = nsec;

      /* Enable asynchronous cancellation.  Required by the standard.  */
      int oldtype = __pthread_enable_asynccancel ();

      err = lll_futex_timed_wait (&isem->value, 0, &rt,
				  isem->private ^ FUTEX_PRIVATE_FLAG);

      /* Disable asynchronous cancellation.  */
      __pthread_disable_asynccancel (oldtype);

      if (err != 0 && err != -EWOULDBLOCK)
	{
	  __set_errno (-err);
	  err = -1;
	  break;
	}

      if (atomic_decrement_if_positive (&isem->value) > 0)
	{
	  err = 0;
	  break;
	}
    }

  pthread_cleanup_pop (0);

  atomic_decrement (&isem->nwaiters);

  return err;
}

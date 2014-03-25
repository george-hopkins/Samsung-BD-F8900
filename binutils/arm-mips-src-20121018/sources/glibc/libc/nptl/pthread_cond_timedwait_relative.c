#include <endian.h>
#include <errno.h>
#include <sysdep.h>
#include <lowlevellock.h>
#include <pthread.h>
#include <pthreadP.h>
#include <kernel-features.h>

#include <shlib-compat.h>


/* Cleanup handler, defined in pthread_cond_wait.c.  */
extern void __condvar_cleanup (void *arg)
     __attribute__ ((visibility ("hidden")));

struct _condvar_cleanup_buffer
{
  int oldtype;
  pthread_cond_t *cond;
  pthread_mutex_t *mutex;
  unsigned int bc_seq;
};

int
__pthread_cond_timedwait_relative (cond, mutex, reltime)
     pthread_cond_t *cond;
     pthread_mutex_t *mutex;
     const struct timespec *reltime;
{
  struct _pthread_cleanup_buffer buffer;
  struct _condvar_cleanup_buffer cbuffer;
  int result = 0;

  /* Catch invalid parameters.  */
  if (reltime->tv_nsec < 0 || reltime->tv_nsec >= 1000000000)
    return EINVAL;

  int pshared = (cond->__data.__mutex == (void *) ~0l)
		? LLL_SHARED : LLL_PRIVATE;

  /* Make sure we are along.  */
  lll_lock (cond->__data.__lock, pshared);

  /* Now we can release the mutex.  */
  int err = __pthread_mutex_unlock_usercnt (mutex, 0);
  if (err)
    {
      lll_unlock (cond->__data.__lock, pshared);
      return err;
    }

  /* We have one new user of the condvar.  */
  ++cond->__data.__total_seq;
  ++cond->__data.__futex;
  cond->__data.__nwaiters += 1 << COND_NWAITERS_SHIFT;

  /* Remember the mutex we are using here.  If there is already a
     different address store this is a bad user bug.  Do not store
     anything for pshared condvars.  */
  if (cond->__data.__mutex != (void *) ~0l)
    cond->__data.__mutex = mutex;

  /* Prepare structure passed to cancellation handler.  */
  cbuffer.cond = cond;
  cbuffer.mutex = mutex;

  /* Before we block we enable cancellation.  Therefore we have to
     install a cancellation handler.  */
  __pthread_cleanup_push (&buffer, __condvar_cleanup, &cbuffer);

  /* The current values of the wakeup counter.  The "woken" counter
     must exceed this value.  */
  unsigned long long int val;
  unsigned long long int seq;
  val = seq = cond->__data.__wakeup_seq;
  /* Remember the broadcast counter.  */
  cbuffer.bc_seq = cond->__data.__broadcast_seq;

  while (1)
    {
      struct timespec rt;
      {
	/* Modified by Young-Ho, 2010-11-09  */
	rt.tv_sec = reltime->tv_sec;
	rt.tv_nsec = reltime->tv_nsec;
      }
      if (rt.tv_nsec < 0)
	{
	  rt.tv_nsec += 1000000000;
	  --rt.tv_sec;
	}
      /* Did we already time out?  */
      if (__builtin_expect (rt.tv_sec < 0, 0))
	{
	  if (cbuffer.bc_seq != cond->__data.__broadcast_seq)
	    goto bc_out;

	  goto timeout;
	}

      unsigned int futex_val = cond->__data.__futex;

      /* Prepare to wait.  Release the condvar futex.  */
      lll_unlock (cond->__data.__lock, pshared);

      /* Enable asynchronous cancellation.  Required by the standard.  */
      cbuffer.oldtype = __pthread_enable_asynccancel ();

      /* Wait until woken by signal or broadcast.  */
      err = lll_futex_timed_wait (&cond->__data.__futex,
				  futex_val, &rt, pshared);

      /* Disable asynchronous cancellation.  */
      __pthread_disable_asynccancel (cbuffer.oldtype);

      /* We are going to look at shared data again, so get the lock.  */
      lll_lock (cond->__data.__lock, pshared);

      /* If a broadcast happened, we are done.  */
      if (cbuffer.bc_seq != cond->__data.__broadcast_seq)
	goto bc_out;

      /* Check whether we are eligible for wakeup.  */
      val = cond->__data.__wakeup_seq;
      if (val != seq && cond->__data.__woken_seq != val)
	break;

      /* Not woken yet.  Maybe the time expired?  */
      if (__builtin_expect (err == -ETIMEDOUT, 0))
	{
	timeout:
	  /* Yep.  Adjust the counters.  */
	  ++cond->__data.__wakeup_seq;
	  ++cond->__data.__futex;

	  /* The error value.  */
	  result = ETIMEDOUT;
	  break;
	}
    }

  /* Another thread woken up.  */
  ++cond->__data.__woken_seq;

 bc_out:

  cond->__data.__nwaiters -= 1 << COND_NWAITERS_SHIFT;

  /* If pthread_cond_destroy was called on this variable already,
     notify the pthread_cond_destroy caller all waiters have left
     and it can be successfully destroyed.  */
  if (cond->__data.__total_seq == -1ULL
      && cond->__data.__nwaiters < (1 << COND_NWAITERS_SHIFT))
    lll_futex_wake (&cond->__data.__nwaiters, 1, pshared);

  /* We are done with the condvar.  */
  lll_unlock (cond->__data.__lock, pshared);

  /* The cancellation handling is back to normal, remove the handler.  */
  __pthread_cleanup_pop (&buffer, 0);

  /* Get the mutex before returning.  */
  err = __pthread_mutex_cond_lock (mutex);

  return err ?: result;
}

versioned_symbol (libpthread, __pthread_cond_timedwait_relative, pthread_cond_timedwait_relative,
		  GLIBC_2_3_2);

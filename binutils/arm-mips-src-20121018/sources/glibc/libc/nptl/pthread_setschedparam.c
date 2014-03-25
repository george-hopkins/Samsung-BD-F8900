/* Copyright (C) 2002, 2003, 2004, 2006, 2007 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@redhat.com>, 2002.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#include <errno.h>    /* perror() */
#include <sched.h>    /* SCHED_FIFO, SCHED_RR, SCHED_NORMAL, SCHED_BATCH */
#include <string.h>   /* memcpy() */
#include <stdlib.h>   /* exit()   */
#include <stdio.h>    /* stderr */
#include <sys/types.h>
#include <unistd.h>   /* getpid(), gettid() */
#include <sys/time.h> /* setpriority(), getpriority() */
#include <sys/resource.h>
#include "pthreadP.h"
#include <lowlevellock.h>

/* Using syscall(ARM/i386=224,MIPS=4,222,PPC=207) to get LWP(thread id in the kernel)
 * inline function is used for faster performance.
 */
static inline pid_t GetUniqueKernelTID(void){
  long int lwp=syscall(224);
  if(lwp < 0) lwp=getpid();
  return lwp;
}

int
__pthread_setschedparam (threadid, policy, param)
     pthread_t threadid;
     int policy;
     const struct sched_param *param;
{
   /* Default is normal value.*/
   int result = 0;
   struct pthread *pd = (struct pthread *) threadid;

   /* To support priority, if used SCHED_FIFO or SCHED_RR, display caution message
    * ( @/usr/include/linux/sched.h )
    * SCHED_OTHER (in 2.4.X) or SCHED_NORMAL(in 2.6.x)  0 (Non-preemptive priority-based scheduling)
    * SCHED_FIFO  (Both 2.4.x and 2.6.x)                1
    * SCHED_RR    (Both 2.4.x and 2.6.x)                2
    * SCHED_BATCH (From 2.6.16 )                        3
    * If you want to effect time sharing between threads about realtime tasks, We recommend SCHED_RR.
    * If you want to adjust priority(CPU Load) between threads about normal tasks, We recommend SCHED_OTHER.
    */
   if (policy == SCHED_OTHER) {

       /* The  scheduling  priority of the process. */
       int which = PRIO_PROCESS ;

       /* Confirm Range of priority that user input */
       if ( param->sched_priority < -20 && param->sched_priority > 19 ) {
           fprintf (stderr, "SELP:ERROR! Range of priority that you can input is between -20 and 19 \r\n");
           return errno;
       }
       /* Getting LWP(thread id in the kernel) to change priority about assigned thread id. */
       // if (setpriority(which, GetUniqueKernelTID(), param->sched_priority) ) {
       if (setpriority(which, pd->tid, param->sched_priority) ) {
           fprintf (stderr, "SELP:setpriority() is ERROR. It seems that System not support setpriority()\n");
           result = errno;
       }
       return result ;
   }

if (policy == SCHED_FIFO || policy == SCHED_RR) {


  /* Make sure the descriptor is valid.  */
  if (INVALID_TD_P (pd))
    /* Not a valid thread handle.  */
    return ESRCH;

//  int result = 0;

  lll_lock (pd->lock, LLL_PRIVATE);

  struct sched_param p;
  const struct sched_param *orig_param = param;

  /* If the thread should have higher priority because of some
     PTHREAD_PRIO_PROTECT mutexes it holds, adjust the priority.  */
  if (__builtin_expect (pd->tpp != NULL, 0)
      && pd->tpp->priomax > param->sched_priority)
    {
      p = *param;
      p.sched_priority = pd->tpp->priomax;
      param = &p;
    }

  /* Try to set the scheduler information.  */
  if (__builtin_expect (__sched_setscheduler (pd->tid, policy,
					      param) == -1, 0))
    result = errno;
  else
    {
      /* We succeeded changing the kernel information.  Reflect this
	 change in the thread descriptor.  */
      pd->schedpolicy = policy;
      memcpy (&pd->schedparam, orig_param, sizeof (struct sched_param));
      pd->flags |= ATTR_FLAG_SCHED_SET | ATTR_FLAG_POLICY_SET;
    }

  lll_unlock (pd->lock, LLL_PRIVATE);

  return result;
 }
}
strong_alias (__pthread_setschedparam, pthread_setschedparam)

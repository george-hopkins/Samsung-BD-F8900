/****************************************************************************
 *   FileName    : TC_Kernel.h
 *   Description :
 ****************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips, Inc.
 *   ALL RIGHTS RESERVED
 *
 ****************************************************************************/
#ifndef _TC_KERNEL_H_
#define _TC_KERNEL_H_

//#include "main.h"
//#include "telechips.h"


/* Global routines */

/**** Define's ***************************************************************
*****************************************************************************/

#include "nucleus.h"

/**** Structure's ************************************************************
*****************************************************************************/
#define TC_MEMORY_POOL	NU_MEMORY_POOL
#define TC_TASK 		NU_TASK
#define TC_SEMAPHORE	NU_SEMAPHORE
#define TC_QUEUE		NU_QUEUE
#define TC_EVENT_FLAG	NU_EVENT_GROUP
#define TC_HISR 		NU_HISR
#define TC_TIMER		NU_TIMER

#define TC_MAX_NAME 	NU_MAX_NAME

#define TC_NO_START 	NU_NO_START
#define TC_START 		NU_START

#define TC_SUSPEND		NU_SUSPEND
#define TC_NO_SUSPEND	NU_NO_SUSPEND
#define TC_SUCCESS		NU_SUCCESS
#define TC_FAIL			-1

enum
{
	TC_PTR_INVALID	= 0,	// invalid pointer
	TC_PTR_NC,	// non-cacheable area
	TC_PTR_C	// cacheable area
};

/**** External variable *****************************************************
*****************************************************************************/

extern TC_MEMORY_POOL System_Memory;
#ifdef MEMORY_SEPERATE_IVR
extern TC_MEMORY_POOL IVR_Memory;
#endif
extern TC_MEMORY_POOL NonCache_Memory;

/**** External routines *****************************************************
*****************************************************************************/

#if 0

#define TC_Delete_Queue(a)							_TC_Delete_Queue(&(a))
#define TC_Reset_Queue(a)							_TC_Reset_Queue(&(a))
#define TC_Send_To_Front_Of_Queue(a, b) 			_TC_Send_To_Front_Of_Queue(&(a), (b))
#define TC_Send_To_Queue(a, b)						_TC_Send_To_Queue(&(a), (b))
#define TC_Broadcast_To_Queue(a, b)					_TC_Broadcast_To_Queue(&(a), (b))
#define TC_Receive_From_Queue(a, b, c)				_TC_Receive_From_Queue(&(a), (b), (c))

#define TC_Delete_Semaphore(a)						_TC_Delete_Semaphore(&(a))
#define TC_Reset_Semaphore(a,b)						_TC_Reset_Semaphore(&(a), (b))
#define TC_Obtain_Semaphore(a,b)					_TC_Obtain_Semaphore(&(a),(b))
#define TC_Release_Semaphore(a)						_TC_Release_Semaphore(&(a))
#define TC_Semaphore_Information(a, b, c, d, e, f)	_TC_Semaphore_Information(&(a), (b), (c), (d), (e), (f))


#define TC_Delete_Event_Group(a)					_TC_Delete_Event_Group(&(a))
#define TC_Set_Events(a,b)							_TC_Set_Events(&(a), (b))
#define TC_Retrieve_Events(a,b,c)					_TC_Retrieve_Events(&(a),(b),(c))
#define TC_Event_Group_Information(a, b, c, d, e)	_TC_Event_Group_Information(&(a), (b), (c), (d), (e))

#endif

/*****************************************************************************/
/* Define task control functions.  */
/*****************************************************************************/

extern	int TC_Create_Task(void *task, char *name,
                          void (*task_entry)(UNSIGNED argc, VOID *argv)  , void *stack_address,
                          unsigned int stack_size, unsigned char priority,
                          unsigned int time_slice, unsigned char auto_start);

extern int TC_Delete_Task(void *task);
extern int TC_Reset_Task(void *task);
extern int TC_Resume_Task(void *task);
extern int TC_Suspend_Task(void *task);
extern void TC_TimeDly(unsigned int ticks);
extern int TC_TimeDlyResume(void *task);
extern unsigned int  TC_Check_Stack(void *task);
extern int TC_Task_Information(void *task, char *name,
                               unsigned char *status, unsigned int *scheduled_count,
                               unsigned char *priority, unsigned char *preempt,
                               unsigned int *time_slice, void **stack_base,
                               unsigned int *stack_size, unsigned int *minimum_stack);
//extern void	TC_SchedLock(unsigned int Num);
extern void	TC_SchedLock(void);
//extern void TC_SchedUnlock(unsigned int Num);
extern void TC_SchedUnlock(void);


/*****************************************************************************/
/* Define Queue management functions.  */
/*****************************************************************************/
extern int	TC_Create_Queue(TC_QUEUE *queue, char *name,
                           void *start_address, unsigned int queue_size);
extern int	TC_Create_QueueEx(TC_QUEUE *queue, char *name,
                    void *start_address, unsigned int queue_size, unsigned int element_size);

#if 1
extern int TC_Delete_Queue(TC_QUEUE *queue);
extern int TC_Reset_Queue(TC_QUEUE *queue);
extern int TC_Send_To_Front_Of_Queue(TC_QUEUE *queue, void *message );
extern int TC_Send_To_Queue(TC_QUEUE *queue, void *message);
extern int TC_Send_To_QueueEx(TC_QUEUE *queue, void *message, unsigned int size);
extern int TC_Broadcast_To_Queue(TC_QUEUE *queue, void *message);
extern int TC_Receive_From_Queue(TC_QUEUE *queue, void *message,  unsigned int suspend);
extern int TC_Receive_From_QueueEx(TC_QUEUE *queue, void *message,  unsigned int size, unsigned int suspend);
#endif

/*****************************************************************************/
/* Define Semaphore management functions.  */
/*****************************************************************************/
extern int TC_Create_Semaphore(TC_SEMAPHORE *semaphore, char *name, unsigned int initial_count);
#if 1
extern int TC_Delete_Semaphore(TC_SEMAPHORE *semaphore);
extern int TC_Reset_Semaphore(TC_SEMAPHORE *semaphore, unsigned int initial_count);
extern int TC_Obtain_Semaphore(TC_SEMAPHORE *semaphore, unsigned int suspend);
extern int TC_Release_Semaphore(TC_SEMAPHORE *semaphore);
extern int TC_Semaphore_Information(TC_SEMAPHORE *semaphore, char *name,
                                    unsigned int *current_count, unsigned char *suspend_type,
                                    unsigned int *tasks_waiting, void **first_task);
#endif

/*****************************************************************************/
/* Define Event Group management functions.  */
/*****************************************************************************/
extern int TC_Create_Event_Group(TC_EVENT_FLAG *group, char *name);
#if 1
extern int TC_Delete_Event_Group(TC_EVENT_FLAG *group);
extern int TC_Set_Events(TC_EVENT_FLAG *group, unsigned int events);
extern int TC_Retrieve_Events(TC_EVENT_FLAG *group, unsigned int requested_flags,  unsigned int suspend);
extern int TC_Event_Group_Information(TC_EVENT_FLAG *group, char *name, unsigned int *event_flags,
                                      unsigned int *tasks_waiting, void **first_task);
#endif

/*****************************************************************************/
/* Define Dynamic memory management functions.  */
/*****************************************************************************/
extern int TC_Create_Memory(char *name, void *start_address, unsigned int pool_size);
#ifdef MEMORY_SEPERATE_IVR
extern int IVR_Create_Memory(char *name, void *start_address, unsigned int pool_size);
#endif
extern int TC_Delete_Memory_Pool(void);
extern void *TC_Allocate_Memory(unsigned int size);
#ifdef MEMORY_SEPERATE_IVR
extern void *IVR_Allocate_Memory(unsigned int size);
#endif
extern void *TC_Allocate_Aligned_Memory(unsigned int size, unsigned int alignment);
extern int TC_Deallocate_Memory(void *memory);
extern int TC_Create_NC_Memory(char *name, void *start_address, unsigned int pool_size);
extern int TC_Delete_NC_Memory_Pool(void);
extern void *TC_Allocate_NC_Memory(unsigned int size);
extern void *TC_Allocate_NC_Aligned_Memory(unsigned int size, unsigned int alignment);
extern int TC_Deallocate_NC_Memory(void *memory);
extern void *TC_ReAllocate_Memory(void *ptr, unsigned int size);
#ifdef MEMORY_SEPERATE_IVR
extern void *IVR_ReAllocate_Memory(void *ptr, unsigned int size);
#endif
extern int TC_Check_CACHE(const void *memory);
extern int TC_Create_Memory_Sub(char *name, void *start_address, unsigned int pool_size);
extern int TC_Delete_Memory_Pool_Sub(void);
extern void *TC_Allocate_Memory_Sub(unsigned int size);
extern void *TC_Allocate_Aligned_Memory_Sub(unsigned int size, unsigned int alignment);
extern int TC_Deallocate_Memory_Sub(void *memory);
extern int TC_Create_NC_Memory_Sub(char *name, void *start_address, unsigned int pool_size);
extern int TC_Delete_NC_Memory_Pool_Sub(void);
extern void *TC_Allocate_NC_Memory_Sub(unsigned int size);
extern int TC_Deallocate_NC_Memory_Sub(void *memory);
#if 0
extern int	TC_Check_Pointer(unsigned uNum, char *pSrc, unsigned uLine, ...);
#endif
extern void *malloc(int size);
extern void free(void *memory);

extern void *calloc(int num, int size);
extern void *realloc(void *ptr, int size);
extern char *strdup(const char *str);

/*****************************************************************************/
/* Define Interrupt management functions.  */
/*****************************************************************************/
#if 0
int	NU_Local_Control_Interrupts(INT new_level);
int	NU_Register_LISR(INT vector,  VOID (*new_lisr)(INT), VOID (**old_lisr)(INT));
#endif
extern int	TC_Activate_HISR(TC_HISR *hisr);
extern int	TC_Create_HISR(TC_HISR *hisr, char *name,  void (*hisr_entry)(void), unsigned char priority,
                          void *stack_address, unsigned int stack_size);
extern int	TC_Delete_HISR(TC_HISR *hisr);
/*****************************************************************************/
/* Timer management functions.  */
/*****************************************************************************/
extern int	TC_Create_Timer(TC_TIMER *timer, char *name,
                           void (*expiration_routine)(unsigned long), unsigned long id,
                           unsigned long initial_time, unsigned long reschedule_time,
                           unsigned char enable);
extern int	TC_Delete_Timer(TC_TIMER *timer);
extern int	TC_Control_Timer(TC_TIMER *timer, unsigned char enable);
extern unsigned int TC_Retrieve_Clock(void);

#endif	// _TC_KERNEL_H_

/* end of file */


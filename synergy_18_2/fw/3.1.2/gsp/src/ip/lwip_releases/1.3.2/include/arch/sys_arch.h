#ifndef SYS_ARCH_H__
#define SYS_ARCH_H__
/*sys_arch.h - Tied to sys_arch.c*/
/*
  Arch dependent types for the following objects:
    sys_sem_t, sys_mbox_t, sys_thread_t,
*/
/*  And, optionally:
    sys_prot_t
*/

#ifdef __cplusplus
extern "C" {
#endif

/*  Defines to set vars of sys_mbox_t and sys_sem_t to NULL.*/
#define SYS_MBOX_NULL NULL
#define SYS_SEM_NULL NULL

#ifdef __cplusplus
}
#endif

#endif

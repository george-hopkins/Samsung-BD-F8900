#ifndef ENV_R_H
#define ENV_R_H


extern char **environ;

#if _LIBC
# include <bits/libc-lock.h>
extern __libc_lock_t envlock;
# define LOCK	__libc_lock_lock (envlock)
# define UNLOCK	__libc_lock_unlock (envlock)
#endif

#ifdef _LIBC
# define setenv_r __setenv_r
# define putenv_r __putenv_r
# define getenv_r __getenv_r
# define unsetenv_r __unsetenv_r
#endif

#endif

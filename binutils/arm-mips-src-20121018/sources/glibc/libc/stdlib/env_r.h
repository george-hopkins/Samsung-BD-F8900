#ifndef ENV_R_H
#define ENV_R_H


extern char **environ;

#if _LIBC

# include <bits/libc-lock.h>
__libc_rwlock_define_initialized (static, _env_lock)
# define WRLOCK  __libc_rwlock_wrlock(_env_lock);
# define RDLOCK __libc_rwlock_rdlock(_env_lock);
# define UNLOCK __libc_rwlock_unlock(_env_lock);

#else

# define LOCK
# define UNLOCK

#endif

#ifdef _LIBC
# define setenv_r __setenv_r
# define putenv_r __putenv_r
# define getenv_r __getenv_r
# define unsetenv_r __unsetenv_r
#endif

#endif

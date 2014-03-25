#include <ftw.h>
#include <bits/libc-lock.h>

__libc_lock_define_initialized (static, ftw_lock)
__libc_lock_define_initialized (static, nftw_lock)

int ftw_r (const char *path, __ftw_func_t func, int descriptors)
{
  int ret;
  __libc_lock_lock (ftw_lock);
  ret = ftw (path, func, descriptors);
  __libc_lock_unlock (ftw_lock);
  return ret;
}

int nftw_r (const char *path, __nftw_func_t func, int descriptors, int flags)
{
  int ret;
  __libc_lock_lock (nftw_lock);
  ret = nftw (path, func, descriptors, flags);
  __libc_lock_unlock (nftw_lock);
  return ret;
}


/* Reentrant version of putenv function */

#include <endian.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "env_r.h"


int __setenv_r (const char *name, const char *value, int replace);

/* Reentrant version of putenv function */
int
putenv_r (const char *string)
{

  register char **ep;

  const char *const name_end = strchr (string, '=');

  if (name_end != NULL)
    {
      char *name;
#ifdef _LIBC
      int use_malloc = !__libc_use_alloca (name_end - string + 1);
      if (__builtin_expect (use_malloc, 0))
        {
          name = strndup (string, name_end - string);
          if (name == NULL)
            return -1;
        }
      else
        name = strndupa (string, name_end - string);
#else
# define use_malloc 1
      name = malloc (name_end - string + 1);
      if (name == NULL)
        return -1;
      memcpy (name, string, name_end - string);
      name[name_end - string] = '\0';
#endif
      return setenv_r(name, string + strlen(name) + 1, 1);
    }

/* unsetenv start */
  WRLOCK;
  size_t len = strlen (string);

  ep = __environ;
  if (ep != NULL)
    while (*ep != NULL)
      if (!strncmp (*ep, string, len) && (*ep)[len] == '=')
        {
          /* Found it.  Remove this pointer by moving later ones back.  */
          char **dp = ep;
          do
            dp[0] = dp[1];
          while (*dp++);
          /* Continue the loop in case NAME appears again.  */
        }
      else
        ++ep;
  UNLOCK;

  return 0;
}
# undef putenv_r
weak_alias (__putenv_r, putenv_r)

/* Reentrant version of getenv function */

#include <endian.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "env_r.h"

/* Reentrant version of getenv function */

int
getenv_r (const char *name, char *buf, int buflen)
{
  int i, len, olen;

  LOCK;

  if (__environ == NULL || name[0] == '\0')
    goto error;

  len = strlen (name);
  for (i = 0; environ[i] != NULL; i++)
    {
      if ((strncmp(name, environ[i], len) == 0) && (environ[i][len] == '='))
        {
          olen = strlen (&environ[i][len+1]);
          if (olen >= buflen)
            {
              UNLOCK;
              __set_errno (ENOSPC);
              return -1;
            }
          strcpy (buf, &environ[i][len+1]);
          UNLOCK;
          return 0;
        }
      }

error:
  UNLOCK;
  __set_errno (ENOENT);
  return -1;
}
# undef getenv_r
weak_alias (__getenv_r, getenv_r)

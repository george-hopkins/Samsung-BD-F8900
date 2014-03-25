#include <endian.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "env_r.h"

static char **last_environ;

/* Reentrant version of setenv function */

int
setenv_r (const char *name, const char *value, int replace)
{
  register char **ep = 0;
  register size_t size;
  const size_t namelen = strlen (name);
  const size_t vallen = strlen (value) + 1;

  LOCK;

  size = 0;
  if (environ != NULL)
    {
      for (ep = environ; *ep != NULL; ++ep)
        {
          if (!strncmp (*ep, name, namelen) && (*ep)[namelen] == '=')
            {
              break;
            }
          else
          {
            ++size;
          }
        }
      }

  if (environ == NULL || *ep == NULL)
    {
    char **new_environ;
    if (environ == last_environ && environ != NULL)
      {
      /* We allocated this space; we can extend it.  */
        new_environ = (char **) realloc (last_environ, (size + 2) * sizeof (char *));
      }
    else
      {
        new_environ = (char **) malloc ((size + 2) * sizeof (char *));
      }
    if (new_environ == NULL)
      {
        UNLOCK;
        return -1;
      }
    new_environ[size] = malloc (namelen + 1 + vallen);
    if (new_environ[size] == NULL)
      {
        free ((char *) new_environ);
        UNLOCK;
        __set_errno (ENOMEM);
        return -1;
      }

    if (environ != last_environ)
      {
      memcpy ((char *) new_environ, (char *) environ, size * sizeof (char *));
      }

    memcpy (new_environ[size], name, namelen);
    new_environ[size][namelen] = '=';
    memcpy (&new_environ[size][namelen + 1], value, vallen);

    new_environ[size + 1] = NULL;

    last_environ = environ = new_environ;
  }
  else if (replace)
    {
      size_t len = strlen (*ep);
      if (len  < namelen + vallen)
        {
      /* The existing string is too short; malloc a new one.  */
          char *new = malloc (namelen + 1 + vallen);
         if (new == NULL)
           {
              UNLOCK;
              return -1;
           }
      *ep = new;
        }
      memcpy (*ep, name, namelen);
      (*ep)[namelen] = '=';
      memcpy (&(*ep)[namelen + 1], value, vallen);
    }

  UNLOCK;
  return 0;
}
# undef setenv_r
weak_alias (__setenv_r, setenv_r)

/* Reentrant version of unsetenv function */
int
unsetenv_r (const char *name)
{

  size_t len;
  char **ep;


  if (name == NULL || *name == '\0' || strchr (name, '=') != NULL)
    {
      __set_errno (EINVAL);
      return -1;
    }

  len = strlen (name);

  LOCK;

  ep = __environ;
  if (ep != NULL)
    while (*ep != NULL)
      if (!strncmp (*ep, name, len) && (*ep)[len] == '=')
        {
          /* Found it.  Remove this pointer by moving later ones back.  */
          char **dp = ep;
          do{
              dp[0] = dp[1];
            }
          while (*dp++);
          /* Continue the loop in case NAME appears again.  */
        }
      else
        ++ep;

  UNLOCK;

  return 0;
}
# undef unsetenv_r
weak_alias (__unsetenv_r, unsetenv_r)

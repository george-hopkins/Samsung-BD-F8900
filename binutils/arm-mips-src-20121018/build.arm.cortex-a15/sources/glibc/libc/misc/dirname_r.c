#include <libgen.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>

/* Reentrant version of dirname function */
int
dirname_r (const char *path, char *buf, size_t buflen)
{
  const char *endp;
  int         result, len;
  static const char dot[] = ".";

  /* Empty or NULL string gets treated as "." */
  if (path == NULL || *path == '\0') {
      len  = 1;
      memcpy (buf, dot, len + 1);
      return len;
  }

      /*As a special case we have to return "//" if there
           are exactly two slashes at the beginning of the string.  See
           XBD 4.10 Path Name Resolution for more information.  */
  if(!strncmp(path, "//", 2) && ((strlen(path) == 2) || path[2] != '/')){
      len = 2;
      goto Exit;
  }
  /* Strip trailing slashes */
  endp = path + strlen(path) - 1;
  while (endp > path && *endp == '/')
      endp--;

  /* Find the start of the dir */
  while (endp > path && *endp != '/')
      endp--;

  /* Either the dir is "/" or there are no slashes */
  if (endp == path) {
      len = 1;
      memcpy (buf, (*endp == '/') ? "/" : ".", len + 1);
      return len;
  }

  do {
      endp--;
  } while (endp > path && *endp == '/');

  len = endp - path +1;

Exit:
  result = len;
  if (len + 1 > MAXPATHLEN) {
      __set_errno (ENAMETOOLONG);
      return -1;
  }
  if (buf == NULL)
      return result;

  if (len > (int)buflen-1) {
      len    = (int)buflen-1;
      result = -1;
      __set_errno (ERANGE);
  }

  if (len >= 0) {
      memcpy (buf, path, len);
      buf[len] = 0;
  }
  return result;
}

/* Test case for getenv_r and setenv_r functions
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_ITER  5
#define BUF_LEN   32
#define VAR_NAME  "FOO"

int stop_var = 0;

void *t_func (void * s);
int compare_env (char **env1, char **env2, int n);
int get_and_compare (const char* name, const char* val1, const char* val2);

int main (void)
{
  pthread_t t1,t2;
  char **backup;
  int need_backup = 0;
  unsigned int c1 = 0, c2 = 0; /* Occurences of foo and bar, respectively */
  const char *foo = {"foo"}, *bar = {"bar"};
  int env_len = 0, i = 0, ret = 0;
  if (__environ[0] != NULL)
    need_backup = 1;
  if (need_backup)
    {
      while (__environ[i] != NULL)
        i++;
      env_len = i - 1;
      backup = (char **) malloc (sizeof (char*) * env_len);
      if (!backup)
        {
          printf ("malloc failed\n");
          abort();
        }

      for (i = 0; i < env_len; i++)
        {
          backup[i] = (char *) malloc (strlen(__environ[i])+1);
          if (!backup[i])
            {
              printf ("malloc failed");
              abort ();
            }
          strcpy (backup[i], __environ[i]);
        }
    }
  ret = pthread_create (&t1, NULL, &t_func, (void *) foo);
  if (ret)
    {
      perror ("pthread_create");
      goto exit;
    }

  ret = pthread_create (&t2, NULL, &t_func, (void *) bar);
  if (ret)
    {
      perror ("pthread_create");
      goto exit;
    }
  sleep (1);

  for (i = 0 ; i < NUM_ITER; i++)
    {
      ret = get_and_compare (VAR_NAME, foo, bar);
      if (ret == 1)
        c1++;
      else if (ret == 2)
        c2++;
      else if (ret == -1)
        {
          printf ("get_and_compare failed\n");
          goto exit;
        }
      ret = 0;
    }

  pthread_join (t1, NULL);
  pthread_join (t2, NULL);
  if (need_backup)
    {
      ret = compare_env (__environ, backup, env_len);
      if (ret){
        printf ("compare_env failed\n");
        goto exit;
      }
    }
  ret = get_and_compare (VAR_NAME, foo, bar);
  if (ret == -1){
    printf ("get_and_compare failed\n");
    goto exit;
  }
  else if (ret == 1)
    c1++;
  else if (ret == 2)
    c2++;
  ret = 0;
exit:
  if (need_backup)
    {
      for (i = 0; i < env_len; i++)
        free (backup[i]);
      free (backup);
    }
  return ret;
}

void *t_func(void * s)
{
  int cnt = 10000;
  int ret;
  while (cnt--)
    {
      ret = setenv_r (VAR_NAME, (char*) s, 1);
      if (ret)
        perror ("setenv_r");
    }
  return NULL;
}

int compare_env (char **env1, char **env2, int n)
{
  int i;
  for (i = 0; i < n; i++)
    {
      if (!env1[i] || !env2[i])
        {
          printf ("Empty var at %d\n", i);
          return 1;
        }
      else
      if (strcmp (env1[i], env2[i]) && strncmp (env1[i], VAR_NAME"=", 4))
        {
          printf ("Compare failed: %s and %s at %d\n", env1[i], env2[i], i);
          return 1;
        }
    }
  return 0;
}

int get_and_compare (const char* name, const char* val1, const char* val2)
{
  int ret = 0;
  char buf[BUF_LEN] = "\0";
  ret = getenv_r (name, buf, BUF_LEN - 1);
  if (ret)
    {
      perror ("getenv_r");
      ret = -1;
    }
  else
  if (!strcmp (buf, val1))
    ret = 1;
  else
  if (!strcmp (buf, val2))
    ret = 2;
  else
    ret = -1;
  return ret;
}

/*
 * Test case by Alexandre Duret-Lutz <duret_g@epita.fr>.
 * Rollover test by Pavel Fedin <pfedin@samsung.com>
 */

#include <obstack.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define obstack_chunk_alloc verbose_malloc
#define obstack_chunk_free verbose_free
#define ALIGN_BOUNDARY 64
#define ALIGN_MASK (ALIGN_BOUNDARY - 1)
#define OBJECT_SIZE 1000

static void *
verbose_malloc (size_t size)
{
  void *buf = malloc (size);
  printf ("malloc (%zu) => %p\n", size, buf);
  return buf;
}

static void
verbose_free (void *buf)
{
  free (buf);
  printf ("free (%p)\n", buf);
}

/*
 * Address rollover test.
 * The idea is to simulate current chunk being placed at high addresses, and supply length for
 * the grow operation large enough to wrap around the end of address space. Rollover must be
 * detected, and chunk allocator called.
 * We can't have control over what malloc() returns, so here we simulate such state.
 * _obstack_newchunk() should replace changed values, so it should be okay. We fake also
 * object_base in order to avoid data copying in _obstack_newchunk().
 * Note that the program segfaults if one of grow tests fails.
 */

#define ROLLOVER_SIZE 0x00009000
#define ROLLOVER_TEST(f, result, obs, ...)      \
  printf ("%s... ", #f);                        \
  obs.object_base = (void *)0xFFFFA000;         \
  obs.next_free = obs.object_base;              \
  obs.chunk_limit = (void *)0xFFFFC000;         \
  f (&obs, __VA_ARGS__);                        \
  if (obs.object_base == (void *)0xFFFFA000)    \
    {                                           \
      printf ("FAIL!\n");                       \
      result = 1;                               \
    }

int
main (void)
{
  int result = 0;
  int align = 2;
  struct obstack obs;
  void *test_obj;

  while (align <= 64)
    {
      int i;
      int align_mask = align - 1;

      printf ("\n Alignment mask: %d\n", align_mask);

      obstack_init (&obs);
      obstack_alignment_mask (&obs) = align_mask;
      /* finish an empty object to take alignment into account */
      obstack_finish (&obs);

      /* let's allocate some objects and print their addresses */
      for (i = 15; i > 0; --i)
	{
	  void *obj = obstack_alloc (&obs, OBJECT_SIZE);

	  printf ("obstack_alloc (%u) => %p \t%s\n", OBJECT_SIZE, obj,
		  ((uintptr_t) obj & align_mask) ? "(not aligned)" : "");
	  result |= ((uintptr_t) obj & align_mask) != 0;
	}

      /* clean up */
      obstack_free (&obs, 0);

      align <<= 1;
    }

  printf ("\n Address rollover test\n");

  test_obj = malloc (ROLLOVER_SIZE);
  if (!test_obj)
    {
      printf ("Failed to allocate test object!\n");
      return 255;
    }

  obstack_init (&obs);

  ROLLOVER_TEST (obstack_make_room, result, obs, ROLLOVER_SIZE);
  ROLLOVER_TEST (obstack_blank, result, obs, ROLLOVER_SIZE);
  ROLLOVER_TEST (obstack_grow, result, obs, test_obj, ROLLOVER_SIZE);
  ROLLOVER_TEST (obstack_grow0, result, obs, test_obj, ROLLOVER_SIZE);

  obstack_free (&obs, 0);
  free(test_obj);

  return result;
}

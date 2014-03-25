/* Used for error checking.  To support hosts without regcomp, this 1
   could come from a configure check.
   On Cygwin we actually have regexp, but locale implementations are
   incompatible, and 'make install-locales' produces false errors on
   UTF-8 locales  */
#ifndef __CYGWIN__
#define __OPTION_POSIX_REGEXP 1
#endif

/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* Define if building universal (internal helper macro) */
/* #undef AC_APPLE_UNIVERSAL_BUILD */

/* Define to your build machine's byte order */
#define BUILD_BYTE_ORDER __LITTLE_ENDIAN

/* Define if you have the asprintf() function */
#define HAVE_ASPRINTF 1

/* Define if you have the blksize_t type */
#define HAVE_BLKSIZE_T 1

/* Define to 1 if you have the <byteswap.h> header file. */
#define HAVE_BYTESWAP_H 1

/* Define if you have the comparison_fn_t type */
#define HAVE_COMPARISON_FN_T 1

/* Define to 1 if you have the <ctype.h> header file. */
#define HAVE_CTYPE_H 1

/* Define to 1 if you have the <dirent.h> header file. */
#define HAVE_DIRENT_H 1

/* Define to 1 if you have the <endian.h> header file. */
#define HAVE_ENDIAN_H 1

/* Define if you have the euidaccess() function */
#define HAVE_EUIDACCESS 1

/* Define to 1 if you have the <fcntl.h> header file. */
#define HAVE_FCNTL_H 1

/* Define to 1 if you have the <features.h> header file. */
#define HAVE_FEATURES_H 1

/* Define if you have the ftruncate64() function */
#define HAVE_FTRUNCATE64 1

/* Define if you have the getdelim() function */
#define HAVE_GETDELIM 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define if you have the int_fast32_t type */
#define HAVE_INT_FAST32_T 1

/* Define to 1 if you have the `intl' library (-lintl). */
/* #undef HAVE_LIBINTL */

/* Define to 1 if you have the <libintl.h> header file. */
#define HAVE_LIBINTL_H 1

/* Define to 1 if you have the `posix4' library (-lposix4). */
/* #undef HAVE_LIBPOSIX4 */

/* Define if you have the locale_t type */
#define HAVE_LOCALE_T 1

/* Define if you have the lseek64() function */
#define HAVE_LSEEK64 1

/* Define to 1 if you have the <mcheck.h> header file. */
#define HAVE_MCHECK_H 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define if you have the mempcpy() function */
#define HAVE_MEMPCPY 1

/* Define if you have the mmap64() function */
#define HAVE_MMAP64 1

/* Define to 1 if you have the <nl_types.h> header file. */
#define HAVE_NL_TYPES_H 1

/* Define if you have the obstack_printf() function */
#define HAVE_OBSTACK_PRINTF 1

/* Define if you have the off64_t type */
#define HAVE_OFF64_T 1

/* Define if you have the open64() function */
#define HAVE_OPEN64 1

/* Define if your headers declare program_invocation_name */
#define HAVE_PROGRAM_INVOCATION_NAME 1

/* Define if your headers declare program_invocation_short_name */
#define HAVE_PROGRAM_INVOCATION_SHORT_NAME 1

/* Define if you have the readdir64() function */
#define HAVE_READDIR64 1

/* Define if you have the stat64() function */
#define HAVE_STAT64 1

/* Define to 1 if you have the <stdbool.h> header file. */
#define HAVE_STDBOOL_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdio_ext.h> header file. */
#define HAVE_STDIO_EXT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define if you have the stpcpy() function */
#define HAVE_STPCPY 1

/* Define if you have the strchrnul() function */
#define HAVE_STRCHRNUL 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define if you have the strndup() function */
#define HAVE_STRNDUP 1

/* Define if you have the strsep() function */
#define HAVE_STRSEP 1

/* Define to 1 if you have the <sys/cdefs.h> header file. */
#define HAVE_SYS_CDEFS_H 1

/* Define to 1 if you have the <sys/mman.h> header file. */
#define HAVE_SYS_MMAN_H 1

/* Define to 1 if you have the <sys/param.h> header file. */
#define HAVE_SYS_PARAM_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <sys/uio.h> header file. */
#define HAVE_SYS_UIO_H 1

/* Define to 1 if you have the <time.h> header file. */
#define HAVE_TIME_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define if you have the vasprintf() function */
#define HAVE_VASPRINTF 1

/* Define if you have the vprintf() function */
#define HAVE_VPRINTF 1

/* Define to 1 if you have the <wctype.h> header file. */
#define HAVE_WCTYPE_H 1

/* Define if you have the __compar_fn_t type */
#define HAVE___COMPAR_FN_T 1

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT ""

/* Define to the full name of this package. */
#define PACKAGE_NAME "glibc build-side support"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "glibc build-side support (see version.h)"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "glibc-build-side-support"

/* Define to the version of this package. */
#define PACKAGE_VERSION "(see version.h)"

/* Additional package description */
#define PKGVERSION "(EGLIBC) "

/* Bug reporting address */
#define REPORT_BUGS_TO "<http://www.eglibc.org/issues/>"

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Define WORDS_BIGENDIAN to 1 if your processor stores words with the most
   significant byte first (like Motorola and SPARC, unlike Intel). */
#if defined AC_APPLE_UNIVERSAL_BUILD
# if defined __BIG_ENDIAN__
#  define WORDS_BIGENDIAN 1
# endif
#else
# ifndef WORDS_BIGENDIAN
/* #  undef WORDS_BIGENDIAN */
# endif
#endif

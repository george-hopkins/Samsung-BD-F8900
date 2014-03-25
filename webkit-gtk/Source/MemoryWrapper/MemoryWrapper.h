/* -*- h -*- ******************************************************************
 *
 * Copyright 2011 by Samsung Electronics, Inc.,
 *
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 *
 ******************************************************************************
 */
/**
 * @file MemoryWrapper.h
 * @brief Implementation of the Memory Monitoring module.
 */

#ifndef _MEMORYWRAPPER_H_
#define _MEMORYWRAPPER_H_

/**
 * on a 64 bit build target things are slightly different -
 * the least obvious is printf when printing size_t arguments ...
 */
#if defined(__LP64__) || defined(_LP64) || __WORDSIZE == 64
#define BUILD64
#endif

/**
 * header to include size_t
 */
#include <sys/types.h>

/**
 * Thread support
 */
#include <pthread.h>

#ifdef __cplusplus
extern "C"
{
#else

#ifndef bool
/** bool is not defined for pure c */
#define bool int
#endif

#ifndef true
/** for pure c - definition for true */
#define true (!0)
#endif

#ifndef false
/** for pure c - definition for false */
#define false (0)
#endif

#endif

/**
 * By calling this function with the parameter true then if the critical limit is
 * reached an exception will be thrown - this can also be disabled with the
 * parameter false - by default they are disabled which results in
 * malloc returning NULL which if not checked will cause a crash.
 *
 * @param [in] flag  If this flag is true exceptions are enabled, false to disable
 **/
void WrapperSetThrowException(bool flag);

/**
 * By calling this function with the parameter true restricts the memory allocations
 * so that if the memory allocations have reched their limit further alocations are
 * prevented until the amount has been reduced.
 *
 * @param [in] flag  If this flag is set true this wrapper is more restrictive.
 */
void WrapperSetStrict(bool flag);

/**
 * Wrapper Callback function
 *
 * @param [in]     ptr   As this function is triggered before the actual memory
 *                       allocation so this pointer points to the address of the
 *                       old memory in case the memory is to be resized.
 *                       This variable can potentially be useful for the garbage
 *                       collection process.
 * @param [in]     size  The size of memory that was attempted to be allocated but
 *                       caused this callback to be raised.
 * @param [in,out] user  The pointer to some user data.
 */
typedef void(*WrapperCallbackFunc)(const void *ptr, size_t size, void *user);

/**
 * This function set the critical limit for the memory wrapper; once this limit is
 * reached an exception will be fired all related memory allocations become invalid;
 * this function must be called in the thread that will spawn other threads.
 *
 * @param [in] limit  The critical limit of this memory wrapper
 */
void WrapperSetLimit(size_t limit);

/**
 * This functions returns true id the critical limit was reached until the lowest
 * threshold is reached; if none was set then 50% of the limit.
 *
 * @return true if the critical limit was reached until the lowest limit was reached;
 *         in case none was set then below 50%, false otherwise
 */
bool WrapperIsCritical(void);

/**
 * Query current memory allocation in bytes
 *
 * @return current memory aloocation in bytes
 */
size_t WrapperGetAlloc(void);

/**
 * Query the currently set limit in bytes
 *
 * @return current set memory limit in bytes
 */
size_t WrapperGetLimit(void);

/**
 * This function allow to set or unset a theshold and when it is reached the callback
 * function will be called; several callbacks can be registered; there can only one
 * function per threshold value
 *
 * @param [in] threshold  Threshold in bytes on which the callback shall be called
 * @param [in] func       The callback function that is to be registered or
 *                        NULL if to be unregistered.
 * @param [in] data       The data that can be provided to the callback function,
 *                        parameter can be NULL
 *
 * @return true on success, false otherwise.
 */
bool WrapperSetCallback(size_t threshold, WrapperCallbackFunc func, void *data);

/**
 * This support function allows to print all memory blocks that were allocated by this
 * wrapper
 *
 * @param [in] prefix  Each line that is printed by this function is prefixed with the
 *                     privided string, if it is left NULL no prefix will be printed,
 *                     parameter can be NULL
 */
void WrapperPrint(const char* prefix);

/**
 * Wrapper support function that limits the output of WrapperAlloc so that allocations
 * that exceed the limit are printed in detail
 *
 * @param [in] limit   Limit that is to be exceeded in order to be printed, if you want
 *                     all recorded memory allocations printed set to 0.
 */
void WrapperSetPrintLimit(size_t limit);

/**
 * This function shall only be called if the critical limit has been reached and the
 * using thread is about to be destroyed; Once this call is completed all the allocated
 * memory is free'd up.
 * This function MUST be called from the same thread as "WrapperSetLimit".
 *
 * @param [in] prefix  Each line that is printed by this function is prefixed with the
 *                     privided string, if it is left NULL no prefix will be printed,
 *                     parameter can be NULL
 *
 * @return true on success, false otherwise
 */
bool WrapperCleanUp(const char* prefix);

/**
 * this fucntion allows the exceptions to be handled when caused by operations in
 * the main function - this is intended for pure C environments
 *
 * @param [in]  func       This is the function that is to be wrapped, typical "main"
 * @param [in]  argc       The argument count
 * @param [in]  argv       The list of parameters
 * @param [out] exception  Pointer to a variable that is to be updated if an exception
 *                         arises - if the value is NULL the exception is ignored. The
 *                         values are described below:
 *                           0 = The "main" function exited normally, no exception
 *                           1 = A WrapperException was thrown, the message is printed
 *                               on stdout
 *                           2 = A exception of type "std::exception" was thrown, the
 *                               exception message is printed on stdout
 *                           3 = An unknow exception was thrown
 *
 * @return the return code of the "main" function.
 */
int WrapperMainWrapper(int(*func)(int, char**), int argc, char *argv[], int *exception);

#ifdef __cplusplus
}
#endif


/**
 * Allow exception handling only in C++ mode
 */
#ifdef __cplusplus
#include <new>

/// Wrapper Exception internal details class
class WrapperExceptionInternals;

/// Wrapper Exception class
class WrapperException : public std::bad_alloc
{
public:
    /// defaulty constructor
    WrapperException(void) throw();

    /// constructor
    ///
    /// @param [in] caller  This is the caller address who called the wrapper function
    /// @param [in] func    This is the function of wrapper function thatwas called
    /// @param [in] info    Textual information in regards of this exception
    WrapperException(const void *caller, const char *func, const char *info) throw();

    /// Copy constructor
    ///
    /// @param [in] rhs  Data that shall be copied
    WrapperException(const WrapperException &rhs) throw();

    /// Assignment operator overload (=)
    ///
    /// @param [in] rhs  Data that shall be copied
    ///
    /// @return the local class
    const WrapperException& operator=(const WrapperException &rhs) throw();

    /// destructor
    virtual ~WrapperException(void) throw();

    /// Return the reason why this exception was thrown
    ///
    /// @return The reason why this exception was thrown
    const char* what(void) const throw();

    /// Query the caller address who called the frapper function
    ///
    /// @return The caller address who called the frapper function
    const void* Caller(void) const throw();

    /// Query the wrapper function that was called
    ///
    /// @return The wrapper function that was called
    const char* Func(void) const throw();

    /// Query the textual information in regards of this exception
    ///
    /// @return The textual information in regards of this exception
    const char* Info(void) const throw();

    /// Query the thread id who caused this exception
    ///
    /// @return The thread id who caused this exception
    pthread_t ThreadId(void) const throw();

private:
    /// The data internals
    WrapperExceptionInternals *m_internals;
};
#endif

/**
 * Windows does not have a memeory wrapper interface so we need to use macros
 * instead.
 */
#ifdef WIN32

void* __wrap_malloc(size_t size);
void* __wrap_calloc(size_t n, size_t size);
void* __wrap_realloc(void *ptr, size_t size);
char* __wrap_strdup(const char *str);
char* __wrap_strndup(const char *str, size_t size);
#ifdef ENABLE_STRDUPA
char* __wrap_strdupa(const char *str);
char* __wrap_strndupa(const char *str, size_t size);
#endif // ENABLE_STRDUPA
void  __wrap_free(void *ptr);

#define malloc                           __wrap_malloc
#define calloc                           __wrap_calloc
#define realloc                          __wrap_realloc
#define strdup                           __wrap_strdup
#define strndup                          __wrap_strndup
#ifdef ENABLE_STRDUPA
#define strdupa                          __wrap_strdupa
#define strndupa                         __wrap_strndupa
#endif // ENABLE_STRDUPA
#define free                             __wrap_free

#define __real_malloc(s)                 _aligned_malloc(s,1)
#define __real_calloc(n, s)              _aligned_malloc((n) * (s), 1)
#define __real_realloc(p, s)             _aligned_realloc(p, s, 1)
#define __real_strdup                    _strdup
#define __real_strndup                   _strndup
#ifdef ENABLE_STRDUPA
#define __real_strdupa                   _strdupa
#define __real_strndupa                  _strndupa
#endif // ENABLE_STRDUPA
#define __real_free(p)                   _aligned_free(p)

#define __real_new(s)                    ::operator new(s)
#define __real_newArray(s)               ::operator new[](s)
#define __real_delete(ptr)               ::operator delete(ptr)
#define __real_deleteArray(ptr)          ::operator delete[](ptr)

#define __real_newNoThrow(s,n)           ::operator new(s,n)
#define __real_newArrayNoThrow(s,n)      ::operator new[](s,n)
#define __real_deleteNoThrow(ptr,n)      ::operator delete(ptr,n)
#define __real_deleteArrayNoThrow(ptr,n) ::operator delete[](ptr,n)

#endif

#endif /* _MEMORYWRAPPER_H_ */


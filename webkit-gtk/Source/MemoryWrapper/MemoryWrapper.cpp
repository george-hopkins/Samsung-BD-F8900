// -*- c++ -*- ////////////////////////////////////////////////////////////////
//
// Copyright 2011 by Samsung Electronics, Inc.,
//
// This software is the confidential and proprietary information
// of Samsung Electronics, Inc. ("Confidential Information").  You
// shall not disclose such Confidential Information and shall use
// it only in accordance with the terms of the license agreement
// you entered into with Samsung.
//
///////////////////////////////////////////////////////////////////////////////
//
/// @file MemoryWrapper.cpp
/// @brief Implementation of the Memory Monitoring module.

// ---------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#ifndef MEMORYWRAPPERSTATIC
#include <dlfcn.h>
#endif
#include <set>
#include <utility>

#include "MemoryWrapper.h"
#include "MemoryWrapper_local.h"

// ---------------------------------------------------------------------------

#ifdef WIN32

#include <windows.h>

/// Remap "windows's sleep() funtion to be like the posix version"
#define sleep(t) Sleep((t) * 1000)

// If the "max" macro is defined we get an error like
//   warning C4003: not enough actual parameters for macro 'max'
// but the template  uses an STL function that is required
//   template <class T> class TWrapAlloc
// uses in one of its functions
//   std::numeric_limits<size_type>::max()
// Actually this macro breaks the code when a function is mistaken for
// a used macro.
#ifdef max
#undef max
#endif

#else
#include <unistd.h>
#endif

// ----------------------------------------------------------------------------

// In case it is required to be strict with memory allocations such as check if
// memory was not allocated through this module and an exception will be fired
// enable the following macro
// #define MEMORYWRAPPERSTRICT

// ----------------------------------------------------------------------------

// If memory allocations are to be traced enable the following macro
// #define MEMORYWRAPPERTRACE

// ----------------------------------------------------------------------------

// If memeory usage statistics are to be printed post every change enable the
// following macro
// #define MEMORYWRAPPERPRINTSTAT

// ----------------------------------------------------------------------------

/// The external name for this module which is used to report to STDOUT.
static const char MODULE[] = "MemoryWrapper";

// ----------------------------------------------------------------------------

#ifdef MEMWRAPPEREXCEPTIONS
/// A flag that if it set to be true will cause an exception to be thrown if
/// the memory allocation attempts to exceeds the limit.
static bool s_throw_exception = false;
#endif // MEMWRAPPEREXCEPTIONS

/// A flag that if set enforced a hard limit on memory allocations; if the memory
/// allocation limit is reached no further memory allocations are allowed until
/// the allocated memory is reduced.
static bool s_strict_limit = false;

/// A flag that is true it the critical limit was reached and once if true remains
/// set until the lowest threshold is reached or in case none was set 50% of the
/// limit was reached
static bool s_critical = false;

/// Mutex for memory allocations
static pthread_mutex_t s_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;

/// A list of memory allocations and their size
static TWrapperAllocMap s_alloc;

/// The thread creatoru thread id - this may spawn other threads and it required
/// the be returned to once the clean-up is complete.
static pthread_t s_creator;

/// This is the current memory allocation limit
static size_t s_limit;

/// This is the current memory allocation (payload data)
static size_t s_current;

/// This is the number of allocations that were performed since the last clean-up
static size_t s_num_alloc;

/// This is the list of callbacks that are triggered if a threshold is reached
static TWrapperCallbackMap s_callbacks;

/// the last theshold that was reached and a callback was fired - only if a different
/// threshold os reached a different callback is fired - dulipcations of calls of the
/// callbacks are not allowed.
static size_t s_threshold;

/// The Print limit - all allocations are group together and in case they exceed the
/// limit are printed in detail
static size_t s_print_limit;

// ----------------------------------------------------------------------------

/// Print memory allocation graph with the usage level in percent.
///
/// @param [in] force  Force to show updated graph instead of applying a threshold
///                    to reduce the "noise".
static void _print_stat(bool force = false);

/// Converts a pthread id into a printable string representation of pthread_t
///
/// @param [in] thread  Thread id that is to be converted
///
/// @return the printable string representation of pthread_t
static const char* _pthread2string(const pthread_t &thread);

// ----------------------------------------------------------------------------

void WrapperSetThrowException(bool flag)
{
    pthread_mutex_lock(&s_mutex);

#ifdef MEMWRAPPEREXCEPTIONS

    s_throw_exception = flag;

#else // MEMWRAPPEREXCEPTIONS

    if(flag)
    {
        printf("%s: Error: Exceptions are not built-in - IGNORE!",
               __FUNCTION__);
    }

#endif // MEMWRAPPEREXCEPTIONS

    pthread_mutex_unlock(&s_mutex);
}

void WrapperSetStrict(bool flag)
{
    pthread_mutex_lock(&s_mutex);

    s_strict_limit = flag;

    pthread_mutex_unlock(&s_mutex);
}

void WrapperSetLimit(size_t limit)
{
    pthread_mutex_lock(&s_mutex);

    s_creator = pthread_self();
    s_limit   = limit;

    // The allocation map is now reset and
    // all allocations are not longer tracked,
    // we can now clear the allocation map.
    if(!s_limit)
    {
        s_alloc.clear();
        s_current = 0;
        s_num_alloc = 0;
    }

    pthread_mutex_unlock(&s_mutex);
}

bool WrapperSetCallback(size_t threshold, WrapperCallbackFunc func, void *data)
{
    pthread_mutex_lock(&s_mutex);

    if(threshold > s_limit)
    {
        // What are we monitoring?
        return false;
    }

    bool rc = false;

    if(func)
    {
        s_callbacks[threshold] = std::make_pair<WrapperCallbackFunc, void*>(func, data);
        rc = true;
    }
    else
    {
        TWrapperCallbackMap::iterator it(s_callbacks.find(threshold));
        if(it != s_callbacks.end())
        {
            s_callbacks.erase(it);
            rc = true;
        }
    }

    pthread_mutex_unlock(&s_mutex);
    return rc;
}

bool WrapperIsCritical(void)
{
    return s_critical;
}

size_t WrapperGetAlloc(void)
{
    pthread_mutex_lock(&s_mutex);
    size_t alloc = s_current;
    pthread_mutex_unlock(&s_mutex);

    return alloc;
}

size_t WrapperGetLimit(void)
{
    pthread_mutex_lock(&s_mutex);
    size_t limit = s_limit;
    pthread_mutex_unlock(&s_mutex);

    return limit;
}

static const char* _allocfunc2string(EWrapCmd cmd)
{
    switch(cmd)
    {
        case EMALLOC:
            return "malloc(size)";
        case ECALLOC:
            return "calloc(num ,size)";
        case EREALLOC:
            return "realloc(ptr, size)";
        case ESTRDUP:
            return "strdup(str)";
        case ESTRNDUP:
            return "strndup(str, size)";
        case EFREE:
            return "free(ptr)";
        case ENEW:
            return "new X()";
        case ENEW_NT:
            return "new (std::nothrow) X()";
        case ENEW_ARRAY:
            return "new X[](num)";
        case ENEW_ARRAY_NT:
            return "new (std::nothrow) X[](num)";
        case EDEL:
            return "delete X";
        case EDEL_NT:
            return "delete (std::nothrow) X";
        case EDEL_ARRAY:
            return "delete [] X";
        case EDEL_ARRAY_NT:
            return "delete (std::nothrow) [] X";
        default:
            return "ERROR: UNKNOWN ALLOCATION TYPE";
    }
}

void WrapperSetPrintLimit(size_t limit)
{
    s_print_limit = limit;
}

void WrapperPrint(const char* prefix)
{
    printf("%s-----< %s >-----\n", prefix ? prefix : "", __FUNCTION__);

    pthread_mutex_lock(&s_mutex);

    if(s_alloc.size())
    {
#ifdef MEMORYWRAPPERDEBUG

        // If debug is enabled a comprehensive allocation map against their cause
        // is generated - this is good for debuggign but not really helpful for
        // a production device.

        printf("%s: Generate allocation map ... please wait.\n",
               prefix ? prefix : "");

        // Group allocators by their address in order to group them
        TWrapGroupCaller group_caller;
        for( TWrapperAllocMap::const_iterator it(s_alloc.begin());
             it != s_alloc.end();
             ++it )
        {
            CWrapperAllocKey key(it->second.Caller(), it->second.Size());
            TWrapGroupCaller::iterator git(group_caller.find(key));
            if(git == group_caller.end())
            {
                group_caller[key] = it->second;
            }
            else
            {
                git->second.Add(it->second);
            }
        }

        // Group entries by their total size and then their count
        TWrapGroupSize group_size;
        for( TWrapGroupCaller::const_iterator it(group_caller.begin());
             it != group_caller.end();
             ++it )
        {
            group_size.insert(CWrapperSizeData(it->first.Caller(),
                                               it->second.Func(),
                                               it->second.Size(),
                                               it->first.Size(),
                                               it->second.Count()));
        }
        group_caller.clear();

        printf("%s--------------------------------------------------\n",
               prefix ? prefix : "");

#ifndef MEMORYWRAPPERSTATIC
        // A record of callers and shared library names
        TWrapSharedLibs shared_lib_map;
#else // MEMORYWRAPPERSTATIC
        // The callers are recorded for reporting
        TWrapCallers callers;
#endif // MEMORYWRAPPERSTATIC

        size_t cnt = 1;
        for( TWrapGroupSize::const_iterator it(group_size.begin());
             it != group_size.end();
             ++it, cnt++ )
        {
            size_t total = it->Total();

            if(total < s_print_limit)
            {
                printf("%s Allocations (size x count) that are less than the limit of "
#ifdef BUILD64
                       "%lu"
#else
                       "%u"
#endif
                       " are ignored.\n",
                       prefix ? prefix : "", s_print_limit);
                break;
            }

            const void *caller = it->Caller();

#ifndef MEMORYWRAPPERSTATIC
            // Update the caller so that the address in the shared library
            // can be reported correctly
            Dl_info dlinfo;

            if(dladdr(caller, &dlinfo))
            {
                caller = (const void*)((char*)(caller) - (char*)dlinfo.dli_fbase);
                shared_lib_map[dlinfo.dli_fname].push_back(caller);
            }
#else // MEMORYWRAPPERSTATIC
            // only report unique callers but preserve the order
            if(caller && callers.find(caller) == callers.end())
            {
                callers[caller] = cnt;
            }
#endif // MEMORYWRAPPERSTATIC

            size_t count = it->Count();
            size_t size  = it->Size();

            printf(
#ifdef BUILD64
                   "%s %8lu: %10p used %-28s "
                   "(total = %8lu, size = %8lu, %7lu call%s)\n",
#else
                   "%s %8u: %10p used %-28s "
                   "(total = %8u, size = %8u, %7u call%s)\n",
#endif
                   prefix ? prefix : "", cnt, caller,
                   _allocfunc2string(it->Func()),
                   total, size, count,
                   count == 1 ? "" : "s");
        }
        group_size.clear();

#ifndef MEMORYWRAPPERSTATIC

        if(shared_lib_map.size())
        {
            printf("\n\n%s: Allocation maps:\n"
                   "To get the code location please run these commands on the console:\n"
                   "=========================================================================\n",
                   prefix ? prefix : "");

            for( TWrapSharedLibs::const_iterator it(shared_lib_map.begin());
                 it != shared_lib_map.end();
                 ++it )
            {
                TMWString str = "addr2line -Cfe ";
                str += it->first;

                char buf[32];

                for( TWrapSharedLibAddr::const_iterator ait(it->second.begin());
                     ait != it->second.end();
                     ++ait )
                {
                    if(sprintf(buf, " %p", *ait))
                    {
                        str += buf;
                    }
                }

                printf("%s\n", str.c_str());
            }

            printf("=========================================================================\n\n");
        }
        shared_lib_map.clear();

#else // MEMORYWRAPPERSTATIC

        if(callers.size())
        {
            TWrapCallerOrder caller_order;
            for( TWrapCallers::const_iterator it(callers.begin());
                 it != callers.end();
                 ++it )
            {
                caller_order[it->second] = it->first;
            }
            callers.clear();

            printf("\n\n%s: Allocation maps:\n"
                   "To get the code location please run these commands on the console:\n"
                   "=========================================================================\n",
                   prefix ? prefix : "");

            TMWString str;
            for( TWrapCallerOrder::const_iterator it(caller_order.begin());
                 it != caller_order.end();
                 ++it )
            {
                if(str.empty())
                {
                    str += "addr2line -Cfe ";
#ifdef EXECUTABLE_NAME
                    str += EXECUTABLE_NAME;
#else // EXECUTABLE_NAME
                    str += "executable";
#endif // EXECUTABLE_NAME
                    str += ' ';
                }

                char buf[32];
                if(sprintf(buf, " %p", it->second))
                {
                    str += buf;
                }
            }
            if(str.size())
            {
                printf("%s\n", str.c_str());
            }

            printf("=========================================================================\n\n");
        }

#endif // MEMORYWRAPPERSTATIC

        printf("%s--------------------------------------------------\n",
               prefix ? prefix : "");

#endif // MEMORYWRAPPERDEBUG
    }

    printf(
#ifdef BUILD64
           "%s Memory alloc'd : %lu/%lu in %lu chunks (avg. size %lu).\n"
           "%s                : num. allocs. %lu\n",
#else
           "%s Memory alloc'd : %u/%u in %u chunks (avg. size %u).\n"
           "%s                : num. allocs. %u\n",
#endif
           prefix ? prefix : "", s_current, s_limit, s_alloc.size(),
           s_alloc.size() ? s_current / s_alloc.size() : 0,
           prefix ? prefix : "", s_num_alloc);

    pthread_mutex_unlock(&s_mutex);
}

bool WrapperCleanUp(const char* prefix)
{
    printf("%s-----< %s >-----\n", prefix ? prefix : "", __FUNCTION__);

    pthread_mutex_lock(&s_mutex);

    pthread_t self = pthread_self();
    if(memcmp(&s_creator, &self, sizeof(pthread_t)))
    {
        // This is not the creator thread.
        pthread_mutex_unlock(&s_mutex);

#ifdef MEMWRAPPEREXCEPTIONS
        if(s_throw_exception)
        {
            throw WrapperException(__builtin_return_address(0), __FUNCTION__,
                                   "Called by wrong thread - use creator thread.");
        }
#endif // MEMWRAPPEREXCEPTIONS

        return false;
    }

    // Check on threads have allocated memory
    std::set<pthread_t, WrapperLessPthread, TWrapAlloc<pthread_t> > threads;

    for( TWrapperAllocMap::const_iterator it(s_alloc.begin());
         it != s_alloc.end();
         ++it )
    {
#ifdef MEMORYWRAPPERDEBUG
        threads.insert(it->second.Thread());
#else // MEMORYWRAPPERDEBUG
        threads.insert(it->second.second);
#endif // MEMORYWRAPPERDEBUG

    }

    // Before we can actually free memory we need to make sure all but
    // the creator thread are destroyed.
    while(threads.size())
    {
        std::set<pthread_t, WrapperLessPthread, TWrapAlloc<pthread_t> >::iterator
            it(threads.begin());
        while(it != threads.end())
        {
            const char *thread_id = _pthread2string(*it);

            if(!memcmp(&s_creator, &(*it), sizeof(pthread_t)))
            {
                // spare the creator thread for now ... kill all its child-threads first
                threads.erase(it++);
                continue;
            }

            printf("%s: %s: Thread %s - about to cancel thread ...\n",
                   MODULE, __FUNCTION__, thread_id);

            // Check if the thread is still there
            if(pthread_kill(*it, 0))
            {
                // remove thread that - it does not exist enymore.
                threads.erase(it++);
                continue;
            }

            printf("%s: %s: Thread %s - wait for cancel completion ...\n",
                   MODULE, __FUNCTION__, thread_id);

            void *res = 0;

            // cancel the thread and wait until it is finished
            pthread_cancel(*it);
            if( !pthread_join(*it, &res) &&
                res == PTHREAD_CANCELED )
            {
                // The child thread is now gone - remove from list.
                printf("%s: %s: Thread %s - cancel complete.\n",
                       MODULE, __FUNCTION__, thread_id);

                threads.erase(it++);
                continue;
            }

            printf("%s: %s: Thread %s - cancel failed.\n",
                   MODULE, __FUNCTION__, thread_id);

            // advance to next thread
            ++it;
        }
    }

    // All but the creator threads are now destroyed and the creator thread is suspended,
    // it shall be safe to free all memory.

    while(s_alloc.size())
    {
        TWrapperAllocMap::iterator it(s_alloc.begin());
        MAKE_MANGLED_FUNC(__wrap,_free)(it->first);
    }
    s_num_alloc = 0;

    _print_stat(true);

    pthread_mutex_unlock(&s_mutex);

    return true;
}

int WrapperMainWrapper(int(*func)(int, char**), int argc, char *argv[], int *exception)
{
    if(!func)
    {
        printf("%s: Error function is NULL\n", __FUNCTION__);
        return -1;
    }

#ifdef MEMWRAPPEREXCEPTIONS

    int rc = 0;
    try
    {
        rc = func(argc, argv);
        if(exception)
        {
            *exception = 0;
        }
    }
    catch(WrapperException &e)
    {
        printf("-----< %s >-----\n  %s\n\n", __FUNCTION__, e.what());
        if(exception)
        {
            *exception = 1;
        }
    }
    catch(std::exception &e)
    {
        printf("-----< %s >-----\n  %s\n\n", __FUNCTION__, e.what());
        if(exception)
        {
            *exception = 2;
        }
    }
    catch(...)
    {
        printf("-----< %s >-----\n  UNKNOWN EXCEPTION\n\n", __FUNCTION__);
        if(exception)
        {
            *exception = 3;
        }
    }

#else // MEMWRAPPEREXCEPTIONS

    int rc = func(argc, argv);
    if(exception)
    {
        *exception = 0;
    }

#endif // MEMWRAPPEREXCEPTIONS

    return rc;
}

// ----------------------------------------------------------------------------

#ifdef MEMWRAPPEREXCEPTIONS

WrapperException::WrapperException(void) throw()
    : std::bad_alloc()
    , m_internals(new WrapperExceptionInternals)
{
}

WrapperException::WrapperException(const void *caller, const char *func,
                                   const char *info) throw()
    : std::bad_alloc()
    , m_internals(new WrapperExceptionInternals(caller, func, info, pthread_self()))
{
}

WrapperException::WrapperException(const WrapperException &rhs) throw()
    : std::bad_alloc()
    , m_internals(new WrapperExceptionInternals(*rhs.m_internals))
{
}

const WrapperException& WrapperException::operator=(const WrapperException &rhs) throw()
{
    *m_internals = *rhs.m_internals;

    return *this;
}

WrapperException::~WrapperException(void) throw()
{
    delete m_internals;
}

const char* WrapperException::what(void) const throw()
{
    static std::basic_string<char, std::char_traits<char>, TWrapAlloc<char> > out;

    out.clear();

    char ptr[32]; // just to be safe from memory scribbles
    if(sprintf(ptr, "%p", m_internals->Caller()))
    {
        out += "Caller at address ";
        out += ptr;
        out += " used ";
    }

    out += '"';
    out += m_internals->Func();
    out += "\" caused \"";
    out += m_internals->Info();
    out += '"';

    return out.c_str();
}

const void* WrapperException::Caller(void) const throw()
{
    return m_internals ? m_internals->Caller() : 0;
}

const char* WrapperException::Func(void) const throw()
{
    return m_internals ? m_internals->Func() : "";
}

const char* WrapperException::Info(void) const throw()
{
    return m_internals ? m_internals->Info() : "";
}

pthread_t WrapperException::ThreadId(void) const throw()
{
    return m_internals ? m_internals->ThreadId() : pthread_self();
}

// ----------------------------------------------------------------------------

WrapperExceptionInternals::WrapperExceptionInternals(void)
    : m_caller(0)
{
}

WrapperExceptionInternals::WrapperExceptionInternals(const void *caller, const char *func,
                                                     const char *info, const pthread_t thread)
    : m_caller(caller)
    , m_func(func ? func : "")
    , m_info(info ? info : "")
    , m_thread(thread)
{
}

WrapperExceptionInternals::WrapperExceptionInternals(const WrapperExceptionInternals &rhs)
{
    operator=(rhs);
}

const WrapperExceptionInternals&
WrapperExceptionInternals::operator=(const WrapperExceptionInternals &rhs)
{
    m_caller = rhs.m_caller;
    m_func   = rhs.m_func;
    m_info   = rhs.m_info;
    m_thread = rhs.m_thread;

    return *this;
}

void* WrapperExceptionInternals::operator new(size_t size)
{
    return MAKE_MANGLED_FUNC(__real,_new)(size);
}

void WrapperExceptionInternals::operator delete(void *ptr)
{
    return MAKE_MANGLED_FUNC(__real,_delete)(ptr);
}

const void* WrapperExceptionInternals::Caller(void) const
{
    return m_caller;
}

const char* WrapperExceptionInternals::Func(void) const
{
    return m_func.c_str();
}

const char* WrapperExceptionInternals::Info(void) const
{
    return m_info.c_str();
}

pthread_t WrapperExceptionInternals::ThreadId(void) const
{
    return m_thread;
}

#endif // MEMWRAPPEREXCEPTIONS

// ----------------------------------------------------------------------------

/// Print memory sage bar-graph when significant change took place
///
/// @param [in] force  Force the print when changes are marginal
static void _print_stat(bool force)
{
    static char   graph[] = "[####################]";
    static size_t last    = 0;
    static size_t prev    = 0;
    const  size_t len     = sizeof(graph) - 3;
    const  size_t margin  = s_limit / len;

    /// Sanity check
    if(!s_limit || !len)
    {
        return;
    }

    size_t level =  s_current / margin;
    if(level >= len)
    {
        level = len;
    }

    // check if there is a noticable difference
    if(!force && last == level)
    {
        return;
    }

    // reduce oscillation which can occur when the
    // difference in the bar-graph is recognised but the
    // change can be a s little as 1 byte.
    if(prev < s_current)
    {
        if((s_current - prev) / margin == 0)
        {
            return;
        }
    }
    else
    {
        if((prev - s_current) / margin == 0)
        {
            return;
        }
    }

    last = level;
    prev = s_current;

    // let's show the bar-graph...
    if(level)
    {
        memset(graph + 1, '=', level);
    }
    if(level < len)
    {
        graph[level + 1] = '>';
        memset(graph + 2 + level, ' ', len - 1 - level);
    }

    printf("%s: %s %3"
#ifdef BUILD64
           "lu"
#else // BUILD64
           "u"
#endif // BUILD64
           "%%\n", MODULE, graph,
           s_limit > 200000
           ? s_current / (s_limit / 100)
           : (s_current * 100) / s_limit);
}

/// Convert the string identifier into a printable form
///
/// @param [in] thread  Thread identifier
///
/// @return the printable identifier
static const char* _pthread2string(const pthread_t &thread)
{
    static char out[sizeof(pthread_t) * 2 + 1];
    for(size_t i=0 ; i<sizeof(pthread_t) ; i++)
    {
        sprintf(out + 2 * i, "%02X",
                reinterpret_cast<const unsigned char*>(&thread)[i]);
    }
    return out;
}

#ifdef WIN32
/// Port of posix's strndup to Windows
///
/// @param [in] str   String to copy
/// @param [in] size  Number of bytes that are to be allocated
///
/// @return the newly allocated memory with the new string on success,
///         NULL otherwise.
char* _strndup(const char *str, size_t size)
{
    char *out = (char*)MAKE_MANGLED_FUNC(__real,_malloc)(size);
    if(str && out && size)
    {
        out[size-1] = 0;
        strncpy(out, str, size-1);
    }
    return (char*)out;
}
#endif // WIN32

/// This support function is the herart of rhe memory allocation wrapper while it
/// combines allocation and deallocation routines while providing an adequate mutex
/// protection.
///
/// @param [in] caller     The external caller of the memory wrapper function, this
///                        helps debugging (e.g. double free) or othere scenarios that
///                        normally cause a core dump.
/// @param [in] func       The calling function of this module
/// @param [in] cmd        Function neumerator to idenify the call
///                        functions to allocate or free memory.
///                        (e.g. delete of a NULL pointer is valid for C++).
/// @param [in] ptr        This pointer is the old memory location and initialisation
///                        strings for strdup variants
///                        (used for realloc, free, delete and strdup initialisers)
/// @param [in] size       Size of the memory block to be allocated
///                        (used for malloc, calloc, realloc and new)
/// @param [in] nmemb      Number of elements to be allocated
///                        (used for calloc only)
///
/// @return The pointer of the newly allocated memory on success, NULL if the call
///         failed. This function also returns NULL for function such as free and delete
///         which do not have return values.
static void *_alloc(void *caller, const char *func, EWrapCmd cmd,
                    void *ptr = 0, size_t size = 0, size_t nmemb = (size_t)-1)
{
    bool is_strdup = false;

    size_t total = size;
    if(nmemb != (size_t)-1)
    {
        total *= nmemb;
    }
    switch(cmd)
    {
        case EREALLOC:
        case EFREE:
        case EDEL:
        case EDEL_NT:
        case EDEL_ARRAY:
        case EDEL_ARRAY_NT:
            if(!ptr)
            {
                break;
            }

#ifdef MEMORYWRAPPERTRACE
            printf("%s: (%i)%s(%p, "
#ifdef BUILD64
                   "%lu, %lu"
#else // BUILD64
                   "%u, %u"
#endif // BUILD64
                   ") - "
#ifdef BUILD64
                   "%lu / %lu"
#else // BUILD64
                   "%u / %u"
#endif // BUILD64
                   "\n",
                   __FUNCTION__, cmd, func ? func : "UNKNOWN", ptr, size,
                   nmemb != (size_t)-1 ? nmemb : 0, s_current, s_limit);
#endif // MEMORYWRAPPERTRACE

            break;

        case ESTRDUP:
            is_strdup = true;

            // no size is provided - take it from the provided string
            if(ptr)
            {
                total = strlen(reinterpret_cast<const char*>(ptr));
            }
            break;

        case ESTRNDUP:
            is_strdup = true;
            break;

        default:
            break;
    }

    pthread_mutex_lock(&s_mutex);
    if(!total)
    {
        // No memory is to be allocated - this call is equivalent to
        // free(ptr) or realloc(pre, 0), C++ will result in a bad_alloc exception thrown
        // or in case of nothrow a NULL pointer is returned.

        if(!ptr)
        {
            // We are atempting to allocate 0 bytes - this is
            // valid but the memory returned cannot be used
            //  - this is a valid behaviour in C but C++ objects cannot have a size of 0

            switch(cmd)
            {
                case ENEW:
                case ENEW_ARRAY:
                    pthread_mutex_unlock(&s_mutex);
#if defined(MEMORYWRAPPERSTRICT) && defined(MEMWRAPPEREXCEPTIONS)
                    throw WrapperException(caller, func, "New or new [] with size 0 not allowed.");
#else // defined(MEMORYWRAPPERSTRICT) && defined(MEMWRAPPEREXCEPTIONS)
                    return 0;
#endif // defined(MEMORYWRAPPERSTRICT) && defined(MEMWRAPPEREXCEPTIONS)
                    break;

                case ENEW_NT:
                case ENEW_ARRAY_NT:
                    pthread_mutex_unlock(&s_mutex);
                    return 0;

                case EMALLOC:
                case ECALLOC:
                case EREALLOC:
                    pthread_mutex_unlock(&s_mutex);
                    return EMPTY_MEMORY_ALLOCATION_ADDRESS;

                default:
                    break;
            }
        }

        if(!is_strdup && ptr != EMPTY_MEMORY_ALLOCATION_ADDRESS)
        {
            TWrapperAllocMap::iterator it(s_alloc.find(ptr));
            if(it != s_alloc.end())
            {
#ifdef MEMORYWRAPPERDEBUG
                s_current -= it->second.Size();
#else // MEMORYWRAPPERDEBUG
                s_current -= it->second.first;
#endif // MEMORYWRAPPERDEBUG

                s_alloc.erase(it);
            }
            else if(ptr)
            {

#if defined(MEMORYWRAPPERSTRICT) && defined(MEMWRAPPEREXCEPTIONS)

                // throw the exception regardless - this is some memory that has been
                // allocated not by this wrapper.
                pthread_mutex_unlock(&s_mutex);
                throw WrapperException(caller, func, "Not allocated by _alloc(...)");

#else

                printf("%s: Error \"%s\" was used on %p but was not allocated by this module!\n",
                       MODULE, _allocfunc2string(cmd), ptr);

#endif // defined(MEMORYWRAPPERSTRICT) && defined(MEMWRAPPEREXCEPTIONS)

            }

            // delete 0 is valid but free(0) causes a crash
            //  - this matches existing behaviour.
            switch(cmd)
            {
                case EFREE:
                case EREALLOC:
                    MAKE_MANGLED_FUNC(__real,_free)(ptr);
                    break;

                case EDEL:
                    MAKE_MANGLED_FUNC(__real,_delete)(ptr);
                    break;

                case EDEL_NT:
                    MAKE_MANGLED_FUNC(__real,_deleteNoThrow)(ptr, std::nothrow);
                    break;

                case EDEL_ARRAY:
                    MAKE_MANGLED_FUNC(__real,_deleteArray)(ptr);
                    break;

                case EDEL_ARRAY_NT:
                    MAKE_MANGLED_FUNC(__real,_deleteArrayNoThrow)(ptr, std::nothrow);
                    break;

                default:
                    pthread_mutex_unlock(&s_mutex);
#ifdef MEMWRAPPEREXCEPTIONS
                    throw WrapperException(caller, func, "Internal error # 1");
#endif // MEMWRAPPEREXCEPTIONS
                    break;
            }
        }

        if(!total)
        {
            // we are finished here.
            pthread_mutex_unlock(&s_mutex);

#ifdef MEMORYWRAPPERPRINTSTAT
            printf("%s: Usage "
#ifdef BUILD64
                   "%lu/%lu"
#else
                   "%u/%u"
#endif
                   "\n",
                   __FUNCTION__, s_current, s_limit);
#endif // MEMORYWRAPPERPRINTSTAT

            _print_stat();
            return 0;
        }
    }

    void *p = EMPTY_MEMORY_ALLOCATION_ADDRESS;

    s_num_alloc++;

    // In case of free, delete and realloc them memory usage might go down therefore
    // the old size has to be considered when allocating new memory.
    TWrapperAllocMap::iterator existing(s_alloc.end());
    size_t old = 0;
    if(!is_strdup && ptr)
    {
        existing = s_alloc.find(ptr);
        if(existing != s_alloc.end())
        {
#ifdef MEMORYWRAPPERDEBUG
            old = existing->second.Size();
#else // MEMORYWRAPPERDEBUG
            old = existing->second.first;
#endif // MEMORYWRAPPERDEBUG
        }
    }

    // Only allow the critical and threshold limits to be active once they are
    // set.
    if(s_limit)
    {
        // This is the critical limit check
        if(s_current - old + total > s_limit)
        {
            // Let's see if we can call the last threshold function
            // to allow it to do some tidy-up.
            for( TWrapperCallbackMap::const_iterator it(s_callbacks.find(s_threshold));
                 it != s_callbacks.end();
                 ++it )
            {
                if(it->second.first)
                {
                    it->second.first(is_strdup ? 0 : ptr, total, it->second.second);

                    // Now we might have free'd the memory we are trying
                    // to resize
                    existing = s_alloc.find(is_strdup ? 0 : ptr);
                    if(old && existing == s_alloc.end())
                    {
                        old = 0;
                    }
                }
            }
        }

        // This is the critical limit check
        if(s_current - old + total > s_limit)
        {
            // Check if we would exceed the limit after callback
            //functions were called
            static time_t initial = 0;
            time_t now = time(0);

            // only allow 1 message per 3 seconds to be displayed
            if(abs((int)(initial - now)) > 3)
            {
                initial = now;

                printf("%s: Caller at address %p used \"%s\" is about to\n%s: exceed limit ("
#ifdef BUILD64
                       "%lu + %lu > %lu"
#else
                       "%u + %u > %u"
#endif
                       ")\n",
                       MODULE, caller, func ? func : __FUNCTION__, MODULE,
                       s_current, total - old, s_limit);
            }

            if(s_strict_limit)
            {
                // limit reached - no more available
                pthread_mutex_unlock(&s_mutex);

#ifdef MEMWRAPPEREXCEPTIONS
                if(s_throw_exception)
                {
                    throw WrapperException(caller, func, "Limit reached");
                }
#endif // MEMWRAPPEREXCEPTIONS

                return 0;
            }
            else
            {
                s_critical = true;
            }
        }

        // We can now check if the memory is still critical.
        // The critical limit is not longer an issue if
        //  1. The lowest allocation threshold is reached
        //  2. If no threshold was set 50% of the limit is reached.
        if(s_critical)
        {
            if(s_callbacks.empty())
            {
                if(s_current <= s_limit / 2)
                {
                    s_critical = false;
                }
            }
            else
            {
                if(s_current <= s_callbacks.begin()->first)
                {
                    s_critical = false;
                }
            }
        }

        // Check which Threshold function is to be called - we do not permanently
        // call a particular threshold again and again, only if the threshold changes.
        TWrapperCallbackMap::const_iterator last(s_callbacks.end());
        for( TWrapperCallbackMap::const_iterator it(s_callbacks.begin());
             it != s_callbacks.end();
             ++it )
        {
            if(s_current - old + total <= it->first)
            {
                break;
            }
            last = it;
        }

        // After all allocation routines are complete we can now process
        // the threshod checks and fire the callbacks
        if(last != s_callbacks.end() && last->first != s_threshold)
        {
            s_threshold = last->first;
            if(last->second.first)
            {
                last->second.first(is_strdup ? 0 : ptr, total, last->second.second);

                // Now we might have free'd the memory we are trying to resize
                existing = s_alloc.find(is_strdup ? 0 : ptr);
            }
        }
    }

    // Now we can allocate memory after all the checks ...
    if( !is_strdup && ptr && ptr != EMPTY_MEMORY_ALLOCATION_ADDRESS &&
        existing != s_alloc.end() )
    {
        size_t exist_size;

#ifdef MEMORYWRAPPERDEBUG
        exist_size = existing->second.Size();
#else // MEMORYWRAPPERDEBUG
        exist_size = existing->second.first;
#endif // MEMORYWRAPPERDEBUG

        if(exist_size == size)
        {
            // the new size is exactly the same as the old one - no work required.
            pthread_mutex_unlock(&s_mutex);
            return ptr;
        }
        s_current -= exist_size;
        s_alloc.erase(existing);

        p = MAKE_MANGLED_FUNC(__real,_realloc)(ptr, size);
    }
    else
    {
        if(nmemb == (size_t)-1)
        {
            switch(cmd)
            {
                case EMALLOC:
                case EREALLOC:
                    p = MAKE_MANGLED_FUNC(__real,_malloc)(size);
                    break;

                case ESTRDUP:
                    p = MAKE_MANGLED_FUNC(__real,_strdup)(reinterpret_cast<const char*>(ptr));
                    break;

                case ESTRNDUP:
                    p = MAKE_MANGLED_FUNC(__real,_strndup)(reinterpret_cast<const char*>(ptr), size);
                    break;

                case ENEW:
                    p = MAKE_MANGLED_FUNC(__real,_new)(size);
                    break;

                case ENEW_NT:
                    p = MAKE_MANGLED_FUNC(__real,_newNoThrow)(size, std::nothrow);
                    break;

                case ENEW_ARRAY:
                    p = MAKE_MANGLED_FUNC(__real,_newArray)(size);
                    break;

                case ENEW_ARRAY_NT:
                    p = MAKE_MANGLED_FUNC(__real,_newArrayNoThrow)(size, std::nothrow);
                    break;

                default:
                    pthread_mutex_unlock(&s_mutex);
#ifdef MEMWRAPPEREXCEPTIONS
                    throw WrapperException(caller, func, "Internal error # 2");
#endif // MEMWRAPPEREXCEPTIONS
                    break;
            }
        }
        else
        {
            p = MAKE_MANGLED_FUNC(__real,_calloc)(nmemb, size);
        }
    }

    // Did everything go well?
    if(p)
    {
        // record the thread who allocate memory and make sure it can be cancelled
        pthread_t thread = pthread_self();
        if(memcmp(&s_creator, &thread, sizeof(pthread_t)))
        {
            pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
            pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
        }

#ifdef MEMORYWRAPPERDEBUG
        s_alloc[p] = CWrapAllocAtom(caller, cmd, total, pthread_self());
#else // MEMORYWRAPPERDEBUG
        s_alloc[p] = std::make_pair<size_t, pthread_t>(total, pthread_self());
#endif // MEMORYWRAPPERDEBUG
        s_current += total;
    }
    else
    {
        switch(cmd)
        {
            case EMALLOC:
            case ECALLOC:
            case EREALLOC:
            case ENEW:
            case ENEW_ARRAY:
#ifdef MEMWRAPPEREXCEPTIONS
                if(s_throw_exception)
                {
                    pthread_mutex_unlock(&s_mutex);
                    throw WrapperException(caller, func, "Allocation failed");
                }
#endif // MEMWRAPPEREXCEPTIONS
                break;

            default:
                break;
        }
    }

    pthread_mutex_unlock(&s_mutex);

    switch(cmd)
    {
        case EMALLOC:
        case ECALLOC:
        case EREALLOC:
        case ENEW:
        case ENEW_NT:
        case ENEW_ARRAY:
        case ENEW_ARRAY_NT:
#ifdef MEMORYWRAPPERTRACE
            printf("%s: (%i)%s(%p, "
#ifdef BUILD64
                   "%lu, %lu"
#else // BUILD64
                   "%u, %u"
#endif // BUILD64
                   ") - "
#ifdef BUILD64
                   "%lu / %lu"
#else // BUILD64
                   "%u / %u"
#endif // BUILD64
                   "\n",
                   __FUNCTION__, cmd, func ? func : "UNKNOWN", p, size,
                   nmemb != (size_t)-1 ? nmemb : 0, s_current, s_limit);
#endif // MEMORYWRAPPERTRACE

            _print_stat();
            break;

        default:
            break;
    }

#ifdef MEMORYWRAPPERPRINTSTAT
    printf("%s: Usage "
#ifdef BUILD64
           "%lu/%lu"
#else
           "%u/%u"
#endif
           "\n",
           __FUNCTION__, s_current, s_limit);
#endif // MEMORYWRAPPERPRINTSTAT

    return p;
}

// ----------------------------------------------------------------------------

void* MAKE_MANGLED_FUNC(__wrap,_malloc)(size_t size)
{
    if(s_limit)
    {
    return _alloc(__builtin_return_address(0), __FUNCTION__, EMALLOC,
                  0, size);
}

    return MAKE_MANGLED_FUNC(__real,_malloc)(size);
}

void* MAKE_MANGLED_FUNC(__wrap,_calloc)(size_t nmemb, size_t size)
{
    if(s_limit)
    {
    return _alloc(__builtin_return_address(0), __FUNCTION__, ECALLOC,
                  0, size, nmemb);
}

    return MAKE_MANGLED_FUNC(__real,_calloc)(nmemb, size);
}

void* MAKE_MANGLED_FUNC(__wrap,_realloc)(void *ptr, size_t size)
{
    if(s_limit)
    {
    return _alloc(__builtin_return_address(0), __FUNCTION__, EREALLOC,
                  ptr, size);
}

    return MAKE_MANGLED_FUNC(__real,_realloc)(ptr, size);
}

char* MAKE_MANGLED_FUNC(__wrap,_strdup)(const char *str)
{
    if(s_limit)
    {
    return (char*)_alloc(__builtin_return_address(0), __FUNCTION__, ESTRDUP,
                         (void*)str);
}

    return MAKE_MANGLED_FUNC(__real,_strdup)(str);
}

char* MAKE_MANGLED_FUNC(__wrap,_strndup)(const char *str, size_t size)
{
    if(s_limit)
    {
    return (char*)_alloc(__builtin_return_address(0), __FUNCTION__, ESTRNDUP,
                         (void*)str, size);
}

    return MAKE_MANGLED_FUNC(__real,_strndup)(str, size);
}

void MAKE_MANGLED_FUNC(__wrap,_free)(void *ptr)
{
    if(s_limit)
    {
    _alloc(__builtin_return_address(0), __FUNCTION__, EFREE,
           ptr);
        return;
    }

    MAKE_MANGLED_FUNC(__real,_free)(ptr);
}

void* MAKE_MANGLED_FUNC(__wrap,_new)(size_t size)
{
    if(s_limit)
    {
    return _alloc(__builtin_return_address(0), __FUNCTION__, ENEW,
                  0, size);
}

    return MAKE_MANGLED_FUNC(__real,_new)(size);
}

void* MAKE_MANGLED_FUNC(__wrap,_newNoThrow)(size_t size, const std::nothrow_t& t)
{
    if(s_limit)
    {
    return _alloc(__builtin_return_address(0), __FUNCTION__, ENEW_NT,
                  0, size);
}

    return MAKE_MANGLED_FUNC(__real,_newNoThrow)(size, t);
}

void* MAKE_MANGLED_FUNC(__wrap,_newArray)(size_t size)
{
    if(s_limit)
    {
    return _alloc(__builtin_return_address(0), __FUNCTION__, ENEW_ARRAY,
                  0, size);
}

    return MAKE_MANGLED_FUNC(__real,_newArray)(size);
}

void* MAKE_MANGLED_FUNC(__wrap,_newArrayNoThrow)(size_t size, const std::nothrow_t& t)
{
    if(s_limit)
    {
    return _alloc(__builtin_return_address(0), __FUNCTION__, ENEW_ARRAY_NT,
                  0, size);
}

    return MAKE_MANGLED_FUNC(__real,_newArrayNoThrow)(size, t);
}

void MAKE_MANGLED_FUNC(__wrap,_delete)(void *ptr)
{
    if(s_limit)
    {
    _alloc(__builtin_return_address(0), __FUNCTION__, EDEL,
           ptr);
        return;
    }

    MAKE_MANGLED_FUNC(__real,_delete)(ptr);
}

void MAKE_MANGLED_FUNC(__wrap,_deleteNoThrow)(void *ptr, const std::nothrow_t& t)
{
    if(s_limit)
    {
    _alloc(__builtin_return_address(0), __FUNCTION__, EDEL_NT,
           ptr);
        return;
    }

    MAKE_MANGLED_FUNC(__real,_deleteNoThrow)(ptr, t);
}

void MAKE_MANGLED_FUNC(__wrap,_deleteArray)(void *ptr)
{
    if(s_limit)
    {
    _alloc(__builtin_return_address(0), __FUNCTION__, EDEL_ARRAY,
           ptr);
        return;
    }

    MAKE_MANGLED_FUNC(__real,_deleteArray)(ptr);
}

void MAKE_MANGLED_FUNC(__wrap,_deleteArrayNoThrow)(void *ptr, const std::nothrow_t& t)
{
    if(s_limit)
    {
    _alloc(__builtin_return_address(0), __FUNCTION__, EDEL_ARRAY_NT,
           ptr);
        return;
    }

    MAKE_MANGLED_FUNC(__real,_deleteArrayNoThrow)(ptr, t);
}

// ----------------------------------------------------------------------------


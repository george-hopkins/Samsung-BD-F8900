// -*- h -*- //////////////////////////////////////////////////////////////////
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
/// @file MemoryWrapper_local.h
/// @brief Implementation of the Memory Monitoring module.

#ifndef _MEMORYWRAPPER_LOCAL_H_
#define _MEMORYWRAPPER_LOCAL_H_

// ----------------------------------------------------------------------------

#include <string.h>
#include <string>
#include <map>
#include <vector>
#include <limits>

// ----------------------------------------------------------------------------

/// this memory address is used in case a memory allocation with size 0 is
/// attempted; we do return a non-NULL value but the usage of this pointer will
/// fail.
#define EMPTY_MEMORY_ALLOCATION_ADDRESS  (reinterpret_cast<void*>((size_t)0xDEADBEEF))

// ----------------------------------------------------------------------------

// __builtin_return_address(0) is only available on GCC compilers
#ifndef __GNUC__
/// on non-gcc compilers this function does not exist
#define __builtin_return_address(x) (0)
#endif

// ----------------------------------------------------------------------------

// recursive mutexes on non GNUC compilers are initialised slightly different
#if !defined(PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP)
/// on some systems the pthreads are initialised slightly different
#define PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP PTHREAD_RECURSIVE_MUTEX_INITIALIZER
#endif

// ----------------------------------------------------------------------------

/// These 'real' functions are called by the wrapper when using the linker switches
/// for 32 bit architectures:
///   -Wl,--wrap,malloc -Wl,--wrap,free -Wl,--wrap,calloc -Wl,--wrap,realloc
///   -Wl,--wrap,strdup -Wl,--wrap,strndup -Wl,--wrap,strdupa -Wl,--wrap,strndupa
///   -Wl,--wrap,_Znwj -Wl,--wrap,_Znaj -Wl,--wrap,_ZdlPv -Wl,--wrap,_ZdaPv
///   -Wl,--wrap,_ZnwjRKSt9nothrow_t -Wl,--wrap,_ZnajRKSt9nothrow_t
///   -Wl,--wrap,_ZdlPvRKSt9nothrow_t -Wl,--wrap,_ZdaPvRKSt9nothrow_t
///
/// for 64 bit architectures:
///   -Wl,--wrap,malloc -Wl,--wrap,free -Wl,--wrap,calloc -Wl,--wrap,realloc
///   -Wl,--wrap,strdup -Wl,--wrap,strndup -Wl,--wrap,strdupa -Wl,--wrap,strndupa
///   -Wl,--wrap,_Znwm -Wl,--wrap,_Znam -Wl,--wrap,_ZdlPv -Wl,--wrap,_ZdaPv
///   -Wl,--wrap,_ZnwmRKSt9nothrow_t -Wl,--wrap,_ZnamRKSt9nothrow_t
///   -Wl,--wrap,_ZdlPvRKSt9nothrow_t -Wl,--wrap,_ZdaPvRKSt9nothrow_t
/// @{

#ifdef __cplusplus
extern "C"
{
#endif

// The 2 macros below assemble the function call for memory allocations
// correct for 32 and 64 bit architectures. Please note that the assembly
// of the function names MUST be in 2 steps otherwise the preprocessor
// will NOT assembe the name correctly.
#define MAKE_MANGLED_FUNC_EX(a,b)   a ## b
#define MAKE_MANGLED_FUNC(a, b)     MAKE_MANGLED_FUNC_EX(a, b)

#if defined(__GNUC__)

#ifdef BUILD64

#define _new             __Znwm
#define _newArray        __Znam
#define _newNoThrow      __ZnwmRKSt9nothrow_t
#define _newArrayNoThrow __ZnamRKSt9nothrow_t

#else // #ifdef BUILD64

#define _new             __Znwj
#define _newArray        __Znaj
#define _newNoThrow      __ZnwjRKSt9nothrow_t
#define _newArrayNoThrow __ZnajRKSt9nothrow_t

#endif // #ifdef BUILD64

#define _delete             __ZdlPv
#define _deleteArray        __ZdaPv
#define _deleteNoThrow      __ZdlPvRKSt9nothrow_t
#define _deleteArrayNoThrow __ZdaPvRKSt9nothrow_t

// malloc
void* MAKE_MANGLED_FUNC(__real,_malloc)(size_t size);
void* MAKE_MANGLED_FUNC(__wrap,_malloc)(size_t size);

// calloc
void* MAKE_MANGLED_FUNC(__real,_calloc)(size_t n, size_t size);
void* MAKE_MANGLED_FUNC(__wrap,_calloc)(size_t n, size_t size);

// realloc
void* MAKE_MANGLED_FUNC(__real,_realloc)(void *ptr, size_t size);
void* MAKE_MANGLED_FUNC(__wrap,_realloc)(void *ptr, size_t size);

// strdup
char* MAKE_MANGLED_FUNC(__real,_strdup)(const char *s);
char* MAKE_MANGLED_FUNC(__wrap,_strdup)(const char *s);

// strndup
char* MAKE_MANGLED_FUNC(__real,_strndup)(const char *s, size_t n);
char* MAKE_MANGLED_FUNC(__wrap,_strndup)(const char *s, size_t n);

// free
void  MAKE_MANGLED_FUNC(__real,_free)(void* ptr);
void  MAKE_MANGLED_FUNC(__wrap,_free)(void* ptr);

// new(size)
void* MAKE_MANGLED_FUNC(__real,_new)(size_t size);
void* MAKE_MANGLED_FUNC(__wrap,_new)(size_t size);

// new [] (size)
void* MAKE_MANGLED_FUNC(__real,_newArray)(size_t size);
void* MAKE_MANGLED_FUNC(__wrap,_newArray)(size_t size);

// new nothrow (size)
void* MAKE_MANGLED_FUNC(__real,_newNoThrow)(size_t size, const std::nothrow_t&);
void* MAKE_MANGLED_FUNC(__wrap,_newNoThrow)(size_t size, const std::nothrow_t&);

// new nothrow [] (size)
void* MAKE_MANGLED_FUNC(__real,_newArrayNoThrow)(size_t size, const std::nothrow_t&);
void* MAKE_MANGLED_FUNC(__wrap,_newArrayNoThrow)(size_t size, const std::nothrow_t&);

// delete p
void  MAKE_MANGLED_FUNC(__real,_delete)(void *ptr);
void  MAKE_MANGLED_FUNC(__wrap,_delete)(void *ptr);

// delete [] p;
void  MAKE_MANGLED_FUNC(__real,_deleteArray)(void *ptr);
void  MAKE_MANGLED_FUNC(__wrap,_deleteArray)(void *ptr);

// delete nothrow p;
void  MAKE_MANGLED_FUNC(__real,_deleteNoThrow)(void *ptr, const std::nothrow_t&);
void  MAKE_MANGLED_FUNC(__wrap,_deleteNoThrow)(void *ptr, const std::nothrow_t&);

// delete nothrow [] p;
void  MAKE_MANGLED_FUNC(__real,_deleteArrayNoThrow)(void *ptr, const std::nothrow_t&);
void  MAKE_MANGLED_FUNC(__wrap,_deleteArrayNoThrow)(void *ptr, const std::nothrow_t&);

#elif defined(WIN32)

// Use Windows specific memory alloc routines
#include <crtdbg.h>

#endif

#ifdef __cplusplus
}
#endif

/// @}

// ----------------------------------------------------------------------------

/// The Wrapper Allocator template helps to bypass the memory monitoring as some
/// routes will fail in case we are about to hit the critical limit, the monitored
/// memory allocation succeeds but the storing the meta data will fail...

template <class T> class TWrapAlloc;

/// specialize for void:
template <> class TWrapAlloc<void>
{
public:
    /// the void* pointer
    typedef void*       pointer;
    /// the const void* pointer
    typedef const void* const_pointer;

    /// reference to void members are impossible.
    typedef void value_type;

    /// rebind the member to another
    template <class U> struct rebind
    {
        /// the other type
        typedef TWrapAlloc<U> other;
    };
};

/// specialise for the user type
template <class T> class TWrapAlloc
{
public:
    /// the size of the type itself
    typedef size_t    size_type;

    /// the difference of 2 pointer from each other
    typedef ptrdiff_t difference_type;

    /// This is the type
    typedef T*        pointer;

    /// The const type
    typedef const T*  const_pointer;

    /// The reference
    typedef T&        reference;

    /// The const reference
    typedef const T&  const_reference;

    /// This is the value
    typedef T         value_type;

    /// rebind the member to another
    template <class U> struct rebind
    {
        /// the other type
        typedef TWrapAlloc<U> other;
    };

    /// The constructor
    TWrapAlloc() throw() {}

    /// The copy constructor
    TWrapAlloc(const TWrapAlloc&) throw() {}

    /// Required for template but not sure what that is
    template <class U> TWrapAlloc(const TWrapAlloc<U>&) throw() {}

    /// The destructor
    ~TWrapAlloc() throw() {}

    /// Returns the address
    ///
    /// @param [in] x the reference into this class
    /// @return the address of the object
    pointer address(reference x) const
    {
        return &x;
    }

    /// Returns the address
    ///
    /// @param [in] x the reference into this class
    /// @return the address of the object
    const_pointer address(const_reference x) const
    {
        return &x;
    }

    /// Allocate the memory
    ///
    /// @param [in] n Size in bytes
    /// @param [in] hint data for copy constructor
    /// @return the address of the new object
    pointer allocate(size_type n, TWrapAlloc<void>::const_pointer hint = 0)
    {
        (void)hint;
        return reinterpret_cast<pointer>(MAKE_MANGLED_FUNC(__real,_new)(n * sizeof(T)));
    }

    /// Free the memory
    ///
    /// @param [in] p  The pointer to the allocated memory
    /// @param [in] n  Size of the memory block
    void deallocate(pointer p, size_type n)
    {
        (void)n;
        MAKE_MANGLED_FUNC(__real,_delete)(p);
    }

    /// Returns the number of objects this class can hold
    ///
    /// @return the number of objects this class can hold
    size_type max_size() const throw()
    {
        return std::numeric_limits<size_type>::max() / sizeof(T);
    }

    /// The constructor
    ///
    /// @param [in] p    the pointer to the constructor that is to be called
    /// @param [in] val  the value that shall be passed t the copy constructor
    void construct(pointer p, const T& val)
    {
        new(p) T(val);
    }

    /// The desctructor
    ///
    /// @param [in] p  the pointer to the object on which the destructor shall be called
    void destroy(pointer p)
    {
#ifdef WIN32
        // prevent warning C4100: 'p' : unreferenced formal parameter
        (void)p;
#endif

        p->~T();
    }
};

/// This template allows to compare this type with the type of another class
template <class T, class U>
bool operator==(const TWrapAlloc<T>&, const TWrapAlloc<U>&) throw()
{
    return true;
}

/// This template allows to compare this type with the type of another class
template <class T, class U>
bool operator!=(const TWrapAlloc<T>&, const TWrapAlloc<U>&) throw()
{
    return false;
}

// ----------------------------------------------------------------------------

/// This string type bypasses the memory monitoring routines provided by this module.
typedef  std::basic_string<char, std::char_traits<char>, TWrapAlloc<char> > TMWString;

/// The Wrapper Exception internal data - hidden away from the external interface.
class WrapperExceptionInternals
{
public:
    /// Default constructor
    WrapperExceptionInternals(void);

    /// Specific Constructor
    ///
    /// @param [in] caller  The caller address who called the wrapper function
    /// @param [in] func    The wrapper function that was called
    /// @param [in] info    The textual information in regards of this exception
    /// @param [in] thread  The thread id where the exception was caused.
    WrapperExceptionInternals(const void *caller, const char *func,
                              const char *info,  pthread_t thread);

    /// Copy constructor
    ///
    /// @param [in] rhs  The details that are to be copied
    WrapperExceptionInternals(const WrapperExceptionInternals &rhs);

    /// Assignment operator overload (=)
    ///
    /// @param [in] rhs  The details that are to be copied
    ///
    /// @return A handle to the class itself
    const WrapperExceptionInternals& operator=(const WrapperExceptionInternals &rhs);

    /// Operator new overload
    ///
    /// @param [in] size  The size of the new object that is to be allocated.
    ///
    /// @return The memory address which was allocated.
    void* operator new(size_t size);

    /// Operator delete overload
    ///
    /// @param [in] ptr  The object that is to be deleted
    void operator delete(void *ptr);

    /// Query the address who called the wrapper function
    ///
    /// @return The address who called the wrapper function
    const void* Caller(void) const;

    /// Query the wrapper function that was called
    ///
    /// @return The wrapper function that was called
    const char* Func(void) const;

    /// Query the textual information in regards of this exception
    ///
    /// @return the textual information in regards of this exception
    const char* Info(void) const;

    /// Query the thread id who caused this exception
    ///
    /// @return The thread id who caused this exception
    pthread_t ThreadId(void) const;

private:

    /// The caller address who called the wrapper function
    const void *m_caller;

    /// The wrapper function that was called
    TMWString m_func;

    /// The textual information in regards of this exception
    TMWString m_info;

    /// Thread id where the exception was caused.
    pthread_t m_thread;
};

// ----------------------------------------------------------------------------

/// This class provides pthread_t to be stored in set or map
class WrapperLessPthread
{
public:
    /// comparison function that enables set ot map STL classes
    /// store an item
    ///
    /// @param [in] lhs  the Left-Hand-Side that is to be compared
    /// @param [in] rhs  the Right-Hand-Side side that is to be compared
    ///
    /// @return true if lhs < rhs, false otherwise
    bool operator()(const pthread_t &lhs, const pthread_t &rhs) const
    {
        return memcmp(&lhs, &rhs, sizeof(pthread_t)) < 0;
    }
};

// ----------------------------------------------------------------------------

/// Commands that the _alloc wrapper supports
typedef enum EWrapCmd
{
    /// no 'proper' allocation type, used for initialisation only
    EINIT = -1,

    /// malloc(size)
    EMALLOC,

    /// calloc(num ,size)
    ECALLOC,

    /// realloc(ptr, size)
    EREALLOC,

    /// strdup(str)
    ESTRDUP,

    /// strndup(str, size)
    ESTRNDUP,

    /// free(ptr)
    EFREE,

    /// new X()
    ENEW,

    /// new nothrow X()
    ENEW_NT,

    /// new X[](num)
    ENEW_ARRAY,

    /// new nothrow X[](num)
    ENEW_ARRAY_NT,

    /// delete X
    EDEL,

    /// delete nothrow X
    EDEL_NT,

    /// delete [] X
    EDEL_ARRAY,

    /// delete nothrow [] X
    EDEL_ARRAY_NT
} EWrapCmd;

// ----------------------------------------------------------------------------

#ifndef MEMORYWRAPPERDEBUG

/// The wrapper allocation map
typedef std::map<void*, std::pair<size_t, pthread_t>, std::less<void*>,
        TWrapAlloc<std::pair<void*, std::pair<size_t, pthread_t> > > > TWrapperAllocMap;

#else // MEMORYWRAPPERDEBUG

/// Memory allocation entity record
class CWrapAllocAtom
{
public:
    /// Constructor
    CWrapAllocAtom(void)
        : m_caller(0), m_func(EINIT), m_size(0)
    { memset(&m_thread, 0, sizeof(pthread_t)); }

    /// Specific constructor
    ///
    /// @param [in] caller  the caller of the allocation
    /// @param [in] func    the used function
    /// @param [in] size    the size of the allocation
    /// @param [in] thread  the thread that called the allocation
    CWrapAllocAtom(const void *caller, EWrapCmd func, size_t size, pthread_t thread)
        : m_caller(caller), m_func(func), m_size(size), m_thread(thread)
    {}

    /// Query the Caller
    ///
    /// @return the Caller
    const void* Caller() const
    { return m_caller; }

    /// Query the used Function
    ///
    /// @return the used Function
    EWrapCmd Func() const
    { return m_func; }

    /// Query the allocated size
    ///
    /// @return the allocated size
    size_t Size() const
    { return m_size; }

    /// Query the thread that called the allocation
    ///
    /// @return the thread that called the allocation
    pthread_t Thread() const
    { return m_thread; }

private:
    /// the caller
    const void *m_caller;

    /// the function
    EWrapCmd m_func;

    /// the size
    size_t m_size;

    /// the thread
    pthread_t m_thread;
};

/// Memory allocation entity caller statistic
class CWrapAllocStatCaller
{
public:
    /// Constructor
    CWrapAllocStatCaller(void)
        : m_func(EINIT), m_size(0), m_count(0)
    {}

    /// Copy constructor
    ///
    /// @param [in] rhs  The details that are to be copied
    CWrapAllocStatCaller(const CWrapAllocStatCaller &rhs)
        : m_func(rhs.m_func), m_size(rhs.m_size), m_count(rhs.m_count)
    {}

    /// Specific Constructor
    ///
    /// @param [in] item  An allocation item
    CWrapAllocStatCaller(const CWrapAllocAtom& item)
        : m_func(item.Func()), m_size(item.Size()), m_count(1)
    {}

    /// Extens the size of the allocation
    ///
    /// @param [in] item  An allocation item that is extending the allocation
    ///
    /// @return new size;
    size_t Add(const CWrapAllocAtom& item)
    {
        if(item.Size())
        {
            m_count++;
        }
        return m_size += item.Size();
    }

    /// Query the used Function
    ///
    /// @return the used Function
    EWrapCmd Func() const
    { return m_func; }

    /// Query the allocated size
    ///
    /// @return the allocated size
    size_t Size() const
    { return m_size; }

    /// Query the count for this allocation
    ///
    /// @return the count for this allocation
    size_t Count() const
    { return m_count; }

private:
    /// the function
    EWrapCmd m_func;

    /// the size
    size_t m_size;

    /// the count
    size_t m_count;
};

/// Memory allocation entity size statistic
class CWrapAllocStatSize
{
public:
    /// Constructor
    CWrapAllocStatSize(void)
        : m_caller(0), m_func(EINIT), m_count(0)
    {}

    /// Copy constructor
    ///
    /// @param [in] rhs     The details that are to be copied
    /// @param [in] caller  updated caller if set
    CWrapAllocStatSize(const CWrapAllocStatSize &rhs, const void *caller = 0)
        : m_caller(caller ? caller : rhs.m_caller), m_func(rhs.m_func), m_count(rhs.m_count)
    {}

    /// Specific Constructor
    ///
    /// @param [in] item  An allocation item
    CWrapAllocStatSize(const CWrapAllocStatCaller &item)
        : m_func(item.Func()), m_count(item.Count())
    {}

    /// Query the Caller
    ///
    /// @return the Caller
    const void* Caller() const
    { return m_caller; }

    /// Query the used Function
    ///
    /// @return the used Function
    EWrapCmd Func() const
    { return m_func; }

    /// Query the count for this allocation
    ///
    /// @return the count for this allocation
    size_t Count() const
    { return m_count; }

private:
    /// the caller
    const void *m_caller;

    /// the function
    EWrapCmd m_func;

    /// the count
    size_t m_count;
};

/// The wrapper allocation map
typedef std::map<void*, CWrapAllocAtom, std::less<void*>,
        TWrapAlloc<std::pair<void*, CWrapAllocAtom> > > TWrapperAllocMap;

/// Memory allocation key (caller and its size)
class CWrapperAllocKey
{
public:
    /// Constructor
    CWrapperAllocKey(void)
        : m_caller(0), m_size(0)
    {}

    /// Specific Constructor
    ///
    /// @param [in] caller  the caller of the allocation
    /// @param [in] size    the size of the allocation
    CWrapperAllocKey(const void *caller, size_t size)
        : m_caller(caller), m_size(size)
    {}

    /// Query the Caller
    ///
    /// @return the Caller
    const void* Caller() const
    { return m_caller; }

    /// Query the allocated
    ///
    /// @return the allocated
    size_t Size() const
    { return m_size; }

    /// Comparison operator
    ///
    /// @param [in] rhs  Right Hand Side
    ///
    /// @return true if lhs is smaller than the rhs, false otherwise.
    bool operator<(const CWrapperAllocKey &rhs) const
    {
        if(m_caller != rhs.m_caller)
        {
            return m_caller < rhs.m_caller;
        }

        if(m_size != rhs.m_size)
        {
            return m_size < rhs.m_size;
        }

        return false;
    }

private:
    /// The caller
    const void *m_caller;

    /// The  of the allocation
    size_t m_size;
};

/// The list of memory allocations and their total memory per caller
typedef std::map<CWrapperAllocKey, CWrapAllocStatCaller, std::less<CWrapperAllocKey>,
        TWrapAlloc<std::pair<CWrapperAllocKey, CWrapAllocStatCaller> > > TWrapGroupCaller;

/// Memory allocation data for statistics
class CWrapperSizeData
{
public:
    /// Constructor
    CWrapperSizeData(void)
        : m_caller(0), m_func(EINIT), m_size(0), m_count(0)
    {}

    /// Specific Constructor
    ///
    /// @param [in] caller  the caller of the allocation
    /// @param [in] func    the function used to allocate
    /// @param [in] total   The size and times calles (for speed improvement)
    /// @param [in] size    the size of the allocation
    /// @param [in] count   the number of times the allocation by the same caller with
    ///                     the same size was performed
    CWrapperSizeData(const void *caller, EWrapCmd func, size_t total, size_t size, size_t count)
        : m_caller(caller), m_func(func), m_total(total), m_size(size), m_count(count)
    {}

    /// Query the Caller
    ///
    /// @return the Caller
    const void* Caller() const
    { return m_caller; }

    /// Query the used Function
    ///
    /// @return the used Function
    EWrapCmd Func() const
    { return m_func; }

    /// Query the total allocated size (size x count)
    ///
    /// @return the total allocated size (size x count)
    size_t Total() const
    { return m_total; }

    /// Query the allocated
    ///
    /// @return the allocated
    size_t Size() const
    { return m_size; }

    /// Query the count for this allocation
    ///
    /// @return the count for this allocation
    size_t Count() const
    { return m_count; }

    /// Comparison operator
    ///
    /// @param [in] rhs  Right Hand Side
    ///
    /// @return true if lhs is smaller than the rhs, false otherwise.
    bool operator<(const CWrapperSizeData &rhs) const
    {
        if(m_total != rhs.m_total)
        {
            // reverse order
            return m_total > rhs.m_total;
        }
        if(m_count != rhs.m_count)
        {
            // reverse order
            return m_count > rhs.m_count;
        }
        if(m_size != rhs.m_size)
        {
            // reverse order
            return m_size > rhs.m_size;
        }
        if(m_caller != rhs.m_caller)
        {
            return m_caller < rhs.m_caller;
        }
        if(m_func != rhs.m_func)
        {
            return m_func < rhs.m_func;
        }

        return false;
    }

private:
    /// The caller
    const void *m_caller;

    /// The function used to allocate
    EWrapCmd m_func;

    /// The size and times allocates size in total
    size_t m_total;

    /// The size of the allocation
    size_t m_size;

    /// The number of times the allocation by the same caller with
    /// the same size was performed
    size_t m_count;
};

/// The list of memory allocations and their total memory per caller
typedef std::set<CWrapperSizeData, std::less<CWrapperSizeData>,
        TWrapAlloc<CWrapperSizeData> > TWrapGroupSize;

#endif // MEMORYWRAPPERDEBUG

// ----------------------------------------------------------------------------

/// a list of callback functions
typedef std::pair<WrapperCallbackFunc, void*> WrapperCallback;

/// The reapper callback map
typedef std::map<size_t, WrapperCallback, std::less<size_t>,
        TWrapAlloc<std::pair<size_t, WrapperCallback> > > TWrapperCallbackMap;

#ifndef MEMORYWRAPPERSTATIC

/// A list of addresses
typedef std::vector<const void*, TWrapAlloc<const void*> > TWrapSharedLibAddr;

/// A list of memory functions that map into shared libraries and from where
/// are they called.
typedef std::map<TMWString, TWrapSharedLibAddr, std::less<TMWString>,
        TWrapAlloc<std::pair<TMWString, TWrapSharedLibAddr> > > TWrapSharedLibs;

#else // MEMORYWRAPPERSTATIC

/// A List of caller addresses
typedef std::map<const void*, size_t, std::less<const void*>,
        TWrapAlloc<std::pair<const void*, size_t> > > TWrapCallers;

/// An inverse list of callers to get their order
typedef std::map<size_t, const void*, std::less<size_t>,
        TWrapAlloc<std::pair<size_t, const void*> > > TWrapCallerOrder;

#endif // MEMORYWRAPPERSTATIC

// ----------------------------------------------------------------------------

/// Wrapper function for the "malloc" call.
///
/// @param [in] size  Number of bytes that are to be allocated.
///
/// @return pointer to the memory allocated on success, NULL otherwise.
void* MAKE_MANGLED_FUNC(__wrap,_malloc)(size_t size);

/// Wrapper function for the "calloc" call.
///
/// @param [in] nmemb  Number of blocks that are to be allocated.
/// @param [in] size   Number of bytes that are to be allocated.
///
/// @return pointer to the memory allocated on success, NULL otherwise.
void* MAKE_MANGLED_FUNC(__wrap,_calloc)(size_t nmemb, size_t size);

/// Wrapper function for the "realloc" call.
///
/// @param [in] ptr   Old memory allocation that is to be changed, on success this
///                   memory block can be considered free'd.
/// @param [in] size  Number of bytes that are to be allocated.
///
/// @return pointer to the memory allocated on success, NULL otherwise.
void* MAKE_MANGLED_FUNC(__wrap,_realloc)(void *ptr, size_t size);

/// Wrapper function for the "free" call.
///
/// @param [in] ptr   The memory allocated that is to be free'd.
void MAKE_MANGLED_FUNC(__wrap,_free)(void *ptr);

/// Wrapper function for the C++ "new" call.
///
/// @param [in] size  Number of bytes that are to be allocated.
///
/// @return pointer to the memory allocated on success, NULL otherwise.
void* MAKE_MANGLED_FUNC(__wrap,_new)(size_t size);

/// Wrapper function for the C++ "new" call.
///
/// @param [in] size  Number of bytes that are to be allocated.
/// @param [in] t     Exception structure that is updated in case of an exception
///
/// @return pointer to the memory allocated on success, NULL otherwise.
void* MAKE_MANGLED_FUNC(__wrap,_newNoThrow)(size_t size, const std::nothrow_t& t);

/// Wrapper function for the C++ "new[]" call.
///
/// @param [in] size  Number of bytes that are to be allocated.
///
/// @return pointer to the memory allocated on success, NULL otherwise.
void* MAKE_MANGLED_FUNC(__wrap,_newArray)(size_t size);

/// Wrapper function for the C++ "new[]" call.
///
/// @param [in] size  Number of bytes that are to be allocated.
/// @param [in] t     Exception structure that is updated in case of an exception
///
/// @return pointer to the memory allocated on success, NULL otherwise.
void* MAKE_MANGLED_FUNC(__wrap,_newArrayNoThrow)(size_t size, const std::nothrow_t& t);

/// Wrapper function for the C++ "delete" call.
///
/// @param [in] ptr   The memory allocated that is to be free'd.
void MAKE_MANGLED_FUNC(__wrap,_delete)(void *ptr);

/// Wrapper function for the C++ "delete" call.
///
/// @param [in] ptr   The memory allocated that is to be free'd.
/// @param [in] t     Exception structure that is updated in case of an exception
void MAKE_MANGLED_FUNC(__wrap,_deleteNoThrow)(void *ptr, const std::nothrow_t& t);

/// Wrapper function for the C++ "delete []" call.
///
/// @param [in] ptr   The memory allocated that is to be free'd.
void MAKE_MANGLED_FUNC(__wrap,_deleteArray)(void *ptr);

/// Wrapper function for the C++ "delete []" call.
///
/// @param [in] ptr   The memory allocated that is to be free'd.
/// @param [in] t     Exception structure that is updated in case of an exception
void MAKE_MANGLED_FUNC(__wrap,_deleteArrayNoThrow)(void *ptr, const std::nothrow_t& t);

// ----------------------------------------------------------------------------

#endif /* _MEMORYWRAPPER_LOCAL_H_ */

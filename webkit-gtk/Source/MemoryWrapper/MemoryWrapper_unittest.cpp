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
/// @file MemoryWrapper_unittest.cpp
/// @brief Implementation of the Memory Monitoring module.

// ---------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <vector>
#include <string>

#include "MemoryWrapper.h"

// ----------------------------------------------------------------------------

/// an invalid memory address
#define INVALID_ADDRESS (reinterpret_cast<void*>((size_t)0xDEADDEAD))

#ifdef WIN32

#include <windows.h>

/// Remap "windows's sleep() funtion to be like the posix version"
#define sleep(t) Sleep((t) * 1000)

#endif

// ----------------------------------------------------------------------------

/// simple macro to return the max of 2 valies
#define MAX(a,b)    ((a) > (b) ? (a) : (b))

// ----------------------------------------------------------------------------

/// A simple class to check if the constructor and destructor is correctly called
/// once the Memory Wrapper is compiled in.
class CTest
{
public:
    /// Constructor
    CTest(void)
    { printf("%s\n", __FUNCTION__); }

    /// Destructor
    ~CTest(void)
    { printf("%s\n", __FUNCTION__); }

    /// Get the content of the internal member variable
    /// @return the content of the internal member variable
    const char* Get(void) const
    { return m_str.c_str(); }

    /// Set the content of the internal member variable
    /// @param [in] str the new content to be set
    void Set(const char *str)
    { m_str = str; }

private:
    /// The member variable
    std::string m_str;
};

// ----------------------------------------------------------------------------

/// A Callback function that is to be called if 25% of the set memory
/// is allocated
/// @param [in]     ptr   The pointer of the "old" variable in case its size is to be
///                       changed - this allows to spare this one.
/// @param [in]     size  New size of the allocation
/// @param [in,out] data  User data
static void limit25(const void *ptr, size_t size, void *data)
{
    (void)ptr;
    (void)size;
    (void)data;
}

/// A Callback function that is to be called if 50% of the set memory
/// is allocated
/// @param [in]     ptr   The pointer of the "old" variable in case its size is to be
///                       changed - this allows to spare this one.
/// @param [in]     size  New size of the allocation
/// @param [in,out] data  User data
static void limit50(const void *ptr, size_t size, void *data)
{
    (void)ptr;
    (void)size;
    (void)data;
}

/// A Callback function that is to be called if 75% of the set memory
/// is allocated
/// @param [in]     ptr   The pointer of the "old" variable in case its size is to be
///                       changed - this allows to spare this one.
/// @param [in]     size  New size of the allocation
/// @param [in]     mode  This allow memory-deallocations to match the prior
///                       allocation.
/// @param [in,out] data  User data
static void limit75(const void *ptr, size_t size, int mode, void *data)
{
    if(!data)
    {
        return;
    }

    // free 25% of the memory allocated starting from the earlies
    // memory allocations.
    size_t current = WrapperGetAlloc();
    size_t gain    = MAX(size, current / 4);
    size_t target  = current > gain ? current - gain : 0;

    std::vector<void*> *mem = reinterpret_cast<std::vector<void*>*>(data);
    while( mem && mem->size() &&
           WrapperGetAlloc() > target )
    {
        std::vector<void*>::iterator it(mem->begin());
        if(it != mem->end())
        {
            if( ptr == *it ||
                mem->size() == 1)
            {
                // preserve this memory block.
                break;
            }

            switch(mode)
            {
                case 0:
                    free(*it);
                    break;

                case 1:
                case 2:
                    delete [] (short*)(*it);
                    break;

                default:
                    break;
            }
            mem->erase(it);
        }
    }
}

/// A Callback function that is to be called if 75% of the set memory
/// is allocated - for pure c
/// @param [in]     ptr   The pointer of the "old" variable in case its size is to be
///                       changed - this allows to spare this one.
/// @param [in]     size  New size of the allocation
/// @param [in,out] data  User data
static void limit75_C(const void *ptr, size_t size, void *data)
{
    limit75(ptr, size, 0, data);
}

/// A Callback function that is to be called if 75% of the set memory
/// is allocated - for c++ (new)
/// @param [in]     ptr   The pointer of the "old" variable in case its size is to be
///                       changed - this allows to spare this one.
/// @param [in]     size  New size of the allocation
/// @param [in,out] data  User data
static void limit75_CPP(const void *ptr, size_t size, void *data)
{
    limit75(ptr, size, 1, data);
}

/// A Callback function that is to be called if 75% of the set memory
/// is allocated - for c++ (new nothrow)
/// @param [in]     ptr   The pointer of the "old" variable in case its size is to be
///                       changed - this allows to spare this one.
/// @param [in]     size  New size of the allocation
/// @param [in,out] data  User data
static void limit75_CPP_NT(const void *ptr, size_t size, void *data)
{
    limit75(ptr, size, 2, data);
}

#ifdef MEMWRAPPEREXCEPTIONS
/// This function checks some basic memorty allocations and once it is finished it
/// throws an exception
static void throwAnException(void)
{
    void *ptr = 0;
    int test  = 0;
    while(1)
    {
        if(test < 0)
        {
            break;
        }

        switch(test)
        {
            case 0:
                assert(ptr == 0);
                ptr = malloc(0);
                break;

            case 2:
                assert(ptr == 0);
                ptr = calloc(0, 0);
                break;

            case 4:
                assert(ptr == 0);
                ptr = realloc(0, 0);
                break;

            case 1:
            case 3:
            case 5:
                printf("(%i) ptr was %p - now free'd.\n", test, ptr);
                //assert(ptr != 0);
                free(ptr);
                ptr = 0;
                break;

            default:
                test = -1;
                continue;
        }

        test++;
    }

    throw WrapperException(INVALID_ADDRESS, __FUNCTION__,
                           "INITIAL TEST COMPLETE");
}
#endif // MEMWRAPPEREXCEPTIONS

/// Test thread that simply prints the time every 10 seconds
/// @return NULL
static void* testThread(void*)
{
    // this thread cannot be cancelled.
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

    time_t *t = (time_t*)malloc(sizeof(time_t));
    for(;;)
    {
        if(!t)
        {
            break;
        }
        *t = time(0);

        printf("%s: %s\n", __FUNCTION__, ctime(t));

        // create cancel point
        for(int i=0 ; i<5 ; i++)
        {
            pthread_testcancel();
            sleep(2);
        }
    }
    free(t);

    return 0;
}


/// The main function
/// @param [in] argc  Number of arguments provided from the command line
/// @param [in] argv  An array of arguments, 0 is always the program name.
/// @return always 0
int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    // configure MemoryWrapper
    WrapperSetThrowException(true);
    WrapperSetStrict(true);

    for(int i=0 ; i<2 ; i++)
    {
        CTest c;
        c.Set(__FUNCTION__);
        printf("CTest is now set to \"%s\"\n", c.Get());
    }

#ifdef MEMWRAPPEREXCEPTIONS
    try
    {
        throwAnException();
    }
    catch(WrapperException &e)
    {
        printf("-----< %s >-----\n  %s\n\n", __FUNCTION__, e.what());
    }
    catch(std::exception &e)
    {
        printf("-----< %s >-----\n  %s\n\n", __FUNCTION__, e.what());
    }
    printf("Exception test finished.\n\n");
#endif // MEMWRAPPEREXCEPTIONS

    size_t limit = 50 * 1024 * 1024;
    WrapperSetLimit(limit);
    WrapperSetStrict(true);

    // check simple memory allocation sequence
    free(realloc(malloc(limit), limit));

    int test = 0;
    while(1)
    {
        if(test < 0)
        {
            printf("-----< %s >-----\n  Test finished\n", __FUNCTION__);
            break;
        }

        size_t size = 0;
        size_t inc  = 0;

        pthread_t child;
        pthread_create(&child, 0, testThread, 0);

#ifdef MEMWRAPPEREXCEPTIONS
        try
#endif // MEMWRAPPEREXCEPTIONS
        {
            std::vector<void*> mem;
            switch(test)
            {
                case 0:
                    // do not register threshold functions.
                    inc = 8;
                    break;

                case 1:
                case 2:
                    // register for pure c threshold functions
                    WrapperSetCallback(limit * 1 / 4, limit25,   &mem);
                    WrapperSetCallback(limit * 2 / 4, limit50,   &mem);
                    WrapperSetCallback(limit * 3 / 4, limit75_C, &mem);
                    inc = 128;
                    break;

                case 3:
                case 4:
                    // register for pure c threshold functions
                    WrapperSetCallback(limit * 1 / 4, limit25,   &mem);
                    WrapperSetCallback(limit * 2 / 4, limit50,   &mem);
                    WrapperSetCallback(limit * 3 / 4, limit75_C, &mem);
                    inc = 512;
                    break;

                case 5:
                case 6:
                    // register C++ threshold functions
                    WrapperSetCallback(limit * 1 / 4, limit25,     &mem);
                    WrapperSetCallback(limit * 2 / 4, limit50,     &mem);
                    WrapperSetCallback(limit * 3 / 4, limit75_CPP, &mem);
                    inc = 256;
                    break;

                case 7:
                case 8:
                    // register C++ (nothrow) threshold functions
                    WrapperSetCallback(limit * 1 / 4, limit25,        &mem);
                    WrapperSetCallback(limit * 2 / 4, limit50,        &mem);
                    WrapperSetCallback(limit * 3 / 4, limit75_CPP_NT, &mem);
                    inc = 256;
                    break;

                default:
                    test = -1;
                    continue;
            }

            printf("####################################\n"
                   "### TEST = %i\n"
                   "####################################\n", test);

            void *p_new = 0;
            void *p_old = 0;
            while(1)
            {
                switch(test)
                {
                    case 0:
                    case 1:
                        for(int j=0 ; j<4 ; j++)
                        {
                            switch(j)
                            {
                                case 0:
                                    p_new = malloc(size);
                                    break;

                                case 1:
                                    p_new = calloc(1, size);
                                    break;

                                case 2:
                                    p_new = strdup("Hello World");
                                    break;

                                case 3:
                                    p_new = strndup("Hello World", size);
                                    break;

                                default:
                                    break;
                            }

                            if(p_new)
                            {
                                mem.push_back(p_new);
                            }
                        }
                        size += inc;
                        break;

                    case 2:
                        p_new = realloc(p_old, size);
                        if(p_new)
                        {
                            if(p_old && mem.size())
                            {
                                // remove old entry as it was replaced.
                                mem.pop_back();
                            }
                            p_old = p_new;
                        }
                        size += inc;
                        break;

                    case 3:
                        p_new = calloc(1, size);
                        size += inc;
                        break;

                    case 4:
                        p_new = calloc(size, 1);
                        size += inc;
                        break;

                    case 5:
#ifdef MEMWRAPPEREXCEPTIONS
                        p_new = new short[size]();
                        size += inc;
#else // MEMWRAPPEREXCEPTIONS
                        printf("Skipped - no exceptions are enabled.");
                        test++;
#endif // MEMWRAPPEREXCEPTIONS
                        break;

                    case 6:
#ifdef MEMWRAPPEREXCEPTIONS
                        size += inc;
                        p_new = new short[size]();
#else // MEMWRAPPEREXCEPTIONS
                        printf("Skipped - no exceptions are enabled.");
                        test++;
#endif // MEMWRAPPEREXCEPTIONS
                        break;

                    case 7:
                        p_new = new (std::nothrow) short[size]();
                        size += inc;
                        break;

                    case 8:
                        size += inc;
                        p_new = new (std::nothrow) short[size]();
                        break;

                    default:
                        // loop test
                        test = -1;
                        continue;
                }
                if(p_new)
                {
                    switch(test)
                    {
                        case 0:
                        case 1:
                            break;

                        default:
                            mem.push_back(p_new);
                            break;
                    }
                }
                else if(test < 0)
                {
                    break;
                }
                else if(size - inc)
                {
                    printf("\nmalloc/calloc/realloc/new nothrow returned NULL\n\n");
                    break;
                }

                size += size / 100;
            }

            printf(
#ifdef BUILD64
                   "-----< %s >-----\n   Application tidy-up (%lu items)\n\n",
#else
                   "-----< %s >-----\n   Application tidy-up (%u items)\n\n",
#endif
                   __FUNCTION__, mem.size());

            while(mem.size())
            {
                free(mem.back());
                mem.pop_back();
            }
        }
#ifdef MEMWRAPPEREXCEPTIONS
        catch(WrapperException &e)
        {
            printf("-----< %s >-----\n  %s\n\n", __FUNCTION__, e.what());
        }
        catch(std::exception &e)
        {
            printf("-----< %s >-----\n  %s\n\n", __FUNCTION__, e.what());
        }
#endif // MEMWRAPPEREXCEPTIONS

        WrapperPrint(__FUNCTION__);
        if(WrapperGetAlloc())
        {
            WrapperCleanUp(__FUNCTION__);
            WrapperPrint(__FUNCTION__);
        }

        pthread_cancel(child);

        test++;
    }

    return 0;
}


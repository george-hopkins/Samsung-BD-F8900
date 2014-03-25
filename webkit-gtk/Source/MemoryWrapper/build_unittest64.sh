#!/bin/sh

echo "Running $0" ...

WEBKIT_FLAGS="
    -Wall -W -Wcast-align -Wchar-subscripts -Wreturn-type
    -Wformat -Wformat-security -Wno-format-y2k -Wundef
    -Wmissing-format-attribute -Wpointer-arith -Wwrite-strings
    -Wno-unused-parameter -Wno-parentheses
    -fno-exceptions
    "

WRAPPER_REMAP="
    -fexceptions
    -pthread
    -Wl,--wrap,malloc
    -Wl,--wrap,free
    -Wl,--wrap,calloc
    -Wl,--wrap,realloc
    -Wl,--wrap,strdup
    -Wl,--wrap,strndup
    -Wl,--wrap,strdupa
    -Wl,--wrap,strndupa
    -Wl,--wrap,_Znwm
    -Wl,--wrap,_Znam
    -Wl,--wrap,_ZdlPv
    -Wl,--wrap,_ZdaPv
    -Wl,--wrap,_ZnwmRKSt9nothrow_t
    -Wl,--wrap,_ZnamRKSt9nothrow_t
    -Wl,--wrap,_ZdlPvRKSt9nothrow_t
    -Wl,--wrap,_ZdaPvRKSt9nothrow_t
    "

EXECUTABLE="MemoryWrapperDebug64"
g++ \
    ${WEBKIT_FLAGS} \
    ${WRAPPER_REMAP} \
    -g -O0 -m64 \
    -o ${EXECUTABLE} \
    -DEXECUTABLE_NAME=\"${EXECUTABLE}\" \
    -DMEMWRAPPEREXCEPTIONS \
    -DMEMORYWRAPPERSTATIC \
    -DMEMORYWRAPPERDEBUG \
    MemoryWrapper.cpp \
    MemoryWrapper_unittest.cpp

test "$?" -ne 0 && exit 1

EXECUTABLE="MemoryWrapper64"
g++ \
    ${WEBKIT_FLAGS} \
    ${WRAPPER_REMAP} \
    -g -O0 -m64 \
    -o ${EXECUTABLE} \
    -DEXECUTABLE_NAME=\"${EXECUTABLE}\" \
    -DMEMWRAPPEREXCEPTIONS \
    -DMEMORYWRAPPERSTATIC \
    MemoryWrapper.cpp \
    MemoryWrapper_unittest.cpp

exit $?

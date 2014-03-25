/*
 * Copyright (C) 2011 Samsung Electronics, Inc.

 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

#include "BELog.h"

// *********************************
/* { @20100922-sanggyu: debug print */
// *********************************

static int bLog;

#include <stdio.h>  // for fprintf
#include <stdarg.h> // for va_start

void BELogImpl(const char* func, int line, const char *fmt, ...)
{
    if (bLog) {
        va_list args;
        va_start(args, fmt);
        fprintf(stderr, "[WEBKIT] %04d %s : ", line, func);
        vfprintf(stderr, fmt, args);
        va_end(args);
    }
}

/* } End of debug print */

void BELogSetLevel(unsigned int level)
{
    bLog = level;
}

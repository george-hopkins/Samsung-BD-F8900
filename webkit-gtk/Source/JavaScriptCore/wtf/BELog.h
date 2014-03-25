/*
 * Copyright (C) 2011 Samsung Electronics, Inc.

 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

#ifndef BELog_h
#define BELog_h

#ifdef __cplusplus
extern "C" {
#endif

void BELogSetLevel(unsigned int level);

//#define BELog(...) ((void)0)
#define BELog(...) BELogImpl(__PRETTY_FUNCTION__, __LINE__, __VA_ARGS__)

extern void BELogImpl(const char* func, int line, const char *fmt, ...)
    __attribute__ ((format (printf, 3, 4)));

#ifdef __cplusplus
}
#endif

#endif /* BELog_h */

#ifndef CSR_EXCEPTIONHANDLER_H__
#define CSR_EXCEPTIONHANDLER_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/
#include "csr_sched.h"
#include "csr_prim_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*csr_exceptionhandler_t)(const char *);

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
/* DEPRECATED - replaced by csr_log_text.h */
#define CsrStateEventException(theTask, theEventClass, theType, theState) \
    CsrStateEventExceptionWithInfo(theTask, theEventClass, theType, theState, __FILE__, __LINE__)
/* DEPRECATED - replaced by csr_log_text.h */
#define CsrGeneralException(theTask, theState, thePrimType, theText) \
    CsrGeneralExceptionWithInfo(theTask, theState, thePrimType, theText, __FILE__, __LINE__)
/* DEPRECATED - replaced by csr_log_text.h */
#define CsrGeneralWarning(theTask, theState, thePrimType, theText) \
    CsrGeneralWarningWithInfo(theTask, theState, thePrimType, theText, __FILE__, __LINE__)
/* DEPRECATED - replaced by csr_log_text.h */
void CsrExceptionHandlerRegister(csr_exceptionhandler_t cb);
/* DEPRECATED - replaced by csr_log_text.h */
void CsrStateEventExceptionWithInfo(const char *theTask,
    CsrUint16 theEventClass,
    CsrUint16 theType,
    CsrUint16 theState,
    const char *theFile,
    CsrUint32 theLine);
/* DEPRECATED - replaced by csr_log_text.h */
void CsrGeneralExceptionWithInfo(const char *theTask,
    CsrUint16 theState,
    CsrPrim thePrimType,
    const char *theText,
    const char *theFile,
    CsrUint32 theLine);
/* DEPRECATED - replaced by csr_log_text.h */
void CsrGeneralWarningWithInfo(const char *theTask,
    CsrUint16 theState,
    CsrPrim theprimType,
    const char *theText,
    const char *theFile,
    CsrUint32 theLine);
#else
#define CsrStateEventException(theTask, theEventClass, theType, theState)

#define CsrStateEventExceptionWithInfo(theTask, theEventClass, theType, theState, file, line)

#define CsrGeneralException(theTask, theState, thePrimType, theText)
#define CsrGeneralExceptionWithInfo(theTask, theState, thePrimType, theText, file, line)

#define CsrGeneralWarning(theTask, theState, thePrimType, theText)
#define CsrGeneralWarningWithInfo(theTask, theState, thePrimType, theText, file, line)

#define CsrExceptionHandlerRegister(cb);
#endif

#ifdef __cplusplus
}
#endif

#endif

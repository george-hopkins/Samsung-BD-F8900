#ifndef SCO_AUDIO_H__    /* Once is enough */
#define SCO_AUDIO_H__
/*******************************************************************************


                (c) CSR plc 2008

                All rights reserved

FILE:           sco_audio.h

DESCRIPTION:    This file contains the definition of the communication interface
                for sending audio over the serial communication channel.

REVISION:       $Revision: #1 $

*******************************************************************************/

#include "sched/csr_types.h"
#include "usr_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NO_SCO_HANDLE   (255)

typedef void (* ScoHandlerFuncType)(char *);

typedef struct
{
    uint16_t            scoHandle;
    ScoHandlerFuncType  scoHandlerFunc;
} ScoHandleStructure;

#if MAX_NUM_OF_SIMULTANIOUS_SCO_CONNECTIONS > 0
bool_t RegisterScoHandle(uint16_t theScoHandle, ScoHandlerFuncType theFunctionPtr);
void SendScoData(char * theData);
void deRegisterScoHandle(uint16_t theScoHandle);
#else
#define RegisterScoHandle(theScoHandle, theFunctionPtr) FALSE
#define SendScoData(theData)
#define deRegisterScoHandle(theScoHandle)
#endif

#ifdef __cplusplus
}
#endif

#endif /* indef SCO_AUDIO_H__ */

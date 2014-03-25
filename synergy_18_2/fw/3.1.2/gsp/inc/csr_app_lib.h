#ifndef CSR_APP_LIB_H__
#define CSR_APP_LIB_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_app_prim.h"
#include "csr_msg_transport.h"
#include "csr_app_task.h"

#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------------------------------------------
   Name
       CSR_APP_REGISTER_REQ

   Description
        Register with the CsrApp task

 * -------------------------------------------------------------------- */
CsrAppRegisterReq *CsrAppRegisterReq_struct(CsrSchedQid pHandle, CsrUtf8String *menuName, CsrUtf8String *subMenuName, CsrUint16 menuIcon, CsrUint16 subMenuIcon, CsrBool top);
#define CsrAppRegisterReqSend(pHandle, menuName, subMenuName){ \
        CsrAppRegisterReq *msg__; \
        msg__ = CsrAppRegisterReq_struct(pHandle, menuName, subMenuName, 0, 0, FALSE); \
        CsrMsgTransport(CSR_APP_IFACEQUEUE, CSR_APP_PRIM, msg__); \
}
#define CsrAppRegisterReqSend2(pHandle, menuName, subMenuName, menuIcon, subMenuIcon, top){ \
        CsrAppRegisterReq *msg__; \
        msg__ = CsrAppRegisterReq_struct(pHandle, menuName, subMenuName, menuIcon, subMenuIcon, top); \
        CsrMsgTransport(CSR_APP_IFACEQUEUE, CSR_APP_PRIM, msg__); \
}

/* --------------------------------------------------------------------
   Name
       CSR_APP_BACKLOG_REQ

   Description
        Send a backlog entry to app task (this is a replacement for printf)

 * -------------------------------------------------------------------- */
void CsrAppBacklogReqSend(CsrUtf8String *menuName, CsrUtf8String *subMenuName, CsrBool causePopup, const CsrCharString *fmt, ...);
void CsrAppBacklogReqSend2(CsrUtf8String *menuName, CsrUtf8String *subMenuName, CsrBool causePopup, CsrBool causeBacklog, const CsrCharString *fmt, ...);

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAppFreeUpstreamMessageContents
 *
 *  DESCRIPTION
 *      Deallocates the  payload in the CSR APP upstream messages
 *
 *
 *    PARAMETERS
 *      eventClass :  Must be CSR_APP_PRIM,
 *      msg:          The message received from CSR APP
 *----------------------------------------------------------------------------*/
void CsrAppFreeUpstreamMessageContents(CsrUint16 eventClass, void *message);

#ifdef __cplusplus
}
#endif

#endif

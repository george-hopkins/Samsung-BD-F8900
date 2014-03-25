#ifndef CSR_BT_HCRP_SEF_H__
#define CSR_BT_HCRP_SEF_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "csr_types.h"
#include "csr_bt_result.h"

/* HCRP PDUs */
enum
{
    CR_DataChannelCreditGrant = 0x0001,
    CR_DataChannelCreditRequest,
    CR_DataChannelCreditReturn,
    CR_DataChannelCreditQuery,
    CR_GetLPTStatus,
    CR_Get1284ID,
    CR_SoftReset,
    CR_HardReset,
    CR_RegisterNotification,
    CR_NotificationConnectionAlive
};

/* HCRP credit request sizes. */
enum
{
    HCRP_CREDIT_0k  = 0,
    HCRP_CREDIT_1K  = 1024,
    HCRP_CREDIT_5K  = HCRP_CREDIT_1K * 5,
    HCRP_CREDIT_10K = HCRP_CREDIT_1K * 10
};

#define HCRP_PDU_ID_SIZE                                (2)
#define HCRP_TRANS_ID_SIZE                              (2)
#define HCRP_PARAM_LENGTH_SIZE                          (2)
#define HCRP_STATUS_CODE_SIZE                           (2)
#define HCRP_LPT_STATUS_SIZE                            (1)

#define HCRP_GET_BYTE_AT(source, index) \
(((CsrUint8 *) source)[(index)])

#define HCRP_GET_WORD_AT(source, index) \
((((CsrUint16) (((CsrUint8 *) source)[(index)])) << 8) | \
  ((CsrUint16) (((CsrUint8 *) source)[(index) + 1])))

#define HCRP_GET_D_WORD_AT(source, index) \
((((CsrUint32) (((CsrUint8 *) source)[(index)])) << 24) | \
 (((CsrUint32) (((CsrUint8 *) source)[(index) + 1])) << 16) | \
 (((CsrUint32) (((CsrUint8 *)  source)[(index) + 2])) << 8) | \
  ((CsrUint32) (((CsrUint8 *) source)[(index) + 3])))

#define HCRP_GET_PDU_ID(source) \
HCRP_GET_WORD_AT((source), 0)

#define HCRP_GET_TRANS_ID(source) \
HCRP_GET_WORD_AT((source), 2)

#define HCRP_GET_PARAM_LENGTH(source) \
HCRP_GET_WORD_AT((source), 4)

#define HCRP_GET_CREDIT_GRANT(source) \
HCRP_GET_D_WORD_AT((source), 6)

#define HCRP_SET_BYTE_AT(destination, index, value) \
(((CsrUint8 *) destination)[(index)] = (value))

#define HCRP_SET_WORD_AT(destination, index, value) \
((((CsrUint8 *) destination)[index] = ((CsrUint8) (((CsrUint16) value) >> 8))), \
 (((CsrUint8 *) destination)[(index) + 1] = ((CsrUint8) (((CsrUint16) value) & 0x00FF))))

#define HCRP_SET_D_WORD_AT(destination, index, value) \
((((CsrUint8 *) destination)[index + 0] = ((CsrUint8) (((CsrUint32) value) >> 24))), \
 (((CsrUint8 *) destination)[index + 1] = ((CsrUint8) ((((CsrUint32) value) >> 16) & 0x000000FF))), \
 (((CsrUint8 *) destination)[index + 2] = ((CsrUint8) ((((CsrUint32) value) >> 8) & 0x0000FFFF))), \
 (((CsrUint8 *) destination)[index + 3] = ((CsrUint8) (((CsrUint32) value) & 0x00FFFFFF))))

#define HCRP_SET_PDU_ID(destination, pduId) \
HCRP_SET_WORD_AT((destination), 0, (pduId))

#define HCRP_SET_TRANS_ID(destination, transId) \
HCRP_SET_WORD_AT((destination), 2, (transId))

#define HCRP_SET_PARAM_LENGTH(destination, paramLength) \
HCRP_SET_WORD_AT((destination), 4, (paramLength))

#define HCRP_SET_STATUS_CODE(destination, statusCode) \
HCRP_SET_WORD_AT((destination), 6, (statusCode))

#define HCRP_SET_LPT_STATUS(destination, lptStatus) \
HCRP_SET_BYTE_AT((destination), 8, (lptStatus))

/* HCRP event handlers. */
void CsrBtHcrpActivateReqHandler(hcrpInstanceData_t* instData);
void CsrBtHcrpDeactiveReqHandler(hcrpInstanceData_t* instData);
void CsrBtHcrpRegisterDataPathReqHandler(hcrpInstanceData_t *instData);
void CsrBtHcrpSaveMessageHandler(hcrpInstanceData_t* instData);
void CsrBtHcrpGetLptStatusResHandler(hcrpInstanceData_t* instData);
void CsrBtHcrpGetIeee1284ResHandler(hcrpInstanceData_t* instData);
void CsrBtHcrpSecurityInReqHandler(hcrpInstanceData_t* instData);
void CsrBtHcrpIgnoreMessageHandler(hcrpInstanceData_t* instData);

/* CM event handlers */
void CsrBtHcrpCmL2caRegisterCfm(hcrpInstanceData_t* instData);
void CsrBtHcrpcmL2caConnectAcceptCfm(hcrpInstanceData_t* instData);
void CsrBtHcrpcmL2caCancelConnectAcceptCfm(hcrpInstanceData_t* instData);
void CsrBtHcrpcmL2caDataInd(hcrpInstanceData_t* instData);
void CsrBtHcrpCmL2CaDisconnectInd(hcrpInstanceData_t* instData);
void CsrBtHcrpCmSdsRegisterCfm(hcrpInstanceData_t* instData);
void CsrBtHcrpcmSdsUnRegisterCfm(hcrpInstanceData_t* instData);

/* Upstream helpers */
void CsrBtHcrpSecurityInCfmSend(CsrSchedQid appHandle, CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier);

/* Prototypes from hcrp_free_down.c */
void CsrBtHcrpFreeDownstreamMessageContents(CsrUint16 eventClass, void * message);


#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_HCRP_SEF_H__ */

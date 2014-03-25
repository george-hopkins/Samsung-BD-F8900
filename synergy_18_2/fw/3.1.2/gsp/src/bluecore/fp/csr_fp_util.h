#ifndef CSR_FP_UTIL_H__
#define CSR_FP_UTIL_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_usr_config.h"
#include "csr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* == DEFINES ============================================================ */

#define BCCMDVARID_FASTPIPE_ENABLE          (0x5041)
#define BCCMDVARID_FASTPIPE_CREATE          (0x5042)
#define BCCMDVARID_FASTPIPE_DESTROY         (0x5043)
#define BCCMDVARID_FASTPIPE_RESIZE          (0x5057)

#define BCCMDPDU_FASTPIPE_ENABLE_LENGTH      (6)
#define BCCMDPDU_FASTPIPE_RESIZE_LENGTH      (10)
#define BCCMDPDU_FASTPIPE_CREATE_LENGTH      (42)
#define BCCMDPDU_FASTPIPE_DESTROY_LENGTH     (4)

/* FP ENABLE SETTINGS */
#define CSR_FP_LIMIT_HOST                   2048

/* FP CREATE FLOW CONTROL PIPE SETTINGS */
#define CSR_FP_CTRL_OVERHEAD_HOST           100
#define CSR_FP_CTRL_CAPACITY_RX_HOST        100
#define CSR_FP_CTRL_REQUIRED_TX_CONTROLLER  256
#define CSR_FP_CTRL_DESIRED_TX_CONTROLLER   256
#define CSR_FP_CTRL_REQUIRED_RX_CONTROLLER  256
#define CSR_FP_CTRL_DESIRED_RX_CONTROLLER   256

#define HCI_DATA_HEADER_SIZE                2
#define HCI_ACL_DATA_LEN_SIZE               2

#define CSR_FP_TOKEN_SIZE                   3


/* == MACROS ============================================================= */

#define INV_XAP_UINT16(_data, _value)                                   \
    {                                                                       \
        ((CsrUint8 *) _data)[1] = (CsrUint8) ((_value) & 0x00FF);                \
        ((CsrUint8 *) _data)[0] = (CsrUint8) (((_value) & 0xFF00) >> 8);        \
    }

#define SET_XAP_UINT16(_data, _value)                                   \
    {                                                                       \
        ((CsrUint8 *) _data)[0] = (CsrUint8) ((_value) & 0x00FF);                \
        ((CsrUint8 *) _data)[1] = (CsrUint8) (((_value) & 0xFF00) >> 8);        \
    }

#define SET_XAP_UINT32(_data, _value)                                   \
    {                                                                       \
        ((CsrUint8 *) _data)[0] = (CsrUint8) (((_value) & 0x00FF0000) >> 16);   \
        ((CsrUint8 *) _data)[1] = (CsrUint8) (((_value) & 0xFF000000) >> 24);   \
        ((CsrUint8 *) _data)[2] = (CsrUint8) ((_value) & 0x000000FF);           \
        ((CsrUint8 *) _data)[3] = (CsrUint8) (((_value) & 0x0000FF00) >> 8);   \
    }

#define GET_XAP_UINT16(_data) (CsrUint16) (((CsrUint8 *) (_data))[0] | ((CsrUint8 *) (_data))[1] << 8)

#define GET_XAP_UINT32(_data) (CsrUint32) (                               \
        ((CsrUint32) ((CsrUint8 *) (_data))[0] << 16)                         \
        | ((CsrUint32) ((CsrUint8 *) (_data))[1] << 24)                         \
        | ((CsrUint32) ((CsrUint8 *) (_data))[2])                               \
        | ((CsrUint32) ((CsrUint8 *) (_data))[3] << 8))


/* Build and send fastpipe upstream prims */

#define fpCreateCfmSend(_appHandle, _fpHandle, _overheadController, _capacityRxController, _capacityTxController, _result) \
    {                                                                       \
        CsrFpCreateCfm *_msg = CsrPmemAlloc(sizeof(CsrFpCreateCfm));          \
        _msg->type = CSR_FP_CREATE_CFM;                                     \
        _msg->fpHandle = (_fpHandle);                                       \
        _msg->overheadController = _overheadController;                     \
        _msg->capacityRxController = (_capacityRxController);               \
        _msg->capacityTxController = (_capacityTxController);               \
        _msg->result = (_result);                                           \
        CsrSchedMessagePut((_appHandle), CSR_FP_PRIM, _msg);                     \
    }

#define fpWriteCfmSend(_appHandle, _fpHandle, _result)                  \
    {                                                                       \
        CsrFpWriteCfm *_msg = CsrPmemAlloc(sizeof(CsrFpWriteCfm));            \
        _msg->type = CSR_FP_WRITE_CFM;                                      \
        _msg->fpHandle = (_fpHandle);                                       \
        _msg->result = (_result);                                           \
        CsrSchedMessagePut((_appHandle), CSR_FP_PRIM, _msg);                     \
    }

#define fpReadResSend(_appHandle, _fpHandle, _payLoadLength, _payload)  \
    {                                                                       \
        CsrFpReadInd *_msg = CsrPmemAlloc(sizeof(CsrFpReadInd));              \
        _msg->type = CSR_FP_READ_IND;                                       \
        _msg->fpHandle = (_fpHandle);                                       \
        _msg->payLoadLength = (_payLoadLength);                             \
        _msg->payload = (_payload);                                         \
        CsrSchedMessagePut((_appHandle), CSR_FP_PRIM, _msg);                     \
    }

#define fpDestroyCfmSend(_appHandle, _fpHandle, _result)                \
    {                                                                       \
        CsrFpDestroyCfm *_msg = CsrPmemAlloc(sizeof(CsrFpDestroyCfm));        \
        _msg->type = CSR_FP_DESTROY_CFM;                                    \
        _msg->fpHandle = (_fpHandle);                                       \
        _msg->result = (_result);                                           \
        CsrSchedMessagePut((_appHandle), CSR_FP_PRIM, _msg);                     \
    }

#define fpClearCfmSend(_appHandle, _fpHandle, _result)                  \
    {                                                                       \
        CsrFpClearCfm *_msg = CsrPmemAlloc(sizeof(CsrFpClearCfm));            \
        _msg->type = CSR_FP_CLEAR_CFM;                                      \
        _msg->fpHandle = (_fpHandle);                                       \
        _msg->result = (_result);                                           \
        CsrSchedMessagePut((_appHandle), CSR_FP_PRIM, _msg);                     \
    }


#ifdef __cplusplus
}
#endif

#endif /* CSR_FP_UTIL_H__ */

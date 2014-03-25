#ifndef CSR_HCI_UTIL_H__
#define CSR_HCI_UTIL_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_hci_handler.h"
#include "csr_tm_bluecore_transport.h"
#include "csr_util.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Define used in in- and decoding of tunneled HCI messages.
   An unfragmented package is both start and end */
#define CSR_HCI_FRAGMENT_END        (0x80)
#define CSR_HCI_FRAGMENT_START      (0x40)
#define CSR_HCI_UNFRAGMENTED        (CSR_HCI_FRAGMENT_START | CSR_HCI_FRAGMENT_END)

#define CHANNEL_ID_DEBUG    (20)
#define CHANNEL_ID_MASK     (0x3F)

#define CSR_HCI_MAX_HCI_CMD_PAYLOAD_SIZE    254
#define CSR_HCI_COMMAND_VENDOR_HDR_SIZE     4
#define CSR_HCI_EVENT_HDR_SIZE              1

#define CSR_HCI_ACL_HDR_SIZE                4
#define CSR_HCI_ACL_HANDLE_MASK             0x0FFF

#define CSR_HCI_SCO_HDR_SIZE                3
#define CSR_HCI_SCO_HANDLE_MASK             0x0FFF

#define CSR_HCI_EV_COMMAND_COMPLETE ((CsrUint8) 0x0E)
#define CSR_HCI_EV_COMMAND_STATUS   ((CsrUint8) 0x0F)

#define CSR_HCI_SET_XAP_UINT16(_data, _value)                           \
    {                                                                       \
        ((CsrUint8 *) _data)[0] = (CsrUint8) ((_value) & 0x00FF);                \
        ((CsrUint8 *) _data)[1] = (CsrUint8) (((_value) & 0xFF00) >> 8);        \
    }

#define CSR_HCI_GET_XAP_UINT16(_data) (CsrUint16) (((CsrUint8 *) (_data))[0] | ((CsrUint8 *) (_data))[1] << 8)

void CsrHciRegisterHandle(csrHciListType **main_list, CsrUint16 handle, CsrSchedQid queueId);
void CsrHciUnregisterHandle(csrHciListType **main_list, CsrUint16 handle, CsrSchedQid queueId);
void CsrHciUnregisterAllHandles(csrHciListType **main_list);

void CsrHciSendRegisterEventHandlerCfm(CsrSchedQid phandle);
void CsrHciSendRegisterAclHandlerCfm(CsrSchedQid phandle, CsrUint16 aclHandle);
void CsrHciSendUnregisterAclHandlerCfm(CsrSchedQid phandle, CsrUint16 aclHandle);
void CsrHciSendRegisterScoHandlerCfm(CsrSchedQid phandle, CsrUint16 scoHandle);
void CsrHciSendUnregisterScoHandlerCfm(CsrSchedQid phandle, CsrUint16 scoHandle);
void CsrHciSendRegisterVendorSpecificEventHandlerCfm(CsrSchedQid phandle, CsrUint8 channel);
void CsrHciSendUnregisterVendorSpecificEventHandlerCfm(CsrSchedQid phandle, CsrUint8 channel);
void CsrHciSendAclDataInd(CsrUint16 handlePlusFlags, CsrSchedQid phandle, CsrMblk *data, CsrBool functionPotential);
void CsrHciSendScoDataInd(CsrUint16 handlePlusFlags, CsrSchedQid phandle, CsrMblk *data);
void CsrHciSendEventInd(CsrSchedQid phandle, CsrUint8 *payload, CsrUint16 length);
void CsrHciSendVendorSpecificEventInd(CsrSchedQid phandle, CsrUint8 channel, CsrMblk *data);

csrHciListType *CsrHciGetHandler(csrHciListType *list, CsrUint16 handle);
CsrSchedQid CsrHciCheckHandlers(csrHciListType *list, CsrUint16 handle);

void CsrHciInsertAclHeader(CsrUint8 *payload, CsrUint16 handlePlusFlags, CsrUint16 length);
void CsrHciInsertScoHeader(CsrUint8 *payload, CsrUint16 handlePlusFlags, CsrUint16 length);
void CsrHciExtractAclHeader(CsrUint8 *payload, CsrUint16 *handlePlusFlags, CsrUint16 *length);
void CsrHciExtractScoHeader(CsrUint8 *payload, CsrUint16 *handlePlusFlags, CsrUint16 *length);

void CsrHciReassembleVendorSpecificEvents(CsrHciInstData *inst, CsrUint8 channel, CsrUint8 fragment, CsrMblk *data);

void CsrHciSaveMessage(CsrHciInstData *inst);
void CsrHciRestoreSavedMessages(CsrHciInstData *inst);
void CsrHciDiscardSavedMessages(CsrHciInstData *inst);

void CsrHciSendTmNopInd(CsrSchedQid phandle, CsrUint8 *payload, CsrUint16 payloadLength);

#ifdef __cplusplus
}
#endif

#endif /* CSR_HCI_UTIL_H__ */

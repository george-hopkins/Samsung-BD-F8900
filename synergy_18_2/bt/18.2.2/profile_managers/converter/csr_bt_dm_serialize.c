/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_msgconv.h"
#include "csr_bt_autogen.h"
#include "csr_unicode.h"
#include "dm_prim.h"

#ifndef EXCLUDE_DM_MODULE

void CsrBtDmPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_DM_AMPM_DISCONNECT_REQ
CsrSize DM_AMPM_DISCONNECT_REQ_TSizeof(void *msg)
{
    DM_AMPM_DISCONNECT_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_AMPM_DISCONNECT_REQ */

#ifndef EXCLUDE_DM_AMPM_DISCONNECT_REQ
CsrUint8 *DM_AMPM_DISCONNECT_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_AMPM_DISCONNECT_REQ_T *primitive;

    primitive = (DM_AMPM_DISCONNECT_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->local_amp_id);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->reason);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->active_links);

    return buffer;
}
#endif /* EXCLUDE_DM_AMPM_DISCONNECT_REQ */

#ifndef EXCLUDE_DM_AMPM_DISCONNECT_REQ
void *DM_AMPM_DISCONNECT_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_AMPM_DISCONNECT_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_AMPM_DISCONNECT_REQ_T *) CsrPmemZalloc(sizeof(DM_AMPM_DISCONNECT_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->local_amp_id, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->reason, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->active_links, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_AMPM_DISCONNECT_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_TRANSMITTER_TEST_REQ
CsrSize DM_HCI_ULP_TRANSMITTER_TEST_REQ_TSizeof(void *msg)
{
    DM_HCI_ULP_TRANSMITTER_TEST_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_TRANSMITTER_TEST_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_TRANSMITTER_TEST_REQ
CsrUint8 *DM_HCI_ULP_TRANSMITTER_TEST_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_TRANSMITTER_TEST_REQ_T *primitive;

    primitive = (DM_HCI_ULP_TRANSMITTER_TEST_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tx_channel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->length_test_data);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->packet_payload);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_TRANSMITTER_TEST_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_TRANSMITTER_TEST_REQ
void *DM_HCI_ULP_TRANSMITTER_TEST_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_TRANSMITTER_TEST_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_TRANSMITTER_TEST_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_TRANSMITTER_TEST_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tx_channel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->length_test_data, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->packet_payload, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_TRANSMITTER_TEST_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_SET_EVENT_MASK_CFM
CsrSize DM_HCI_ULP_SET_EVENT_MASK_CFM_TSizeof(void *msg)
{
    DM_HCI_ULP_SET_EVENT_MASK_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_EVENT_MASK_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_SET_EVENT_MASK_CFM
CsrUint8 *DM_HCI_ULP_SET_EVENT_MASK_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_SET_EVENT_MASK_CFM_T *primitive;

    primitive = (DM_HCI_ULP_SET_EVENT_MASK_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_EVENT_MASK_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_SET_EVENT_MASK_CFM
void *DM_HCI_ULP_SET_EVENT_MASK_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_SET_EVENT_MASK_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_SET_EVENT_MASK_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_SET_EVENT_MASK_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_EVENT_MASK_CFM */

#ifndef EXCLUDE_DM_HCI_REM_HOST_SUPPORTED_FEATURES_IND
CsrSize DM_HCI_REM_HOST_SUPPORTED_FEATURES_IND_TSizeof(void *msg)
{
    DM_HCI_REM_HOST_SUPPORTED_FEATURES_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_REM_HOST_SUPPORTED_FEATURES_IND */

#ifndef EXCLUDE_DM_HCI_REM_HOST_SUPPORTED_FEATURES_IND
CsrUint8 *DM_HCI_REM_HOST_SUPPORTED_FEATURES_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_REM_HOST_SUPPORTED_FEATURES_IND_T *primitive;

    primitive = (DM_HCI_REM_HOST_SUPPORTED_FEATURES_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrMemCpySer(buffer, length, ((const void *) primitive->host_features), ((CsrSize) 2 * (4)));

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_REM_HOST_SUPPORTED_FEATURES_IND */

#ifndef EXCLUDE_DM_HCI_REM_HOST_SUPPORTED_FEATURES_IND
void *DM_HCI_REM_HOST_SUPPORTED_FEATURES_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_REM_HOST_SUPPORTED_FEATURES_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_REM_HOST_SUPPORTED_FEATURES_IND_T *) CsrPmemZalloc(sizeof(DM_HCI_REM_HOST_SUPPORTED_FEATURES_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->host_features), buffer, &offset, ((CsrSize) 2 * (4)));

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_REM_HOST_SUPPORTED_FEATURES_IND */

#ifndef EXCLUDE_DM_HCI_RESET_FAILED_CONTACT_COUNT_CFM
CsrSize DM_HCI_RESET_FAILED_CONTACT_COUNT_CFM_TSizeof(void *msg)
{
    DM_HCI_RESET_FAILED_CONTACT_COUNT_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_RESET_FAILED_CONTACT_COUNT_CFM */

#ifndef EXCLUDE_DM_HCI_RESET_FAILED_CONTACT_COUNT_CFM
CsrUint8 *DM_HCI_RESET_FAILED_CONTACT_COUNT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_RESET_FAILED_CONTACT_COUNT_CFM_T *primitive;

    primitive = (DM_HCI_RESET_FAILED_CONTACT_COUNT_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_RESET_FAILED_CONTACT_COUNT_CFM */

#ifndef EXCLUDE_DM_HCI_RESET_FAILED_CONTACT_COUNT_CFM
void *DM_HCI_RESET_FAILED_CONTACT_COUNT_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_RESET_FAILED_CONTACT_COUNT_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_RESET_FAILED_CONTACT_COUNT_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_RESET_FAILED_CONTACT_COUNT_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_RESET_FAILED_CONTACT_COUNT_CFM */

#ifndef EXCLUDE_DM_HCI_READ_LOCAL_VER_INFO_REQ
CsrSize DM_HCI_READ_LOCAL_VER_INFO_REQ_TSizeof(void *msg)
{
    DM_HCI_READ_LOCAL_VER_INFO_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_LOCAL_VER_INFO_REQ */

#ifndef EXCLUDE_DM_HCI_READ_LOCAL_VER_INFO_REQ
CsrUint8 *DM_HCI_READ_LOCAL_VER_INFO_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_LOCAL_VER_INFO_REQ_T *primitive;

    primitive = (DM_HCI_READ_LOCAL_VER_INFO_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_LOCAL_VER_INFO_REQ */

#ifndef EXCLUDE_DM_HCI_READ_LOCAL_VER_INFO_REQ
void *DM_HCI_READ_LOCAL_VER_INFO_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_LOCAL_VER_INFO_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_LOCAL_VER_INFO_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_LOCAL_VER_INFO_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_LOCAL_VER_INFO_REQ */

#ifndef EXCLUDE_DM_HCI_HOST_NUM_COMPLETED_PACKETS_CFM
CsrSize DM_HCI_HOST_NUM_COMPLETED_PACKETS_CFM_TSizeof(void *msg)
{
    DM_HCI_HOST_NUM_COMPLETED_PACKETS_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_HOST_NUM_COMPLETED_PACKETS_CFM */

#ifndef EXCLUDE_DM_HCI_HOST_NUM_COMPLETED_PACKETS_CFM
CsrUint8 *DM_HCI_HOST_NUM_COMPLETED_PACKETS_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_HOST_NUM_COMPLETED_PACKETS_CFM_T *primitive;

    primitive = (DM_HCI_HOST_NUM_COMPLETED_PACKETS_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_HOST_NUM_COMPLETED_PACKETS_CFM */

#ifndef EXCLUDE_DM_HCI_HOST_NUM_COMPLETED_PACKETS_CFM
void *DM_HCI_HOST_NUM_COMPLETED_PACKETS_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_HOST_NUM_COMPLETED_PACKETS_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_HOST_NUM_COMPLETED_PACKETS_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_HOST_NUM_COMPLETED_PACKETS_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_HOST_NUM_COMPLETED_PACKETS_CFM */

#ifndef EXCLUDE_DM_HCI_READ_VOICE_SETTING_CFM
CsrSize DM_HCI_READ_VOICE_SETTING_CFM_TSizeof(void *msg)
{
    DM_HCI_READ_VOICE_SETTING_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_VOICE_SETTING_CFM */

#ifndef EXCLUDE_DM_HCI_READ_VOICE_SETTING_CFM
CsrUint8 *DM_HCI_READ_VOICE_SETTING_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_VOICE_SETTING_CFM_T *primitive;

    primitive = (DM_HCI_READ_VOICE_SETTING_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->voice_setting);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_VOICE_SETTING_CFM */

#ifndef EXCLUDE_DM_HCI_READ_VOICE_SETTING_CFM
void *DM_HCI_READ_VOICE_SETTING_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_VOICE_SETTING_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_VOICE_SETTING_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_VOICE_SETTING_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->voice_setting, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_VOICE_SETTING_CFM */

#ifndef EXCLUDE_DM_HCI_READ_INQUIRY_MODE_CFM
CsrSize DM_HCI_READ_INQUIRY_MODE_CFM_TSizeof(void *msg)
{
    DM_HCI_READ_INQUIRY_MODE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_INQUIRY_MODE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_INQUIRY_MODE_CFM
CsrUint8 *DM_HCI_READ_INQUIRY_MODE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_INQUIRY_MODE_CFM_T *primitive;

    primitive = (DM_HCI_READ_INQUIRY_MODE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mode);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_INQUIRY_MODE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_INQUIRY_MODE_CFM
void *DM_HCI_READ_INQUIRY_MODE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_INQUIRY_MODE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_INQUIRY_MODE_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_INQUIRY_MODE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_INQUIRY_MODE_CFM */

#ifndef EXCLUDE_DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_CFM
CsrSize DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_CFM_TSizeof(void *msg)
{
    DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_CFM */

#ifndef EXCLUDE_DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_CFM
CsrUint8 *DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_CFM_T *primitive;

    primitive = (DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_CFM */

#ifndef EXCLUDE_DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_CFM
void *DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_REQ
CsrSize DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_REQ_TSizeof(void *msg)
{
    DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_REQ
CsrUint8 *DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_REQ_T *primitive;

    primitive = (DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->timeout);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_REQ
void *DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->timeout, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_REQ */

#ifndef EXCLUDE_DM_HCI_READ_CONN_ACCEPT_TIMEOUT_REQ
CsrSize DM_HCI_READ_CONN_ACCEPT_TIMEOUT_REQ_TSizeof(void *msg)
{
    DM_HCI_READ_CONN_ACCEPT_TIMEOUT_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_CONN_ACCEPT_TIMEOUT_REQ */

#ifndef EXCLUDE_DM_HCI_READ_CONN_ACCEPT_TIMEOUT_REQ
CsrUint8 *DM_HCI_READ_CONN_ACCEPT_TIMEOUT_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_CONN_ACCEPT_TIMEOUT_REQ_T *primitive;

    primitive = (DM_HCI_READ_CONN_ACCEPT_TIMEOUT_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_CONN_ACCEPT_TIMEOUT_REQ */

#ifndef EXCLUDE_DM_HCI_READ_CONN_ACCEPT_TIMEOUT_REQ
void *DM_HCI_READ_CONN_ACCEPT_TIMEOUT_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_CONN_ACCEPT_TIMEOUT_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_CONN_ACCEPT_TIMEOUT_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_CONN_ACCEPT_TIMEOUT_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_CONN_ACCEPT_TIMEOUT_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_CFM
CsrSize DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_CFM_TSizeof(void *msg)
{
    DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_CFM
CsrUint8 *DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_CFM_T *primitive;

    primitive = (DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_CFM
void *DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_CLOCK_OFFSET_CFM
CsrSize DM_HCI_READ_CLOCK_OFFSET_CFM_TSizeof(void *msg)
{
    DM_HCI_READ_CLOCK_OFFSET_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_CLOCK_OFFSET_CFM */

#ifndef EXCLUDE_DM_HCI_READ_CLOCK_OFFSET_CFM
CsrUint8 *DM_HCI_READ_CLOCK_OFFSET_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_CLOCK_OFFSET_CFM_T *primitive;

    primitive = (DM_HCI_READ_CLOCK_OFFSET_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->clock_offset);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_CLOCK_OFFSET_CFM */

#ifndef EXCLUDE_DM_HCI_READ_CLOCK_OFFSET_CFM
void *DM_HCI_READ_CLOCK_OFFSET_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_CLOCK_OFFSET_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_CLOCK_OFFSET_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_CLOCK_OFFSET_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->clock_offset, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_CLOCK_OFFSET_CFM */

#ifndef EXCLUDE_DM_HCI_READ_HOLD_MODE_ACTIVITY_REQ
CsrSize DM_HCI_READ_HOLD_MODE_ACTIVITY_REQ_TSizeof(void *msg)
{
    DM_HCI_READ_HOLD_MODE_ACTIVITY_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_HOLD_MODE_ACTIVITY_REQ */

#ifndef EXCLUDE_DM_HCI_READ_HOLD_MODE_ACTIVITY_REQ
CsrUint8 *DM_HCI_READ_HOLD_MODE_ACTIVITY_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_HOLD_MODE_ACTIVITY_REQ_T *primitive;

    primitive = (DM_HCI_READ_HOLD_MODE_ACTIVITY_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_HOLD_MODE_ACTIVITY_REQ */

#ifndef EXCLUDE_DM_HCI_READ_HOLD_MODE_ACTIVITY_REQ
void *DM_HCI_READ_HOLD_MODE_ACTIVITY_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_HOLD_MODE_ACTIVITY_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_HOLD_MODE_ACTIVITY_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_HOLD_MODE_ACTIVITY_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_HOLD_MODE_ACTIVITY_REQ */

#ifndef EXCLUDE_DM_HCI_READ_INQUIRYSCAN_ACTIVITY_CFM
CsrSize DM_HCI_READ_INQUIRYSCAN_ACTIVITY_CFM_TSizeof(void *msg)
{
    DM_HCI_READ_INQUIRYSCAN_ACTIVITY_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_INQUIRYSCAN_ACTIVITY_CFM */

#ifndef EXCLUDE_DM_HCI_READ_INQUIRYSCAN_ACTIVITY_CFM
CsrUint8 *DM_HCI_READ_INQUIRYSCAN_ACTIVITY_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_INQUIRYSCAN_ACTIVITY_CFM_T *primitive;

    primitive = (DM_HCI_READ_INQUIRYSCAN_ACTIVITY_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->inqscan_interval);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->inqscan_window);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_INQUIRYSCAN_ACTIVITY_CFM */

#ifndef EXCLUDE_DM_HCI_READ_INQUIRYSCAN_ACTIVITY_CFM
void *DM_HCI_READ_INQUIRYSCAN_ACTIVITY_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_INQUIRYSCAN_ACTIVITY_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_INQUIRYSCAN_ACTIVITY_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_INQUIRYSCAN_ACTIVITY_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->inqscan_interval, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->inqscan_window, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_INQUIRYSCAN_ACTIVITY_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_CLEAR_WHITE_LIST_CFM
CsrSize DM_HCI_ULP_CLEAR_WHITE_LIST_CFM_TSizeof(void *msg)
{
    DM_HCI_ULP_CLEAR_WHITE_LIST_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_CLEAR_WHITE_LIST_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_CLEAR_WHITE_LIST_CFM
CsrUint8 *DM_HCI_ULP_CLEAR_WHITE_LIST_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_CLEAR_WHITE_LIST_CFM_T *primitive;

    primitive = (DM_HCI_ULP_CLEAR_WHITE_LIST_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_CLEAR_WHITE_LIST_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_CLEAR_WHITE_LIST_CFM
void *DM_HCI_ULP_CLEAR_WHITE_LIST_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_CLEAR_WHITE_LIST_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_CLEAR_WHITE_LIST_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_CLEAR_WHITE_LIST_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_CLEAR_WHITE_LIST_CFM */

#ifndef EXCLUDE_DM_HCI_READ_LOCAL_SUPP_FEATURES_REQ
CsrSize DM_HCI_READ_LOCAL_SUPP_FEATURES_REQ_TSizeof(void *msg)
{
    DM_HCI_READ_LOCAL_SUPP_FEATURES_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_LOCAL_SUPP_FEATURES_REQ */

#ifndef EXCLUDE_DM_HCI_READ_LOCAL_SUPP_FEATURES_REQ
CsrUint8 *DM_HCI_READ_LOCAL_SUPP_FEATURES_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_LOCAL_SUPP_FEATURES_REQ_T *primitive;

    primitive = (DM_HCI_READ_LOCAL_SUPP_FEATURES_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_LOCAL_SUPP_FEATURES_REQ */

#ifndef EXCLUDE_DM_HCI_READ_LOCAL_SUPP_FEATURES_REQ
void *DM_HCI_READ_LOCAL_SUPP_FEATURES_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_LOCAL_SUPP_FEATURES_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_LOCAL_SUPP_FEATURES_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_LOCAL_SUPP_FEATURES_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_LOCAL_SUPP_FEATURES_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_ENCRYPT_CFM
CsrSize DM_HCI_ULP_ENCRYPT_CFM_TSizeof(void *msg)
{
    DM_HCI_ULP_ENCRYPT_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_ENCRYPT_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_ENCRYPT_CFM
CsrUint8 *DM_HCI_ULP_ENCRYPT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_ENCRYPT_CFM_T *primitive;

    primitive = (DM_HCI_ULP_ENCRYPT_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrMemCpySer(buffer, length, ((const void *) primitive->encrypted_data), ((CsrSize) (16)));

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_ENCRYPT_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_ENCRYPT_CFM
void *DM_HCI_ULP_ENCRYPT_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_ENCRYPT_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_ENCRYPT_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_ENCRYPT_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->encrypted_data), buffer, &offset, ((CsrSize) (16)));

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_ENCRYPT_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_PAGESCAN_ACTIVITY_REQ
CsrSize DM_HCI_WRITE_PAGESCAN_ACTIVITY_REQ_TSizeof(void *msg)
{
    DM_HCI_WRITE_PAGESCAN_ACTIVITY_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_PAGESCAN_ACTIVITY_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_PAGESCAN_ACTIVITY_REQ
CsrUint8 *DM_HCI_WRITE_PAGESCAN_ACTIVITY_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_PAGESCAN_ACTIVITY_REQ_T *primitive;

    primitive = (DM_HCI_WRITE_PAGESCAN_ACTIVITY_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pagescan_interval);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pagescan_window);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_PAGESCAN_ACTIVITY_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_PAGESCAN_ACTIVITY_REQ
void *DM_HCI_WRITE_PAGESCAN_ACTIVITY_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_PAGESCAN_ACTIVITY_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_PAGESCAN_ACTIVITY_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_PAGESCAN_ACTIVITY_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pagescan_interval, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pagescan_window, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_PAGESCAN_ACTIVITY_REQ */

#ifndef EXCLUDE_DM_AMPM_VERIFY_PHYSICAL_LINK_RSP
CsrSize DM_AMPM_VERIFY_PHYSICAL_LINK_RSP_TSizeof(void *msg)
{
    DM_AMPM_VERIFY_PHYSICAL_LINK_RSP_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_AMPM_VERIFY_PHYSICAL_LINK_RSP */

#ifndef EXCLUDE_DM_AMPM_VERIFY_PHYSICAL_LINK_RSP
CsrUint8 *DM_AMPM_VERIFY_PHYSICAL_LINK_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_AMPM_VERIFY_PHYSICAL_LINK_RSP_T *primitive;

    primitive = (DM_AMPM_VERIFY_PHYSICAL_LINK_RSP_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->identifier);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->exists);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->link_supervision_timeout);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->best_effort_flush_timeout);

    return buffer;
}
#endif /* EXCLUDE_DM_AMPM_VERIFY_PHYSICAL_LINK_RSP */

#ifndef EXCLUDE_DM_AMPM_VERIFY_PHYSICAL_LINK_RSP
void *DM_AMPM_VERIFY_PHYSICAL_LINK_RSP_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_AMPM_VERIFY_PHYSICAL_LINK_RSP_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_AMPM_VERIFY_PHYSICAL_LINK_RSP_T *) CsrPmemZalloc(sizeof(DM_AMPM_VERIFY_PHYSICAL_LINK_RSP_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->identifier, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->exists, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->link_supervision_timeout, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->best_effort_flush_timeout, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_AMPM_VERIFY_PHYSICAL_LINK_RSP */

#ifndef EXCLUDE_DM_HCI_READ_CURRENT_IAC_LAP_REQ
CsrSize DM_HCI_READ_CURRENT_IAC_LAP_REQ_TSizeof(void *msg)
{
    DM_HCI_READ_CURRENT_IAC_LAP_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_CURRENT_IAC_LAP_REQ */

#ifndef EXCLUDE_DM_HCI_READ_CURRENT_IAC_LAP_REQ
CsrUint8 *DM_HCI_READ_CURRENT_IAC_LAP_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_CURRENT_IAC_LAP_REQ_T *primitive;

    primitive = (DM_HCI_READ_CURRENT_IAC_LAP_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_CURRENT_IAC_LAP_REQ */

#ifndef EXCLUDE_DM_HCI_READ_CURRENT_IAC_LAP_REQ
void *DM_HCI_READ_CURRENT_IAC_LAP_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_CURRENT_IAC_LAP_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_CURRENT_IAC_LAP_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_CURRENT_IAC_LAP_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_CURRENT_IAC_LAP_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_REQ
CsrSize DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_REQ_TSizeof(void *msg)
{
    DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_REQ
CsrUint8 *DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_REQ_T *primitive;

    primitive = (DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->default_lps);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_REQ
void *DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->default_lps, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_RECEIVER_TEST_CFM
CsrSize DM_HCI_ULP_RECEIVER_TEST_CFM_TSizeof(void *msg)
{
    DM_HCI_ULP_RECEIVER_TEST_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_RECEIVER_TEST_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_RECEIVER_TEST_CFM
CsrUint8 *DM_HCI_ULP_RECEIVER_TEST_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_RECEIVER_TEST_CFM_T *primitive;

    primitive = (DM_HCI_ULP_RECEIVER_TEST_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_RECEIVER_TEST_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_RECEIVER_TEST_CFM
void *DM_HCI_ULP_RECEIVER_TEST_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_RECEIVER_TEST_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_RECEIVER_TEST_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_RECEIVER_TEST_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_RECEIVER_TEST_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_RAND_CFM
CsrSize DM_HCI_ULP_RAND_CFM_TSizeof(void *msg)
{
    DM_HCI_ULP_RAND_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_RAND_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_RAND_CFM
CsrUint8 *DM_HCI_ULP_RAND_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_RAND_CFM_T *primitive;

    primitive = (DM_HCI_ULP_RAND_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrMemCpySer(buffer, length, ((const void *) primitive->random_number), ((CsrSize) (8)));

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_RAND_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_RAND_CFM
void *DM_HCI_ULP_RAND_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_RAND_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_RAND_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_RAND_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->random_number), buffer, &offset, ((CsrSize) (8)));

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_RAND_CFM */

#ifndef EXCLUDE_DM_SYNC_CONNECT_RSP
CsrSize DM_SYNC_CONNECT_RSP_TSizeof(void *msg)
{
    DM_SYNC_CONNECT_RSP_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SYNC_CONNECT_RSP */

#ifndef EXCLUDE_DM_SYNC_CONNECT_RSP
CsrUint8 *DM_SYNC_CONNECT_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SYNC_CONNECT_RSP_T *primitive;

    primitive = (DM_SYNC_CONNECT_RSP_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->response);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.max_latency);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.retx_effort);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.packet_type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.voice_settings);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.tx_bdw);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.rx_bdw);

    return buffer;
}
#endif /* EXCLUDE_DM_SYNC_CONNECT_RSP */

#ifndef EXCLUDE_DM_SYNC_CONNECT_RSP
void *DM_SYNC_CONNECT_RSP_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SYNC_CONNECT_RSP_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SYNC_CONNECT_RSP_T *) CsrPmemZalloc(sizeof(DM_SYNC_CONNECT_RSP_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->response, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config.max_latency, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config.retx_effort, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config.packet_type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config.voice_settings, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.tx_bdw, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.rx_bdw, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SYNC_CONNECT_RSP */

#ifndef EXCLUDE_DM_HCI_SET_EVENT_MASK_CFM
CsrSize DM_HCI_SET_EVENT_MASK_CFM_TSizeof(void *msg)
{
    DM_HCI_SET_EVENT_MASK_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_SET_EVENT_MASK_CFM */

#ifndef EXCLUDE_DM_HCI_SET_EVENT_MASK_CFM
CsrUint8 *DM_HCI_SET_EVENT_MASK_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_SET_EVENT_MASK_CFM_T *primitive;

    primitive = (DM_HCI_SET_EVENT_MASK_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_SET_EVENT_MASK_CFM */

#ifndef EXCLUDE_DM_HCI_SET_EVENT_MASK_CFM
void *DM_HCI_SET_EVENT_MASK_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_SET_EVENT_MASK_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_SET_EVENT_MASK_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_SET_EVENT_MASK_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_SET_EVENT_MASK_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_SET_ADVERTISING_DATA_REQ
CsrSize DM_HCI_ULP_SET_ADVERTISING_DATA_REQ_TSizeof(void *msg)
{
    DM_HCI_ULP_SET_ADVERTISING_DATA_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_ADVERTISING_DATA_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_SET_ADVERTISING_DATA_REQ
CsrUint8 *DM_HCI_ULP_SET_ADVERTISING_DATA_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_SET_ADVERTISING_DATA_REQ_T *primitive;

    primitive = (DM_HCI_ULP_SET_ADVERTISING_DATA_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->advertising_data_len);
    CsrMemCpySer(buffer, length, ((const void *) primitive->advertising_data), ((CsrSize) (31)));

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_ADVERTISING_DATA_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_SET_ADVERTISING_DATA_REQ
void *DM_HCI_ULP_SET_ADVERTISING_DATA_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_SET_ADVERTISING_DATA_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_SET_ADVERTISING_DATA_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_SET_ADVERTISING_DATA_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->advertising_data_len, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->advertising_data), buffer, &offset, ((CsrSize) (31)));

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_ADVERTISING_DATA_REQ */

#ifndef EXCLUDE_DM_HCI_REMOTE_NAME_REQ_CANCEL_REQ
CsrSize DM_HCI_REMOTE_NAME_REQ_CANCEL_REQ_TSizeof(void *msg)
{
    DM_HCI_REMOTE_NAME_REQ_CANCEL_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_REMOTE_NAME_REQ_CANCEL_REQ */

#ifndef EXCLUDE_DM_HCI_REMOTE_NAME_REQ_CANCEL_REQ
CsrUint8 *DM_HCI_REMOTE_NAME_REQ_CANCEL_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_REMOTE_NAME_REQ_CANCEL_REQ_T *primitive;

    primitive = (DM_HCI_REMOTE_NAME_REQ_CANCEL_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_REMOTE_NAME_REQ_CANCEL_REQ */

#ifndef EXCLUDE_DM_HCI_REMOTE_NAME_REQ_CANCEL_REQ
void *DM_HCI_REMOTE_NAME_REQ_CANCEL_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_REMOTE_NAME_REQ_CANCEL_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_REMOTE_NAME_REQ_CANCEL_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_REMOTE_NAME_REQ_CANCEL_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_REMOTE_NAME_REQ_CANCEL_REQ */

#ifndef EXCLUDE_DM_AM_REGISTER_WRONG_API_REQ
CsrSize DM_AM_REGISTER_WRONG_API_REQ_TSizeof(void *msg)
{
    DM_AM_REGISTER_WRONG_API_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_AM_REGISTER_WRONG_API_REQ */

#ifndef EXCLUDE_DM_AM_REGISTER_WRONG_API_REQ
CsrUint8 *DM_AM_REGISTER_WRONG_API_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_AM_REGISTER_WRONG_API_REQ_T *primitive;

    primitive = (DM_AM_REGISTER_WRONG_API_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);

    return buffer;
}
#endif /* EXCLUDE_DM_AM_REGISTER_WRONG_API_REQ */

#ifndef EXCLUDE_DM_AM_REGISTER_WRONG_API_REQ
void *DM_AM_REGISTER_WRONG_API_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_AM_REGISTER_WRONG_API_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_AM_REGISTER_WRONG_API_REQ_T *) CsrPmemZalloc(sizeof(DM_AM_REGISTER_WRONG_API_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_AM_REGISTER_WRONG_API_REQ */

#ifndef EXCLUDE_DM_SM_AUTHORISE_RSP
CsrSize DM_SM_AUTHORISE_RSP_TSizeof(void *msg)
{
    DM_SM_AUTHORISE_RSP_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_AUTHORISE_RSP */

#ifndef EXCLUDE_DM_SM_AUTHORISE_RSP
CsrUint8 *DM_SM_AUTHORISE_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_AUTHORISE_RSP_T *primitive;

    primitive = (DM_SM_AUTHORISE_RSP_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cs.connection.service.protocol_id);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cs.connection.service.channel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->cs.connection.addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->cs.connection.addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->cs.connection.addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cs.connection.addrt.addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->cs.incoming);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->authorisation);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_AUTHORISE_RSP */

#ifndef EXCLUDE_DM_SM_AUTHORISE_RSP
void *DM_SM_AUTHORISE_RSP_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_AUTHORISE_RSP_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_AUTHORISE_RSP_T *) CsrPmemZalloc(sizeof(DM_SM_AUTHORISE_RSP_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cs.connection.service.protocol_id, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cs.connection.service.channel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->cs.connection.addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->cs.connection.addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->cs.connection.addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cs.connection.addrt.addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->cs.incoming, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->authorisation, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_AUTHORISE_RSP */

#ifndef EXCLUDE_DM_SM_AUTHORISE_IND
CsrSize DM_SM_AUTHORISE_IND_TSizeof(void *msg)
{
    DM_SM_AUTHORISE_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_AUTHORISE_IND */

#ifndef EXCLUDE_DM_SM_AUTHORISE_IND
CsrUint8 *DM_SM_AUTHORISE_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_AUTHORISE_IND_T *primitive;

    primitive = (DM_SM_AUTHORISE_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cs.connection.service.protocol_id);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cs.connection.service.channel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->cs.connection.addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->cs.connection.addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->cs.connection.addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cs.connection.addrt.addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->cs.incoming);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_AUTHORISE_IND */

#ifndef EXCLUDE_DM_SM_AUTHORISE_IND
void *DM_SM_AUTHORISE_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_AUTHORISE_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_AUTHORISE_IND_T *) CsrPmemZalloc(sizeof(DM_SM_AUTHORISE_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cs.connection.service.protocol_id, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cs.connection.service.channel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->cs.connection.addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->cs.connection.addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->cs.connection.addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cs.connection.addrt.addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->cs.incoming, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_AUTHORISE_IND */

#ifndef EXCLUDE_DM_HCI_READ_SCO_FLOW_CON_ENABLE_CFM
CsrSize DM_HCI_READ_SCO_FLOW_CON_ENABLE_CFM_TSizeof(void *msg)
{
    DM_HCI_READ_SCO_FLOW_CON_ENABLE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_SCO_FLOW_CON_ENABLE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_SCO_FLOW_CON_ENABLE_CFM
CsrUint8 *DM_HCI_READ_SCO_FLOW_CON_ENABLE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_SCO_FLOW_CON_ENABLE_CFM_T *primitive;

    primitive = (DM_HCI_READ_SCO_FLOW_CON_ENABLE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->FlowControlEnabled);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_SCO_FLOW_CON_ENABLE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_SCO_FLOW_CON_ENABLE_CFM
void *DM_HCI_READ_SCO_FLOW_CON_ENABLE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_SCO_FLOW_CON_ENABLE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_SCO_FLOW_CON_ENABLE_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_SCO_FLOW_CON_ENABLE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->FlowControlEnabled, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_SCO_FLOW_CON_ENABLE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_PAGESCAN_ACTIVITY_CFM
CsrSize DM_HCI_READ_PAGESCAN_ACTIVITY_CFM_TSizeof(void *msg)
{
    DM_HCI_READ_PAGESCAN_ACTIVITY_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_PAGESCAN_ACTIVITY_CFM */

#ifndef EXCLUDE_DM_HCI_READ_PAGESCAN_ACTIVITY_CFM
CsrUint8 *DM_HCI_READ_PAGESCAN_ACTIVITY_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_PAGESCAN_ACTIVITY_CFM_T *primitive;

    primitive = (DM_HCI_READ_PAGESCAN_ACTIVITY_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pagescan_interval);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pagescan_window);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_PAGESCAN_ACTIVITY_CFM */

#ifndef EXCLUDE_DM_HCI_READ_PAGESCAN_ACTIVITY_CFM
void *DM_HCI_READ_PAGESCAN_ACTIVITY_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_PAGESCAN_ACTIVITY_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_PAGESCAN_ACTIVITY_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_PAGESCAN_ACTIVITY_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pagescan_interval, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pagescan_window, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_PAGESCAN_ACTIVITY_CFM */

#ifndef EXCLUDE_DM_HCI_SNIFF_SUB_RATE_CFM
CsrSize DM_HCI_SNIFF_SUB_RATE_CFM_TSizeof(void *msg)
{
    DM_HCI_SNIFF_SUB_RATE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_SNIFF_SUB_RATE_CFM */

#ifndef EXCLUDE_DM_HCI_SNIFF_SUB_RATE_CFM
CsrUint8 *DM_HCI_SNIFF_SUB_RATE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_SNIFF_SUB_RATE_CFM_T *primitive;

    primitive = (DM_HCI_SNIFF_SUB_RATE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_SNIFF_SUB_RATE_CFM */

#ifndef EXCLUDE_DM_HCI_SNIFF_SUB_RATE_CFM
void *DM_HCI_SNIFF_SUB_RATE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_SNIFF_SUB_RATE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_SNIFF_SUB_RATE_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_SNIFF_SUB_RATE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_SNIFF_SUB_RATE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_BD_ADDR_REQ
CsrSize DM_HCI_READ_BD_ADDR_REQ_TSizeof(void *msg)
{
    DM_HCI_READ_BD_ADDR_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_BD_ADDR_REQ */

#ifndef EXCLUDE_DM_HCI_READ_BD_ADDR_REQ
CsrUint8 *DM_HCI_READ_BD_ADDR_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_BD_ADDR_REQ_T *primitive;

    primitive = (DM_HCI_READ_BD_ADDR_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_BD_ADDR_REQ */

#ifndef EXCLUDE_DM_HCI_READ_BD_ADDR_REQ
void *DM_HCI_READ_BD_ADDR_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_BD_ADDR_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_BD_ADDR_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_BD_ADDR_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_BD_ADDR_REQ */

#ifndef EXCLUDE_DM_SM_REGISTER_REQ
CsrSize DM_SM_REGISTER_REQ_TSizeof(void *msg)
{
    DM_SM_REGISTER_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_REGISTER_REQ */

#ifndef EXCLUDE_DM_SM_REGISTER_REQ
CsrUint8 *DM_SM_REGISTER_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_REGISTER_REQ_T *primitive;

    primitive = (DM_SM_REGISTER_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->service.protocol_id);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->service.channel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->outgoing_ok);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->security_level);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_REGISTER_REQ */

#ifndef EXCLUDE_DM_SM_REGISTER_REQ
void *DM_SM_REGISTER_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_REGISTER_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_REGISTER_REQ_T *) CsrPmemZalloc(sizeof(DM_SM_REGISTER_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->service.protocol_id, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->service.channel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->outgoing_ok, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->security_level, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_REGISTER_REQ */

#ifndef EXCLUDE_DM_SYNC_DISCONNECT_CFM
CsrSize DM_SYNC_DISCONNECT_CFM_TSizeof(void *msg)
{
    DM_SYNC_DISCONNECT_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SYNC_DISCONNECT_CFM */

#ifndef EXCLUDE_DM_SYNC_DISCONNECT_CFM
CsrUint8 *DM_SYNC_DISCONNECT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SYNC_DISCONNECT_CFM_T *primitive;

    primitive = (DM_SYNC_DISCONNECT_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pv_cbarg);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_SYNC_DISCONNECT_CFM */

#ifndef EXCLUDE_DM_SYNC_DISCONNECT_CFM
void *DM_SYNC_DISCONNECT_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SYNC_DISCONNECT_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SYNC_DISCONNECT_CFM_T *) CsrPmemZalloc(sizeof(DM_SYNC_DISCONNECT_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pv_cbarg, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SYNC_DISCONNECT_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_CFM
CsrSize DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_CFM_TSizeof(void *msg)
{
    DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_CFM
CsrUint8 *DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_CFM_T *primitive;

    primitive = (DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_CFM
void *DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_CFM */

#ifndef EXCLUDE_DM_SM_USER_PASSKEY_NOTIFICATION_IND
CsrSize DM_SM_USER_PASSKEY_NOTIFICATION_IND_TSizeof(void *msg)
{
    DM_SM_USER_PASSKEY_NOTIFICATION_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_USER_PASSKEY_NOTIFICATION_IND */

#ifndef EXCLUDE_DM_SM_USER_PASSKEY_NOTIFICATION_IND
CsrUint8 *DM_SM_USER_PASSKEY_NOTIFICATION_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_USER_PASSKEY_NOTIFICATION_IND_T *primitive;

    primitive = (DM_SM_USER_PASSKEY_NOTIFICATION_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->passkey);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->dev_class);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_USER_PASSKEY_NOTIFICATION_IND */

#ifndef EXCLUDE_DM_SM_USER_PASSKEY_NOTIFICATION_IND
void *DM_SM_USER_PASSKEY_NOTIFICATION_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_USER_PASSKEY_NOTIFICATION_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_USER_PASSKEY_NOTIFICATION_IND_T *) CsrPmemZalloc(sizeof(DM_SM_USER_PASSKEY_NOTIFICATION_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->passkey, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->dev_class, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_USER_PASSKEY_NOTIFICATION_IND */

#ifndef EXCLUDE_DM_HCI_WRITE_PAGE_SCAN_TYPE_REQ
CsrSize DM_HCI_WRITE_PAGE_SCAN_TYPE_REQ_TSizeof(void *msg)
{
    DM_HCI_WRITE_PAGE_SCAN_TYPE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_PAGE_SCAN_TYPE_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_PAGE_SCAN_TYPE_REQ
CsrUint8 *DM_HCI_WRITE_PAGE_SCAN_TYPE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_PAGE_SCAN_TYPE_REQ_T *primitive;

    primitive = (DM_HCI_WRITE_PAGE_SCAN_TYPE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mode);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_PAGE_SCAN_TYPE_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_PAGE_SCAN_TYPE_REQ
void *DM_HCI_WRITE_PAGE_SCAN_TYPE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_PAGE_SCAN_TYPE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_PAGE_SCAN_TYPE_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_PAGE_SCAN_TYPE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_PAGE_SCAN_TYPE_REQ */

#ifndef EXCLUDE_DM_SYNC_CONNECT_COMPLETE_IND
CsrSize DM_SYNC_CONNECT_COMPLETE_IND_TSizeof(void *msg)
{
    DM_SYNC_CONNECT_COMPLETE_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SYNC_CONNECT_COMPLETE_IND */

#ifndef EXCLUDE_DM_SYNC_CONNECT_COMPLETE_IND
CsrUint8 *DM_SYNC_CONNECT_COMPLETE_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SYNC_CONNECT_COMPLETE_IND_T *primitive;

    primitive = (DM_SYNC_CONNECT_COMPLETE_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pv_cbarg);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->link_type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tx_interval);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->wesco);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->rx_packet_length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->tx_packet_length);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->air_mode);

    return buffer;
}
#endif /* EXCLUDE_DM_SYNC_CONNECT_COMPLETE_IND */

#ifndef EXCLUDE_DM_SYNC_CONNECT_COMPLETE_IND
void *DM_SYNC_CONNECT_COMPLETE_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SYNC_CONNECT_COMPLETE_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SYNC_CONNECT_COMPLETE_IND_T *) CsrPmemZalloc(sizeof(DM_SYNC_CONNECT_COMPLETE_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pv_cbarg, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->link_type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tx_interval, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->wesco, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->rx_packet_length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->tx_packet_length, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->air_mode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SYNC_CONNECT_COMPLETE_IND */

#ifndef EXCLUDE_DM_HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_CFM
CsrSize DM_HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_CFM_TSizeof(void *msg)
{
    DM_HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_CFM
CsrUint8 *DM_HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_CFM_T *primitive;

    primitive = (DM_HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrMemCpySer(buffer, length, ((const void *) primitive->feature_set), ((CsrSize) (8)));

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_CFM
void *DM_HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->feature_set), buffer, &offset, ((CsrSize) (8)));

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_CFM */

#ifndef EXCLUDE_DM_SM_BONDING_REQ
CsrSize DM_SM_BONDING_REQ_TSizeof(void *msg)
{
    DM_SM_BONDING_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_BONDING_REQ */

#ifndef EXCLUDE_DM_SM_BONDING_REQ
CsrUint8 *DM_SM_BONDING_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_BONDING_REQ_T *primitive;

    primitive = (DM_SM_BONDING_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_BONDING_REQ */

#ifndef EXCLUDE_DM_SM_BONDING_REQ
void *DM_SM_BONDING_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_BONDING_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_BONDING_REQ_T *) CsrPmemZalloc(sizeof(DM_SM_BONDING_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_BONDING_REQ */

#ifndef EXCLUDE_DM_SM_REMOVE_DEVICE_REQ
CsrSize DM_SM_REMOVE_DEVICE_REQ_TSizeof(void *msg)
{
    DM_SM_REMOVE_DEVICE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_REMOVE_DEVICE_REQ */

#ifndef EXCLUDE_DM_SM_REMOVE_DEVICE_REQ
CsrUint8 *DM_SM_REMOVE_DEVICE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_REMOVE_DEVICE_REQ_T *primitive;

    primitive = (DM_SM_REMOVE_DEVICE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_REMOVE_DEVICE_REQ */

#ifndef EXCLUDE_DM_SM_REMOVE_DEVICE_REQ
void *DM_SM_REMOVE_DEVICE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_REMOVE_DEVICE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_REMOVE_DEVICE_REQ_T *) CsrPmemZalloc(sizeof(DM_SM_REMOVE_DEVICE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_REMOVE_DEVICE_REQ */

#ifndef EXCLUDE_DM_LP_WRITE_ROLESWITCH_POLICY_REQ
CsrSize DM_LP_WRITE_ROLESWITCH_POLICY_REQ_TSizeof(void *msg)
{
    DM_LP_WRITE_ROLESWITCH_POLICY_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->length * sizeof(CsrUint16)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_LP_WRITE_ROLESWITCH_POLICY_REQ */

#ifndef EXCLUDE_DM_LP_WRITE_ROLESWITCH_POLICY_REQ
CsrUint8 *DM_LP_WRITE_ROLESWITCH_POLICY_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_LP_WRITE_ROLESWITCH_POLICY_REQ_T *primitive;

    primitive = (DM_LP_WRITE_ROLESWITCH_POLICY_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->version);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->length);
    CsrMemCpySer(buffer, length, &primitive->rs_table, sizeof(primitive->rs_table));
    if(primitive->rs_table)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->rs_table), ((CsrSize) (primitive->length * sizeof(CsrUint16))));
    }

    return buffer;
}
#endif /* EXCLUDE_DM_LP_WRITE_ROLESWITCH_POLICY_REQ */

#ifndef EXCLUDE_DM_LP_WRITE_ROLESWITCH_POLICY_REQ
void *DM_LP_WRITE_ROLESWITCH_POLICY_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_LP_WRITE_ROLESWITCH_POLICY_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_LP_WRITE_ROLESWITCH_POLICY_REQ_T *) CsrPmemZalloc(sizeof(DM_LP_WRITE_ROLESWITCH_POLICY_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->version, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->length, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->rs_table), buffer, &offset, ((CsrSize) sizeof(primitive->rs_table)));
    if (primitive->length)
    {
        primitive->rs_table = CsrPmemZalloc(((CsrUint32) (primitive->length * sizeof(CsrUint16))));
        CsrMemCpyDes(((void *) primitive->rs_table), buffer, &offset, ((CsrSize) (primitive->length * sizeof(CsrUint16))));
    }
    else
    {
        primitive->rs_table = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_DM_LP_WRITE_ROLESWITCH_POLICY_REQ */

#ifndef EXCLUDE_DM_LP_WRITE_ROLESWITCH_POLICY_REQ
void DM_LP_WRITE_ROLESWITCH_POLICY_REQ_TSerFree(void *msg)
{
    DM_LP_WRITE_ROLESWITCH_POLICY_REQ_T *primitive;

    primitive = (DM_LP_WRITE_ROLESWITCH_POLICY_REQ_T *) msg;

    if(primitive->rs_table != NULL)
    {
        CsrPmemFree(primitive->rs_table);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_DM_LP_WRITE_ROLESWITCH_POLICY_REQ */

#ifndef EXCLUDE_DM_SM_SIMPLE_PAIRING_COMPLETE_IND
CsrSize DM_SM_SIMPLE_PAIRING_COMPLETE_IND_TSizeof(void *msg)
{
    DM_SM_SIMPLE_PAIRING_COMPLETE_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_SIMPLE_PAIRING_COMPLETE_IND */

#ifndef EXCLUDE_DM_SM_SIMPLE_PAIRING_COMPLETE_IND
CsrUint8 *DM_SM_SIMPLE_PAIRING_COMPLETE_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_SIMPLE_PAIRING_COMPLETE_IND_T *primitive;

    primitive = (DM_SM_SIMPLE_PAIRING_COMPLETE_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_SIMPLE_PAIRING_COMPLETE_IND */

#ifndef EXCLUDE_DM_SM_SIMPLE_PAIRING_COMPLETE_IND
void *DM_SM_SIMPLE_PAIRING_COMPLETE_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_SIMPLE_PAIRING_COMPLETE_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_SIMPLE_PAIRING_COMPLETE_IND_T *) CsrPmemZalloc(sizeof(DM_SM_SIMPLE_PAIRING_COMPLETE_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_SIMPLE_PAIRING_COMPLETE_IND */

#ifndef EXCLUDE_DM_HCI_READ_STORED_LINK_KEY_REQ
CsrSize DM_HCI_READ_STORED_LINK_KEY_REQ_TSizeof(void *msg)
{
    DM_HCI_READ_STORED_LINK_KEY_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_STORED_LINK_KEY_REQ */

#ifndef EXCLUDE_DM_HCI_READ_STORED_LINK_KEY_REQ
CsrUint8 *DM_HCI_READ_STORED_LINK_KEY_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_STORED_LINK_KEY_REQ_T *primitive;

    primitive = (DM_HCI_READ_STORED_LINK_KEY_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->read_all);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_STORED_LINK_KEY_REQ */

#ifndef EXCLUDE_DM_HCI_READ_STORED_LINK_KEY_REQ
void *DM_HCI_READ_STORED_LINK_KEY_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_STORED_LINK_KEY_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_STORED_LINK_KEY_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_STORED_LINK_KEY_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->read_all, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_STORED_LINK_KEY_REQ */

#ifndef EXCLUDE_DM_HCI_REFRESH_ENCRYPTION_KEY_REQ
CsrSize DM_HCI_REFRESH_ENCRYPTION_KEY_REQ_TSizeof(void *msg)
{
    DM_HCI_REFRESH_ENCRYPTION_KEY_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_REFRESH_ENCRYPTION_KEY_REQ */

#ifndef EXCLUDE_DM_HCI_REFRESH_ENCRYPTION_KEY_REQ
CsrUint8 *DM_HCI_REFRESH_ENCRYPTION_KEY_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_REFRESH_ENCRYPTION_KEY_REQ_T *primitive;

    primitive = (DM_HCI_REFRESH_ENCRYPTION_KEY_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_REFRESH_ENCRYPTION_KEY_REQ */

#ifndef EXCLUDE_DM_HCI_REFRESH_ENCRYPTION_KEY_REQ
void *DM_HCI_REFRESH_ENCRYPTION_KEY_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_REFRESH_ENCRYPTION_KEY_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_REFRESH_ENCRYPTION_KEY_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_REFRESH_ENCRYPTION_KEY_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_REFRESH_ENCRYPTION_KEY_REQ */

#ifndef EXCLUDE_DM_BLUESTACK_STATE_TRANSITION_EXIT_IND
CsrSize DM_BLUESTACK_STATE_TRANSITION_EXIT_IND_TSizeof(void *msg)
{
    DM_BLUESTACK_STATE_TRANSITION_EXIT_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_BLUESTACK_STATE_TRANSITION_EXIT_IND */

#ifndef EXCLUDE_DM_BLUESTACK_STATE_TRANSITION_EXIT_IND
CsrUint8 *DM_BLUESTACK_STATE_TRANSITION_EXIT_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_BLUESTACK_STATE_TRANSITION_EXIT_IND_T *primitive;

    primitive = (DM_BLUESTACK_STATE_TRANSITION_EXIT_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->state_machine);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->event);

    return buffer;
}
#endif /* EXCLUDE_DM_BLUESTACK_STATE_TRANSITION_EXIT_IND */

#ifndef EXCLUDE_DM_BLUESTACK_STATE_TRANSITION_EXIT_IND
void *DM_BLUESTACK_STATE_TRANSITION_EXIT_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_BLUESTACK_STATE_TRANSITION_EXIT_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_BLUESTACK_STATE_TRANSITION_EXIT_IND_T *) CsrPmemZalloc(sizeof(DM_BLUESTACK_STATE_TRANSITION_EXIT_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->state_machine, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->event, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_BLUESTACK_STATE_TRANSITION_EXIT_IND */

#ifndef EXCLUDE_DM_HCI_WRITE_INQUIRY_SCAN_TYPE_REQ
CsrSize DM_HCI_WRITE_INQUIRY_SCAN_TYPE_REQ_TSizeof(void *msg)
{
    DM_HCI_WRITE_INQUIRY_SCAN_TYPE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_INQUIRY_SCAN_TYPE_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_INQUIRY_SCAN_TYPE_REQ
CsrUint8 *DM_HCI_WRITE_INQUIRY_SCAN_TYPE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_INQUIRY_SCAN_TYPE_REQ_T *primitive;

    primitive = (DM_HCI_WRITE_INQUIRY_SCAN_TYPE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mode);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_INQUIRY_SCAN_TYPE_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_INQUIRY_SCAN_TYPE_REQ
void *DM_HCI_WRITE_INQUIRY_SCAN_TYPE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_INQUIRY_SCAN_TYPE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_INQUIRY_SCAN_TYPE_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_INQUIRY_SCAN_TYPE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_INQUIRY_SCAN_TYPE_REQ */

#ifndef EXCLUDE_DM_HCI_QOS_SETUP_CFM
CsrSize DM_HCI_QOS_SETUP_CFM_TSizeof(void *msg)
{
    DM_HCI_QOS_SETUP_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_QOS_SETUP_CFM */

#ifndef EXCLUDE_DM_HCI_QOS_SETUP_CFM
CsrUint8 *DM_HCI_QOS_SETUP_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_QOS_SETUP_CFM_T *primitive;

    primitive = (DM_HCI_QOS_SETUP_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->flags);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->service_type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->token_rate);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->peak_bandwidth);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->latency);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->delay_variation);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_QOS_SETUP_CFM */

#ifndef EXCLUDE_DM_HCI_QOS_SETUP_CFM
void *DM_HCI_QOS_SETUP_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_QOS_SETUP_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_QOS_SETUP_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_QOS_SETUP_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->flags, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->service_type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->token_rate, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->peak_bandwidth, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->latency, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->delay_variation, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_QOS_SETUP_CFM */

#ifndef EXCLUDE_DM_HANDLE_DEVICE_BLACK_LIST_CFM
CsrSize DM_HANDLE_DEVICE_BLACK_LIST_CFM_TSizeof(void *msg)
{
    DM_HANDLE_DEVICE_BLACK_LIST_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HANDLE_DEVICE_BLACK_LIST_CFM */

#ifndef EXCLUDE_DM_HANDLE_DEVICE_BLACK_LIST_CFM
CsrUint8 *DM_HANDLE_DEVICE_BLACK_LIST_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HANDLE_DEVICE_BLACK_LIST_CFM_T *primitive;

    primitive = (DM_HANDLE_DEVICE_BLACK_LIST_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HANDLE_DEVICE_BLACK_LIST_CFM */

#ifndef EXCLUDE_DM_HANDLE_DEVICE_BLACK_LIST_CFM
void *DM_HANDLE_DEVICE_BLACK_LIST_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HANDLE_DEVICE_BLACK_LIST_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HANDLE_DEVICE_BLACK_LIST_CFM_T *) CsrPmemZalloc(sizeof(DM_HANDLE_DEVICE_BLACK_LIST_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HANDLE_DEVICE_BLACK_LIST_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_INQUIRY_MODE_REQ
CsrSize DM_HCI_WRITE_INQUIRY_MODE_REQ_TSizeof(void *msg)
{
    DM_HCI_WRITE_INQUIRY_MODE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_INQUIRY_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_INQUIRY_MODE_REQ
CsrUint8 *DM_HCI_WRITE_INQUIRY_MODE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_INQUIRY_MODE_REQ_T *primitive;

    primitive = (DM_HCI_WRITE_INQUIRY_MODE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mode);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_INQUIRY_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_INQUIRY_MODE_REQ
void *DM_HCI_WRITE_INQUIRY_MODE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_INQUIRY_MODE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_INQUIRY_MODE_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_INQUIRY_MODE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_INQUIRY_MODE_REQ */

#ifndef EXCLUDE_DM_AMPM_REGISTER_CFM
CsrSize DM_AMPM_REGISTER_CFM_TSizeof(void *msg)
{
    DM_AMPM_REGISTER_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_AMPM_REGISTER_CFM */

#ifndef EXCLUDE_DM_AMPM_REGISTER_CFM
CsrUint8 *DM_AMPM_REGISTER_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_AMPM_REGISTER_CFM_T *primitive;

    primitive = (DM_AMPM_REGISTER_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);

    return buffer;
}
#endif /* EXCLUDE_DM_AMPM_REGISTER_CFM */

#ifndef EXCLUDE_DM_AMPM_REGISTER_CFM
void *DM_AMPM_REGISTER_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_AMPM_REGISTER_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_AMPM_REGISTER_CFM_T *) CsrPmemZalloc(sizeof(DM_AMPM_REGISTER_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_AMPM_REGISTER_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_SCAN_ENABLE_REQ
CsrSize DM_HCI_WRITE_SCAN_ENABLE_REQ_TSizeof(void *msg)
{
    DM_HCI_WRITE_SCAN_ENABLE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_SCAN_ENABLE_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_SCAN_ENABLE_REQ
CsrUint8 *DM_HCI_WRITE_SCAN_ENABLE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_SCAN_ENABLE_REQ_T *primitive;

    primitive = (DM_HCI_WRITE_SCAN_ENABLE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->scan_enable);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_SCAN_ENABLE_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_SCAN_ENABLE_REQ
void *DM_HCI_WRITE_SCAN_ENABLE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_SCAN_ENABLE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_SCAN_ENABLE_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_SCAN_ENABLE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->scan_enable, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_SCAN_ENABLE_REQ */

#ifndef EXCLUDE_DM_HCI_INQUIRY_REQ
CsrSize DM_HCI_INQUIRY_REQ_TSizeof(void *msg)
{
    DM_HCI_INQUIRY_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_INQUIRY_REQ */

#ifndef EXCLUDE_DM_HCI_INQUIRY_REQ
CsrUint8 *DM_HCI_INQUIRY_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_INQUIRY_REQ_T *primitive;

    primitive = (DM_HCI_INQUIRY_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->inquiry_length);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->num_responses);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_INQUIRY_REQ */

#ifndef EXCLUDE_DM_HCI_INQUIRY_REQ
void *DM_HCI_INQUIRY_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_INQUIRY_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_INQUIRY_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_INQUIRY_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->inquiry_length, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->num_responses, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_INQUIRY_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_READ_REMOTE_USED_FEATURES_REQ
CsrSize DM_HCI_ULP_READ_REMOTE_USED_FEATURES_REQ_TSizeof(void *msg)
{
    DM_HCI_ULP_READ_REMOTE_USED_FEATURES_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_READ_REMOTE_USED_FEATURES_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_READ_REMOTE_USED_FEATURES_REQ
CsrUint8 *DM_HCI_ULP_READ_REMOTE_USED_FEATURES_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_READ_REMOTE_USED_FEATURES_REQ_T *primitive;

    primitive = (DM_HCI_ULP_READ_REMOTE_USED_FEATURES_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->connection_handle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_READ_REMOTE_USED_FEATURES_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_READ_REMOTE_USED_FEATURES_REQ
void *DM_HCI_ULP_READ_REMOTE_USED_FEATURES_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_READ_REMOTE_USED_FEATURES_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_READ_REMOTE_USED_FEATURES_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_READ_REMOTE_USED_FEATURES_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->connection_handle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_READ_REMOTE_USED_FEATURES_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_CFM
CsrSize DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_CFM_TSizeof(void *msg)
{
    DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_CFM
CsrUint8 *DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_CFM_T *primitive;

    primitive = (DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_CFM
void *DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_CFM */

#ifndef EXCLUDE_DM_SM_USER_PASSKEY_REQUEST_RSP
CsrSize DM_SM_USER_PASSKEY_REQUEST_RSP_TSizeof(void *msg)
{
    DM_SM_USER_PASSKEY_REQUEST_RSP_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_USER_PASSKEY_REQUEST_RSP */

#ifndef EXCLUDE_DM_SM_USER_PASSKEY_REQUEST_RSP
CsrUint8 *DM_SM_USER_PASSKEY_REQUEST_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_USER_PASSKEY_REQUEST_RSP_T *primitive;

    primitive = (DM_SM_USER_PASSKEY_REQUEST_RSP_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->numeric_value);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_USER_PASSKEY_REQUEST_RSP */

#ifndef EXCLUDE_DM_SM_USER_PASSKEY_REQUEST_RSP
void *DM_SM_USER_PASSKEY_REQUEST_RSP_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_USER_PASSKEY_REQUEST_RSP_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_USER_PASSKEY_REQUEST_RSP_T *) CsrPmemZalloc(sizeof(DM_SM_USER_PASSKEY_REQUEST_RSP_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->numeric_value, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_USER_PASSKEY_REQUEST_RSP */

#ifndef EXCLUDE_DM_BLE_UPDATE_CONNECTION_PARAMETERS_CFM
CsrSize DM_BLE_UPDATE_CONNECTION_PARAMETERS_CFM_TSizeof(void *msg)
{
    DM_BLE_UPDATE_CONNECTION_PARAMETERS_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_BLE_UPDATE_CONNECTION_PARAMETERS_CFM */

#ifndef EXCLUDE_DM_BLE_UPDATE_CONNECTION_PARAMETERS_CFM
CsrUint8 *DM_BLE_UPDATE_CONNECTION_PARAMETERS_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_BLE_UPDATE_CONNECTION_PARAMETERS_CFM_T *primitive;

    primitive = (DM_BLE_UPDATE_CONNECTION_PARAMETERS_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_BLE_UPDATE_CONNECTION_PARAMETERS_CFM */

#ifndef EXCLUDE_DM_BLE_UPDATE_CONNECTION_PARAMETERS_CFM
void *DM_BLE_UPDATE_CONNECTION_PARAMETERS_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_BLE_UPDATE_CONNECTION_PARAMETERS_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_BLE_UPDATE_CONNECTION_PARAMETERS_CFM_T *) CsrPmemZalloc(sizeof(DM_BLE_UPDATE_CONNECTION_PARAMETERS_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_BLE_UPDATE_CONNECTION_PARAMETERS_CFM */

#ifndef EXCLUDE_DM_SET_BLE_CONNECTION_PARAMETERS_CFM
CsrSize DM_SET_BLE_CONNECTION_PARAMETERS_CFM_TSizeof(void *msg)
{
    DM_SET_BLE_CONNECTION_PARAMETERS_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SET_BLE_CONNECTION_PARAMETERS_CFM */

#ifndef EXCLUDE_DM_SET_BLE_CONNECTION_PARAMETERS_CFM
CsrUint8 *DM_SET_BLE_CONNECTION_PARAMETERS_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SET_BLE_CONNECTION_PARAMETERS_CFM_T *primitive;

    primitive = (DM_SET_BLE_CONNECTION_PARAMETERS_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_SET_BLE_CONNECTION_PARAMETERS_CFM */

#ifndef EXCLUDE_DM_SET_BLE_CONNECTION_PARAMETERS_CFM
void *DM_SET_BLE_CONNECTION_PARAMETERS_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SET_BLE_CONNECTION_PARAMETERS_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SET_BLE_CONNECTION_PARAMETERS_CFM_T *) CsrPmemZalloc(sizeof(DM_SET_BLE_CONNECTION_PARAMETERS_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SET_BLE_CONNECTION_PARAMETERS_CFM */

#ifndef EXCLUDE_DM_CLEAR_PARAM_CACHE_REQ
CsrSize DM_CLEAR_PARAM_CACHE_REQ_TSizeof(void *msg)
{
    DM_CLEAR_PARAM_CACHE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_CLEAR_PARAM_CACHE_REQ */

#ifndef EXCLUDE_DM_CLEAR_PARAM_CACHE_REQ
CsrUint8 *DM_CLEAR_PARAM_CACHE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_CLEAR_PARAM_CACHE_REQ_T *primitive;

    primitive = (DM_CLEAR_PARAM_CACHE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_CLEAR_PARAM_CACHE_REQ */

#ifndef EXCLUDE_DM_CLEAR_PARAM_CACHE_REQ
void *DM_CLEAR_PARAM_CACHE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_CLEAR_PARAM_CACHE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_CLEAR_PARAM_CACHE_REQ_T *) CsrPmemZalloc(sizeof(DM_CLEAR_PARAM_CACHE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_CLEAR_PARAM_CACHE_REQ */

#ifndef EXCLUDE_DM_SM_USER_CONFIRMATION_REQUEST_RSP
CsrSize DM_SM_USER_CONFIRMATION_REQUEST_RSP_TSizeof(void *msg)
{
    DM_SM_USER_CONFIRMATION_REQUEST_RSP_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_USER_CONFIRMATION_REQUEST_RSP */

#ifndef EXCLUDE_DM_SM_USER_CONFIRMATION_REQUEST_RSP
CsrUint8 *DM_SM_USER_CONFIRMATION_REQUEST_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_USER_CONFIRMATION_REQUEST_RSP_T *primitive;

    primitive = (DM_SM_USER_CONFIRMATION_REQUEST_RSP_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_USER_CONFIRMATION_REQUEST_RSP */

#ifndef EXCLUDE_DM_SM_USER_CONFIRMATION_REQUEST_RSP
void *DM_SM_USER_CONFIRMATION_REQUEST_RSP_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_USER_CONFIRMATION_REQUEST_RSP_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_USER_CONFIRMATION_REQUEST_RSP_T *) CsrPmemZalloc(sizeof(DM_SM_USER_CONFIRMATION_REQUEST_RSP_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_USER_CONFIRMATION_REQUEST_RSP */

#ifndef EXCLUDE_DM_HCI_READ_LOCAL_NAME_REQ
CsrSize DM_HCI_READ_LOCAL_NAME_REQ_TSizeof(void *msg)
{
    DM_HCI_READ_LOCAL_NAME_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_LOCAL_NAME_REQ */

#ifndef EXCLUDE_DM_HCI_READ_LOCAL_NAME_REQ
CsrUint8 *DM_HCI_READ_LOCAL_NAME_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_LOCAL_NAME_REQ_T *primitive;

    primitive = (DM_HCI_READ_LOCAL_NAME_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_LOCAL_NAME_REQ */

#ifndef EXCLUDE_DM_HCI_READ_LOCAL_NAME_REQ
void *DM_HCI_READ_LOCAL_NAME_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_LOCAL_NAME_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_LOCAL_NAME_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_LOCAL_NAME_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_LOCAL_NAME_REQ */

#ifndef EXCLUDE_DM_AM_REGISTER_REQ
CsrSize DM_AM_REGISTER_REQ_TSizeof(void *msg)
{
    DM_AM_REGISTER_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_AM_REGISTER_REQ */

#ifndef EXCLUDE_DM_AM_REGISTER_REQ
CsrUint8 *DM_AM_REGISTER_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_AM_REGISTER_REQ_T *primitive;

    primitive = (DM_AM_REGISTER_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);

    return buffer;
}
#endif /* EXCLUDE_DM_AM_REGISTER_REQ */

#ifndef EXCLUDE_DM_AM_REGISTER_REQ
void *DM_AM_REGISTER_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_AM_REGISTER_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_AM_REGISTER_REQ_T *) CsrPmemZalloc(sizeof(DM_AM_REGISTER_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_AM_REGISTER_REQ */

#ifndef EXCLUDE_DM_HCI_CREATE_NEW_UNIT_KEY_CFM
CsrSize DM_HCI_CREATE_NEW_UNIT_KEY_CFM_TSizeof(void *msg)
{
    DM_HCI_CREATE_NEW_UNIT_KEY_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_CREATE_NEW_UNIT_KEY_CFM */

#ifndef EXCLUDE_DM_HCI_CREATE_NEW_UNIT_KEY_CFM
CsrUint8 *DM_HCI_CREATE_NEW_UNIT_KEY_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_CREATE_NEW_UNIT_KEY_CFM_T *primitive;

    primitive = (DM_HCI_CREATE_NEW_UNIT_KEY_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_CREATE_NEW_UNIT_KEY_CFM */

#ifndef EXCLUDE_DM_HCI_CREATE_NEW_UNIT_KEY_CFM
void *DM_HCI_CREATE_NEW_UNIT_KEY_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_CREATE_NEW_UNIT_KEY_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_CREATE_NEW_UNIT_KEY_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_CREATE_NEW_UNIT_KEY_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_CREATE_NEW_UNIT_KEY_CFM */

#ifndef EXCLUDE_DM_HCI_CREATE_CONNECTION_CANCEL_CFM
CsrSize DM_HCI_CREATE_CONNECTION_CANCEL_CFM_TSizeof(void *msg)
{
    DM_HCI_CREATE_CONNECTION_CANCEL_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_CREATE_CONNECTION_CANCEL_CFM */

#ifndef EXCLUDE_DM_HCI_CREATE_CONNECTION_CANCEL_CFM
CsrUint8 *DM_HCI_CREATE_CONNECTION_CANCEL_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_CREATE_CONNECTION_CANCEL_CFM_T *primitive;

    primitive = (DM_HCI_CREATE_CONNECTION_CANCEL_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_CREATE_CONNECTION_CANCEL_CFM */

#ifndef EXCLUDE_DM_HCI_CREATE_CONNECTION_CANCEL_CFM
void *DM_HCI_CREATE_CONNECTION_CANCEL_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_CREATE_CONNECTION_CANCEL_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_CREATE_CONNECTION_CANCEL_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_CREATE_CONNECTION_CANCEL_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_CREATE_CONNECTION_CANCEL_CFM */

#ifndef EXCLUDE_DM_HCI_READ_SCAN_ENABLE_REQ
CsrSize DM_HCI_READ_SCAN_ENABLE_REQ_TSizeof(void *msg)
{
    DM_HCI_READ_SCAN_ENABLE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_SCAN_ENABLE_REQ */

#ifndef EXCLUDE_DM_HCI_READ_SCAN_ENABLE_REQ
CsrUint8 *DM_HCI_READ_SCAN_ENABLE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_SCAN_ENABLE_REQ_T *primitive;

    primitive = (DM_HCI_READ_SCAN_ENABLE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_SCAN_ENABLE_REQ */

#ifndef EXCLUDE_DM_HCI_READ_SCAN_ENABLE_REQ
void *DM_HCI_READ_SCAN_ENABLE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_SCAN_ENABLE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_SCAN_ENABLE_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_SCAN_ENABLE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_SCAN_ENABLE_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_CFM
CsrSize DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_CFM_TSizeof(void *msg)
{
    DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_CFM
CsrUint8 *DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_CFM_T *primitive;

    primitive = (DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_CFM
void *DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_CFM */

#ifndef EXCLUDE_DM_SM_REGISTER_CFM
CsrSize DM_SM_REGISTER_CFM_TSizeof(void *msg)
{
    DM_SM_REGISTER_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_REGISTER_CFM */

#ifndef EXCLUDE_DM_SM_REGISTER_CFM
CsrUint8 *DM_SM_REGISTER_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_REGISTER_CFM_T *primitive;

    primitive = (DM_SM_REGISTER_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->service.protocol_id);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->service.channel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->outgoing_ok);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->security_level);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_REGISTER_CFM */

#ifndef EXCLUDE_DM_SM_REGISTER_CFM
void *DM_SM_REGISTER_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_REGISTER_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_REGISTER_CFM_T *) CsrPmemZalloc(sizeof(DM_SM_REGISTER_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->service.protocol_id, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->service.channel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->outgoing_ok, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->security_level, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_REGISTER_CFM */

#ifndef EXCLUDE_DM_SM_CONFIGURE_LOCAL_ADDRESS_CFM
CsrSize DM_SM_CONFIGURE_LOCAL_ADDRESS_CFM_TSizeof(void *msg)
{
    DM_SM_CONFIGURE_LOCAL_ADDRESS_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_CONFIGURE_LOCAL_ADDRESS_CFM */

#ifndef EXCLUDE_DM_SM_CONFIGURE_LOCAL_ADDRESS_CFM
CsrUint8 *DM_SM_CONFIGURE_LOCAL_ADDRESS_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_CONFIGURE_LOCAL_ADDRESS_CFM_T *primitive;

    primitive = (DM_SM_CONFIGURE_LOCAL_ADDRESS_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->permanent_address_type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->static_addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->static_addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->static_addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->static_addrt.addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->privacy);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_CONFIGURE_LOCAL_ADDRESS_CFM */

#ifndef EXCLUDE_DM_SM_CONFIGURE_LOCAL_ADDRESS_CFM
void *DM_SM_CONFIGURE_LOCAL_ADDRESS_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_CONFIGURE_LOCAL_ADDRESS_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_CONFIGURE_LOCAL_ADDRESS_CFM_T *) CsrPmemZalloc(sizeof(DM_SM_CONFIGURE_LOCAL_ADDRESS_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->permanent_address_type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->static_addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->static_addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->static_addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->static_addrt.addr.nap, buffer, &offset);
    {
        CsrUint16 data;
        CsrUint16Des((CsrUint16 *) &data, buffer, &offset);
        primitive->privacy = (DM_SM_PRIVACY_T) data;
    }

    return primitive;
}
#endif /* EXCLUDE_DM_SM_CONFIGURE_LOCAL_ADDRESS_CFM */

#ifndef EXCLUDE_DM_AMPM_READ_BD_ADDR_REQ
CsrSize DM_AMPM_READ_BD_ADDR_REQ_TSizeof(void *msg)
{
    DM_AMPM_READ_BD_ADDR_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_AMPM_READ_BD_ADDR_REQ */

#ifndef EXCLUDE_DM_AMPM_READ_BD_ADDR_REQ
CsrUint8 *DM_AMPM_READ_BD_ADDR_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_AMPM_READ_BD_ADDR_REQ_T *primitive;

    primitive = (DM_AMPM_READ_BD_ADDR_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_DM_AMPM_READ_BD_ADDR_REQ */

#ifndef EXCLUDE_DM_AMPM_READ_BD_ADDR_REQ
void *DM_AMPM_READ_BD_ADDR_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_AMPM_READ_BD_ADDR_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_AMPM_READ_BD_ADDR_REQ_T *) CsrPmemZalloc(sizeof(DM_AMPM_READ_BD_ADDR_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_AMPM_READ_BD_ADDR_REQ */

#ifndef EXCLUDE_DM_SM_IO_CAPABILITY_REQUEST_IND
CsrSize DM_SM_IO_CAPABILITY_REQUEST_IND_TSizeof(void *msg)
{
    DM_SM_IO_CAPABILITY_REQUEST_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_IO_CAPABILITY_REQUEST_IND */

#ifndef EXCLUDE_DM_SM_IO_CAPABILITY_REQUEST_IND
CsrUint8 *DM_SM_IO_CAPABILITY_REQUEST_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_IO_CAPABILITY_REQUEST_IND_T *primitive;

    primitive = (DM_SM_IO_CAPABILITY_REQUEST_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->dev_class);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_IO_CAPABILITY_REQUEST_IND */

#ifndef EXCLUDE_DM_SM_IO_CAPABILITY_REQUEST_IND
void *DM_SM_IO_CAPABILITY_REQUEST_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_IO_CAPABILITY_REQUEST_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_IO_CAPABILITY_REQUEST_IND_T *) CsrPmemZalloc(sizeof(DM_SM_IO_CAPABILITY_REQUEST_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->dev_class, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_IO_CAPABILITY_REQUEST_IND */

#ifndef EXCLUDE_DM_HCI_READ_REMOTE_SUPP_FEATURES_REQ
CsrSize DM_HCI_READ_REMOTE_SUPP_FEATURES_REQ_TSizeof(void *msg)
{
    DM_HCI_READ_REMOTE_SUPP_FEATURES_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_REMOTE_SUPP_FEATURES_REQ */

#ifndef EXCLUDE_DM_HCI_READ_REMOTE_SUPP_FEATURES_REQ
CsrUint8 *DM_HCI_READ_REMOTE_SUPP_FEATURES_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_REMOTE_SUPP_FEATURES_REQ_T *primitive;

    primitive = (DM_HCI_READ_REMOTE_SUPP_FEATURES_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_REMOTE_SUPP_FEATURES_REQ */

#ifndef EXCLUDE_DM_HCI_READ_REMOTE_SUPP_FEATURES_REQ
void *DM_HCI_READ_REMOTE_SUPP_FEATURES_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_REMOTE_SUPP_FEATURES_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_REMOTE_SUPP_FEATURES_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_REMOTE_SUPP_FEATURES_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_REMOTE_SUPP_FEATURES_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_ENCRYPT_REQ
CsrSize DM_HCI_ULP_ENCRYPT_REQ_TSizeof(void *msg)
{
    DM_HCI_ULP_ENCRYPT_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_ENCRYPT_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_ENCRYPT_REQ
CsrUint8 *DM_HCI_ULP_ENCRYPT_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_ENCRYPT_REQ_T *primitive;

    primitive = (DM_HCI_ULP_ENCRYPT_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrMemCpySer(buffer, length, ((const void *) primitive->aes_key), ((CsrSize) (16)));
    CsrMemCpySer(buffer, length, ((const void *) primitive->plaintext_data), ((CsrSize) (16)));

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_ENCRYPT_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_ENCRYPT_REQ
void *DM_HCI_ULP_ENCRYPT_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_ENCRYPT_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_ENCRYPT_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_ENCRYPT_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->aes_key), buffer, &offset, ((CsrSize) (16)));
    CsrMemCpyDes(((void *) primitive->plaintext_data), buffer, &offset, ((CsrSize) (16)));

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_ENCRYPT_REQ */

#ifndef EXCLUDE_DM_HCI_READ_CLOCK_OFFSET_REQ
CsrSize DM_HCI_READ_CLOCK_OFFSET_REQ_TSizeof(void *msg)
{
    DM_HCI_READ_CLOCK_OFFSET_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_CLOCK_OFFSET_REQ */

#ifndef EXCLUDE_DM_HCI_READ_CLOCK_OFFSET_REQ
CsrUint8 *DM_HCI_READ_CLOCK_OFFSET_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_CLOCK_OFFSET_REQ_T *primitive;

    primitive = (DM_HCI_READ_CLOCK_OFFSET_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_CLOCK_OFFSET_REQ */

#ifndef EXCLUDE_DM_HCI_READ_CLOCK_OFFSET_REQ
void *DM_HCI_READ_CLOCK_OFFSET_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_CLOCK_OFFSET_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_CLOCK_OFFSET_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_CLOCK_OFFSET_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_CLOCK_OFFSET_REQ */

#ifndef EXCLUDE_DM_HCI_READ_LOCAL_EXT_FEATURES_CFM
CsrSize DM_HCI_READ_LOCAL_EXT_FEATURES_CFM_TSizeof(void *msg)
{
    DM_HCI_READ_LOCAL_EXT_FEATURES_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_LOCAL_EXT_FEATURES_CFM */

#ifndef EXCLUDE_DM_HCI_READ_LOCAL_EXT_FEATURES_CFM
CsrUint8 *DM_HCI_READ_LOCAL_EXT_FEATURES_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_LOCAL_EXT_FEATURES_CFM_T *primitive;

    primitive = (DM_HCI_READ_LOCAL_EXT_FEATURES_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->page_num);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->max_page_num);
    CsrMemCpySer(buffer, length, ((const void *) primitive->lmp_ext_features), ((CsrSize) (8)));

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_LOCAL_EXT_FEATURES_CFM */

#ifndef EXCLUDE_DM_HCI_READ_LOCAL_EXT_FEATURES_CFM
void *DM_HCI_READ_LOCAL_EXT_FEATURES_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_LOCAL_EXT_FEATURES_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_LOCAL_EXT_FEATURES_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_LOCAL_EXT_FEATURES_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->page_num, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->max_page_num, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->lmp_ext_features), buffer, &offset, ((CsrSize) (8)));

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_LOCAL_EXT_FEATURES_CFM */

#ifndef EXCLUDE_DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_CFM
CsrSize DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_CFM_TSizeof(void *msg)
{
    DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_CFM */

#ifndef EXCLUDE_DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_CFM
CsrUint8 *DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_CFM_T *primitive;

    primitive = (DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tx_power);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_CFM */

#ifndef EXCLUDE_DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_CFM
void *DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tx_power, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_REQ
CsrSize DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_REQ_TSizeof(void *msg)
{
    DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_REQ
CsrUint8 *DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_REQ_T *primitive;

    primitive = (DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_REQ
void *DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_CFM
CsrSize DM_HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_CFM_TSizeof(void *msg)
{
    DM_HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_CFM
CsrUint8 *DM_HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_CFM_T *primitive;

    primitive = (DM_HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_CFM
void *DM_HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_CFM */

#ifndef EXCLUDE_DM_SM_USER_PASSKEY_REQUEST_NEG_RSP
CsrSize DM_SM_USER_PASSKEY_REQUEST_NEG_RSP_TSizeof(void *msg)
{
    DM_SM_USER_PASSKEY_REQUEST_NEG_RSP_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_USER_PASSKEY_REQUEST_NEG_RSP */

#ifndef EXCLUDE_DM_SM_USER_PASSKEY_REQUEST_NEG_RSP
CsrUint8 *DM_SM_USER_PASSKEY_REQUEST_NEG_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_USER_PASSKEY_REQUEST_NEG_RSP_T *primitive;

    primitive = (DM_SM_USER_PASSKEY_REQUEST_NEG_RSP_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_USER_PASSKEY_REQUEST_NEG_RSP */

#ifndef EXCLUDE_DM_SM_USER_PASSKEY_REQUEST_NEG_RSP
void *DM_SM_USER_PASSKEY_REQUEST_NEG_RSP_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_USER_PASSKEY_REQUEST_NEG_RSP_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_USER_PASSKEY_REQUEST_NEG_RSP_T *) CsrPmemZalloc(sizeof(DM_SM_USER_PASSKEY_REQUEST_NEG_RSP_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_USER_PASSKEY_REQUEST_NEG_RSP */

#ifndef EXCLUDE_DM_SYNC_RENEGOTIATE_IND
CsrSize DM_SYNC_RENEGOTIATE_IND_TSizeof(void *msg)
{
    DM_SYNC_RENEGOTIATE_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SYNC_RENEGOTIATE_IND */

#ifndef EXCLUDE_DM_SYNC_RENEGOTIATE_IND
CsrUint8 *DM_SYNC_RENEGOTIATE_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SYNC_RENEGOTIATE_IND_T *primitive;

    primitive = (DM_SYNC_RENEGOTIATE_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pv_cbarg);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tx_interval);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->wesco);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->rx_packet_length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->tx_packet_length);

    return buffer;
}
#endif /* EXCLUDE_DM_SYNC_RENEGOTIATE_IND */

#ifndef EXCLUDE_DM_SYNC_RENEGOTIATE_IND
void *DM_SYNC_RENEGOTIATE_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SYNC_RENEGOTIATE_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SYNC_RENEGOTIATE_IND_T *) CsrPmemZalloc(sizeof(DM_SYNC_RENEGOTIATE_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pv_cbarg, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tx_interval, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->wesco, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->rx_packet_length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->tx_packet_length, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SYNC_RENEGOTIATE_IND */

#ifndef EXCLUDE_DM_SM_SECURITY_REQ
CsrSize DM_SM_SECURITY_REQ_TSizeof(void *msg)
{
    DM_SM_SECURITY_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_SECURITY_REQ */

#ifndef EXCLUDE_DM_SM_SECURITY_REQ
CsrUint8 *DM_SM_SECURITY_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_SECURITY_REQ_T *primitive;

    primitive = (DM_SM_SECURITY_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->connection_flags);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->security_requirements);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_SECURITY_REQ */

#ifndef EXCLUDE_DM_SM_SECURITY_REQ
void *DM_SM_SECURITY_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_SECURITY_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_SECURITY_REQ_T *) CsrPmemZalloc(sizeof(DM_SM_SECURITY_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->connection_flags, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->security_requirements, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_SECURITY_REQ */

#ifndef EXCLUDE_DM_SM_SECURITY_CFM
CsrSize DM_SM_SECURITY_CFM_TSizeof(void *msg)
{
    DM_SM_SECURITY_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_SECURITY_CFM */

#ifndef EXCLUDE_DM_SM_SECURITY_CFM
CsrUint8 *DM_SM_SECURITY_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_SECURITY_CFM_T *primitive;

    primitive = (DM_SM_SECURITY_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->connection_flags);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->security_requirements);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_SECURITY_CFM */

#ifndef EXCLUDE_DM_SM_SECURITY_CFM
void *DM_SM_SECURITY_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_SECURITY_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_SECURITY_CFM_T *) CsrPmemZalloc(sizeof(DM_SM_SECURITY_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->connection_flags, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->security_requirements, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_SECURITY_CFM */

#ifndef EXCLUDE_DM_SET_DEFAULT_LINK_POLICY_REQ
CsrSize DM_SET_DEFAULT_LINK_POLICY_REQ_TSizeof(void *msg)
{
    DM_SET_DEFAULT_LINK_POLICY_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SET_DEFAULT_LINK_POLICY_REQ */

#ifndef EXCLUDE_DM_SET_DEFAULT_LINK_POLICY_REQ
CsrUint8 *DM_SET_DEFAULT_LINK_POLICY_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SET_DEFAULT_LINK_POLICY_REQ_T *primitive;

    primitive = (DM_SET_DEFAULT_LINK_POLICY_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->link_policy_settings_in);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->link_policy_settings_out);

    return buffer;
}
#endif /* EXCLUDE_DM_SET_DEFAULT_LINK_POLICY_REQ */

#ifndef EXCLUDE_DM_SET_DEFAULT_LINK_POLICY_REQ
void *DM_SET_DEFAULT_LINK_POLICY_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SET_DEFAULT_LINK_POLICY_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SET_DEFAULT_LINK_POLICY_REQ_T *) CsrPmemZalloc(sizeof(DM_SET_DEFAULT_LINK_POLICY_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->link_policy_settings_in, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->link_policy_settings_out, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SET_DEFAULT_LINK_POLICY_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_HOLD_MODE_ACTIVITY_REQ
CsrSize DM_HCI_WRITE_HOLD_MODE_ACTIVITY_REQ_TSizeof(void *msg)
{
    DM_HCI_WRITE_HOLD_MODE_ACTIVITY_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_HOLD_MODE_ACTIVITY_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_HOLD_MODE_ACTIVITY_REQ
CsrUint8 *DM_HCI_WRITE_HOLD_MODE_ACTIVITY_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_HOLD_MODE_ACTIVITY_REQ_T *primitive;

    primitive = (DM_HCI_WRITE_HOLD_MODE_ACTIVITY_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->activity);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_HOLD_MODE_ACTIVITY_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_HOLD_MODE_ACTIVITY_REQ
void *DM_HCI_WRITE_HOLD_MODE_ACTIVITY_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_HOLD_MODE_ACTIVITY_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_HOLD_MODE_ACTIVITY_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_HOLD_MODE_ACTIVITY_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->activity, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_HOLD_MODE_ACTIVITY_REQ */

#ifndef EXCLUDE_DM_HCI_FLUSH_REQ
CsrSize DM_HCI_FLUSH_REQ_TSizeof(void *msg)
{
    DM_HCI_FLUSH_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_FLUSH_REQ */

#ifndef EXCLUDE_DM_HCI_FLUSH_REQ
CsrUint8 *DM_HCI_FLUSH_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_FLUSH_REQ_T *primitive;

    primitive = (DM_HCI_FLUSH_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_FLUSH_REQ */

#ifndef EXCLUDE_DM_HCI_FLUSH_REQ
void *DM_HCI_FLUSH_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_FLUSH_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_FLUSH_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_FLUSH_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_FLUSH_REQ */

#ifndef EXCLUDE_DM_HCI_CREATE_NEW_UNIT_KEY_REQ
CsrSize DM_HCI_CREATE_NEW_UNIT_KEY_REQ_TSizeof(void *msg)
{
    DM_HCI_CREATE_NEW_UNIT_KEY_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_CREATE_NEW_UNIT_KEY_REQ */

#ifndef EXCLUDE_DM_HCI_CREATE_NEW_UNIT_KEY_REQ
CsrUint8 *DM_HCI_CREATE_NEW_UNIT_KEY_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_CREATE_NEW_UNIT_KEY_REQ_T *primitive;

    primitive = (DM_HCI_CREATE_NEW_UNIT_KEY_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_CREATE_NEW_UNIT_KEY_REQ */

#ifndef EXCLUDE_DM_HCI_CREATE_NEW_UNIT_KEY_REQ
void *DM_HCI_CREATE_NEW_UNIT_KEY_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_CREATE_NEW_UNIT_KEY_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_CREATE_NEW_UNIT_KEY_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_CREATE_NEW_UNIT_KEY_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_CREATE_NEW_UNIT_KEY_REQ */

#ifndef EXCLUDE_DM_HCI_ENHANCED_FLUSH_REQ
CsrSize DM_HCI_ENHANCED_FLUSH_REQ_TSizeof(void *msg)
{
    DM_HCI_ENHANCED_FLUSH_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ENHANCED_FLUSH_REQ */

#ifndef EXCLUDE_DM_HCI_ENHANCED_FLUSH_REQ
CsrUint8 *DM_HCI_ENHANCED_FLUSH_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ENHANCED_FLUSH_REQ_T *primitive;

    primitive = (DM_HCI_ENHANCED_FLUSH_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->pkt_type);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ENHANCED_FLUSH_REQ */

#ifndef EXCLUDE_DM_HCI_ENHANCED_FLUSH_REQ
void *DM_HCI_ENHANCED_FLUSH_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ENHANCED_FLUSH_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ENHANCED_FLUSH_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_ENHANCED_FLUSH_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->pkt_type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ENHANCED_FLUSH_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_REQ
CsrSize DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_REQ_TSizeof(void *msg)
{
    DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_REQ
CsrUint8 *DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_REQ_T *primitive;

    primitive = (DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address_type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->address.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->address.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_REQ
void *DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address_type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->address.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->address.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_REQ */

#ifndef EXCLUDE_DM_SM_ACCESS_CFM
CsrSize DM_SM_ACCESS_CFM_TSizeof(void *msg)
{
    DM_SM_ACCESS_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_ACCESS_CFM */

#ifndef EXCLUDE_DM_SM_ACCESS_CFM
CsrUint8 *DM_SM_ACCESS_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_ACCESS_CFM_T *primitive;

    primitive = (DM_SM_ACCESS_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_setup.connection.service.protocol_id);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_setup.connection.service.channel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->conn_setup.connection.addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->conn_setup.connection.addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->conn_setup.connection.addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_setup.connection.addrt.addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->conn_setup.incoming);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->context);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_ACCESS_CFM */

#ifndef EXCLUDE_DM_SM_ACCESS_CFM
void *DM_SM_ACCESS_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_ACCESS_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_ACCESS_CFM_T *) CsrPmemZalloc(sizeof(DM_SM_ACCESS_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_setup.connection.service.protocol_id, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_setup.connection.service.channel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->conn_setup.connection.addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->conn_setup.connection.addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->conn_setup.connection.addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_setup.connection.addrt.addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->conn_setup.incoming, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->context, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_ACCESS_CFM */

#ifndef EXCLUDE_DM_HCI_EXIT_PERIODIC_INQUIRY_MODE_REQ
CsrSize DM_HCI_EXIT_PERIODIC_INQUIRY_MODE_REQ_TSizeof(void *msg)
{
    DM_HCI_EXIT_PERIODIC_INQUIRY_MODE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_EXIT_PERIODIC_INQUIRY_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_EXIT_PERIODIC_INQUIRY_MODE_REQ
CsrUint8 *DM_HCI_EXIT_PERIODIC_INQUIRY_MODE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_EXIT_PERIODIC_INQUIRY_MODE_REQ_T *primitive;

    primitive = (DM_HCI_EXIT_PERIODIC_INQUIRY_MODE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_EXIT_PERIODIC_INQUIRY_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_EXIT_PERIODIC_INQUIRY_MODE_REQ
void *DM_HCI_EXIT_PERIODIC_INQUIRY_MODE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_EXIT_PERIODIC_INQUIRY_MODE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_EXIT_PERIODIC_INQUIRY_MODE_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_EXIT_PERIODIC_INQUIRY_MODE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_EXIT_PERIODIC_INQUIRY_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_READ_REMOTE_SUPP_FEATURES_CFM
CsrSize DM_HCI_READ_REMOTE_SUPP_FEATURES_CFM_TSizeof(void *msg)
{
    DM_HCI_READ_REMOTE_SUPP_FEATURES_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_REMOTE_SUPP_FEATURES_CFM */

#ifndef EXCLUDE_DM_HCI_READ_REMOTE_SUPP_FEATURES_CFM
CsrUint8 *DM_HCI_READ_REMOTE_SUPP_FEATURES_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_REMOTE_SUPP_FEATURES_CFM_T *primitive;

    primitive = (DM_HCI_READ_REMOTE_SUPP_FEATURES_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrMemCpySer(buffer, length, ((const void *) primitive->features), ((CsrSize) 2 * (4)));

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_REMOTE_SUPP_FEATURES_CFM */

#ifndef EXCLUDE_DM_HCI_READ_REMOTE_SUPP_FEATURES_CFM
void *DM_HCI_READ_REMOTE_SUPP_FEATURES_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_REMOTE_SUPP_FEATURES_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_REMOTE_SUPP_FEATURES_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_REMOTE_SUPP_FEATURES_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->features), buffer, &offset, ((CsrSize) 2 * (4)));

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_REMOTE_SUPP_FEATURES_CFM */

#ifndef EXCLUDE_DM_HCI_SET_AFH_CHANNEL_CLASS_REQ
CsrSize DM_HCI_SET_AFH_CHANNEL_CLASS_REQ_TSizeof(void *msg)
{
    DM_HCI_SET_AFH_CHANNEL_CLASS_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_SET_AFH_CHANNEL_CLASS_REQ */

#ifndef EXCLUDE_DM_HCI_SET_AFH_CHANNEL_CLASS_REQ
CsrUint8 *DM_HCI_SET_AFH_CHANNEL_CLASS_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_SET_AFH_CHANNEL_CLASS_REQ_T *primitive;

    primitive = (DM_HCI_SET_AFH_CHANNEL_CLASS_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrMemCpySer(buffer, length, ((const void *) primitive->map), ((CsrSize) (10)));

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_SET_AFH_CHANNEL_CLASS_REQ */

#ifndef EXCLUDE_DM_HCI_SET_AFH_CHANNEL_CLASS_REQ
void *DM_HCI_SET_AFH_CHANNEL_CLASS_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_SET_AFH_CHANNEL_CLASS_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_SET_AFH_CHANNEL_CLASS_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_SET_AFH_CHANNEL_CLASS_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->map), buffer, &offset, ((CsrSize) (10)));

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_SET_AFH_CHANNEL_CLASS_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_REQ
CsrSize DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_REQ_TSizeof(void *msg)
{
    DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_REQ
CsrUint8 *DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_REQ_T *primitive;

    primitive = (DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->scan_response_data_len);
    CsrMemCpySer(buffer, length, ((const void *) primitive->scan_response_data), ((CsrSize) (31)));

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_REQ
void *DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->scan_response_data_len, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->scan_response_data), buffer, &offset, ((CsrSize) (31)));

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_REQ */

#ifndef EXCLUDE_DM_HCI_READ_COUNTRY_CODE_REQ
CsrSize DM_HCI_READ_COUNTRY_CODE_REQ_TSizeof(void *msg)
{
    DM_HCI_READ_COUNTRY_CODE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_COUNTRY_CODE_REQ */

#ifndef EXCLUDE_DM_HCI_READ_COUNTRY_CODE_REQ
CsrUint8 *DM_HCI_READ_COUNTRY_CODE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_COUNTRY_CODE_REQ_T *primitive;

    primitive = (DM_HCI_READ_COUNTRY_CODE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_COUNTRY_CODE_REQ */

#ifndef EXCLUDE_DM_HCI_READ_COUNTRY_CODE_REQ
void *DM_HCI_READ_COUNTRY_CODE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_COUNTRY_CODE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_COUNTRY_CODE_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_COUNTRY_CODE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_COUNTRY_CODE_REQ */

#ifndef EXCLUDE_DM_SM_PIN_REQUEST_RSP
CsrSize DM_SM_PIN_REQUEST_RSP_TSizeof(void *msg)
{
    DM_SM_PIN_REQUEST_RSP_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_PIN_REQUEST_RSP */

#ifndef EXCLUDE_DM_SM_PIN_REQUEST_RSP
CsrUint8 *DM_SM_PIN_REQUEST_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_PIN_REQUEST_RSP_T *primitive;

    primitive = (DM_SM_PIN_REQUEST_RSP_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->pin_length);
    CsrMemCpySer(buffer, length, ((const void *) primitive->pin), ((CsrSize) (16)));

    return buffer;
}
#endif /* EXCLUDE_DM_SM_PIN_REQUEST_RSP */

#ifndef EXCLUDE_DM_SM_PIN_REQUEST_RSP
void *DM_SM_PIN_REQUEST_RSP_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_PIN_REQUEST_RSP_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_PIN_REQUEST_RSP_T *) CsrPmemZalloc(sizeof(DM_SM_PIN_REQUEST_RSP_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->pin_length, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->pin), buffer, &offset, ((CsrSize) (16)));

    return primitive;
}
#endif /* EXCLUDE_DM_SM_PIN_REQUEST_RSP */

#ifndef EXCLUDE_DM_HCI_WRITE_NUM_BCAST_RETXS_CFM
CsrSize DM_HCI_WRITE_NUM_BCAST_RETXS_CFM_TSizeof(void *msg)
{
    DM_HCI_WRITE_NUM_BCAST_RETXS_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_NUM_BCAST_RETXS_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_NUM_BCAST_RETXS_CFM
CsrUint8 *DM_HCI_WRITE_NUM_BCAST_RETXS_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_NUM_BCAST_RETXS_CFM_T *primitive;

    primitive = (DM_HCI_WRITE_NUM_BCAST_RETXS_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_NUM_BCAST_RETXS_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_NUM_BCAST_RETXS_CFM
void *DM_HCI_WRITE_NUM_BCAST_RETXS_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_NUM_BCAST_RETXS_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_NUM_BCAST_RETXS_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_NUM_BCAST_RETXS_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_NUM_BCAST_RETXS_CFM */

#ifndef EXCLUDE_DM_HCI_SNIFF_SUB_RATING_IND
CsrSize DM_HCI_SNIFF_SUB_RATING_IND_TSizeof(void *msg)
{
    DM_HCI_SNIFF_SUB_RATING_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_SNIFF_SUB_RATING_IND */

#ifndef EXCLUDE_DM_HCI_SNIFF_SUB_RATING_IND
CsrUint8 *DM_HCI_SNIFF_SUB_RATING_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_SNIFF_SUB_RATING_IND_T *primitive;

    primitive = (DM_HCI_SNIFF_SUB_RATING_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->transmit_latency);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->receive_latency);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->remote_timeout);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->local_timeout);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_SNIFF_SUB_RATING_IND */

#ifndef EXCLUDE_DM_HCI_SNIFF_SUB_RATING_IND
void *DM_HCI_SNIFF_SUB_RATING_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_SNIFF_SUB_RATING_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_SNIFF_SUB_RATING_IND_T *) CsrPmemZalloc(sizeof(DM_HCI_SNIFF_SUB_RATING_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->transmit_latency, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->receive_latency, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->remote_timeout, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->local_timeout, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_SNIFF_SUB_RATING_IND */

#ifndef EXCLUDE_DM_HCI_SWITCH_ROLE_CFM
CsrSize DM_HCI_SWITCH_ROLE_CFM_TSizeof(void *msg)
{
    DM_HCI_SWITCH_ROLE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_SWITCH_ROLE_CFM */

#ifndef EXCLUDE_DM_HCI_SWITCH_ROLE_CFM
CsrUint8 *DM_HCI_SWITCH_ROLE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_SWITCH_ROLE_CFM_T *primitive;

    primitive = (DM_HCI_SWITCH_ROLE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->role);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_SWITCH_ROLE_CFM */

#ifndef EXCLUDE_DM_HCI_SWITCH_ROLE_CFM
void *DM_HCI_SWITCH_ROLE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_SWITCH_ROLE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_SWITCH_ROLE_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_SWITCH_ROLE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->role, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_SWITCH_ROLE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_LINK_SUPERV_TIMEOUT_REQ
CsrSize DM_HCI_READ_LINK_SUPERV_TIMEOUT_REQ_TSizeof(void *msg)
{
    DM_HCI_READ_LINK_SUPERV_TIMEOUT_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_LINK_SUPERV_TIMEOUT_REQ */

#ifndef EXCLUDE_DM_HCI_READ_LINK_SUPERV_TIMEOUT_REQ
CsrUint8 *DM_HCI_READ_LINK_SUPERV_TIMEOUT_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_LINK_SUPERV_TIMEOUT_REQ_T *primitive;

    primitive = (DM_HCI_READ_LINK_SUPERV_TIMEOUT_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_LINK_SUPERV_TIMEOUT_REQ */

#ifndef EXCLUDE_DM_HCI_READ_LINK_SUPERV_TIMEOUT_REQ
void *DM_HCI_READ_LINK_SUPERV_TIMEOUT_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_LINK_SUPERV_TIMEOUT_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_LINK_SUPERV_TIMEOUT_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_LINK_SUPERV_TIMEOUT_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_LINK_SUPERV_TIMEOUT_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_PAGESCAN_MODE_REQ
CsrSize DM_HCI_WRITE_PAGESCAN_MODE_REQ_TSizeof(void *msg)
{
    DM_HCI_WRITE_PAGESCAN_MODE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_PAGESCAN_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_PAGESCAN_MODE_REQ
CsrUint8 *DM_HCI_WRITE_PAGESCAN_MODE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_PAGESCAN_MODE_REQ_T *primitive;

    primitive = (DM_HCI_WRITE_PAGESCAN_MODE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mode);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_PAGESCAN_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_PAGESCAN_MODE_REQ
void *DM_HCI_WRITE_PAGESCAN_MODE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_PAGESCAN_MODE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_PAGESCAN_MODE_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_PAGESCAN_MODE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_PAGESCAN_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_READ_ENCRYPTION_KEY_SIZE_CFM
CsrSize DM_HCI_READ_ENCRYPTION_KEY_SIZE_CFM_TSizeof(void *msg)
{
    DM_HCI_READ_ENCRYPTION_KEY_SIZE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_ENCRYPTION_KEY_SIZE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_ENCRYPTION_KEY_SIZE_CFM
CsrUint8 *DM_HCI_READ_ENCRYPTION_KEY_SIZE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_ENCRYPTION_KEY_SIZE_CFM_T *primitive;

    primitive = (DM_HCI_READ_ENCRYPTION_KEY_SIZE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->key_size);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_ENCRYPTION_KEY_SIZE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_ENCRYPTION_KEY_SIZE_CFM
void *DM_HCI_READ_ENCRYPTION_KEY_SIZE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_ENCRYPTION_KEY_SIZE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_ENCRYPTION_KEY_SIZE_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_ENCRYPTION_KEY_SIZE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->key_size, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_ENCRYPTION_KEY_SIZE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_PAGESCAN_ACTIVITY_REQ
CsrSize DM_HCI_READ_PAGESCAN_ACTIVITY_REQ_TSizeof(void *msg)
{
    DM_HCI_READ_PAGESCAN_ACTIVITY_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_PAGESCAN_ACTIVITY_REQ */

#ifndef EXCLUDE_DM_HCI_READ_PAGESCAN_ACTIVITY_REQ
CsrUint8 *DM_HCI_READ_PAGESCAN_ACTIVITY_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_PAGESCAN_ACTIVITY_REQ_T *primitive;

    primitive = (DM_HCI_READ_PAGESCAN_ACTIVITY_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_PAGESCAN_ACTIVITY_REQ */

#ifndef EXCLUDE_DM_HCI_READ_PAGESCAN_ACTIVITY_REQ
void *DM_HCI_READ_PAGESCAN_ACTIVITY_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_PAGESCAN_ACTIVITY_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_PAGESCAN_ACTIVITY_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_PAGESCAN_ACTIVITY_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_PAGESCAN_ACTIVITY_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_REQ
CsrSize DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_REQ_TSizeof(void *msg)
{
    DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_REQ
CsrUint8 *DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_REQ_T *primitive;

    primitive = (DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tx_power);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_REQ
void *DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tx_power, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_REQ */

#ifndef EXCLUDE_DM_HCI_LINK_SUPERV_TIMEOUT_IND
CsrSize DM_HCI_LINK_SUPERV_TIMEOUT_IND_TSizeof(void *msg)
{
    DM_HCI_LINK_SUPERV_TIMEOUT_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_LINK_SUPERV_TIMEOUT_IND */

#ifndef EXCLUDE_DM_HCI_LINK_SUPERV_TIMEOUT_IND
CsrUint8 *DM_HCI_LINK_SUPERV_TIMEOUT_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_LINK_SUPERV_TIMEOUT_IND_T *primitive;

    primitive = (DM_HCI_LINK_SUPERV_TIMEOUT_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->timeout);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_LINK_SUPERV_TIMEOUT_IND */

#ifndef EXCLUDE_DM_HCI_LINK_SUPERV_TIMEOUT_IND
void *DM_HCI_LINK_SUPERV_TIMEOUT_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_LINK_SUPERV_TIMEOUT_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_LINK_SUPERV_TIMEOUT_IND_T *) CsrPmemZalloc(sizeof(DM_HCI_LINK_SUPERV_TIMEOUT_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->timeout, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_LINK_SUPERV_TIMEOUT_IND */

#ifndef EXCLUDE_DM_AMPM_READ_DATA_BLOCK_SIZE_RSP
CsrSize DM_AMPM_READ_DATA_BLOCK_SIZE_RSP_TSizeof(void *msg)
{
    DM_AMPM_READ_DATA_BLOCK_SIZE_RSP_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_AMPM_READ_DATA_BLOCK_SIZE_RSP */

#ifndef EXCLUDE_DM_AMPM_READ_DATA_BLOCK_SIZE_RSP
CsrUint8 *DM_AMPM_READ_DATA_BLOCK_SIZE_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_AMPM_READ_DATA_BLOCK_SIZE_RSP_T *primitive;

    primitive = (DM_AMPM_READ_DATA_BLOCK_SIZE_RSP_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->local_amp_id);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->fragmentable);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->max_pdu_length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->max_acl_data_packet_length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->data_block_length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->total_num_data_blocks);

    return buffer;
}
#endif /* EXCLUDE_DM_AMPM_READ_DATA_BLOCK_SIZE_RSP */

#ifndef EXCLUDE_DM_AMPM_READ_DATA_BLOCK_SIZE_RSP
void *DM_AMPM_READ_DATA_BLOCK_SIZE_RSP_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_AMPM_READ_DATA_BLOCK_SIZE_RSP_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_AMPM_READ_DATA_BLOCK_SIZE_RSP_T *) CsrPmemZalloc(sizeof(DM_AMPM_READ_DATA_BLOCK_SIZE_RSP_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->local_amp_id, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->fragmentable, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->max_pdu_length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->max_acl_data_packet_length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->data_block_length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->total_num_data_blocks, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_AMPM_READ_DATA_BLOCK_SIZE_RSP */

#ifndef EXCLUDE_DM_SM_IO_CAPABILITY_RESPONSE_IND
CsrSize DM_SM_IO_CAPABILITY_RESPONSE_IND_TSizeof(void *msg)
{
    DM_SM_IO_CAPABILITY_RESPONSE_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_IO_CAPABILITY_RESPONSE_IND */

#ifndef EXCLUDE_DM_SM_IO_CAPABILITY_RESPONSE_IND
CsrUint8 *DM_SM_IO_CAPABILITY_RESPONSE_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_IO_CAPABILITY_RESPONSE_IND_T *primitive;

    primitive = (DM_SM_IO_CAPABILITY_RESPONSE_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->io_capability);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->oob_data_present);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->authentication_requirements);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->key_distribution);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_IO_CAPABILITY_RESPONSE_IND */

#ifndef EXCLUDE_DM_SM_IO_CAPABILITY_RESPONSE_IND
void *DM_SM_IO_CAPABILITY_RESPONSE_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_IO_CAPABILITY_RESPONSE_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_IO_CAPABILITY_RESPONSE_IND_T *) CsrPmemZalloc(sizeof(DM_SM_IO_CAPABILITY_RESPONSE_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->io_capability, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->oob_data_present, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->authentication_requirements, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->key_distribution, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_IO_CAPABILITY_RESPONSE_IND */

#ifndef EXCLUDE_DM_HCI_READ_PIN_TYPE_REQ
CsrSize DM_HCI_READ_PIN_TYPE_REQ_TSizeof(void *msg)
{
    DM_HCI_READ_PIN_TYPE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_PIN_TYPE_REQ */

#ifndef EXCLUDE_DM_HCI_READ_PIN_TYPE_REQ
CsrUint8 *DM_HCI_READ_PIN_TYPE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_PIN_TYPE_REQ_T *primitive;

    primitive = (DM_HCI_READ_PIN_TYPE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_PIN_TYPE_REQ */

#ifndef EXCLUDE_DM_HCI_READ_PIN_TYPE_REQ
void *DM_HCI_READ_PIN_TYPE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_PIN_TYPE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_PIN_TYPE_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_PIN_TYPE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_PIN_TYPE_REQ */

#ifndef EXCLUDE_DM_HCI_READ_PAGE_SCAN_TYPE_CFM
CsrSize DM_HCI_READ_PAGE_SCAN_TYPE_CFM_TSizeof(void *msg)
{
    DM_HCI_READ_PAGE_SCAN_TYPE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_PAGE_SCAN_TYPE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_PAGE_SCAN_TYPE_CFM
CsrUint8 *DM_HCI_READ_PAGE_SCAN_TYPE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_PAGE_SCAN_TYPE_CFM_T *primitive;

    primitive = (DM_HCI_READ_PAGE_SCAN_TYPE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mode);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_PAGE_SCAN_TYPE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_PAGE_SCAN_TYPE_CFM
void *DM_HCI_READ_PAGE_SCAN_TYPE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_PAGE_SCAN_TYPE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_PAGE_SCAN_TYPE_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_PAGE_SCAN_TYPE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_PAGE_SCAN_TYPE_CFM */

#ifndef EXCLUDE_DM_AM_REGISTER_CFM
CsrSize DM_AM_REGISTER_CFM_TSizeof(void *msg)
{
    DM_AM_REGISTER_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_AM_REGISTER_CFM */

#ifndef EXCLUDE_DM_AM_REGISTER_CFM
CsrUint8 *DM_AM_REGISTER_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_AM_REGISTER_CFM_T *primitive;

    primitive = (DM_AM_REGISTER_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->version);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->patch_level);

    return buffer;
}
#endif /* EXCLUDE_DM_AM_REGISTER_CFM */

#ifndef EXCLUDE_DM_AM_REGISTER_CFM
void *DM_AM_REGISTER_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_AM_REGISTER_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_AM_REGISTER_CFM_T *) CsrPmemZalloc(sizeof(DM_AM_REGISTER_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->version, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->patch_level, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_AM_REGISTER_CFM */

#ifndef EXCLUDE_DM_SET_BT_VERSION_REQ
CsrSize DM_SET_BT_VERSION_REQ_TSizeof(void *msg)
{
    DM_SET_BT_VERSION_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SET_BT_VERSION_REQ */

#ifndef EXCLUDE_DM_SET_BT_VERSION_REQ
CsrUint8 *DM_SET_BT_VERSION_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SET_BT_VERSION_REQ_T *primitive;

    primitive = (DM_SET_BT_VERSION_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->version);

    return buffer;
}
#endif /* EXCLUDE_DM_SET_BT_VERSION_REQ */

#ifndef EXCLUDE_DM_SET_BT_VERSION_REQ
void *DM_SET_BT_VERSION_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SET_BT_VERSION_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SET_BT_VERSION_REQ_T *) CsrPmemZalloc(sizeof(DM_SET_BT_VERSION_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->version, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SET_BT_VERSION_REQ */

#ifndef EXCLUDE_DM_HCI_READ_PAGESCAN_MODE_CFM
CsrSize DM_HCI_READ_PAGESCAN_MODE_CFM_TSizeof(void *msg)
{
    DM_HCI_READ_PAGESCAN_MODE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_PAGESCAN_MODE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_PAGESCAN_MODE_CFM
CsrUint8 *DM_HCI_READ_PAGESCAN_MODE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_PAGESCAN_MODE_CFM_T *primitive;

    primitive = (DM_HCI_READ_PAGESCAN_MODE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mode);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_PAGESCAN_MODE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_PAGESCAN_MODE_CFM
void *DM_HCI_READ_PAGESCAN_MODE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_PAGESCAN_MODE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_PAGESCAN_MODE_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_PAGESCAN_MODE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_PAGESCAN_MODE_CFM */

#ifndef EXCLUDE_DM_HCI_CREATE_CONNECTION_CANCEL_REQ
CsrSize DM_HCI_CREATE_CONNECTION_CANCEL_REQ_TSizeof(void *msg)
{
    DM_HCI_CREATE_CONNECTION_CANCEL_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_CREATE_CONNECTION_CANCEL_REQ */

#ifndef EXCLUDE_DM_HCI_CREATE_CONNECTION_CANCEL_REQ
CsrUint8 *DM_HCI_CREATE_CONNECTION_CANCEL_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_CREATE_CONNECTION_CANCEL_REQ_T *primitive;

    primitive = (DM_HCI_CREATE_CONNECTION_CANCEL_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_CREATE_CONNECTION_CANCEL_REQ */

#ifndef EXCLUDE_DM_HCI_CREATE_CONNECTION_CANCEL_REQ
void *DM_HCI_CREATE_CONNECTION_CANCEL_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_CREATE_CONNECTION_CANCEL_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_CREATE_CONNECTION_CANCEL_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_CREATE_CONNECTION_CANCEL_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_CREATE_CONNECTION_CANCEL_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_LINK_POLICY_SETTINGS_CFM
CsrSize DM_HCI_WRITE_LINK_POLICY_SETTINGS_CFM_TSizeof(void *msg)
{
    DM_HCI_WRITE_LINK_POLICY_SETTINGS_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_LINK_POLICY_SETTINGS_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_LINK_POLICY_SETTINGS_CFM
CsrUint8 *DM_HCI_WRITE_LINK_POLICY_SETTINGS_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_LINK_POLICY_SETTINGS_CFM_T *primitive;

    primitive = (DM_HCI_WRITE_LINK_POLICY_SETTINGS_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_LINK_POLICY_SETTINGS_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_LINK_POLICY_SETTINGS_CFM
void *DM_HCI_WRITE_LINK_POLICY_SETTINGS_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_LINK_POLICY_SETTINGS_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_LINK_POLICY_SETTINGS_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_LINK_POLICY_SETTINGS_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_LINK_POLICY_SETTINGS_CFM */

#ifndef EXCLUDE_DM_HCI_EXIT_SNIFF_MODE_REQ
CsrSize DM_HCI_EXIT_SNIFF_MODE_REQ_TSizeof(void *msg)
{
    DM_HCI_EXIT_SNIFF_MODE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_EXIT_SNIFF_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_EXIT_SNIFF_MODE_REQ
CsrUint8 *DM_HCI_EXIT_SNIFF_MODE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_EXIT_SNIFF_MODE_REQ_T *primitive;

    primitive = (DM_HCI_EXIT_SNIFF_MODE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_EXIT_SNIFF_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_EXIT_SNIFF_MODE_REQ
void *DM_HCI_EXIT_SNIFF_MODE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_EXIT_SNIFF_MODE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_EXIT_SNIFF_MODE_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_EXIT_SNIFF_MODE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_EXIT_SNIFF_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_REFRESH_ENCRYPTION_KEY_IND
CsrSize DM_HCI_REFRESH_ENCRYPTION_KEY_IND_TSizeof(void *msg)
{
    DM_HCI_REFRESH_ENCRYPTION_KEY_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_REFRESH_ENCRYPTION_KEY_IND */

#ifndef EXCLUDE_DM_HCI_REFRESH_ENCRYPTION_KEY_IND
CsrUint8 *DM_HCI_REFRESH_ENCRYPTION_KEY_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_REFRESH_ENCRYPTION_KEY_IND_T *primitive;

    primitive = (DM_HCI_REFRESH_ENCRYPTION_KEY_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_REFRESH_ENCRYPTION_KEY_IND */

#ifndef EXCLUDE_DM_HCI_REFRESH_ENCRYPTION_KEY_IND
void *DM_HCI_REFRESH_ENCRYPTION_KEY_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_REFRESH_ENCRYPTION_KEY_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_REFRESH_ENCRYPTION_KEY_IND_T *) CsrPmemZalloc(sizeof(DM_HCI_REFRESH_ENCRYPTION_KEY_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_REFRESH_ENCRYPTION_KEY_IND */

#ifndef EXCLUDE_DM_HCI_INQUIRY_CANCEL_REQ
CsrSize DM_HCI_INQUIRY_CANCEL_REQ_TSizeof(void *msg)
{
    DM_HCI_INQUIRY_CANCEL_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_INQUIRY_CANCEL_REQ */

#ifndef EXCLUDE_DM_HCI_INQUIRY_CANCEL_REQ
CsrUint8 *DM_HCI_INQUIRY_CANCEL_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_INQUIRY_CANCEL_REQ_T *primitive;

    primitive = (DM_HCI_INQUIRY_CANCEL_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_INQUIRY_CANCEL_REQ */

#ifndef EXCLUDE_DM_HCI_INQUIRY_CANCEL_REQ
void *DM_HCI_INQUIRY_CANCEL_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_INQUIRY_CANCEL_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_INQUIRY_CANCEL_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_INQUIRY_CANCEL_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_INQUIRY_CANCEL_REQ */

#ifndef EXCLUDE_DM_HCI_READ_SCAN_ENABLE_CFM
CsrSize DM_HCI_READ_SCAN_ENABLE_CFM_TSizeof(void *msg)
{
    DM_HCI_READ_SCAN_ENABLE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_SCAN_ENABLE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_SCAN_ENABLE_CFM
CsrUint8 *DM_HCI_READ_SCAN_ENABLE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_SCAN_ENABLE_CFM_T *primitive;

    primitive = (DM_HCI_READ_SCAN_ENABLE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->scan_enable);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_SCAN_ENABLE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_SCAN_ENABLE_CFM
void *DM_HCI_READ_SCAN_ENABLE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_SCAN_ENABLE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_SCAN_ENABLE_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_SCAN_ENABLE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->scan_enable, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_SCAN_ENABLE_CFM */

#ifndef EXCLUDE_DM_SM_SECURITY_IND
CsrSize DM_SM_SECURITY_IND_TSizeof(void *msg)
{
    DM_SM_SECURITY_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_SECURITY_IND */

#ifndef EXCLUDE_DM_SM_SECURITY_IND
CsrUint8 *DM_SM_SECURITY_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_SECURITY_IND_T *primitive;

    primitive = (DM_SM_SECURITY_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->connection_flags);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->security_requirements);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_SECURITY_IND */

#ifndef EXCLUDE_DM_SM_SECURITY_IND
void *DM_SM_SECURITY_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_SECURITY_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_SECURITY_IND_T *) CsrPmemZalloc(sizeof(DM_SM_SECURITY_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->connection_flags, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->security_requirements, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_SECURITY_IND */

#ifndef EXCLUDE_DM_SM_LINK_KEY_REQUEST_IND
CsrSize DM_SM_LINK_KEY_REQUEST_IND_TSizeof(void *msg)
{
    DM_SM_LINK_KEY_REQUEST_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_LINK_KEY_REQUEST_IND */

#ifndef EXCLUDE_DM_SM_LINK_KEY_REQUEST_IND
CsrUint8 *DM_SM_LINK_KEY_REQUEST_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_LINK_KEY_REQUEST_IND_T *primitive;

    primitive = (DM_SM_LINK_KEY_REQUEST_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->dev_class);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->authenticated);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->initiated_outgoing);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->rhsf);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_LINK_KEY_REQUEST_IND */

#ifndef EXCLUDE_DM_SM_LINK_KEY_REQUEST_IND
void *DM_SM_LINK_KEY_REQUEST_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_LINK_KEY_REQUEST_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_LINK_KEY_REQUEST_IND_T *) CsrPmemZalloc(sizeof(DM_SM_LINK_KEY_REQUEST_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->dev_class, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->authenticated, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->initiated_outgoing, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->rhsf, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_LINK_KEY_REQUEST_IND */

#ifndef EXCLUDE_DM_SM_IO_CAPABILITY_REQUEST_NEG_RSP
CsrSize DM_SM_IO_CAPABILITY_REQUEST_NEG_RSP_TSizeof(void *msg)
{
    DM_SM_IO_CAPABILITY_REQUEST_NEG_RSP_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_IO_CAPABILITY_REQUEST_NEG_RSP */

#ifndef EXCLUDE_DM_SM_IO_CAPABILITY_REQUEST_NEG_RSP
CsrUint8 *DM_SM_IO_CAPABILITY_REQUEST_NEG_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_IO_CAPABILITY_REQUEST_NEG_RSP_T *primitive;

    primitive = (DM_SM_IO_CAPABILITY_REQUEST_NEG_RSP_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->reason);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_IO_CAPABILITY_REQUEST_NEG_RSP */

#ifndef EXCLUDE_DM_SM_IO_CAPABILITY_REQUEST_NEG_RSP
void *DM_SM_IO_CAPABILITY_REQUEST_NEG_RSP_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_IO_CAPABILITY_REQUEST_NEG_RSP_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_IO_CAPABILITY_REQUEST_NEG_RSP_T *) CsrPmemZalloc(sizeof(DM_SM_IO_CAPABILITY_REQUEST_NEG_RSP_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->reason, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_IO_CAPABILITY_REQUEST_NEG_RSP */

#ifndef EXCLUDE_DM_HCI_WRITE_CLASS_OF_DEVICE_REQ
CsrSize DM_HCI_WRITE_CLASS_OF_DEVICE_REQ_TSizeof(void *msg)
{
    DM_HCI_WRITE_CLASS_OF_DEVICE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_CLASS_OF_DEVICE_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_CLASS_OF_DEVICE_REQ
CsrUint8 *DM_HCI_WRITE_CLASS_OF_DEVICE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_CLASS_OF_DEVICE_REQ_T *primitive;

    primitive = (DM_HCI_WRITE_CLASS_OF_DEVICE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->dev_class);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_CLASS_OF_DEVICE_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_CLASS_OF_DEVICE_REQ
void *DM_HCI_WRITE_CLASS_OF_DEVICE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_CLASS_OF_DEVICE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_CLASS_OF_DEVICE_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_CLASS_OF_DEVICE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->dev_class, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_CLASS_OF_DEVICE_REQ */

#ifndef EXCLUDE_DM_HCI_READ_FAILED_CONTACT_COUNT_CFM
CsrSize DM_HCI_READ_FAILED_CONTACT_COUNT_CFM_TSizeof(void *msg)
{
    DM_HCI_READ_FAILED_CONTACT_COUNT_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_FAILED_CONTACT_COUNT_CFM */

#ifndef EXCLUDE_DM_HCI_READ_FAILED_CONTACT_COUNT_CFM
CsrUint8 *DM_HCI_READ_FAILED_CONTACT_COUNT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_FAILED_CONTACT_COUNT_CFM_T *primitive;

    primitive = (DM_HCI_READ_FAILED_CONTACT_COUNT_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->failed_contact_count);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_FAILED_CONTACT_COUNT_CFM */

#ifndef EXCLUDE_DM_HCI_READ_FAILED_CONTACT_COUNT_CFM
void *DM_HCI_READ_FAILED_CONTACT_COUNT_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_FAILED_CONTACT_COUNT_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_FAILED_CONTACT_COUNT_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_FAILED_CONTACT_COUNT_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->failed_contact_count, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_FAILED_CONTACT_COUNT_CFM */

#ifndef EXCLUDE_DM_SM_ACCESS_IND
CsrSize DM_SM_ACCESS_IND_TSizeof(void *msg)
{
    DM_SM_ACCESS_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_ACCESS_IND */

#ifndef EXCLUDE_DM_SM_ACCESS_IND
CsrUint8 *DM_SM_ACCESS_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_ACCESS_IND_T *primitive;

    primitive = (DM_SM_ACCESS_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_setup.connection.service.protocol_id);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_setup.connection.service.channel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->conn_setup.connection.addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->conn_setup.connection.addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->conn_setup.connection.addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_setup.connection.addrt.addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->conn_setup.incoming);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->context);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_ACCESS_IND */

#ifndef EXCLUDE_DM_SM_ACCESS_IND
void *DM_SM_ACCESS_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_ACCESS_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_ACCESS_IND_T *) CsrPmemZalloc(sizeof(DM_SM_ACCESS_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_setup.connection.service.protocol_id, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_setup.connection.service.channel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->conn_setup.connection.addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->conn_setup.connection.addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->conn_setup.connection.addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_setup.connection.addrt.addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->conn_setup.incoming, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->context, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_ACCESS_IND */

#ifndef EXCLUDE_DM_HCI_RESET_CFM
CsrSize DM_HCI_RESET_CFM_TSizeof(void *msg)
{
    DM_HCI_RESET_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_RESET_CFM */

#ifndef EXCLUDE_DM_HCI_RESET_CFM
CsrUint8 *DM_HCI_RESET_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_RESET_CFM_T *primitive;

    primitive = (DM_HCI_RESET_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_RESET_CFM */

#ifndef EXCLUDE_DM_HCI_RESET_CFM
void *DM_HCI_RESET_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_RESET_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_RESET_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_RESET_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_RESET_CFM */

#ifndef EXCLUDE_DM_SM_READ_DEVICE_REQ
CsrSize DM_SM_READ_DEVICE_REQ_TSizeof(void *msg)
{
    DM_SM_READ_DEVICE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_READ_DEVICE_REQ */

#ifndef EXCLUDE_DM_SM_READ_DEVICE_REQ
CsrUint8 *DM_SM_READ_DEVICE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_READ_DEVICE_REQ_T *primitive;

    primitive = (DM_SM_READ_DEVICE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_READ_DEVICE_REQ */

#ifndef EXCLUDE_DM_SM_READ_DEVICE_REQ
void *DM_SM_READ_DEVICE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_READ_DEVICE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_READ_DEVICE_REQ_T *) CsrPmemZalloc(sizeof(DM_SM_READ_DEVICE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_READ_DEVICE_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_CURRENT_IAC_LAP_CFM
CsrSize DM_HCI_WRITE_CURRENT_IAC_LAP_CFM_TSizeof(void *msg)
{
    DM_HCI_WRITE_CURRENT_IAC_LAP_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_CURRENT_IAC_LAP_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_CURRENT_IAC_LAP_CFM
CsrUint8 *DM_HCI_WRITE_CURRENT_IAC_LAP_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_CURRENT_IAC_LAP_CFM_T *primitive;

    primitive = (DM_HCI_WRITE_CURRENT_IAC_LAP_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_CURRENT_IAC_LAP_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_CURRENT_IAC_LAP_CFM
void *DM_HCI_WRITE_CURRENT_IAC_LAP_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_CURRENT_IAC_LAP_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_CURRENT_IAC_LAP_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_CURRENT_IAC_LAP_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_CURRENT_IAC_LAP_CFM */

#ifndef EXCLUDE_DM_SM_KEYPRESS_NOTIFICATION_IND
CsrSize DM_SM_KEYPRESS_NOTIFICATION_IND_TSizeof(void *msg)
{
    DM_SM_KEYPRESS_NOTIFICATION_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_KEYPRESS_NOTIFICATION_IND */

#ifndef EXCLUDE_DM_SM_KEYPRESS_NOTIFICATION_IND
CsrUint8 *DM_SM_KEYPRESS_NOTIFICATION_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_KEYPRESS_NOTIFICATION_IND_T *primitive;

    primitive = (DM_SM_KEYPRESS_NOTIFICATION_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->notification_type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_KEYPRESS_NOTIFICATION_IND */

#ifndef EXCLUDE_DM_SM_KEYPRESS_NOTIFICATION_IND
void *DM_SM_KEYPRESS_NOTIFICATION_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_KEYPRESS_NOTIFICATION_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_KEYPRESS_NOTIFICATION_IND_T *) CsrPmemZalloc(sizeof(DM_SM_KEYPRESS_NOTIFICATION_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->notification_type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_KEYPRESS_NOTIFICATION_IND */

#ifndef EXCLUDE_DM_HCI_READ_PAGE_SCAN_TYPE_REQ
CsrSize DM_HCI_READ_PAGE_SCAN_TYPE_REQ_TSizeof(void *msg)
{
    DM_HCI_READ_PAGE_SCAN_TYPE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_PAGE_SCAN_TYPE_REQ */

#ifndef EXCLUDE_DM_HCI_READ_PAGE_SCAN_TYPE_REQ
CsrUint8 *DM_HCI_READ_PAGE_SCAN_TYPE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_PAGE_SCAN_TYPE_REQ_T *primitive;

    primitive = (DM_HCI_READ_PAGE_SCAN_TYPE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_PAGE_SCAN_TYPE_REQ */

#ifndef EXCLUDE_DM_HCI_READ_PAGE_SCAN_TYPE_REQ
void *DM_HCI_READ_PAGE_SCAN_TYPE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_PAGE_SCAN_TYPE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_PAGE_SCAN_TYPE_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_PAGE_SCAN_TYPE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_PAGE_SCAN_TYPE_REQ */

#ifndef EXCLUDE_DM_LP_WRITE_POWERSTATES_CFM
CsrSize DM_LP_WRITE_POWERSTATES_CFM_TSizeof(void *msg)
{
    DM_LP_WRITE_POWERSTATES_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_LP_WRITE_POWERSTATES_CFM */

#ifndef EXCLUDE_DM_LP_WRITE_POWERSTATES_CFM
CsrUint8 *DM_LP_WRITE_POWERSTATES_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_LP_WRITE_POWERSTATES_CFM_T *primitive;

    primitive = (DM_LP_WRITE_POWERSTATES_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->result);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_LP_WRITE_POWERSTATES_CFM */

#ifndef EXCLUDE_DM_LP_WRITE_POWERSTATES_CFM
void *DM_LP_WRITE_POWERSTATES_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_LP_WRITE_POWERSTATES_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_LP_WRITE_POWERSTATES_CFM_T *) CsrPmemZalloc(sizeof(DM_LP_WRITE_POWERSTATES_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->result, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_LP_WRITE_POWERSTATES_CFM */

#ifndef EXCLUDE_DM_AMPM_VERIFY_PHYSICAL_LINK_IND
CsrSize DM_AMPM_VERIFY_PHYSICAL_LINK_IND_TSizeof(void *msg)
{
    DM_AMPM_VERIFY_PHYSICAL_LINK_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_AMPM_VERIFY_PHYSICAL_LINK_IND */

#ifndef EXCLUDE_DM_AMPM_VERIFY_PHYSICAL_LINK_IND
CsrUint8 *DM_AMPM_VERIFY_PHYSICAL_LINK_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_AMPM_VERIFY_PHYSICAL_LINK_IND_T *primitive;

    primitive = (DM_AMPM_VERIFY_PHYSICAL_LINK_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->identifier);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->local_amp_id);

    return buffer;
}
#endif /* EXCLUDE_DM_AMPM_VERIFY_PHYSICAL_LINK_IND */

#ifndef EXCLUDE_DM_AMPM_VERIFY_PHYSICAL_LINK_IND
void *DM_AMPM_VERIFY_PHYSICAL_LINK_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_AMPM_VERIFY_PHYSICAL_LINK_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_AMPM_VERIFY_PHYSICAL_LINK_IND_T *) CsrPmemZalloc(sizeof(DM_AMPM_VERIFY_PHYSICAL_LINK_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->identifier, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->local_amp_id, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_AMPM_VERIFY_PHYSICAL_LINK_IND */

#ifndef EXCLUDE_DM_HCI_SET_EVENT_MASK_REQ
CsrSize DM_HCI_SET_EVENT_MASK_REQ_TSizeof(void *msg)
{
    DM_HCI_SET_EVENT_MASK_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_SET_EVENT_MASK_REQ */

#ifndef EXCLUDE_DM_HCI_SET_EVENT_MASK_REQ
CsrUint8 *DM_HCI_SET_EVENT_MASK_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_SET_EVENT_MASK_REQ_T *primitive;

    primitive = (DM_HCI_SET_EVENT_MASK_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrMemCpySer(buffer, length, ((const void *) primitive->event_mask), ((CsrSize) 4 * (2)));

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_SET_EVENT_MASK_REQ */

#ifndef EXCLUDE_DM_HCI_SET_EVENT_MASK_REQ
void *DM_HCI_SET_EVENT_MASK_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_SET_EVENT_MASK_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_SET_EVENT_MASK_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_SET_EVENT_MASK_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->event_mask), buffer, &offset, ((CsrSize) 4 * (2)));

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_SET_EVENT_MASK_REQ */

#ifndef EXCLUDE_DM_SYNC_CONNECT_IND
CsrSize DM_SYNC_CONNECT_IND_TSizeof(void *msg)
{
    DM_SYNC_CONNECT_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SYNC_CONNECT_IND */

#ifndef EXCLUDE_DM_SYNC_CONNECT_IND
CsrUint8 *DM_SYNC_CONNECT_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SYNC_CONNECT_IND_T *primitive;

    primitive = (DM_SYNC_CONNECT_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pv_cbarg);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->link_type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->dev_class);

    return buffer;
}
#endif /* EXCLUDE_DM_SYNC_CONNECT_IND */

#ifndef EXCLUDE_DM_SYNC_CONNECT_IND
void *DM_SYNC_CONNECT_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SYNC_CONNECT_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SYNC_CONNECT_IND_T *) CsrPmemZalloc(sizeof(DM_SYNC_CONNECT_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pv_cbarg, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->link_type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->dev_class, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SYNC_CONNECT_IND */

#ifndef EXCLUDE_DM_HCI_WRITE_CONN_ACCEPT_TIMEOUT_REQ
CsrSize DM_HCI_WRITE_CONN_ACCEPT_TIMEOUT_REQ_TSizeof(void *msg)
{
    DM_HCI_WRITE_CONN_ACCEPT_TIMEOUT_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_CONN_ACCEPT_TIMEOUT_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_CONN_ACCEPT_TIMEOUT_REQ
CsrUint8 *DM_HCI_WRITE_CONN_ACCEPT_TIMEOUT_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_CONN_ACCEPT_TIMEOUT_REQ_T *primitive;

    primitive = (DM_HCI_WRITE_CONN_ACCEPT_TIMEOUT_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_accept_timeout);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_CONN_ACCEPT_TIMEOUT_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_CONN_ACCEPT_TIMEOUT_REQ
void *DM_HCI_WRITE_CONN_ACCEPT_TIMEOUT_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_CONN_ACCEPT_TIMEOUT_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_CONN_ACCEPT_TIMEOUT_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_CONN_ACCEPT_TIMEOUT_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_accept_timeout, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_CONN_ACCEPT_TIMEOUT_REQ */

#ifndef EXCLUDE_DM_HCI_READ_ENC_MODE_REQ
CsrSize DM_HCI_READ_ENC_MODE_REQ_TSizeof(void *msg)
{
    DM_HCI_READ_ENC_MODE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_ENC_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_READ_ENC_MODE_REQ
CsrUint8 *DM_HCI_READ_ENC_MODE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_ENC_MODE_REQ_T *primitive;

    primitive = (DM_HCI_READ_ENC_MODE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_ENC_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_READ_ENC_MODE_REQ
void *DM_HCI_READ_ENC_MODE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_ENC_MODE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_ENC_MODE_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_ENC_MODE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_ENC_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_READ_PIN_TYPE_CFM
CsrSize DM_HCI_READ_PIN_TYPE_CFM_TSizeof(void *msg)
{
    DM_HCI_READ_PIN_TYPE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_PIN_TYPE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_PIN_TYPE_CFM
CsrUint8 *DM_HCI_READ_PIN_TYPE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_PIN_TYPE_CFM_T *primitive;

    primitive = (DM_HCI_READ_PIN_TYPE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->pin_type);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_PIN_TYPE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_PIN_TYPE_CFM
void *DM_HCI_READ_PIN_TYPE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_PIN_TYPE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_PIN_TYPE_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_PIN_TYPE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->pin_type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_PIN_TYPE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_INQUIRY_SCAN_TYPE_REQ
CsrSize DM_HCI_READ_INQUIRY_SCAN_TYPE_REQ_TSizeof(void *msg)
{
    DM_HCI_READ_INQUIRY_SCAN_TYPE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_INQUIRY_SCAN_TYPE_REQ */

#ifndef EXCLUDE_DM_HCI_READ_INQUIRY_SCAN_TYPE_REQ
CsrUint8 *DM_HCI_READ_INQUIRY_SCAN_TYPE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_INQUIRY_SCAN_TYPE_REQ_T *primitive;

    primitive = (DM_HCI_READ_INQUIRY_SCAN_TYPE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_INQUIRY_SCAN_TYPE_REQ */

#ifndef EXCLUDE_DM_HCI_READ_INQUIRY_SCAN_TYPE_REQ
void *DM_HCI_READ_INQUIRY_SCAN_TYPE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_INQUIRY_SCAN_TYPE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_INQUIRY_SCAN_TYPE_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_INQUIRY_SCAN_TYPE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_INQUIRY_SCAN_TYPE_REQ */

#ifndef EXCLUDE_DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_REQ
CsrSize DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_REQ_TSizeof(void *msg)
{
    DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_REQ */

#ifndef EXCLUDE_DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_REQ
CsrUint8 *DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_REQ_T *primitive;

    primitive = (DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_REQ */

#ifndef EXCLUDE_DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_REQ
void *DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_REQ_T *) CsrPmemZalloc(sizeof(DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_REQ */

#ifndef EXCLUDE_DM_HCI_READ_CLOCK_CFM
CsrSize DM_HCI_READ_CLOCK_CFM_TSizeof(void *msg)
{
    DM_HCI_READ_CLOCK_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_CLOCK_CFM */

#ifndef EXCLUDE_DM_HCI_READ_CLOCK_CFM
CsrUint8 *DM_HCI_READ_CLOCK_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_CLOCK_CFM_T *primitive;

    primitive = (DM_HCI_READ_CLOCK_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->clock);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->accuracy);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_CLOCK_CFM */

#ifndef EXCLUDE_DM_HCI_READ_CLOCK_CFM
void *DM_HCI_READ_CLOCK_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_CLOCK_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_CLOCK_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_CLOCK_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->clock, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->accuracy, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_CLOCK_CFM */

#ifndef EXCLUDE_DM_AMPM_REMOTE_FEATURES_IND
CsrSize DM_AMPM_REMOTE_FEATURES_IND_TSizeof(void *msg)
{
    DM_AMPM_REMOTE_FEATURES_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_AMPM_REMOTE_FEATURES_IND */

#ifndef EXCLUDE_DM_AMPM_REMOTE_FEATURES_IND
CsrUint8 *DM_AMPM_REMOTE_FEATURES_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_AMPM_REMOTE_FEATURES_IND_T *primitive;

    primitive = (DM_AMPM_REMOTE_FEATURES_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);

    return buffer;
}
#endif /* EXCLUDE_DM_AMPM_REMOTE_FEATURES_IND */

#ifndef EXCLUDE_DM_AMPM_REMOTE_FEATURES_IND
void *DM_AMPM_REMOTE_FEATURES_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_AMPM_REMOTE_FEATURES_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_AMPM_REMOTE_FEATURES_IND_T *) CsrPmemZalloc(sizeof(DM_AMPM_REMOTE_FEATURES_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_AMPM_REMOTE_FEATURES_IND */

#ifndef EXCLUDE_DM_SM_ADD_DEVICE_CFM
CsrSize DM_SM_ADD_DEVICE_CFM_TSizeof(void *msg)
{
    DM_SM_ADD_DEVICE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_ADD_DEVICE_CFM */

#ifndef EXCLUDE_DM_SM_ADD_DEVICE_CFM
CsrUint8 *DM_SM_ADD_DEVICE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_ADD_DEVICE_CFM_T *primitive;

    primitive = (DM_SM_ADD_DEVICE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_ADD_DEVICE_CFM */

#ifndef EXCLUDE_DM_SM_ADD_DEVICE_CFM
void *DM_SM_ADD_DEVICE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_ADD_DEVICE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_ADD_DEVICE_CFM_T *) CsrPmemZalloc(sizeof(DM_SM_ADD_DEVICE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_ADD_DEVICE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_RSSI_CFM
CsrSize DM_HCI_READ_RSSI_CFM_TSizeof(void *msg)
{
    DM_HCI_READ_RSSI_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_RSSI_CFM */

#ifndef EXCLUDE_DM_HCI_READ_RSSI_CFM
CsrUint8 *DM_HCI_READ_RSSI_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_RSSI_CFM_T *primitive;

    primitive = (DM_HCI_READ_RSSI_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->rssi);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_RSSI_CFM */

#ifndef EXCLUDE_DM_HCI_READ_RSSI_CFM
void *DM_HCI_READ_RSSI_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_RSSI_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_RSSI_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_RSSI_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->rssi, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_RSSI_CFM */

#ifndef EXCLUDE_DM_DATA_FROM_HCI_REQ
CsrSize DM_DATA_FROM_HCI_REQ_TSizeof(void *msg)
{
    DM_DATA_FROM_HCI_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrMblkGetLength(primitive->data) + sizeof(CsrUint16));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_DATA_FROM_HCI_REQ */

#ifndef EXCLUDE_DM_DATA_FROM_HCI_REQ
CsrUint8 *DM_DATA_FROM_HCI_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_DATA_FROM_HCI_REQ_T *primitive;

    primitive = (DM_DATA_FROM_HCI_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->controller);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->physical_handle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->logical_handle);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    if (primitive->data)
    {
        CsrUint16 mblkLength = CsrMblkGetLength(primitive->data);
        CsrUint16Ser(buffer, length, mblkLength);
        CsrMblkCopyToMemory(primitive->data, 0, mblkLength, &buffer[*length]);
        *length += mblkLength;
    }
    else
    {
        buffer[*length] = 0;
        buffer[*length + 1] = 0;
        *length += 2;
    }

    return buffer;
}
#endif /* EXCLUDE_DM_DATA_FROM_HCI_REQ */

#ifndef EXCLUDE_DM_DATA_FROM_HCI_REQ
void *DM_DATA_FROM_HCI_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_DATA_FROM_HCI_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_DATA_FROM_HCI_REQ_T *) CsrPmemZalloc(sizeof(DM_DATA_FROM_HCI_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->controller, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->physical_handle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->logical_handle, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
    {
        CsrUint8 *mblkData;
        CsrUint16 mblkLength;
        CsrUint16Des((CsrUint16 *) &mblkLength, buffer, &offset);
        primitive->data = CsrMblkMallocCreate((void **) &mblkData, mblkLength);
        CsrMemCpy(mblkData,&buffer[offset], mblkLength);
        offset += mblkLength;
    }

    return primitive;
}
#endif /* EXCLUDE_DM_DATA_FROM_HCI_REQ */

#ifndef EXCLUDE_DM_DATA_FROM_HCI_REQ
void DM_DATA_FROM_HCI_REQ_TSerFree(void *msg)
{
    DM_DATA_FROM_HCI_REQ_T *primitive;

    primitive = (DM_DATA_FROM_HCI_REQ_T *) msg;

    if(primitive->data != NULL)
    {
        CsrMblkDestroy(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_DM_DATA_FROM_HCI_REQ */

#ifndef EXCLUDE_DM_LP_WRITE_ROLESWITCH_POLICY_CFM
CsrSize DM_LP_WRITE_ROLESWITCH_POLICY_CFM_TSizeof(void *msg)
{
    DM_LP_WRITE_ROLESWITCH_POLICY_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_LP_WRITE_ROLESWITCH_POLICY_CFM */

#ifndef EXCLUDE_DM_LP_WRITE_ROLESWITCH_POLICY_CFM
CsrUint8 *DM_LP_WRITE_ROLESWITCH_POLICY_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_LP_WRITE_ROLESWITCH_POLICY_CFM_T *primitive;

    primitive = (DM_LP_WRITE_ROLESWITCH_POLICY_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_LP_WRITE_ROLESWITCH_POLICY_CFM */

#ifndef EXCLUDE_DM_LP_WRITE_ROLESWITCH_POLICY_CFM
void *DM_LP_WRITE_ROLESWITCH_POLICY_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_LP_WRITE_ROLESWITCH_POLICY_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_LP_WRITE_ROLESWITCH_POLICY_CFM_T *) CsrPmemZalloc(sizeof(DM_LP_WRITE_ROLESWITCH_POLICY_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_LP_WRITE_ROLESWITCH_POLICY_CFM */

#ifndef EXCLUDE_DM_HCI_READ_AUTO_FLUSH_TIMEOUT_CFM
CsrSize DM_HCI_READ_AUTO_FLUSH_TIMEOUT_CFM_TSizeof(void *msg)
{
    DM_HCI_READ_AUTO_FLUSH_TIMEOUT_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_AUTO_FLUSH_TIMEOUT_CFM */

#ifndef EXCLUDE_DM_HCI_READ_AUTO_FLUSH_TIMEOUT_CFM
CsrUint8 *DM_HCI_READ_AUTO_FLUSH_TIMEOUT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_AUTO_FLUSH_TIMEOUT_CFM_T *primitive;

    primitive = (DM_HCI_READ_AUTO_FLUSH_TIMEOUT_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->FlushTimeout);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_AUTO_FLUSH_TIMEOUT_CFM */

#ifndef EXCLUDE_DM_HCI_READ_AUTO_FLUSH_TIMEOUT_CFM
void *DM_HCI_READ_AUTO_FLUSH_TIMEOUT_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_AUTO_FLUSH_TIMEOUT_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_AUTO_FLUSH_TIMEOUT_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_AUTO_FLUSH_TIMEOUT_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->FlushTimeout, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_AUTO_FLUSH_TIMEOUT_CFM */

#ifndef EXCLUDE_DM_AMPM_CONNECT_CHANNEL_RSP
CsrSize DM_AMPM_CONNECT_CHANNEL_RSP_TSizeof(void *msg)
{
    DM_AMPM_CONNECT_CHANNEL_RSP_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_AMPM_CONNECT_CHANNEL_RSP */

#ifndef EXCLUDE_DM_AMPM_CONNECT_CHANNEL_RSP
CsrUint8 *DM_AMPM_CONNECT_CHANNEL_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_AMPM_CONNECT_CHANNEL_RSP_T *primitive;

    primitive = (DM_AMPM_CONNECT_CHANNEL_RSP_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->local_amp_id);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->physical_link_id);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->logical_link_id);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->hci_data_queue);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tx_flowspec.fs_identifier);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tx_flowspec.fs_service_type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->tx_flowspec.fs_max_sdu);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->tx_flowspec.fs_interarrival);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->tx_flowspec.fs_latency);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->tx_flowspec.fs_flush_to);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->rx_flowspec.fs_identifier);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->rx_flowspec.fs_service_type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->rx_flowspec.fs_max_sdu);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->rx_flowspec.fs_interarrival);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->rx_flowspec.fs_latency);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->rx_flowspec.fs_flush_to);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_AMPM_CONNECT_CHANNEL_RSP */

#ifndef EXCLUDE_DM_AMPM_CONNECT_CHANNEL_RSP
void *DM_AMPM_CONNECT_CHANNEL_RSP_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_AMPM_CONNECT_CHANNEL_RSP_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_AMPM_CONNECT_CHANNEL_RSP_T *) CsrPmemZalloc(sizeof(DM_AMPM_CONNECT_CHANNEL_RSP_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->local_amp_id, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->physical_link_id, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->logical_link_id, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->hci_data_queue, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tx_flowspec.fs_identifier, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tx_flowspec.fs_service_type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->tx_flowspec.fs_max_sdu, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->tx_flowspec.fs_interarrival, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->tx_flowspec.fs_latency, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->tx_flowspec.fs_flush_to, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->rx_flowspec.fs_identifier, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->rx_flowspec.fs_service_type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->rx_flowspec.fs_max_sdu, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->rx_flowspec.fs_interarrival, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->rx_flowspec.fs_latency, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->rx_flowspec.fs_flush_to, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_AMPM_CONNECT_CHANNEL_RSP */

#ifndef EXCLUDE_DM_WRITE_CACHED_PAGE_MODE_CFM
CsrSize DM_WRITE_CACHED_PAGE_MODE_CFM_TSizeof(void *msg)
{
    DM_WRITE_CACHED_PAGE_MODE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_WRITE_CACHED_PAGE_MODE_CFM */

#ifndef EXCLUDE_DM_WRITE_CACHED_PAGE_MODE_CFM
CsrUint8 *DM_WRITE_CACHED_PAGE_MODE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_WRITE_CACHED_PAGE_MODE_CFM_T *primitive;

    primitive = (DM_WRITE_CACHED_PAGE_MODE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_WRITE_CACHED_PAGE_MODE_CFM */

#ifndef EXCLUDE_DM_WRITE_CACHED_PAGE_MODE_CFM
void *DM_WRITE_CACHED_PAGE_MODE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_WRITE_CACHED_PAGE_MODE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_WRITE_CACHED_PAGE_MODE_CFM_T *) CsrPmemZalloc(sizeof(DM_WRITE_CACHED_PAGE_MODE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_WRITE_CACHED_PAGE_MODE_CFM */

#ifndef EXCLUDE_DM_SM_READ_LOCAL_OOB_DATA_REQ
CsrSize DM_SM_READ_LOCAL_OOB_DATA_REQ_TSizeof(void *msg)
{
    DM_SM_READ_LOCAL_OOB_DATA_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_READ_LOCAL_OOB_DATA_REQ */

#ifndef EXCLUDE_DM_SM_READ_LOCAL_OOB_DATA_REQ
CsrUint8 *DM_SM_READ_LOCAL_OOB_DATA_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_READ_LOCAL_OOB_DATA_REQ_T *primitive;

    primitive = (DM_SM_READ_LOCAL_OOB_DATA_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->unused);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_READ_LOCAL_OOB_DATA_REQ */

#ifndef EXCLUDE_DM_SM_READ_LOCAL_OOB_DATA_REQ
void *DM_SM_READ_LOCAL_OOB_DATA_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_READ_LOCAL_OOB_DATA_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_READ_LOCAL_OOB_DATA_REQ_T *) CsrPmemZalloc(sizeof(DM_SM_READ_LOCAL_OOB_DATA_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->unused, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_READ_LOCAL_OOB_DATA_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_TEST_END_CFM
CsrSize DM_HCI_ULP_TEST_END_CFM_TSizeof(void *msg)
{
    DM_HCI_ULP_TEST_END_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_TEST_END_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_TEST_END_CFM
CsrUint8 *DM_HCI_ULP_TEST_END_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_TEST_END_CFM_T *primitive;

    primitive = (DM_HCI_ULP_TEST_END_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->number_of_packets);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_TEST_END_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_TEST_END_CFM
void *DM_HCI_ULP_TEST_END_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_TEST_END_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_TEST_END_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_TEST_END_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->number_of_packets, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_TEST_END_CFM */

#ifndef EXCLUDE_DM_LP_WRITE_POWERSTATES_REQ
CsrSize DM_LP_WRITE_POWERSTATES_REQ_TSizeof(void *msg)
{
    DM_LP_WRITE_POWERSTATES_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->num_states * sizeof(LP_POWERSTATE_T)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_LP_WRITE_POWERSTATES_REQ */

#ifndef EXCLUDE_DM_LP_WRITE_POWERSTATES_REQ
CsrUint8 *DM_LP_WRITE_POWERSTATES_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_LP_WRITE_POWERSTATES_REQ_T *primitive;

    primitive = (DM_LP_WRITE_POWERSTATES_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->num_states);
    CsrMemCpySer(buffer, length, &primitive->states, sizeof(primitive->states));
    if(primitive->states)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->states), ((CsrSize) (primitive->num_states * sizeof(LP_POWERSTATE_T))));
    }

    return buffer;
}
#endif /* EXCLUDE_DM_LP_WRITE_POWERSTATES_REQ */

#ifndef EXCLUDE_DM_LP_WRITE_POWERSTATES_REQ
void *DM_LP_WRITE_POWERSTATES_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_LP_WRITE_POWERSTATES_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_LP_WRITE_POWERSTATES_REQ_T *) CsrPmemZalloc(sizeof(DM_LP_WRITE_POWERSTATES_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->num_states, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->states), buffer, &offset, ((CsrSize) sizeof(primitive->states)));
    if (primitive->num_states)
    {
        primitive->states = CsrPmemZalloc(((CsrUint32) (primitive->num_states * sizeof(LP_POWERSTATE_T))));
        CsrMemCpyDes(((void *) primitive->states), buffer, &offset, ((CsrSize) (primitive->num_states * sizeof(LP_POWERSTATE_T))));
    }
    else
    {
        primitive->states = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_DM_LP_WRITE_POWERSTATES_REQ */

#ifndef EXCLUDE_DM_LP_WRITE_POWERSTATES_REQ
void DM_LP_WRITE_POWERSTATES_REQ_TSerFree(void *msg)
{
    DM_LP_WRITE_POWERSTATES_REQ_T *primitive;

    primitive = (DM_LP_WRITE_POWERSTATES_REQ_T *) msg;

    if(primitive->states != NULL)
    {
        CsrPmemFree(primitive->states);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_DM_LP_WRITE_POWERSTATES_REQ */

#ifndef EXCLUDE_DM_SM_ENCRYPT_REQ
CsrSize DM_SM_ENCRYPT_REQ_TSizeof(void *msg)
{
    DM_SM_ENCRYPT_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_ENCRYPT_REQ */

#ifndef EXCLUDE_DM_SM_ENCRYPT_REQ
CsrUint8 *DM_SM_ENCRYPT_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_ENCRYPT_REQ_T *primitive;

    primitive = (DM_SM_ENCRYPT_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->encrypt);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_ENCRYPT_REQ */

#ifndef EXCLUDE_DM_SM_ENCRYPT_REQ
void *DM_SM_ENCRYPT_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_ENCRYPT_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_ENCRYPT_REQ_T *) CsrPmemZalloc(sizeof(DM_SM_ENCRYPT_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->encrypt, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_ENCRYPT_REQ */

#ifndef EXCLUDE_DM_SM_KEY_REQUEST_NEG_RSP
CsrSize DM_SM_KEY_REQUEST_NEG_RSP_TSizeof(void *msg)
{
    DM_SM_KEY_REQUEST_NEG_RSP_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_KEY_REQUEST_NEG_RSP */

#ifndef EXCLUDE_DM_SM_KEY_REQUEST_NEG_RSP
CsrUint8 *DM_SM_KEY_REQUEST_NEG_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_KEY_REQUEST_NEG_RSP_T *primitive;

    primitive = (DM_SM_KEY_REQUEST_NEG_RSP_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->key_type);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_KEY_REQUEST_NEG_RSP */

#ifndef EXCLUDE_DM_SM_KEY_REQUEST_NEG_RSP
void *DM_SM_KEY_REQUEST_NEG_RSP_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_KEY_REQUEST_NEG_RSP_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_KEY_REQUEST_NEG_RSP_T *) CsrPmemZalloc(sizeof(DM_SM_KEY_REQUEST_NEG_RSP_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    {
        CsrUint16 data;
        CsrUint16Des((CsrUint16 *) &data, buffer, &offset);
        primitive->key_type = (DM_SM_KEY_TYPE_T) data;
    }

    return primitive;
}
#endif /* EXCLUDE_DM_SM_KEY_REQUEST_NEG_RSP */

#ifndef EXCLUDE_DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_REQ
CsrSize DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_REQ_TSizeof(void *msg)
{
    DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_REQ
CsrUint8 *DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_REQ_T *primitive;

    primitive = (DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->class_mode);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_REQ
void *DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->class_mode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_REQ */

#ifndef EXCLUDE_DM_HCI_CONN_PACKET_TYPE_CHANGED_IND
CsrSize DM_HCI_CONN_PACKET_TYPE_CHANGED_IND_TSizeof(void *msg)
{
    DM_HCI_CONN_PACKET_TYPE_CHANGED_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_CONN_PACKET_TYPE_CHANGED_IND */

#ifndef EXCLUDE_DM_HCI_CONN_PACKET_TYPE_CHANGED_IND
CsrUint8 *DM_HCI_CONN_PACKET_TYPE_CHANGED_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_CONN_PACKET_TYPE_CHANGED_IND_T *primitive;

    primitive = (DM_HCI_CONN_PACKET_TYPE_CHANGED_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_CONN_PACKET_TYPE_CHANGED_IND */

#ifndef EXCLUDE_DM_HCI_CONN_PACKET_TYPE_CHANGED_IND
void *DM_HCI_CONN_PACKET_TYPE_CHANGED_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_CONN_PACKET_TYPE_CHANGED_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_CONN_PACKET_TYPE_CHANGED_IND_T *) CsrPmemZalloc(sizeof(DM_HCI_CONN_PACKET_TYPE_CHANGED_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_CONN_PACKET_TYPE_CHANGED_IND */

#ifndef EXCLUDE_DM_HCI_SET_EVENT_FILTER_CFM
CsrSize DM_HCI_SET_EVENT_FILTER_CFM_TSizeof(void *msg)
{
    DM_HCI_SET_EVENT_FILTER_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_SET_EVENT_FILTER_CFM */

#ifndef EXCLUDE_DM_HCI_SET_EVENT_FILTER_CFM
CsrUint8 *DM_HCI_SET_EVENT_FILTER_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_SET_EVENT_FILTER_CFM_T *primitive;

    primitive = (DM_HCI_SET_EVENT_FILTER_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_SET_EVENT_FILTER_CFM */

#ifndef EXCLUDE_DM_HCI_SET_EVENT_FILTER_CFM
void *DM_HCI_SET_EVENT_FILTER_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_SET_EVENT_FILTER_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_SET_EVENT_FILTER_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_SET_EVENT_FILTER_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_SET_EVENT_FILTER_CFM */

#ifndef EXCLUDE_DM_HCI_READ_INQUIRY_MODE_REQ
CsrSize DM_HCI_READ_INQUIRY_MODE_REQ_TSizeof(void *msg)
{
    DM_HCI_READ_INQUIRY_MODE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_INQUIRY_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_READ_INQUIRY_MODE_REQ
CsrUint8 *DM_HCI_READ_INQUIRY_MODE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_INQUIRY_MODE_REQ_T *primitive;

    primitive = (DM_HCI_READ_INQUIRY_MODE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_INQUIRY_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_READ_INQUIRY_MODE_REQ
void *DM_HCI_READ_INQUIRY_MODE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_INQUIRY_MODE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_INQUIRY_MODE_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_INQUIRY_MODE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_INQUIRY_MODE_REQ */

#ifndef EXCLUDE_DM_SM_AUTHENTICATE_REQ
CsrSize DM_SM_AUTHENTICATE_REQ_TSizeof(void *msg)
{
    DM_SM_AUTHENTICATE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_AUTHENTICATE_REQ */

#ifndef EXCLUDE_DM_SM_AUTHENTICATE_REQ
CsrUint8 *DM_SM_AUTHENTICATE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_AUTHENTICATE_REQ_T *primitive;

    primitive = (DM_SM_AUTHENTICATE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_AUTHENTICATE_REQ */

#ifndef EXCLUDE_DM_SM_AUTHENTICATE_REQ
void *DM_SM_AUTHENTICATE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_AUTHENTICATE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_AUTHENTICATE_REQ_T *) CsrPmemZalloc(sizeof(DM_SM_AUTHENTICATE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_AUTHENTICATE_REQ */

#ifndef EXCLUDE_DM_BLE_UPDATE_CONNECTION_PARAMETERS_REQ
CsrSize DM_BLE_UPDATE_CONNECTION_PARAMETERS_REQ_TSizeof(void *msg)
{
    DM_BLE_UPDATE_CONNECTION_PARAMETERS_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_BLE_UPDATE_CONNECTION_PARAMETERS_REQ */

#ifndef EXCLUDE_DM_BLE_UPDATE_CONNECTION_PARAMETERS_REQ
CsrUint8 *DM_BLE_UPDATE_CONNECTION_PARAMETERS_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_BLE_UPDATE_CONNECTION_PARAMETERS_REQ_T *primitive;

    primitive = (DM_BLE_UPDATE_CONNECTION_PARAMETERS_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->connection_handle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_interval_min);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_interval_max);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_latency);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->supervision_timeout);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->minimum_ce_length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maximum_ce_length);

    return buffer;
}
#endif /* EXCLUDE_DM_BLE_UPDATE_CONNECTION_PARAMETERS_REQ */

#ifndef EXCLUDE_DM_BLE_UPDATE_CONNECTION_PARAMETERS_REQ
void *DM_BLE_UPDATE_CONNECTION_PARAMETERS_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_BLE_UPDATE_CONNECTION_PARAMETERS_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_BLE_UPDATE_CONNECTION_PARAMETERS_REQ_T *) CsrPmemZalloc(sizeof(DM_BLE_UPDATE_CONNECTION_PARAMETERS_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->connection_handle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_interval_min, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_interval_max, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_latency, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->supervision_timeout, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->minimum_ce_length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maximum_ce_length, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_BLE_UPDATE_CONNECTION_PARAMETERS_REQ */

#ifndef EXCLUDE_DM_LP_WRITE_ALWAYS_MASTER_DEVICES_REQ
CsrSize DM_LP_WRITE_ALWAYS_MASTER_DEVICES_REQ_TSizeof(void *msg)
{
    DM_LP_WRITE_ALWAYS_MASTER_DEVICES_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_LP_WRITE_ALWAYS_MASTER_DEVICES_REQ */

#ifndef EXCLUDE_DM_LP_WRITE_ALWAYS_MASTER_DEVICES_REQ
CsrUint8 *DM_LP_WRITE_ALWAYS_MASTER_DEVICES_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_LP_WRITE_ALWAYS_MASTER_DEVICES_REQ_T *primitive;

    primitive = (DM_LP_WRITE_ALWAYS_MASTER_DEVICES_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->operation);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_LP_WRITE_ALWAYS_MASTER_DEVICES_REQ */

#ifndef EXCLUDE_DM_LP_WRITE_ALWAYS_MASTER_DEVICES_REQ
void *DM_LP_WRITE_ALWAYS_MASTER_DEVICES_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_LP_WRITE_ALWAYS_MASTER_DEVICES_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_LP_WRITE_ALWAYS_MASTER_DEVICES_REQ_T *) CsrPmemZalloc(sizeof(DM_LP_WRITE_ALWAYS_MASTER_DEVICES_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->operation, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_LP_WRITE_ALWAYS_MASTER_DEVICES_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_PAGESCAN_ACTIVITY_CFM
CsrSize DM_HCI_WRITE_PAGESCAN_ACTIVITY_CFM_TSizeof(void *msg)
{
    DM_HCI_WRITE_PAGESCAN_ACTIVITY_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_PAGESCAN_ACTIVITY_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_PAGESCAN_ACTIVITY_CFM
CsrUint8 *DM_HCI_WRITE_PAGESCAN_ACTIVITY_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_PAGESCAN_ACTIVITY_CFM_T *primitive;

    primitive = (DM_HCI_WRITE_PAGESCAN_ACTIVITY_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_PAGESCAN_ACTIVITY_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_PAGESCAN_ACTIVITY_CFM
void *DM_HCI_WRITE_PAGESCAN_ACTIVITY_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_PAGESCAN_ACTIVITY_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_PAGESCAN_ACTIVITY_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_PAGESCAN_ACTIVITY_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_PAGESCAN_ACTIVITY_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_SET_RANDOM_ADDRESS_REQ
CsrSize DM_HCI_ULP_SET_RANDOM_ADDRESS_REQ_TSizeof(void *msg)
{
    DM_HCI_ULP_SET_RANDOM_ADDRESS_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_RANDOM_ADDRESS_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_SET_RANDOM_ADDRESS_REQ
CsrUint8 *DM_HCI_ULP_SET_RANDOM_ADDRESS_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_SET_RANDOM_ADDRESS_REQ_T *primitive;

    primitive = (DM_HCI_ULP_SET_RANDOM_ADDRESS_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->random_address.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->random_address.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->random_address.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_RANDOM_ADDRESS_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_SET_RANDOM_ADDRESS_REQ
void *DM_HCI_ULP_SET_RANDOM_ADDRESS_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_SET_RANDOM_ADDRESS_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_SET_RANDOM_ADDRESS_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_SET_RANDOM_ADDRESS_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->random_address.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->random_address.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->random_address.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_RANDOM_ADDRESS_REQ */

#ifndef EXCLUDE_DM_HCI_CHANGE_CONN_LINK_KEY_REQ
CsrSize DM_HCI_CHANGE_CONN_LINK_KEY_REQ_TSizeof(void *msg)
{
    DM_HCI_CHANGE_CONN_LINK_KEY_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_CHANGE_CONN_LINK_KEY_REQ */

#ifndef EXCLUDE_DM_HCI_CHANGE_CONN_LINK_KEY_REQ
CsrUint8 *DM_HCI_CHANGE_CONN_LINK_KEY_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_CHANGE_CONN_LINK_KEY_REQ_T *primitive;

    primitive = (DM_HCI_CHANGE_CONN_LINK_KEY_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_CHANGE_CONN_LINK_KEY_REQ */

#ifndef EXCLUDE_DM_HCI_CHANGE_CONN_LINK_KEY_REQ
void *DM_HCI_CHANGE_CONN_LINK_KEY_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_CHANGE_CONN_LINK_KEY_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_CHANGE_CONN_LINK_KEY_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_CHANGE_CONN_LINK_KEY_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_CHANGE_CONN_LINK_KEY_REQ */

#ifndef EXCLUDE_DM_SYNC_UNREGISTER_CFM
CsrSize DM_SYNC_UNREGISTER_CFM_TSizeof(void *msg)
{
    DM_SYNC_UNREGISTER_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SYNC_UNREGISTER_CFM */

#ifndef EXCLUDE_DM_SYNC_UNREGISTER_CFM
CsrUint8 *DM_SYNC_UNREGISTER_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SYNC_UNREGISTER_CFM_T *primitive;

    primitive = (DM_SYNC_UNREGISTER_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pv_cbarg);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_SYNC_UNREGISTER_CFM */

#ifndef EXCLUDE_DM_SYNC_UNREGISTER_CFM
void *DM_SYNC_UNREGISTER_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SYNC_UNREGISTER_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SYNC_UNREGISTER_CFM_T *) CsrPmemZalloc(sizeof(DM_SYNC_UNREGISTER_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pv_cbarg, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SYNC_UNREGISTER_CFM */

#ifndef EXCLUDE_DM_HCI_ROLE_DISCOVERY_REQ
CsrSize DM_HCI_ROLE_DISCOVERY_REQ_TSizeof(void *msg)
{
    DM_HCI_ROLE_DISCOVERY_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ROLE_DISCOVERY_REQ */

#ifndef EXCLUDE_DM_HCI_ROLE_DISCOVERY_REQ
CsrUint8 *DM_HCI_ROLE_DISCOVERY_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ROLE_DISCOVERY_REQ_T *primitive;

    primitive = (DM_HCI_ROLE_DISCOVERY_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ROLE_DISCOVERY_REQ */

#ifndef EXCLUDE_DM_HCI_ROLE_DISCOVERY_REQ
void *DM_HCI_ROLE_DISCOVERY_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ROLE_DISCOVERY_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ROLE_DISCOVERY_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_ROLE_DISCOVERY_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ROLE_DISCOVERY_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_CFM
CsrSize DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_CFM_TSizeof(void *msg)
{
    DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_CFM
CsrUint8 *DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_CFM_T *primitive;

    primitive = (DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_CFM
void *DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_CFM */

#ifndef EXCLUDE_DM_AMPM_REGISTER_REQ
CsrSize DM_AMPM_REGISTER_REQ_TSizeof(void *msg)
{
    DM_AMPM_REGISTER_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_AMPM_REGISTER_REQ */

#ifndef EXCLUDE_DM_AMPM_REGISTER_REQ
CsrUint8 *DM_AMPM_REGISTER_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_AMPM_REGISTER_REQ_T *primitive;

    primitive = (DM_AMPM_REGISTER_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);

    return buffer;
}
#endif /* EXCLUDE_DM_AMPM_REGISTER_REQ */

#ifndef EXCLUDE_DM_AMPM_REGISTER_REQ
void *DM_AMPM_REGISTER_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_AMPM_REGISTER_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_AMPM_REGISTER_REQ_T *) CsrPmemZalloc(sizeof(DM_AMPM_REGISTER_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_AMPM_REGISTER_REQ */

#ifndef EXCLUDE_DM_HCI_READ_REMOTE_EXT_FEATURES_CFM
CsrSize DM_HCI_READ_REMOTE_EXT_FEATURES_CFM_TSizeof(void *msg)
{
    DM_HCI_READ_REMOTE_EXT_FEATURES_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_REMOTE_EXT_FEATURES_CFM */

#ifndef EXCLUDE_DM_HCI_READ_REMOTE_EXT_FEATURES_CFM
CsrUint8 *DM_HCI_READ_REMOTE_EXT_FEATURES_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_REMOTE_EXT_FEATURES_CFM_T *primitive;

    primitive = (DM_HCI_READ_REMOTE_EXT_FEATURES_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->page_num);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->max_page_num);
    CsrMemCpySer(buffer, length, ((const void *) primitive->lmp_ext_features), ((CsrSize) 2 * (4)));

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_REMOTE_EXT_FEATURES_CFM */

#ifndef EXCLUDE_DM_HCI_READ_REMOTE_EXT_FEATURES_CFM
void *DM_HCI_READ_REMOTE_EXT_FEATURES_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_REMOTE_EXT_FEATURES_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_REMOTE_EXT_FEATURES_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_REMOTE_EXT_FEATURES_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->page_num, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->max_page_num, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->lmp_ext_features), buffer, &offset, ((CsrSize) 2 * (4)));

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_REMOTE_EXT_FEATURES_CFM */

#ifndef EXCLUDE_DM_HCI_READ_STORED_LINK_KEY_CFM
CsrSize DM_HCI_READ_STORED_LINK_KEY_CFM_TSizeof(void *msg)
{
    DM_HCI_READ_STORED_LINK_KEY_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_STORED_LINK_KEY_CFM */

#ifndef EXCLUDE_DM_HCI_READ_STORED_LINK_KEY_CFM
CsrUint8 *DM_HCI_READ_STORED_LINK_KEY_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_STORED_LINK_KEY_CFM_T *primitive;

    primitive = (DM_HCI_READ_STORED_LINK_KEY_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->max_num_keys);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->num_keys_read);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_STORED_LINK_KEY_CFM */

#ifndef EXCLUDE_DM_HCI_READ_STORED_LINK_KEY_CFM
void *DM_HCI_READ_STORED_LINK_KEY_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_STORED_LINK_KEY_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_STORED_LINK_KEY_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_STORED_LINK_KEY_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->max_num_keys, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->num_keys_read, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_STORED_LINK_KEY_CFM */

#ifndef EXCLUDE_DM_HANDLE_DEVICE_BLACK_LIST_REQ
CsrSize DM_HANDLE_DEVICE_BLACK_LIST_REQ_TSizeof(void *msg)
{
    DM_HANDLE_DEVICE_BLACK_LIST_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HANDLE_DEVICE_BLACK_LIST_REQ */

#ifndef EXCLUDE_DM_HANDLE_DEVICE_BLACK_LIST_REQ
CsrUint8 *DM_HANDLE_DEVICE_BLACK_LIST_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HANDLE_DEVICE_BLACK_LIST_REQ_T *primitive;

    primitive = (DM_HANDLE_DEVICE_BLACK_LIST_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);

    return buffer;
}
#endif /* EXCLUDE_DM_HANDLE_DEVICE_BLACK_LIST_REQ */

#ifndef EXCLUDE_DM_HANDLE_DEVICE_BLACK_LIST_REQ
void *DM_HANDLE_DEVICE_BLACK_LIST_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HANDLE_DEVICE_BLACK_LIST_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HANDLE_DEVICE_BLACK_LIST_REQ_T *) CsrPmemZalloc(sizeof(DM_HANDLE_DEVICE_BLACK_LIST_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HANDLE_DEVICE_BLACK_LIST_REQ */

#ifndef EXCLUDE_DM_SM_UNREGISTER_REQ
CsrSize DM_SM_UNREGISTER_REQ_TSizeof(void *msg)
{
    DM_SM_UNREGISTER_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_UNREGISTER_REQ */

#ifndef EXCLUDE_DM_SM_UNREGISTER_REQ
CsrUint8 *DM_SM_UNREGISTER_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_UNREGISTER_REQ_T *primitive;

    primitive = (DM_SM_UNREGISTER_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->service.protocol_id);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->service.channel);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_UNREGISTER_REQ */

#ifndef EXCLUDE_DM_SM_UNREGISTER_REQ
void *DM_SM_UNREGISTER_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_UNREGISTER_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_UNREGISTER_REQ_T *) CsrPmemZalloc(sizeof(DM_SM_UNREGISTER_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->service.protocol_id, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->service.channel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_UNREGISTER_REQ */

#ifndef EXCLUDE_DM_SM_DATA_SIGN_REQ
CsrSize DM_SM_DATA_SIGN_REQ_TSizeof(void *msg)
{
    DM_SM_DATA_SIGN_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->length * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_DATA_SIGN_REQ */

#ifndef EXCLUDE_DM_SM_DATA_SIGN_REQ
CsrUint8 *DM_SM_DATA_SIGN_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_DATA_SIGN_REQ_T *primitive;

    primitive = (DM_SM_DATA_SIGN_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->verify);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->length);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    if(primitive->data)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->data), ((CsrSize) (primitive->length * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_DM_SM_DATA_SIGN_REQ */

#ifndef EXCLUDE_DM_SM_DATA_SIGN_REQ
void *DM_SM_DATA_SIGN_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_DATA_SIGN_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_DATA_SIGN_REQ_T *) CsrPmemZalloc(sizeof(DM_SM_DATA_SIGN_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->verify, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->length, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    if (primitive->length)
    {
        primitive->data = CsrPmemZalloc(((CsrUint32) (primitive->length * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->data), buffer, &offset, ((CsrSize) (primitive->length * sizeof(CsrUint8))));
    }
    else
    {
        primitive->data = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_DM_SM_DATA_SIGN_REQ */

#ifndef EXCLUDE_DM_SM_DATA_SIGN_REQ
void DM_SM_DATA_SIGN_REQ_TSerFree(void *msg)
{
    DM_SM_DATA_SIGN_REQ_T *primitive;

    primitive = (DM_SM_DATA_SIGN_REQ_T *) msg;

    if(primitive->data != NULL)
    {
        CsrPmemFree(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_DM_SM_DATA_SIGN_REQ */

#ifndef EXCLUDE_DM_HCI_MODE_CHANGE_EVENT_IND
CsrSize DM_HCI_MODE_CHANGE_EVENT_IND_TSizeof(void *msg)
{
    DM_HCI_MODE_CHANGE_EVENT_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_MODE_CHANGE_EVENT_IND */

#ifndef EXCLUDE_DM_HCI_MODE_CHANGE_EVENT_IND
CsrUint8 *DM_HCI_MODE_CHANGE_EVENT_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_MODE_CHANGE_EVENT_IND_T *primitive;

    primitive = (DM_HCI_MODE_CHANGE_EVENT_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->length);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_MODE_CHANGE_EVENT_IND */

#ifndef EXCLUDE_DM_HCI_MODE_CHANGE_EVENT_IND
void *DM_HCI_MODE_CHANGE_EVENT_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_MODE_CHANGE_EVENT_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_MODE_CHANGE_EVENT_IND_T *) CsrPmemZalloc(sizeof(DM_HCI_MODE_CHANGE_EVENT_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->length, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_MODE_CHANGE_EVENT_IND */

#ifndef EXCLUDE_DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_RSP
CsrSize DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_RSP_TSizeof(void *msg)
{
    DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_RSP_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_RSP */

#ifndef EXCLUDE_DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_RSP
CsrUint8 *DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_RSP_T *primitive;

    primitive = (DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_RSP_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->signal_id);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addrt.addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_interval_min);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_interval_max);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_latency);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->supervision_timeout);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_RSP */

#ifndef EXCLUDE_DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_RSP
void *DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_RSP_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_RSP_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_RSP_T *) CsrPmemZalloc(sizeof(DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_RSP_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->signal_id, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addrt.addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_interval_min, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_interval_max, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_latency, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->supervision_timeout, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_RSP */

#ifndef EXCLUDE_DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_CFM
CsrSize DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_CFM_TSizeof(void *msg)
{
    DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_CFM */

#ifndef EXCLUDE_DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_CFM
CsrUint8 *DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_CFM_T *primitive;

    primitive = (DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_CFM */

#ifndef EXCLUDE_DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_CFM
void *DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_CFM_T *) CsrPmemZalloc(sizeof(DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_CFM */

#ifndef EXCLUDE_DM_HCI_PARK_MODE_REQ
CsrSize DM_HCI_PARK_MODE_REQ_TSizeof(void *msg)
{
    DM_HCI_PARK_MODE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_PARK_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_PARK_MODE_REQ
CsrUint8 *DM_HCI_PARK_MODE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_PARK_MODE_REQ_T *primitive;

    primitive = (DM_HCI_PARK_MODE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->max_interval);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->min_interval);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_PARK_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_PARK_MODE_REQ
void *DM_HCI_PARK_MODE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_PARK_MODE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_PARK_MODE_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_PARK_MODE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->max_interval, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->min_interval, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_PARK_MODE_REQ */

#ifndef EXCLUDE_DM_BLUESTACK_STATE_TRANSITION_ENTRY_IND
CsrSize DM_BLUESTACK_STATE_TRANSITION_ENTRY_IND_TSizeof(void *msg)
{
    DM_BLUESTACK_STATE_TRANSITION_ENTRY_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_BLUESTACK_STATE_TRANSITION_ENTRY_IND */

#ifndef EXCLUDE_DM_BLUESTACK_STATE_TRANSITION_ENTRY_IND
CsrUint8 *DM_BLUESTACK_STATE_TRANSITION_ENTRY_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_BLUESTACK_STATE_TRANSITION_ENTRY_IND_T *primitive;

    primitive = (DM_BLUESTACK_STATE_TRANSITION_ENTRY_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->state_machine);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->event);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->old_state);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->new_state);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->action);

    return buffer;
}
#endif /* EXCLUDE_DM_BLUESTACK_STATE_TRANSITION_ENTRY_IND */

#ifndef EXCLUDE_DM_BLUESTACK_STATE_TRANSITION_ENTRY_IND
void *DM_BLUESTACK_STATE_TRANSITION_ENTRY_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_BLUESTACK_STATE_TRANSITION_ENTRY_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_BLUESTACK_STATE_TRANSITION_ENTRY_IND_T *) CsrPmemZalloc(sizeof(DM_BLUESTACK_STATE_TRANSITION_ENTRY_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->state_machine, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->event, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->old_state, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->new_state, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->action, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_BLUESTACK_STATE_TRANSITION_ENTRY_IND */

#ifndef EXCLUDE_DM_AMPM_LINK_KEY_NOTIFICATION_IND
CsrSize DM_AMPM_LINK_KEY_NOTIFICATION_IND_TSizeof(void *msg)
{
    DM_AMPM_LINK_KEY_NOTIFICATION_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_AMPM_LINK_KEY_NOTIFICATION_IND */

#ifndef EXCLUDE_DM_AMPM_LINK_KEY_NOTIFICATION_IND
CsrUint8 *DM_AMPM_LINK_KEY_NOTIFICATION_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_AMPM_LINK_KEY_NOTIFICATION_IND_T *primitive;

    primitive = (DM_AMPM_LINK_KEY_NOTIFICATION_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->key_type);
    CsrMemCpySer(buffer, length, ((const void *) primitive->key), ((CsrSize) (16)));

    return buffer;
}
#endif /* EXCLUDE_DM_AMPM_LINK_KEY_NOTIFICATION_IND */

#ifndef EXCLUDE_DM_AMPM_LINK_KEY_NOTIFICATION_IND
void *DM_AMPM_LINK_KEY_NOTIFICATION_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_AMPM_LINK_KEY_NOTIFICATION_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_AMPM_LINK_KEY_NOTIFICATION_IND_T *) CsrPmemZalloc(sizeof(DM_AMPM_LINK_KEY_NOTIFICATION_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->key_type, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->key), buffer, &offset, ((CsrSize) (16)));

    return primitive;
}
#endif /* EXCLUDE_DM_AMPM_LINK_KEY_NOTIFICATION_IND */

#ifndef EXCLUDE_DM_HCI_SET_AFH_CHANNEL_CLASS_CFM
CsrSize DM_HCI_SET_AFH_CHANNEL_CLASS_CFM_TSizeof(void *msg)
{
    DM_HCI_SET_AFH_CHANNEL_CLASS_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_SET_AFH_CHANNEL_CLASS_CFM */

#ifndef EXCLUDE_DM_HCI_SET_AFH_CHANNEL_CLASS_CFM
CsrUint8 *DM_HCI_SET_AFH_CHANNEL_CLASS_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_SET_AFH_CHANNEL_CLASS_CFM_T *primitive;

    primitive = (DM_HCI_SET_AFH_CHANNEL_CLASS_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_SET_AFH_CHANNEL_CLASS_CFM */

#ifndef EXCLUDE_DM_HCI_SET_AFH_CHANNEL_CLASS_CFM
void *DM_HCI_SET_AFH_CHANNEL_CLASS_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_SET_AFH_CHANNEL_CLASS_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_SET_AFH_CHANNEL_CLASS_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_SET_AFH_CHANNEL_CLASS_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_SET_AFH_CHANNEL_CLASS_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_PIN_TYPE_REQ
CsrSize DM_HCI_WRITE_PIN_TYPE_REQ_TSizeof(void *msg)
{
    DM_HCI_WRITE_PIN_TYPE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_PIN_TYPE_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_PIN_TYPE_REQ
CsrUint8 *DM_HCI_WRITE_PIN_TYPE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_PIN_TYPE_REQ_T *primitive;

    primitive = (DM_HCI_WRITE_PIN_TYPE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->pin_type);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_PIN_TYPE_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_PIN_TYPE_REQ
void *DM_HCI_WRITE_PIN_TYPE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_PIN_TYPE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_PIN_TYPE_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_PIN_TYPE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->pin_type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_PIN_TYPE_REQ */

#ifndef EXCLUDE_DM_SM_INIT_REQ
CsrSize DM_SM_INIT_REQ_TSizeof(void *msg)
{
    DM_SM_INIT_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_INIT_REQ */

#ifndef EXCLUDE_DM_SM_INIT_REQ
CsrUint8 *DM_SM_INIT_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_INIT_REQ_T *primitive;

    primitive = (DM_SM_INIT_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->options);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->security_mode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->security_level_default);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->write_auth_enable);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mode3_enc);
    CsrMemCpySer(buffer, length, ((const void *) primitive->sm_key_state.er), ((CsrSize) 2 * (8)));
    CsrMemCpySer(buffer, length, ((const void *) primitive->sm_key_state.ir), ((CsrSize) 2 * (8)));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->sm_div_state);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->sm_sign_counter);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->max_enc_key_size_thres);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->min_enc_key_size_thres);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_INIT_REQ */

#ifndef EXCLUDE_DM_SM_INIT_REQ
void *DM_SM_INIT_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_INIT_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_INIT_REQ_T *) CsrPmemZalloc(sizeof(DM_SM_INIT_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->options, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->security_mode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->security_level_default, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->write_auth_enable, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mode3_enc, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->sm_key_state.er), buffer, &offset, ((CsrSize) 2 * (8)));
    CsrMemCpyDes(((void *) primitive->sm_key_state.ir), buffer, &offset, ((CsrSize) 2 * (8)));
    CsrUint16Des((CsrUint16 *) &primitive->sm_div_state, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->sm_sign_counter, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->max_enc_key_size_thres, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->min_enc_key_size_thres, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_INIT_REQ */

#ifndef EXCLUDE_DM_HCI_READ_LOCAL_SUPP_FEATURES_CFM
CsrSize DM_HCI_READ_LOCAL_SUPP_FEATURES_CFM_TSizeof(void *msg)
{
    DM_HCI_READ_LOCAL_SUPP_FEATURES_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_LOCAL_SUPP_FEATURES_CFM */

#ifndef EXCLUDE_DM_HCI_READ_LOCAL_SUPP_FEATURES_CFM
CsrUint8 *DM_HCI_READ_LOCAL_SUPP_FEATURES_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_LOCAL_SUPP_FEATURES_CFM_T *primitive;

    primitive = (DM_HCI_READ_LOCAL_SUPP_FEATURES_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrMemCpySer(buffer, length, ((const void *) primitive->lmp_supp_features), ((CsrSize) (8)));

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_LOCAL_SUPP_FEATURES_CFM */

#ifndef EXCLUDE_DM_HCI_READ_LOCAL_SUPP_FEATURES_CFM
void *DM_HCI_READ_LOCAL_SUPP_FEATURES_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_LOCAL_SUPP_FEATURES_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_LOCAL_SUPP_FEATURES_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_LOCAL_SUPP_FEATURES_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->lmp_supp_features), buffer, &offset, ((CsrSize) (8)));

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_LOCAL_SUPP_FEATURES_CFM */

#ifndef EXCLUDE_DM_CLEAR_PARAM_CACHE_CFM
CsrSize DM_CLEAR_PARAM_CACHE_CFM_TSizeof(void *msg)
{
    DM_CLEAR_PARAM_CACHE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_CLEAR_PARAM_CACHE_CFM */

#ifndef EXCLUDE_DM_CLEAR_PARAM_CACHE_CFM
CsrUint8 *DM_CLEAR_PARAM_CACHE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_CLEAR_PARAM_CACHE_CFM_T *primitive;

    primitive = (DM_CLEAR_PARAM_CACHE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_CLEAR_PARAM_CACHE_CFM */

#ifndef EXCLUDE_DM_CLEAR_PARAM_CACHE_CFM
void *DM_CLEAR_PARAM_CACHE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_CLEAR_PARAM_CACHE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_CLEAR_PARAM_CACHE_CFM_T *) CsrPmemZalloc(sizeof(DM_CLEAR_PARAM_CACHE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_CLEAR_PARAM_CACHE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_CFM
CsrSize DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_CFM_TSizeof(void *msg)
{
    DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_CFM */

#ifndef EXCLUDE_DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_CFM
CsrUint8 *DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_CFM_T *primitive;

    primitive = (DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->default_lps);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_CFM */

#ifndef EXCLUDE_DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_CFM
void *DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->default_lps, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_CFM */

#ifndef EXCLUDE_DM_AMPM_CONNECT_IND
CsrSize DM_AMPM_CONNECT_IND_TSizeof(void *msg)
{
    DM_AMPM_CONNECT_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_AMPM_CONNECT_IND */

#ifndef EXCLUDE_DM_AMPM_CONNECT_IND
CsrUint8 *DM_AMPM_CONNECT_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_AMPM_CONNECT_IND_T *primitive;

    primitive = (DM_AMPM_CONNECT_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->local_amp_id);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->remote_amp_id);

    return buffer;
}
#endif /* EXCLUDE_DM_AMPM_CONNECT_IND */

#ifndef EXCLUDE_DM_AMPM_CONNECT_IND
void *DM_AMPM_CONNECT_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_AMPM_CONNECT_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_AMPM_CONNECT_IND_T *) CsrPmemZalloc(sizeof(DM_AMPM_CONNECT_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->local_amp_id, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->remote_amp_id, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_AMPM_CONNECT_IND */

#ifndef EXCLUDE_DM_HCI_READ_PAGESCAN_MODE_REQ
CsrSize DM_HCI_READ_PAGESCAN_MODE_REQ_TSizeof(void *msg)
{
    DM_HCI_READ_PAGESCAN_MODE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_PAGESCAN_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_READ_PAGESCAN_MODE_REQ
CsrUint8 *DM_HCI_READ_PAGESCAN_MODE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_PAGESCAN_MODE_REQ_T *primitive;

    primitive = (DM_HCI_READ_PAGESCAN_MODE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_PAGESCAN_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_READ_PAGESCAN_MODE_REQ
void *DM_HCI_READ_PAGESCAN_MODE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_PAGESCAN_MODE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_PAGESCAN_MODE_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_PAGESCAN_MODE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_PAGESCAN_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_RESET_REQ
CsrSize DM_HCI_RESET_REQ_TSizeof(void *msg)
{
    DM_HCI_RESET_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_RESET_REQ */

#ifndef EXCLUDE_DM_HCI_RESET_REQ
CsrUint8 *DM_HCI_RESET_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_RESET_REQ_T *primitive;

    primitive = (DM_HCI_RESET_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_RESET_REQ */

#ifndef EXCLUDE_DM_HCI_RESET_REQ
void *DM_HCI_RESET_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_RESET_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_RESET_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_RESET_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_RESET_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_SET_ADVERTISE_ENABLE_REQ
CsrSize DM_HCI_ULP_SET_ADVERTISE_ENABLE_REQ_TSizeof(void *msg)
{
    DM_HCI_ULP_SET_ADVERTISE_ENABLE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_ADVERTISE_ENABLE_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_SET_ADVERTISE_ENABLE_REQ
CsrUint8 *DM_HCI_ULP_SET_ADVERTISE_ENABLE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_SET_ADVERTISE_ENABLE_REQ_T *primitive;

    primitive = (DM_HCI_ULP_SET_ADVERTISE_ENABLE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->advertising_enable);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_ADVERTISE_ENABLE_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_SET_ADVERTISE_ENABLE_REQ
void *DM_HCI_ULP_SET_ADVERTISE_ENABLE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_SET_ADVERTISE_ENABLE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_SET_ADVERTISE_ENABLE_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_SET_ADVERTISE_ENABLE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->advertising_enable, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_ADVERTISE_ENABLE_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_CLEAR_WHITE_LIST_REQ
CsrSize DM_HCI_ULP_CLEAR_WHITE_LIST_REQ_TSizeof(void *msg)
{
    DM_HCI_ULP_CLEAR_WHITE_LIST_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_CLEAR_WHITE_LIST_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_CLEAR_WHITE_LIST_REQ
CsrUint8 *DM_HCI_ULP_CLEAR_WHITE_LIST_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_CLEAR_WHITE_LIST_REQ_T *primitive;

    primitive = (DM_HCI_ULP_CLEAR_WHITE_LIST_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_CLEAR_WHITE_LIST_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_CLEAR_WHITE_LIST_REQ
void *DM_HCI_ULP_CLEAR_WHITE_LIST_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_CLEAR_WHITE_LIST_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_CLEAR_WHITE_LIST_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_CLEAR_WHITE_LIST_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_CLEAR_WHITE_LIST_REQ */

#ifndef EXCLUDE_DM_HCI_READ_NUM_BCAST_RETXS_CFM
CsrSize DM_HCI_READ_NUM_BCAST_RETXS_CFM_TSizeof(void *msg)
{
    DM_HCI_READ_NUM_BCAST_RETXS_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_NUM_BCAST_RETXS_CFM */

#ifndef EXCLUDE_DM_HCI_READ_NUM_BCAST_RETXS_CFM
CsrUint8 *DM_HCI_READ_NUM_BCAST_RETXS_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_NUM_BCAST_RETXS_CFM_T *primitive;

    primitive = (DM_HCI_READ_NUM_BCAST_RETXS_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->num);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_NUM_BCAST_RETXS_CFM */

#ifndef EXCLUDE_DM_HCI_READ_NUM_BCAST_RETXS_CFM
void *DM_HCI_READ_NUM_BCAST_RETXS_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_NUM_BCAST_RETXS_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_NUM_BCAST_RETXS_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_NUM_BCAST_RETXS_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->num, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_NUM_BCAST_RETXS_CFM */

#ifndef EXCLUDE_DM_HCI_READ_LOCAL_VER_INFO_CFM
CsrSize DM_HCI_READ_LOCAL_VER_INFO_CFM_TSizeof(void *msg)
{
    DM_HCI_READ_LOCAL_VER_INFO_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_LOCAL_VER_INFO_CFM */

#ifndef EXCLUDE_DM_HCI_READ_LOCAL_VER_INFO_CFM
CsrUint8 *DM_HCI_READ_LOCAL_VER_INFO_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_LOCAL_VER_INFO_CFM_T *primitive;

    primitive = (DM_HCI_READ_LOCAL_VER_INFO_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->hci_version);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->hci_revision);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->lmp_version);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->manuf_name);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->lmp_subversion);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_LOCAL_VER_INFO_CFM */

#ifndef EXCLUDE_DM_HCI_READ_LOCAL_VER_INFO_CFM
void *DM_HCI_READ_LOCAL_VER_INFO_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_LOCAL_VER_INFO_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_LOCAL_VER_INFO_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_LOCAL_VER_INFO_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->hci_version, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->hci_revision, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->lmp_version, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->manuf_name, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->lmp_subversion, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_LOCAL_VER_INFO_CFM */

#ifndef EXCLUDE_DM_HCI_QOS_SETUP_REQ
CsrSize DM_HCI_QOS_SETUP_REQ_TSizeof(void *msg)
{
    DM_HCI_QOS_SETUP_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_QOS_SETUP_REQ */

#ifndef EXCLUDE_DM_HCI_QOS_SETUP_REQ
CsrUint8 *DM_HCI_QOS_SETUP_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_QOS_SETUP_REQ_T *primitive;

    primitive = (DM_HCI_QOS_SETUP_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->flags);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->service_type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->token_rate);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->peak_bandwidth);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->latency);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->delay_variation);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_QOS_SETUP_REQ */

#ifndef EXCLUDE_DM_HCI_QOS_SETUP_REQ
void *DM_HCI_QOS_SETUP_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_QOS_SETUP_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_QOS_SETUP_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_QOS_SETUP_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->flags, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->service_type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->token_rate, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->peak_bandwidth, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->latency, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->delay_variation, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_QOS_SETUP_REQ */

#ifndef EXCLUDE_DM_HCI_READ_AFH_CHANNEL_MAP_CFM
CsrSize DM_HCI_READ_AFH_CHANNEL_MAP_CFM_TSizeof(void *msg)
{
    DM_HCI_READ_AFH_CHANNEL_MAP_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_AFH_CHANNEL_MAP_CFM */

#ifndef EXCLUDE_DM_HCI_READ_AFH_CHANNEL_MAP_CFM
CsrUint8 *DM_HCI_READ_AFH_CHANNEL_MAP_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_AFH_CHANNEL_MAP_CFM_T *primitive;

    primitive = (DM_HCI_READ_AFH_CHANNEL_MAP_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mode);
    CsrMemCpySer(buffer, length, ((const void *) primitive->map), ((CsrSize) (10)));

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_AFH_CHANNEL_MAP_CFM */

#ifndef EXCLUDE_DM_HCI_READ_AFH_CHANNEL_MAP_CFM
void *DM_HCI_READ_AFH_CHANNEL_MAP_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_AFH_CHANNEL_MAP_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_AFH_CHANNEL_MAP_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_AFH_CHANNEL_MAP_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mode, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->map), buffer, &offset, ((CsrSize) (10)));

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_AFH_CHANNEL_MAP_CFM */

#ifndef EXCLUDE_DM_SM_CSRK_COUNTER_CHANGE_IND
CsrSize DM_SM_CSRK_COUNTER_CHANGE_IND_TSizeof(void *msg)
{
    DM_SM_CSRK_COUNTER_CHANGE_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_CSRK_COUNTER_CHANGE_IND */

#ifndef EXCLUDE_DM_SM_CSRK_COUNTER_CHANGE_IND
CsrUint8 *DM_SM_CSRK_COUNTER_CHANGE_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_CSRK_COUNTER_CHANGE_IND_T *primitive;

    primitive = (DM_SM_CSRK_COUNTER_CHANGE_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->local_csrk);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->counter);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_CSRK_COUNTER_CHANGE_IND */

#ifndef EXCLUDE_DM_SM_CSRK_COUNTER_CHANGE_IND
void *DM_SM_CSRK_COUNTER_CHANGE_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_CSRK_COUNTER_CHANGE_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_CSRK_COUNTER_CHANGE_IND_T *) CsrPmemZalloc(sizeof(DM_SM_CSRK_COUNTER_CHANGE_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->local_csrk, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->counter, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_CSRK_COUNTER_CHANGE_IND */

#ifndef EXCLUDE_DM_HCI_INQUIRY_CFM
CsrSize DM_HCI_INQUIRY_CFM_TSizeof(void *msg)
{
    DM_HCI_INQUIRY_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_INQUIRY_CFM */

#ifndef EXCLUDE_DM_HCI_INQUIRY_CFM
CsrUint8 *DM_HCI_INQUIRY_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_INQUIRY_CFM_T *primitive;

    primitive = (DM_HCI_INQUIRY_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_INQUIRY_CFM */

#ifndef EXCLUDE_DM_HCI_INQUIRY_CFM
void *DM_HCI_INQUIRY_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_INQUIRY_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_INQUIRY_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_INQUIRY_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_INQUIRY_CFM */

#ifndef EXCLUDE_DM_HCI_READ_AUTH_ENABLE_CFM
CsrSize DM_HCI_READ_AUTH_ENABLE_CFM_TSizeof(void *msg)
{
    DM_HCI_READ_AUTH_ENABLE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_AUTH_ENABLE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_AUTH_ENABLE_CFM
CsrUint8 *DM_HCI_READ_AUTH_ENABLE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_AUTH_ENABLE_CFM_T *primitive;

    primitive = (DM_HCI_READ_AUTH_ENABLE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->auth_enable);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_AUTH_ENABLE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_AUTH_ENABLE_CFM
void *DM_HCI_READ_AUTH_ENABLE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_AUTH_ENABLE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_AUTH_ENABLE_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_AUTH_ENABLE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->auth_enable, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_AUTH_ENABLE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_CLASS_OF_DEVICE_CFM
CsrSize DM_HCI_READ_CLASS_OF_DEVICE_CFM_TSizeof(void *msg)
{
    DM_HCI_READ_CLASS_OF_DEVICE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_CLASS_OF_DEVICE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_CLASS_OF_DEVICE_CFM
CsrUint8 *DM_HCI_READ_CLASS_OF_DEVICE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_CLASS_OF_DEVICE_CFM_T *primitive;

    primitive = (DM_HCI_READ_CLASS_OF_DEVICE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->dev_class);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_CLASS_OF_DEVICE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_CLASS_OF_DEVICE_CFM
void *DM_HCI_READ_CLASS_OF_DEVICE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_CLASS_OF_DEVICE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_CLASS_OF_DEVICE_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_CLASS_OF_DEVICE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->dev_class, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_CLASS_OF_DEVICE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_NUM_SUPPORTED_IAC_REQ
CsrSize DM_HCI_READ_NUM_SUPPORTED_IAC_REQ_TSizeof(void *msg)
{
    DM_HCI_READ_NUM_SUPPORTED_IAC_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_NUM_SUPPORTED_IAC_REQ */

#ifndef EXCLUDE_DM_HCI_READ_NUM_SUPPORTED_IAC_REQ
CsrUint8 *DM_HCI_READ_NUM_SUPPORTED_IAC_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_NUM_SUPPORTED_IAC_REQ_T *primitive;

    primitive = (DM_HCI_READ_NUM_SUPPORTED_IAC_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_NUM_SUPPORTED_IAC_REQ */

#ifndef EXCLUDE_DM_HCI_READ_NUM_SUPPORTED_IAC_REQ
void *DM_HCI_READ_NUM_SUPPORTED_IAC_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_NUM_SUPPORTED_IAC_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_NUM_SUPPORTED_IAC_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_NUM_SUPPORTED_IAC_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_NUM_SUPPORTED_IAC_REQ */

#ifndef EXCLUDE_DM_HCI_HOLD_MODE_REQ
CsrSize DM_HCI_HOLD_MODE_REQ_TSizeof(void *msg)
{
    DM_HCI_HOLD_MODE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_HOLD_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_HOLD_MODE_REQ
CsrUint8 *DM_HCI_HOLD_MODE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_HOLD_MODE_REQ_T *primitive;

    primitive = (DM_HCI_HOLD_MODE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->max_interval);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->min_interval);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_HOLD_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_HOLD_MODE_REQ
void *DM_HCI_HOLD_MODE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_HOLD_MODE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_HOLD_MODE_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_HOLD_MODE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->max_interval, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->min_interval, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_HOLD_MODE_REQ */

#ifndef EXCLUDE_DM_SET_BT_VERSION_CFM
CsrSize DM_SET_BT_VERSION_CFM_TSizeof(void *msg)
{
    DM_SET_BT_VERSION_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SET_BT_VERSION_CFM */

#ifndef EXCLUDE_DM_SET_BT_VERSION_CFM
CsrUint8 *DM_SET_BT_VERSION_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SET_BT_VERSION_CFM_T *primitive;

    primitive = (DM_SET_BT_VERSION_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->version);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_SET_BT_VERSION_CFM */

#ifndef EXCLUDE_DM_SET_BT_VERSION_CFM
void *DM_SET_BT_VERSION_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SET_BT_VERSION_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SET_BT_VERSION_CFM_T *) CsrPmemZalloc(sizeof(DM_SET_BT_VERSION_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->version, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SET_BT_VERSION_CFM */

#ifndef EXCLUDE_DM_SM_SEND_KEYPRESS_NOTIFICATION_REQ
CsrSize DM_SM_SEND_KEYPRESS_NOTIFICATION_REQ_TSizeof(void *msg)
{
    DM_SM_SEND_KEYPRESS_NOTIFICATION_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_SEND_KEYPRESS_NOTIFICATION_REQ */

#ifndef EXCLUDE_DM_SM_SEND_KEYPRESS_NOTIFICATION_REQ
CsrUint8 *DM_SM_SEND_KEYPRESS_NOTIFICATION_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_SEND_KEYPRESS_NOTIFICATION_REQ_T *primitive;

    primitive = (DM_SM_SEND_KEYPRESS_NOTIFICATION_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->unused);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->notification_type);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_SEND_KEYPRESS_NOTIFICATION_REQ */

#ifndef EXCLUDE_DM_SM_SEND_KEYPRESS_NOTIFICATION_REQ
void *DM_SM_SEND_KEYPRESS_NOTIFICATION_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_SEND_KEYPRESS_NOTIFICATION_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_SEND_KEYPRESS_NOTIFICATION_REQ_T *) CsrPmemZalloc(sizeof(DM_SM_SEND_KEYPRESS_NOTIFICATION_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->unused, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->notification_type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_SEND_KEYPRESS_NOTIFICATION_REQ */

#ifndef EXCLUDE_DM_ACL_CLOSE_REQ
CsrSize DM_ACL_CLOSE_REQ_TSizeof(void *msg)
{
    DM_ACL_CLOSE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_ACL_CLOSE_REQ */

#ifndef EXCLUDE_DM_ACL_CLOSE_REQ
CsrUint8 *DM_ACL_CLOSE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_ACL_CLOSE_REQ_T *primitive;

    primitive = (DM_ACL_CLOSE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->reason);

    return buffer;
}
#endif /* EXCLUDE_DM_ACL_CLOSE_REQ */

#ifndef EXCLUDE_DM_ACL_CLOSE_REQ
void *DM_ACL_CLOSE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_ACL_CLOSE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_ACL_CLOSE_REQ_T *) CsrPmemZalloc(sizeof(DM_ACL_CLOSE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->reason, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_ACL_CLOSE_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_CFM
CsrSize DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_CFM_TSizeof(void *msg)
{
    DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_CFM
CsrUint8 *DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_CFM_T *primitive;

    primitive = (DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tx_power);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_CFM
void *DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tx_power, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_CFM */

#ifndef EXCLUDE_DM_SM_KEY_REQUEST_IND
CsrSize DM_SM_KEY_REQUEST_IND_TSizeof(void *msg)
{
    DM_SM_KEY_REQUEST_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_KEY_REQUEST_IND */

#ifndef EXCLUDE_DM_SM_KEY_REQUEST_IND
CsrUint8 *DM_SM_KEY_REQUEST_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_KEY_REQUEST_IND_T *primitive;

    primitive = (DM_SM_KEY_REQUEST_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->security_requirements);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->key_type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->diversifier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->dev_class);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->initiated_outgoing);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->rhsf);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_KEY_REQUEST_IND */

#ifndef EXCLUDE_DM_SM_KEY_REQUEST_IND
void *DM_SM_KEY_REQUEST_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_KEY_REQUEST_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_KEY_REQUEST_IND_T *) CsrPmemZalloc(sizeof(DM_SM_KEY_REQUEST_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->security_requirements, buffer, &offset);
    {
        CsrUint16 data;
        CsrUint16Des((CsrUint16 *) &data, buffer, &offset);
        primitive->key_type = (DM_SM_KEY_TYPE_T) data;
    }
    CsrUint16Des((CsrUint16 *) &primitive->diversifier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->dev_class, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->initiated_outgoing, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->rhsf, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_KEY_REQUEST_IND */

#ifndef EXCLUDE_DM_HCI_WRITE_PAGE_TIMEOUT_REQ
CsrSize DM_HCI_WRITE_PAGE_TIMEOUT_REQ_TSizeof(void *msg)
{
    DM_HCI_WRITE_PAGE_TIMEOUT_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_PAGE_TIMEOUT_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_PAGE_TIMEOUT_REQ
CsrUint8 *DM_HCI_WRITE_PAGE_TIMEOUT_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_PAGE_TIMEOUT_REQ_T *primitive;

    primitive = (DM_HCI_WRITE_PAGE_TIMEOUT_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->page_timeout);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_PAGE_TIMEOUT_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_PAGE_TIMEOUT_REQ
void *DM_HCI_WRITE_PAGE_TIMEOUT_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_PAGE_TIMEOUT_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_PAGE_TIMEOUT_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_PAGE_TIMEOUT_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->page_timeout, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_PAGE_TIMEOUT_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_READ_CHANNEL_MAP_REQ
CsrSize DM_HCI_ULP_READ_CHANNEL_MAP_REQ_TSizeof(void *msg)
{
    DM_HCI_ULP_READ_CHANNEL_MAP_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_READ_CHANNEL_MAP_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_READ_CHANNEL_MAP_REQ
CsrUint8 *DM_HCI_ULP_READ_CHANNEL_MAP_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_READ_CHANNEL_MAP_REQ_T *primitive;

    primitive = (DM_HCI_ULP_READ_CHANNEL_MAP_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->connection_handle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_READ_CHANNEL_MAP_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_READ_CHANNEL_MAP_REQ
void *DM_HCI_ULP_READ_CHANNEL_MAP_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_READ_CHANNEL_MAP_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_READ_CHANNEL_MAP_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_READ_CHANNEL_MAP_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->connection_handle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_READ_CHANNEL_MAP_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_READ_WHITE_LIST_SIZE_REQ
CsrSize DM_HCI_ULP_READ_WHITE_LIST_SIZE_REQ_TSizeof(void *msg)
{
    DM_HCI_ULP_READ_WHITE_LIST_SIZE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_READ_WHITE_LIST_SIZE_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_READ_WHITE_LIST_SIZE_REQ
CsrUint8 *DM_HCI_ULP_READ_WHITE_LIST_SIZE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_READ_WHITE_LIST_SIZE_REQ_T *primitive;

    primitive = (DM_HCI_ULP_READ_WHITE_LIST_SIZE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_READ_WHITE_LIST_SIZE_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_READ_WHITE_LIST_SIZE_REQ
void *DM_HCI_ULP_READ_WHITE_LIST_SIZE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_READ_WHITE_LIST_SIZE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_READ_WHITE_LIST_SIZE_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_READ_WHITE_LIST_SIZE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_READ_WHITE_LIST_SIZE_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_SET_SCAN_ENABLE_REQ
CsrSize DM_HCI_ULP_SET_SCAN_ENABLE_REQ_TSizeof(void *msg)
{
    DM_HCI_ULP_SET_SCAN_ENABLE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_SCAN_ENABLE_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_SET_SCAN_ENABLE_REQ
CsrUint8 *DM_HCI_ULP_SET_SCAN_ENABLE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_SET_SCAN_ENABLE_REQ_T *primitive;

    primitive = (DM_HCI_ULP_SET_SCAN_ENABLE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->scan_enable);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->filter_duplicates);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_SCAN_ENABLE_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_SET_SCAN_ENABLE_REQ
void *DM_HCI_ULP_SET_SCAN_ENABLE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_SET_SCAN_ENABLE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_SET_SCAN_ENABLE_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_SET_SCAN_ENABLE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->scan_enable, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->filter_duplicates, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_SCAN_ENABLE_REQ */

#ifndef EXCLUDE_DM_HCI_READ_REMOTE_VER_INFO_REQ
CsrSize DM_HCI_READ_REMOTE_VER_INFO_REQ_TSizeof(void *msg)
{
    DM_HCI_READ_REMOTE_VER_INFO_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_REMOTE_VER_INFO_REQ */

#ifndef EXCLUDE_DM_HCI_READ_REMOTE_VER_INFO_REQ
CsrUint8 *DM_HCI_READ_REMOTE_VER_INFO_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_REMOTE_VER_INFO_REQ_T *primitive;

    primitive = (DM_HCI_READ_REMOTE_VER_INFO_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_REMOTE_VER_INFO_REQ */

#ifndef EXCLUDE_DM_HCI_READ_REMOTE_VER_INFO_REQ
void *DM_HCI_READ_REMOTE_VER_INFO_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_REMOTE_VER_INFO_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_REMOTE_VER_INFO_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_REMOTE_VER_INFO_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_REMOTE_VER_INFO_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_SET_SCAN_PARAMETERS_REQ
CsrSize DM_HCI_ULP_SET_SCAN_PARAMETERS_REQ_TSizeof(void *msg)
{
    DM_HCI_ULP_SET_SCAN_PARAMETERS_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_SCAN_PARAMETERS_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_SET_SCAN_PARAMETERS_REQ
CsrUint8 *DM_HCI_ULP_SET_SCAN_PARAMETERS_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_SET_SCAN_PARAMETERS_REQ_T *primitive;

    primitive = (DM_HCI_ULP_SET_SCAN_PARAMETERS_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->scan_type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->scan_interval);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->scan_window);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->own_address_type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->scanning_filter_policy);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_SCAN_PARAMETERS_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_SET_SCAN_PARAMETERS_REQ
void *DM_HCI_ULP_SET_SCAN_PARAMETERS_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_SET_SCAN_PARAMETERS_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_SET_SCAN_PARAMETERS_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_SET_SCAN_PARAMETERS_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->scan_type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->scan_interval, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->scan_window, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->own_address_type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->scanning_filter_policy, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_SCAN_PARAMETERS_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_SET_RANDOM_ADDRESS_CFM
CsrSize DM_HCI_ULP_SET_RANDOM_ADDRESS_CFM_TSizeof(void *msg)
{
    DM_HCI_ULP_SET_RANDOM_ADDRESS_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_RANDOM_ADDRESS_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_SET_RANDOM_ADDRESS_CFM
CsrUint8 *DM_HCI_ULP_SET_RANDOM_ADDRESS_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_SET_RANDOM_ADDRESS_CFM_T *primitive;

    primitive = (DM_HCI_ULP_SET_RANDOM_ADDRESS_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_RANDOM_ADDRESS_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_SET_RANDOM_ADDRESS_CFM
void *DM_HCI_ULP_SET_RANDOM_ADDRESS_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_SET_RANDOM_ADDRESS_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_SET_RANDOM_ADDRESS_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_SET_RANDOM_ADDRESS_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_RANDOM_ADDRESS_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_PAGE_SCAN_TYPE_CFM
CsrSize DM_HCI_WRITE_PAGE_SCAN_TYPE_CFM_TSizeof(void *msg)
{
    DM_HCI_WRITE_PAGE_SCAN_TYPE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_PAGE_SCAN_TYPE_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_PAGE_SCAN_TYPE_CFM
CsrUint8 *DM_HCI_WRITE_PAGE_SCAN_TYPE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_PAGE_SCAN_TYPE_CFM_T *primitive;

    primitive = (DM_HCI_WRITE_PAGE_SCAN_TYPE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_PAGE_SCAN_TYPE_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_PAGE_SCAN_TYPE_CFM
void *DM_HCI_WRITE_PAGE_SCAN_TYPE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_PAGE_SCAN_TYPE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_PAGE_SCAN_TYPE_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_PAGE_SCAN_TYPE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_PAGE_SCAN_TYPE_CFM */

#ifndef EXCLUDE_DM_WRITE_CACHED_CLOCK_OFFSET_CFM
CsrSize DM_WRITE_CACHED_CLOCK_OFFSET_CFM_TSizeof(void *msg)
{
    DM_WRITE_CACHED_CLOCK_OFFSET_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_WRITE_CACHED_CLOCK_OFFSET_CFM */

#ifndef EXCLUDE_DM_WRITE_CACHED_CLOCK_OFFSET_CFM
CsrUint8 *DM_WRITE_CACHED_CLOCK_OFFSET_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_WRITE_CACHED_CLOCK_OFFSET_CFM_T *primitive;

    primitive = (DM_WRITE_CACHED_CLOCK_OFFSET_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_WRITE_CACHED_CLOCK_OFFSET_CFM */

#ifndef EXCLUDE_DM_WRITE_CACHED_CLOCK_OFFSET_CFM
void *DM_WRITE_CACHED_CLOCK_OFFSET_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_WRITE_CACHED_CLOCK_OFFSET_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_WRITE_CACHED_CLOCK_OFFSET_CFM_T *) CsrPmemZalloc(sizeof(DM_WRITE_CACHED_CLOCK_OFFSET_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_WRITE_CACHED_CLOCK_OFFSET_CFM */

#ifndef EXCLUDE_DM_SM_REMOVE_DEVICE_CFM
CsrSize DM_SM_REMOVE_DEVICE_CFM_TSizeof(void *msg)
{
    DM_SM_REMOVE_DEVICE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_REMOVE_DEVICE_CFM */

#ifndef EXCLUDE_DM_SM_REMOVE_DEVICE_CFM
CsrUint8 *DM_SM_REMOVE_DEVICE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_REMOVE_DEVICE_CFM_T *primitive;

    primitive = (DM_SM_REMOVE_DEVICE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_REMOVE_DEVICE_CFM */

#ifndef EXCLUDE_DM_SM_REMOVE_DEVICE_CFM
void *DM_SM_REMOVE_DEVICE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_REMOVE_DEVICE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_REMOVE_DEVICE_CFM_T *) CsrPmemZalloc(sizeof(DM_SM_REMOVE_DEVICE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_REMOVE_DEVICE_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_SET_ADVERTISE_ENABLE_CFM
CsrSize DM_HCI_ULP_SET_ADVERTISE_ENABLE_CFM_TSizeof(void *msg)
{
    DM_HCI_ULP_SET_ADVERTISE_ENABLE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_ADVERTISE_ENABLE_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_SET_ADVERTISE_ENABLE_CFM
CsrUint8 *DM_HCI_ULP_SET_ADVERTISE_ENABLE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_SET_ADVERTISE_ENABLE_CFM_T *primitive;

    primitive = (DM_HCI_ULP_SET_ADVERTISE_ENABLE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_ADVERTISE_ENABLE_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_SET_ADVERTISE_ENABLE_CFM
void *DM_HCI_ULP_SET_ADVERTISE_ENABLE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_SET_ADVERTISE_ENABLE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_SET_ADVERTISE_ENABLE_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_SET_ADVERTISE_ENABLE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_ADVERTISE_ENABLE_CFM */

#ifndef EXCLUDE_DM_HCI_CHANGE_LOCAL_NAME_CFM
CsrSize DM_HCI_CHANGE_LOCAL_NAME_CFM_TSizeof(void *msg)
{
    DM_HCI_CHANGE_LOCAL_NAME_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_CHANGE_LOCAL_NAME_CFM */

#ifndef EXCLUDE_DM_HCI_CHANGE_LOCAL_NAME_CFM
CsrUint8 *DM_HCI_CHANGE_LOCAL_NAME_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_CHANGE_LOCAL_NAME_CFM_T *primitive;

    primitive = (DM_HCI_CHANGE_LOCAL_NAME_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_CHANGE_LOCAL_NAME_CFM */

#ifndef EXCLUDE_DM_HCI_CHANGE_LOCAL_NAME_CFM
void *DM_HCI_CHANGE_LOCAL_NAME_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_CHANGE_LOCAL_NAME_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_CHANGE_LOCAL_NAME_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_CHANGE_LOCAL_NAME_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_CHANGE_LOCAL_NAME_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_REQ
CsrSize DM_HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_REQ_TSizeof(void *msg)
{
    DM_HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_REQ
CsrUint8 *DM_HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_REQ_T *primitive;

    primitive = (DM_HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_REQ
void *DM_HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_REQ */

#ifndef EXCLUDE_DM_HCI_REMOTE_NAME_REQ_CANCEL_CFM
CsrSize DM_HCI_REMOTE_NAME_REQ_CANCEL_CFM_TSizeof(void *msg)
{
    DM_HCI_REMOTE_NAME_REQ_CANCEL_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_REMOTE_NAME_REQ_CANCEL_CFM */

#ifndef EXCLUDE_DM_HCI_REMOTE_NAME_REQ_CANCEL_CFM
CsrUint8 *DM_HCI_REMOTE_NAME_REQ_CANCEL_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_REMOTE_NAME_REQ_CANCEL_CFM_T *primitive;

    primitive = (DM_HCI_REMOTE_NAME_REQ_CANCEL_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_REMOTE_NAME_REQ_CANCEL_CFM */

#ifndef EXCLUDE_DM_HCI_REMOTE_NAME_REQ_CANCEL_CFM
void *DM_HCI_REMOTE_NAME_REQ_CANCEL_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_REMOTE_NAME_REQ_CANCEL_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_REMOTE_NAME_REQ_CANCEL_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_REMOTE_NAME_REQ_CANCEL_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_REMOTE_NAME_REQ_CANCEL_CFM */

#ifndef EXCLUDE_DM_HCI_INQUIRY_CANCEL_CFM
CsrSize DM_HCI_INQUIRY_CANCEL_CFM_TSizeof(void *msg)
{
    DM_HCI_INQUIRY_CANCEL_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_INQUIRY_CANCEL_CFM */

#ifndef EXCLUDE_DM_HCI_INQUIRY_CANCEL_CFM
CsrUint8 *DM_HCI_INQUIRY_CANCEL_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_INQUIRY_CANCEL_CFM_T *primitive;

    primitive = (DM_HCI_INQUIRY_CANCEL_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_INQUIRY_CANCEL_CFM */

#ifndef EXCLUDE_DM_HCI_INQUIRY_CANCEL_CFM
void *DM_HCI_INQUIRY_CANCEL_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_INQUIRY_CANCEL_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_INQUIRY_CANCEL_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_INQUIRY_CANCEL_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_INQUIRY_CANCEL_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_READ_SUPPORTED_STATES_CFM
CsrSize DM_HCI_ULP_READ_SUPPORTED_STATES_CFM_TSizeof(void *msg)
{
    DM_HCI_ULP_READ_SUPPORTED_STATES_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_READ_SUPPORTED_STATES_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_READ_SUPPORTED_STATES_CFM
CsrUint8 *DM_HCI_ULP_READ_SUPPORTED_STATES_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_READ_SUPPORTED_STATES_CFM_T *primitive;

    primitive = (DM_HCI_ULP_READ_SUPPORTED_STATES_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrMemCpySer(buffer, length, ((const void *) primitive->supported_states), ((CsrSize) (8)));

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_READ_SUPPORTED_STATES_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_READ_SUPPORTED_STATES_CFM
void *DM_HCI_ULP_READ_SUPPORTED_STATES_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_READ_SUPPORTED_STATES_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_READ_SUPPORTED_STATES_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_READ_SUPPORTED_STATES_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->supported_states), buffer, &offset, ((CsrSize) (8)));

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_READ_SUPPORTED_STATES_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_READ_CHANNEL_MAP_CFM
CsrSize DM_HCI_ULP_READ_CHANNEL_MAP_CFM_TSizeof(void *msg)
{
    DM_HCI_ULP_READ_CHANNEL_MAP_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_READ_CHANNEL_MAP_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_READ_CHANNEL_MAP_CFM
CsrUint8 *DM_HCI_ULP_READ_CHANNEL_MAP_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_READ_CHANNEL_MAP_CFM_T *primitive;

    primitive = (DM_HCI_ULP_READ_CHANNEL_MAP_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrMemCpySer(buffer, length, ((const void *) primitive->ulp_channel_map), ((CsrSize) (5)));

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_READ_CHANNEL_MAP_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_READ_CHANNEL_MAP_CFM
void *DM_HCI_ULP_READ_CHANNEL_MAP_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_READ_CHANNEL_MAP_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_READ_CHANNEL_MAP_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_READ_CHANNEL_MAP_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->ulp_channel_map), buffer, &offset, ((CsrSize) (5)));

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_READ_CHANNEL_MAP_CFM */

#ifndef EXCLUDE_DM_LP_WRITE_ALWAYS_MASTER_DEVICES_CFM
CsrSize DM_LP_WRITE_ALWAYS_MASTER_DEVICES_CFM_TSizeof(void *msg)
{
    DM_LP_WRITE_ALWAYS_MASTER_DEVICES_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_LP_WRITE_ALWAYS_MASTER_DEVICES_CFM */

#ifndef EXCLUDE_DM_LP_WRITE_ALWAYS_MASTER_DEVICES_CFM
CsrUint8 *DM_LP_WRITE_ALWAYS_MASTER_DEVICES_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_LP_WRITE_ALWAYS_MASTER_DEVICES_CFM_T *primitive;

    primitive = (DM_LP_WRITE_ALWAYS_MASTER_DEVICES_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_LP_WRITE_ALWAYS_MASTER_DEVICES_CFM */

#ifndef EXCLUDE_DM_LP_WRITE_ALWAYS_MASTER_DEVICES_CFM
void *DM_LP_WRITE_ALWAYS_MASTER_DEVICES_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_LP_WRITE_ALWAYS_MASTER_DEVICES_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_LP_WRITE_ALWAYS_MASTER_DEVICES_CFM_T *) CsrPmemZalloc(sizeof(DM_LP_WRITE_ALWAYS_MASTER_DEVICES_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_LP_WRITE_ALWAYS_MASTER_DEVICES_CFM */

#ifndef EXCLUDE_DM_HCI_READ_TX_POWER_LEVEL_REQ
CsrSize DM_HCI_READ_TX_POWER_LEVEL_REQ_TSizeof(void *msg)
{
    DM_HCI_READ_TX_POWER_LEVEL_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_TX_POWER_LEVEL_REQ */

#ifndef EXCLUDE_DM_HCI_READ_TX_POWER_LEVEL_REQ
CsrUint8 *DM_HCI_READ_TX_POWER_LEVEL_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_TX_POWER_LEVEL_REQ_T *primitive;

    primitive = (DM_HCI_READ_TX_POWER_LEVEL_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_TX_POWER_LEVEL_REQ */

#ifndef EXCLUDE_DM_HCI_READ_TX_POWER_LEVEL_REQ
void *DM_HCI_READ_TX_POWER_LEVEL_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_TX_POWER_LEVEL_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_TX_POWER_LEVEL_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_TX_POWER_LEVEL_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_TX_POWER_LEVEL_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_RAND_REQ
CsrSize DM_HCI_ULP_RAND_REQ_TSizeof(void *msg)
{
    DM_HCI_ULP_RAND_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_RAND_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_RAND_REQ
CsrUint8 *DM_HCI_ULP_RAND_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_RAND_REQ_T *primitive;

    primitive = (DM_HCI_ULP_RAND_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_RAND_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_RAND_REQ
void *DM_HCI_ULP_RAND_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_RAND_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_RAND_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_RAND_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_RAND_REQ */

#ifndef EXCLUDE_DM_HCI_READ_AFH_CHANNEL_MAP_REQ
CsrSize DM_HCI_READ_AFH_CHANNEL_MAP_REQ_TSizeof(void *msg)
{
    DM_HCI_READ_AFH_CHANNEL_MAP_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_AFH_CHANNEL_MAP_REQ */

#ifndef EXCLUDE_DM_HCI_READ_AFH_CHANNEL_MAP_REQ
CsrUint8 *DM_HCI_READ_AFH_CHANNEL_MAP_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_AFH_CHANNEL_MAP_REQ_T *primitive;

    primitive = (DM_HCI_READ_AFH_CHANNEL_MAP_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_AFH_CHANNEL_MAP_REQ */

#ifndef EXCLUDE_DM_HCI_READ_AFH_CHANNEL_MAP_REQ
void *DM_HCI_READ_AFH_CHANNEL_MAP_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_AFH_CHANNEL_MAP_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_AFH_CHANNEL_MAP_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_AFH_CHANNEL_MAP_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_AFH_CHANNEL_MAP_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_SCAN_ENABLE_CFM
CsrSize DM_HCI_WRITE_SCAN_ENABLE_CFM_TSizeof(void *msg)
{
    DM_HCI_WRITE_SCAN_ENABLE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_SCAN_ENABLE_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_SCAN_ENABLE_CFM
CsrUint8 *DM_HCI_WRITE_SCAN_ENABLE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_SCAN_ENABLE_CFM_T *primitive;

    primitive = (DM_HCI_WRITE_SCAN_ENABLE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_SCAN_ENABLE_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_SCAN_ENABLE_CFM
void *DM_HCI_WRITE_SCAN_ENABLE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_SCAN_ENABLE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_SCAN_ENABLE_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_SCAN_ENABLE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_SCAN_ENABLE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_LINK_POLICY_SETTINGS_REQ
CsrSize DM_HCI_READ_LINK_POLICY_SETTINGS_REQ_TSizeof(void *msg)
{
    DM_HCI_READ_LINK_POLICY_SETTINGS_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_LINK_POLICY_SETTINGS_REQ */

#ifndef EXCLUDE_DM_HCI_READ_LINK_POLICY_SETTINGS_REQ
CsrUint8 *DM_HCI_READ_LINK_POLICY_SETTINGS_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_LINK_POLICY_SETTINGS_REQ_T *primitive;

    primitive = (DM_HCI_READ_LINK_POLICY_SETTINGS_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_LINK_POLICY_SETTINGS_REQ */

#ifndef EXCLUDE_DM_HCI_READ_LINK_POLICY_SETTINGS_REQ
void *DM_HCI_READ_LINK_POLICY_SETTINGS_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_LINK_POLICY_SETTINGS_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_LINK_POLICY_SETTINGS_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_LINK_POLICY_SETTINGS_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_LINK_POLICY_SETTINGS_REQ */

#ifndef EXCLUDE_DM_HCI_PERIODIC_INQUIRY_MODE_REQ
CsrSize DM_HCI_PERIODIC_INQUIRY_MODE_REQ_TSizeof(void *msg)
{
    DM_HCI_PERIODIC_INQUIRY_MODE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_PERIODIC_INQUIRY_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_PERIODIC_INQUIRY_MODE_REQ
CsrUint8 *DM_HCI_PERIODIC_INQUIRY_MODE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_PERIODIC_INQUIRY_MODE_REQ_T *primitive;

    primitive = (DM_HCI_PERIODIC_INQUIRY_MODE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->max_period_length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->min_period_length);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->inquiry_length);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->num_responses);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_PERIODIC_INQUIRY_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_PERIODIC_INQUIRY_MODE_REQ
void *DM_HCI_PERIODIC_INQUIRY_MODE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_PERIODIC_INQUIRY_MODE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_PERIODIC_INQUIRY_MODE_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_PERIODIC_INQUIRY_MODE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->max_period_length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->min_period_length, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->inquiry_length, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->num_responses, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_PERIODIC_INQUIRY_MODE_REQ */

#ifndef EXCLUDE_DM_SYNC_CONNECT_CFM
CsrSize DM_SYNC_CONNECT_CFM_TSizeof(void *msg)
{
    DM_SYNC_CONNECT_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SYNC_CONNECT_CFM */

#ifndef EXCLUDE_DM_SYNC_CONNECT_CFM
CsrUint8 *DM_SYNC_CONNECT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SYNC_CONNECT_CFM_T *primitive;

    primitive = (DM_SYNC_CONNECT_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pv_cbarg);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->link_type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tx_interval);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->wesco);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->rx_packet_length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->tx_packet_length);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->air_mode);

    return buffer;
}
#endif /* EXCLUDE_DM_SYNC_CONNECT_CFM */

#ifndef EXCLUDE_DM_SYNC_CONNECT_CFM
void *DM_SYNC_CONNECT_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SYNC_CONNECT_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SYNC_CONNECT_CFM_T *) CsrPmemZalloc(sizeof(DM_SYNC_CONNECT_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pv_cbarg, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->link_type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tx_interval, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->wesco, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->rx_packet_length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->tx_packet_length, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->air_mode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SYNC_CONNECT_CFM */

#ifndef EXCLUDE_DM_SM_INIT_CFM
CsrSize DM_SM_INIT_CFM_TSizeof(void *msg)
{
    DM_SM_INIT_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_INIT_CFM */

#ifndef EXCLUDE_DM_SM_INIT_CFM
CsrUint8 *DM_SM_INIT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_INIT_CFM_T *primitive;

    primitive = (DM_SM_INIT_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->security_mode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->security_level_default);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->write_auth_enable);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mode3_enc);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->status);
    CsrMemCpySer(buffer, length, ((const void *) primitive->sm_key_state.er), ((CsrSize) 2 * (8)));
    CsrMemCpySer(buffer, length, ((const void *) primitive->sm_key_state.ir), ((CsrSize) 2 * (8)));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->sm_div_state);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->sm_sign_counter);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_INIT_CFM */

#ifndef EXCLUDE_DM_SM_INIT_CFM
void *DM_SM_INIT_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_INIT_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_INIT_CFM_T *) CsrPmemZalloc(sizeof(DM_SM_INIT_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->security_mode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->security_level_default, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->write_auth_enable, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mode3_enc, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->status, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->sm_key_state.er), buffer, &offset, ((CsrSize) 2 * (8)));
    CsrMemCpyDes(((void *) primitive->sm_key_state.ir), buffer, &offset, ((CsrSize) 2 * (8)));
    CsrUint16Des((CsrUint16 *) &primitive->sm_div_state, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->sm_sign_counter, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_INIT_CFM */

#ifndef EXCLUDE_DM_SM_PIN_REQUEST_IND
CsrSize DM_SM_PIN_REQUEST_IND_TSizeof(void *msg)
{
    DM_SM_PIN_REQUEST_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_PIN_REQUEST_IND */

#ifndef EXCLUDE_DM_SM_PIN_REQUEST_IND
CsrUint8 *DM_SM_PIN_REQUEST_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_PIN_REQUEST_IND_T *primitive;

    primitive = (DM_SM_PIN_REQUEST_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->dev_class);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->initiator);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_PIN_REQUEST_IND */

#ifndef EXCLUDE_DM_SM_PIN_REQUEST_IND
void *DM_SM_PIN_REQUEST_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_PIN_REQUEST_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_PIN_REQUEST_IND_T *) CsrPmemZalloc(sizeof(DM_SM_PIN_REQUEST_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->dev_class, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->initiator, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_PIN_REQUEST_IND */

#ifndef EXCLUDE_DM_HCI_READ_RSSI_REQ
CsrSize DM_HCI_READ_RSSI_REQ_TSizeof(void *msg)
{
    DM_HCI_READ_RSSI_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_RSSI_REQ */

#ifndef EXCLUDE_DM_HCI_READ_RSSI_REQ
CsrUint8 *DM_HCI_READ_RSSI_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_RSSI_REQ_T *primitive;

    primitive = (DM_HCI_READ_RSSI_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_RSSI_REQ */

#ifndef EXCLUDE_DM_HCI_READ_RSSI_REQ
void *DM_HCI_READ_RSSI_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_RSSI_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_RSSI_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_RSSI_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_RSSI_REQ */

#ifndef EXCLUDE_DM_AM_REGISTER_WRONG_API_CFM
CsrSize DM_AM_REGISTER_WRONG_API_CFM_TSizeof(void *msg)
{
    DM_AM_REGISTER_WRONG_API_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_AM_REGISTER_WRONG_API_CFM */

#ifndef EXCLUDE_DM_AM_REGISTER_WRONG_API_CFM
CsrUint8 *DM_AM_REGISTER_WRONG_API_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_AM_REGISTER_WRONG_API_CFM_T *primitive;

    primitive = (DM_AM_REGISTER_WRONG_API_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);

    return buffer;
}
#endif /* EXCLUDE_DM_AM_REGISTER_WRONG_API_CFM */

#ifndef EXCLUDE_DM_AM_REGISTER_WRONG_API_CFM
void *DM_AM_REGISTER_WRONG_API_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_AM_REGISTER_WRONG_API_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_AM_REGISTER_WRONG_API_CFM_T *) CsrPmemZalloc(sizeof(DM_AM_REGISTER_WRONG_API_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_AM_REGISTER_WRONG_API_CFM */

#ifndef EXCLUDE_DM_ACL_CONN_HANDLE_IND
CsrSize DM_ACL_CONN_HANDLE_IND_TSizeof(void *msg)
{
    DM_ACL_CONN_HANDLE_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_ACL_CONN_HANDLE_IND */

#ifndef EXCLUDE_DM_ACL_CONN_HANDLE_IND
CsrUint8 *DM_ACL_CONN_HANDLE_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_ACL_CONN_HANDLE_IND_T *primitive;

    primitive = (DM_ACL_CONN_HANDLE_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->dev_class);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ble_conn_params.conn_interval);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ble_conn_params.conn_latency);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ble_conn_params.supervision_timeout);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->ble_conn_params.clock_accuracy);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->acl_conn_handle);

    return buffer;
}
#endif /* EXCLUDE_DM_ACL_CONN_HANDLE_IND */

#ifndef EXCLUDE_DM_ACL_CONN_HANDLE_IND
void *DM_ACL_CONN_HANDLE_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_ACL_CONN_HANDLE_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_ACL_CONN_HANDLE_IND_T *) CsrPmemZalloc(sizeof(DM_ACL_CONN_HANDLE_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->dev_class, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->ble_conn_params.conn_interval, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->ble_conn_params.conn_latency, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->ble_conn_params.supervision_timeout, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->ble_conn_params.clock_accuracy, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->acl_conn_handle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_ACL_CONN_HANDLE_IND */

#ifndef EXCLUDE_DM_SYNC_DISCONNECT_REQ
CsrSize DM_SYNC_DISCONNECT_REQ_TSizeof(void *msg)
{
    DM_SYNC_DISCONNECT_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SYNC_DISCONNECT_REQ */

#ifndef EXCLUDE_DM_SYNC_DISCONNECT_REQ
CsrUint8 *DM_SYNC_DISCONNECT_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SYNC_DISCONNECT_REQ_T *primitive;

    primitive = (DM_SYNC_DISCONNECT_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->reason);

    return buffer;
}
#endif /* EXCLUDE_DM_SYNC_DISCONNECT_REQ */

#ifndef EXCLUDE_DM_SYNC_DISCONNECT_REQ
void *DM_SYNC_DISCONNECT_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SYNC_DISCONNECT_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SYNC_DISCONNECT_REQ_T *) CsrPmemZalloc(sizeof(DM_SYNC_DISCONNECT_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->reason, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SYNC_DISCONNECT_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_LINK_POLICY_SETTINGS_REQ
CsrSize DM_HCI_WRITE_LINK_POLICY_SETTINGS_REQ_TSizeof(void *msg)
{
    DM_HCI_WRITE_LINK_POLICY_SETTINGS_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_LINK_POLICY_SETTINGS_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_LINK_POLICY_SETTINGS_REQ
CsrUint8 *DM_HCI_WRITE_LINK_POLICY_SETTINGS_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_LINK_POLICY_SETTINGS_REQ_T *primitive;

    primitive = (DM_HCI_WRITE_LINK_POLICY_SETTINGS_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->link_policy_settings);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_LINK_POLICY_SETTINGS_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_LINK_POLICY_SETTINGS_REQ
void *DM_HCI_WRITE_LINK_POLICY_SETTINGS_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_LINK_POLICY_SETTINGS_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_LINK_POLICY_SETTINGS_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_LINK_POLICY_SETTINGS_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->link_policy_settings, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_LINK_POLICY_SETTINGS_REQ */

#ifndef EXCLUDE_DM_SYNC_REGISTER_REQ
CsrSize DM_SYNC_REGISTER_REQ_TSizeof(void *msg)
{
    DM_SYNC_REGISTER_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SYNC_REGISTER_REQ */

#ifndef EXCLUDE_DM_SYNC_REGISTER_REQ
CsrUint8 *DM_SYNC_REGISTER_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SYNC_REGISTER_REQ_T *primitive;

    primitive = (DM_SYNC_REGISTER_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pv_cbarg);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->streams.buffer_size_sink);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->streams.buffer_size_source);

    return buffer;
}
#endif /* EXCLUDE_DM_SYNC_REGISTER_REQ */

#ifndef EXCLUDE_DM_SYNC_REGISTER_REQ
void *DM_SYNC_REGISTER_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SYNC_REGISTER_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SYNC_REGISTER_REQ_T *) CsrPmemZalloc(sizeof(DM_SYNC_REGISTER_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pv_cbarg, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->streams.buffer_size_sink, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->streams.buffer_size_source, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SYNC_REGISTER_REQ */

#ifndef EXCLUDE_DM_HCI_READ_PAGESCAN_PERIOD_MODE_CFM
CsrSize DM_HCI_READ_PAGESCAN_PERIOD_MODE_CFM_TSizeof(void *msg)
{
    DM_HCI_READ_PAGESCAN_PERIOD_MODE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_PAGESCAN_PERIOD_MODE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_PAGESCAN_PERIOD_MODE_CFM
CsrUint8 *DM_HCI_READ_PAGESCAN_PERIOD_MODE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_PAGESCAN_PERIOD_MODE_CFM_T *primitive;

    primitive = (DM_HCI_READ_PAGESCAN_PERIOD_MODE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mode);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_PAGESCAN_PERIOD_MODE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_PAGESCAN_PERIOD_MODE_CFM
void *DM_HCI_READ_PAGESCAN_PERIOD_MODE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_PAGESCAN_PERIOD_MODE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_PAGESCAN_PERIOD_MODE_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_PAGESCAN_PERIOD_MODE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_PAGESCAN_PERIOD_MODE_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_CREATE_CONNECTION_CANCEL_REQ
CsrSize DM_HCI_ULP_CREATE_CONNECTION_CANCEL_REQ_TSizeof(void *msg)
{
    DM_HCI_ULP_CREATE_CONNECTION_CANCEL_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_CREATE_CONNECTION_CANCEL_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_CREATE_CONNECTION_CANCEL_REQ
CsrUint8 *DM_HCI_ULP_CREATE_CONNECTION_CANCEL_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_CREATE_CONNECTION_CANCEL_REQ_T *primitive;

    primitive = (DM_HCI_ULP_CREATE_CONNECTION_CANCEL_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_CREATE_CONNECTION_CANCEL_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_CREATE_CONNECTION_CANCEL_REQ
void *DM_HCI_ULP_CREATE_CONNECTION_CANCEL_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_CREATE_CONNECTION_CANCEL_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_CREATE_CONNECTION_CANCEL_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_CREATE_CONNECTION_CANCEL_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_CREATE_CONNECTION_CANCEL_REQ */

#ifndef EXCLUDE_DM_HCI_READ_AFH_CHANNEL_CLASS_M_REQ
CsrSize DM_HCI_READ_AFH_CHANNEL_CLASS_M_REQ_TSizeof(void *msg)
{
    DM_HCI_READ_AFH_CHANNEL_CLASS_M_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_AFH_CHANNEL_CLASS_M_REQ */

#ifndef EXCLUDE_DM_HCI_READ_AFH_CHANNEL_CLASS_M_REQ
CsrUint8 *DM_HCI_READ_AFH_CHANNEL_CLASS_M_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_AFH_CHANNEL_CLASS_M_REQ_T *primitive;

    primitive = (DM_HCI_READ_AFH_CHANNEL_CLASS_M_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_AFH_CHANNEL_CLASS_M_REQ */

#ifndef EXCLUDE_DM_HCI_READ_AFH_CHANNEL_CLASS_M_REQ
void *DM_HCI_READ_AFH_CHANNEL_CLASS_M_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_AFH_CHANNEL_CLASS_M_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_AFH_CHANNEL_CLASS_M_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_AFH_CHANNEL_CLASS_M_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_AFH_CHANNEL_CLASS_M_REQ */

#ifndef EXCLUDE_DM_HCI_SNIFF_SUB_RATE_REQ
CsrSize DM_HCI_SNIFF_SUB_RATE_REQ_TSizeof(void *msg)
{
    DM_HCI_SNIFF_SUB_RATE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_SNIFF_SUB_RATE_REQ */

#ifndef EXCLUDE_DM_HCI_SNIFF_SUB_RATE_REQ
CsrUint8 *DM_HCI_SNIFF_SUB_RATE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_SNIFF_SUB_RATE_REQ_T *primitive;

    primitive = (DM_HCI_SNIFF_SUB_RATE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->max_remote_latency);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->min_remote_timeout);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->min_local_timeout);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_SNIFF_SUB_RATE_REQ */

#ifndef EXCLUDE_DM_HCI_SNIFF_SUB_RATE_REQ
void *DM_HCI_SNIFF_SUB_RATE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_SNIFF_SUB_RATE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_SNIFF_SUB_RATE_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_SNIFF_SUB_RATE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->max_remote_latency, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->min_remote_timeout, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->min_local_timeout, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_SNIFF_SUB_RATE_REQ */

#ifndef EXCLUDE_DM_SM_AUTHENTICATE_CFM
CsrSize DM_SM_AUTHENTICATE_CFM_TSizeof(void *msg)
{
    DM_SM_AUTHENTICATE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_AUTHENTICATE_CFM */

#ifndef EXCLUDE_DM_SM_AUTHENTICATE_CFM
CsrUint8 *DM_SM_AUTHENTICATE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_AUTHENTICATE_CFM_T *primitive;

    primitive = (DM_SM_AUTHENTICATE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->success);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_AUTHENTICATE_CFM */

#ifndef EXCLUDE_DM_SM_AUTHENTICATE_CFM
void *DM_SM_AUTHENTICATE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_AUTHENTICATE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_AUTHENTICATE_CFM_T *) CsrPmemZalloc(sizeof(DM_SM_AUTHENTICATE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->success, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_AUTHENTICATE_CFM */

#ifndef EXCLUDE_DM_HCI_FLUSH_CFM
CsrSize DM_HCI_FLUSH_CFM_TSizeof(void *msg)
{
    DM_HCI_FLUSH_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_FLUSH_CFM */

#ifndef EXCLUDE_DM_HCI_FLUSH_CFM
CsrUint8 *DM_HCI_FLUSH_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_FLUSH_CFM_T *primitive;

    primitive = (DM_HCI_FLUSH_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_FLUSH_CFM */

#ifndef EXCLUDE_DM_HCI_FLUSH_CFM
void *DM_HCI_FLUSH_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_FLUSH_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_FLUSH_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_FLUSH_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_FLUSH_CFM */

#ifndef EXCLUDE_DM_AMPM_READ_BD_ADDR_CFM
CsrSize DM_AMPM_READ_BD_ADDR_CFM_TSizeof(void *msg)
{
    DM_AMPM_READ_BD_ADDR_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_AMPM_READ_BD_ADDR_CFM */

#ifndef EXCLUDE_DM_AMPM_READ_BD_ADDR_CFM
CsrUint8 *DM_AMPM_READ_BD_ADDR_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_AMPM_READ_BD_ADDR_CFM_T *primitive;

    primitive = (DM_AMPM_READ_BD_ADDR_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_AMPM_READ_BD_ADDR_CFM */

#ifndef EXCLUDE_DM_AMPM_READ_BD_ADDR_CFM
void *DM_AMPM_READ_BD_ADDR_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_AMPM_READ_BD_ADDR_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_AMPM_READ_BD_ADDR_CFM_T *) CsrPmemZalloc(sizeof(DM_AMPM_READ_BD_ADDR_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_AMPM_READ_BD_ADDR_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_SET_SCAN_ENABLE_CFM
CsrSize DM_HCI_ULP_SET_SCAN_ENABLE_CFM_TSizeof(void *msg)
{
    DM_HCI_ULP_SET_SCAN_ENABLE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_SCAN_ENABLE_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_SET_SCAN_ENABLE_CFM
CsrUint8 *DM_HCI_ULP_SET_SCAN_ENABLE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_SET_SCAN_ENABLE_CFM_T *primitive;

    primitive = (DM_HCI_ULP_SET_SCAN_ENABLE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_SCAN_ENABLE_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_SET_SCAN_ENABLE_CFM
void *DM_HCI_ULP_SET_SCAN_ENABLE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_SET_SCAN_ENABLE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_SET_SCAN_ENABLE_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_SET_SCAN_ENABLE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_SCAN_ENABLE_CFM */

#ifndef EXCLUDE_DM_HCI_MASTER_LINK_KEY_CFM
CsrSize DM_HCI_MASTER_LINK_KEY_CFM_TSizeof(void *msg)
{
    DM_HCI_MASTER_LINK_KEY_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_MASTER_LINK_KEY_CFM */

#ifndef EXCLUDE_DM_HCI_MASTER_LINK_KEY_CFM
CsrUint8 *DM_HCI_MASTER_LINK_KEY_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_MASTER_LINK_KEY_CFM_T *primitive;

    primitive = (DM_HCI_MASTER_LINK_KEY_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->key_flag);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_MASTER_LINK_KEY_CFM */

#ifndef EXCLUDE_DM_HCI_MASTER_LINK_KEY_CFM
void *DM_HCI_MASTER_LINK_KEY_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_MASTER_LINK_KEY_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_MASTER_LINK_KEY_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_MASTER_LINK_KEY_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->key_flag, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_MASTER_LINK_KEY_CFM */

#ifndef EXCLUDE_DM_HCI_READ_LINK_SUPERV_TIMEOUT_CFM
CsrSize DM_HCI_READ_LINK_SUPERV_TIMEOUT_CFM_TSizeof(void *msg)
{
    DM_HCI_READ_LINK_SUPERV_TIMEOUT_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_LINK_SUPERV_TIMEOUT_CFM */

#ifndef EXCLUDE_DM_HCI_READ_LINK_SUPERV_TIMEOUT_CFM
CsrUint8 *DM_HCI_READ_LINK_SUPERV_TIMEOUT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_LINK_SUPERV_TIMEOUT_CFM_T *primitive;

    primitive = (DM_HCI_READ_LINK_SUPERV_TIMEOUT_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->timeout);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_LINK_SUPERV_TIMEOUT_CFM */

#ifndef EXCLUDE_DM_HCI_READ_LINK_SUPERV_TIMEOUT_CFM
void *DM_HCI_READ_LINK_SUPERV_TIMEOUT_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_LINK_SUPERV_TIMEOUT_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_LINK_SUPERV_TIMEOUT_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_LINK_SUPERV_TIMEOUT_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->timeout, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_LINK_SUPERV_TIMEOUT_CFM */

#ifndef EXCLUDE_DM_ACL_OPEN_CFM
CsrSize DM_ACL_OPEN_CFM_TSizeof(void *msg)
{
    DM_ACL_OPEN_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_ACL_OPEN_CFM */

#ifndef EXCLUDE_DM_ACL_OPEN_CFM
CsrUint8 *DM_ACL_OPEN_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_ACL_OPEN_CFM_T *primitive;

    primitive = (DM_ACL_OPEN_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->success);

    return buffer;
}
#endif /* EXCLUDE_DM_ACL_OPEN_CFM */

#ifndef EXCLUDE_DM_ACL_OPEN_CFM
void *DM_ACL_OPEN_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_ACL_OPEN_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_ACL_OPEN_CFM_T *) CsrPmemZalloc(sizeof(DM_ACL_OPEN_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->success, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_ACL_OPEN_CFM */

#ifndef EXCLUDE_DM_HCI_GET_LINK_QUALITY_REQ
CsrSize DM_HCI_GET_LINK_QUALITY_REQ_TSizeof(void *msg)
{
    DM_HCI_GET_LINK_QUALITY_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_GET_LINK_QUALITY_REQ */

#ifndef EXCLUDE_DM_HCI_GET_LINK_QUALITY_REQ
CsrUint8 *DM_HCI_GET_LINK_QUALITY_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_GET_LINK_QUALITY_REQ_T *primitive;

    primitive = (DM_HCI_GET_LINK_QUALITY_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_GET_LINK_QUALITY_REQ */

#ifndef EXCLUDE_DM_HCI_GET_LINK_QUALITY_REQ
void *DM_HCI_GET_LINK_QUALITY_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_GET_LINK_QUALITY_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_GET_LINK_QUALITY_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_GET_LINK_QUALITY_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_GET_LINK_QUALITY_REQ */

#ifndef EXCLUDE_DM_SYNC_DISCONNECT_IND
CsrSize DM_SYNC_DISCONNECT_IND_TSizeof(void *msg)
{
    DM_SYNC_DISCONNECT_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SYNC_DISCONNECT_IND */

#ifndef EXCLUDE_DM_SYNC_DISCONNECT_IND
CsrUint8 *DM_SYNC_DISCONNECT_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SYNC_DISCONNECT_IND_T *primitive;

    primitive = (DM_SYNC_DISCONNECT_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pv_cbarg);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->reason);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_SYNC_DISCONNECT_IND */

#ifndef EXCLUDE_DM_SYNC_DISCONNECT_IND
void *DM_SYNC_DISCONNECT_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SYNC_DISCONNECT_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SYNC_DISCONNECT_IND_T *) CsrPmemZalloc(sizeof(DM_SYNC_DISCONNECT_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pv_cbarg, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->reason, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SYNC_DISCONNECT_IND */

#ifndef EXCLUDE_DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_REQ
CsrSize DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_REQ_TSizeof(void *msg)
{
    DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_REQ */

#ifndef EXCLUDE_DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_REQ
CsrUint8 *DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_REQ_T *primitive;

    primitive = (DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pkt_type);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_REQ */

#ifndef EXCLUDE_DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_REQ
void *DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pkt_type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_REQ */

#ifndef EXCLUDE_DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_REQ
CsrSize DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_REQ_TSizeof(void *msg)
{
    DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_REQ */

#ifndef EXCLUDE_DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_REQ
CsrUint8 *DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_REQ_T *primitive;

    primitive = (DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_REQ */

#ifndef EXCLUDE_DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_REQ
void *DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_SET_ADVERTISING_DATA_CFM
CsrSize DM_HCI_ULP_SET_ADVERTISING_DATA_CFM_TSizeof(void *msg)
{
    DM_HCI_ULP_SET_ADVERTISING_DATA_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_ADVERTISING_DATA_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_SET_ADVERTISING_DATA_CFM
CsrUint8 *DM_HCI_ULP_SET_ADVERTISING_DATA_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_SET_ADVERTISING_DATA_CFM_T *primitive;

    primitive = (DM_HCI_ULP_SET_ADVERTISING_DATA_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_ADVERTISING_DATA_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_SET_ADVERTISING_DATA_CFM
void *DM_HCI_ULP_SET_ADVERTISING_DATA_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_SET_ADVERTISING_DATA_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_SET_ADVERTISING_DATA_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_SET_ADVERTISING_DATA_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_ADVERTISING_DATA_CFM */

#ifndef EXCLUDE_DM_AMPM_CONNECT_RSP
CsrSize DM_AMPM_CONNECT_RSP_TSizeof(void *msg)
{
    DM_AMPM_CONNECT_RSP_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_AMPM_CONNECT_RSP */

#ifndef EXCLUDE_DM_AMPM_CONNECT_RSP
CsrUint8 *DM_AMPM_CONNECT_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_AMPM_CONNECT_RSP_T *primitive;

    primitive = (DM_AMPM_CONNECT_RSP_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->local_amp_id);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->remote_amp_id);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_AMPM_CONNECT_RSP */

#ifndef EXCLUDE_DM_AMPM_CONNECT_RSP
void *DM_AMPM_CONNECT_RSP_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_AMPM_CONNECT_RSP_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_AMPM_CONNECT_RSP_T *) CsrPmemZalloc(sizeof(DM_AMPM_CONNECT_RSP_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->local_amp_id, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->remote_amp_id, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_AMPM_CONNECT_RSP */

#ifndef EXCLUDE_DM_AMPM_CONNECT_CHANNEL_IND
CsrSize DM_AMPM_CONNECT_CHANNEL_IND_TSizeof(void *msg)
{
    DM_AMPM_CONNECT_CHANNEL_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_AMPM_CONNECT_CHANNEL_IND */

#ifndef EXCLUDE_DM_AMPM_CONNECT_CHANNEL_IND
CsrUint8 *DM_AMPM_CONNECT_CHANNEL_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_AMPM_CONNECT_CHANNEL_IND_T *primitive;

    primitive = (DM_AMPM_CONNECT_CHANNEL_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->local_amp_id);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tx_flowspec.fs_identifier);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tx_flowspec.fs_service_type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->tx_flowspec.fs_max_sdu);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->tx_flowspec.fs_interarrival);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->tx_flowspec.fs_latency);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->tx_flowspec.fs_flush_to);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->rx_flowspec.fs_identifier);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->rx_flowspec.fs_service_type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->rx_flowspec.fs_max_sdu);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->rx_flowspec.fs_interarrival);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->rx_flowspec.fs_latency);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->rx_flowspec.fs_flush_to);

    return buffer;
}
#endif /* EXCLUDE_DM_AMPM_CONNECT_CHANNEL_IND */

#ifndef EXCLUDE_DM_AMPM_CONNECT_CHANNEL_IND
void *DM_AMPM_CONNECT_CHANNEL_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_AMPM_CONNECT_CHANNEL_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_AMPM_CONNECT_CHANNEL_IND_T *) CsrPmemZalloc(sizeof(DM_AMPM_CONNECT_CHANNEL_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->local_amp_id, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tx_flowspec.fs_identifier, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tx_flowspec.fs_service_type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->tx_flowspec.fs_max_sdu, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->tx_flowspec.fs_interarrival, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->tx_flowspec.fs_latency, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->tx_flowspec.fs_flush_to, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->rx_flowspec.fs_identifier, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->rx_flowspec.fs_service_type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->rx_flowspec.fs_max_sdu, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->rx_flowspec.fs_interarrival, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->rx_flowspec.fs_latency, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->rx_flowspec.fs_flush_to, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_AMPM_CONNECT_CHANNEL_IND */

#ifndef EXCLUDE_DM_HCI_READ_NUM_SUPPORTED_IAC_CFM
CsrSize DM_HCI_READ_NUM_SUPPORTED_IAC_CFM_TSizeof(void *msg)
{
    DM_HCI_READ_NUM_SUPPORTED_IAC_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_NUM_SUPPORTED_IAC_CFM */

#ifndef EXCLUDE_DM_HCI_READ_NUM_SUPPORTED_IAC_CFM
CsrUint8 *DM_HCI_READ_NUM_SUPPORTED_IAC_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_NUM_SUPPORTED_IAC_CFM_T *primitive;

    primitive = (DM_HCI_READ_NUM_SUPPORTED_IAC_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->num);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_NUM_SUPPORTED_IAC_CFM */

#ifndef EXCLUDE_DM_HCI_READ_NUM_SUPPORTED_IAC_CFM
void *DM_HCI_READ_NUM_SUPPORTED_IAC_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_NUM_SUPPORTED_IAC_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_NUM_SUPPORTED_IAC_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_NUM_SUPPORTED_IAC_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->num, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_NUM_SUPPORTED_IAC_CFM */

#ifndef EXCLUDE_DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_IND
CsrSize DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_IND_TSizeof(void *msg)
{
    DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_IND */

#ifndef EXCLUDE_DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_IND
CsrUint8 *DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_IND_T *primitive;

    primitive = (DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addrt.addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->signal_id);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_interval_min);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_interval_max);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_latency);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->supervision_timeout);

    return buffer;
}
#endif /* EXCLUDE_DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_IND */

#ifndef EXCLUDE_DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_IND
void *DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_IND_T *) CsrPmemZalloc(sizeof(DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addrt.addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->signal_id, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_interval_min, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_interval_max, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_latency, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->supervision_timeout, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_IND */

#ifndef EXCLUDE_DM_HCI_CHANGE_CONN_LINK_KEY_CFM
CsrSize DM_HCI_CHANGE_CONN_LINK_KEY_CFM_TSizeof(void *msg)
{
    DM_HCI_CHANGE_CONN_LINK_KEY_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_CHANGE_CONN_LINK_KEY_CFM */

#ifndef EXCLUDE_DM_HCI_CHANGE_CONN_LINK_KEY_CFM
CsrUint8 *DM_HCI_CHANGE_CONN_LINK_KEY_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_CHANGE_CONN_LINK_KEY_CFM_T *primitive;

    primitive = (DM_HCI_CHANGE_CONN_LINK_KEY_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_CHANGE_CONN_LINK_KEY_CFM */

#ifndef EXCLUDE_DM_HCI_CHANGE_CONN_LINK_KEY_CFM
void *DM_HCI_CHANGE_CONN_LINK_KEY_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_CHANGE_CONN_LINK_KEY_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_CHANGE_CONN_LINK_KEY_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_CHANGE_CONN_LINK_KEY_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_CHANGE_CONN_LINK_KEY_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_CONNECTION_UPDATE_COMPLETE_IND
CsrSize DM_HCI_ULP_CONNECTION_UPDATE_COMPLETE_IND_TSizeof(void *msg)
{
    DM_HCI_ULP_CONNECTION_UPDATE_COMPLETE_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_CONNECTION_UPDATE_COMPLETE_IND */

#ifndef EXCLUDE_DM_HCI_ULP_CONNECTION_UPDATE_COMPLETE_IND
CsrUint8 *DM_HCI_ULP_CONNECTION_UPDATE_COMPLETE_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_CONNECTION_UPDATE_COMPLETE_IND_T *primitive;

    primitive = (DM_HCI_ULP_CONNECTION_UPDATE_COMPLETE_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_interval);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_latency);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->supervision_timeout);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_CONNECTION_UPDATE_COMPLETE_IND */

#ifndef EXCLUDE_DM_HCI_ULP_CONNECTION_UPDATE_COMPLETE_IND
void *DM_HCI_ULP_CONNECTION_UPDATE_COMPLETE_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_CONNECTION_UPDATE_COMPLETE_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_CONNECTION_UPDATE_COMPLETE_IND_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_CONNECTION_UPDATE_COMPLETE_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_interval, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_latency, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->supervision_timeout, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_CONNECTION_UPDATE_COMPLETE_IND */

#ifndef EXCLUDE_DM_HCI_READ_REMOTE_EXT_FEATURES_REQ
CsrSize DM_HCI_READ_REMOTE_EXT_FEATURES_REQ_TSizeof(void *msg)
{
    DM_HCI_READ_REMOTE_EXT_FEATURES_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_REMOTE_EXT_FEATURES_REQ */

#ifndef EXCLUDE_DM_HCI_READ_REMOTE_EXT_FEATURES_REQ
CsrUint8 *DM_HCI_READ_REMOTE_EXT_FEATURES_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_REMOTE_EXT_FEATURES_REQ_T *primitive;

    primitive = (DM_HCI_READ_REMOTE_EXT_FEATURES_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->page_num);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_REMOTE_EXT_FEATURES_REQ */

#ifndef EXCLUDE_DM_HCI_READ_REMOTE_EXT_FEATURES_REQ
void *DM_HCI_READ_REMOTE_EXT_FEATURES_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_REMOTE_EXT_FEATURES_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_REMOTE_EXT_FEATURES_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_REMOTE_EXT_FEATURES_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->page_num, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_REMOTE_EXT_FEATURES_REQ */

#ifndef EXCLUDE_DM_HCI_READ_AUTH_ENABLE_REQ
CsrSize DM_HCI_READ_AUTH_ENABLE_REQ_TSizeof(void *msg)
{
    DM_HCI_READ_AUTH_ENABLE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_AUTH_ENABLE_REQ */

#ifndef EXCLUDE_DM_HCI_READ_AUTH_ENABLE_REQ
CsrUint8 *DM_HCI_READ_AUTH_ENABLE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_AUTH_ENABLE_REQ_T *primitive;

    primitive = (DM_HCI_READ_AUTH_ENABLE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_AUTH_ENABLE_REQ */

#ifndef EXCLUDE_DM_HCI_READ_AUTH_ENABLE_REQ
void *DM_HCI_READ_AUTH_ENABLE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_AUTH_ENABLE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_AUTH_ENABLE_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_AUTH_ENABLE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_AUTH_ENABLE_REQ */

#ifndef EXCLUDE_DM_HCI_READ_TX_POWER_LEVEL_CFM
CsrSize DM_HCI_READ_TX_POWER_LEVEL_CFM_TSizeof(void *msg)
{
    DM_HCI_READ_TX_POWER_LEVEL_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_TX_POWER_LEVEL_CFM */

#ifndef EXCLUDE_DM_HCI_READ_TX_POWER_LEVEL_CFM
CsrUint8 *DM_HCI_READ_TX_POWER_LEVEL_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_TX_POWER_LEVEL_CFM_T *primitive;

    primitive = (DM_HCI_READ_TX_POWER_LEVEL_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->pwr_level);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_TX_POWER_LEVEL_CFM */

#ifndef EXCLUDE_DM_HCI_READ_TX_POWER_LEVEL_CFM
void *DM_HCI_READ_TX_POWER_LEVEL_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_TX_POWER_LEVEL_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_TX_POWER_LEVEL_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_TX_POWER_LEVEL_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->pwr_level, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_TX_POWER_LEVEL_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_SET_EVENT_MASK_REQ
CsrSize DM_HCI_ULP_SET_EVENT_MASK_REQ_TSizeof(void *msg)
{
    DM_HCI_ULP_SET_EVENT_MASK_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_EVENT_MASK_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_SET_EVENT_MASK_REQ
CsrUint8 *DM_HCI_ULP_SET_EVENT_MASK_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_SET_EVENT_MASK_REQ_T *primitive;

    primitive = (DM_HCI_ULP_SET_EVENT_MASK_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrMemCpySer(buffer, length, ((const void *) primitive->ulp_event_mask), ((CsrSize) 4 * (2)));

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_EVENT_MASK_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_SET_EVENT_MASK_REQ
void *DM_HCI_ULP_SET_EVENT_MASK_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_SET_EVENT_MASK_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_SET_EVENT_MASK_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_SET_EVENT_MASK_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->ulp_event_mask), buffer, &offset, ((CsrSize) 4 * (2)));

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_EVENT_MASK_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_REQ
CsrSize DM_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_REQ_TSizeof(void *msg)
{
    DM_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_REQ
CsrUint8 *DM_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_REQ_T *primitive;

    primitive = (DM_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrMemCpySer(buffer, length, ((const void *) primitive->channel_map), ((CsrSize) (5)));

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_REQ
void *DM_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->channel_map), buffer, &offset, ((CsrSize) (5)));

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_CLASS_OF_DEVICE_CFM
CsrSize DM_HCI_WRITE_CLASS_OF_DEVICE_CFM_TSizeof(void *msg)
{
    DM_HCI_WRITE_CLASS_OF_DEVICE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_CLASS_OF_DEVICE_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_CLASS_OF_DEVICE_CFM
CsrUint8 *DM_HCI_WRITE_CLASS_OF_DEVICE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_CLASS_OF_DEVICE_CFM_T *primitive;

    primitive = (DM_HCI_WRITE_CLASS_OF_DEVICE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_CLASS_OF_DEVICE_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_CLASS_OF_DEVICE_CFM
void *DM_HCI_WRITE_CLASS_OF_DEVICE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_CLASS_OF_DEVICE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_CLASS_OF_DEVICE_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_CLASS_OF_DEVICE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_CLASS_OF_DEVICE_CFM */

#ifndef EXCLUDE_DM_HCI_ROLE_DISCOVERY_CFM
CsrSize DM_HCI_ROLE_DISCOVERY_CFM_TSizeof(void *msg)
{
    DM_HCI_ROLE_DISCOVERY_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ROLE_DISCOVERY_CFM */

#ifndef EXCLUDE_DM_HCI_ROLE_DISCOVERY_CFM
CsrUint8 *DM_HCI_ROLE_DISCOVERY_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ROLE_DISCOVERY_CFM_T *primitive;

    primitive = (DM_HCI_ROLE_DISCOVERY_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->role);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ROLE_DISCOVERY_CFM */

#ifndef EXCLUDE_DM_HCI_ROLE_DISCOVERY_CFM
void *DM_HCI_ROLE_DISCOVERY_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ROLE_DISCOVERY_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ROLE_DISCOVERY_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_ROLE_DISCOVERY_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->role, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ROLE_DISCOVERY_CFM */

#ifndef EXCLUDE_DM_HCI_READ_CLASS_OF_DEVICE_REQ
CsrSize DM_HCI_READ_CLASS_OF_DEVICE_REQ_TSizeof(void *msg)
{
    DM_HCI_READ_CLASS_OF_DEVICE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_CLASS_OF_DEVICE_REQ */

#ifndef EXCLUDE_DM_HCI_READ_CLASS_OF_DEVICE_REQ
CsrUint8 *DM_HCI_READ_CLASS_OF_DEVICE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_CLASS_OF_DEVICE_REQ_T *primitive;

    primitive = (DM_HCI_READ_CLASS_OF_DEVICE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_CLASS_OF_DEVICE_REQ */

#ifndef EXCLUDE_DM_HCI_READ_CLASS_OF_DEVICE_REQ
void *DM_HCI_READ_CLASS_OF_DEVICE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_CLASS_OF_DEVICE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_CLASS_OF_DEVICE_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_CLASS_OF_DEVICE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_CLASS_OF_DEVICE_REQ */

#ifndef EXCLUDE_DM_AMPM_READ_DATA_BLOCK_SIZE_IND
CsrSize DM_AMPM_READ_DATA_BLOCK_SIZE_IND_TSizeof(void *msg)
{
    DM_AMPM_READ_DATA_BLOCK_SIZE_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_AMPM_READ_DATA_BLOCK_SIZE_IND */

#ifndef EXCLUDE_DM_AMPM_READ_DATA_BLOCK_SIZE_IND
CsrUint8 *DM_AMPM_READ_DATA_BLOCK_SIZE_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_AMPM_READ_DATA_BLOCK_SIZE_IND_T *primitive;

    primitive = (DM_AMPM_READ_DATA_BLOCK_SIZE_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->local_amp_id);

    return buffer;
}
#endif /* EXCLUDE_DM_AMPM_READ_DATA_BLOCK_SIZE_IND */

#ifndef EXCLUDE_DM_AMPM_READ_DATA_BLOCK_SIZE_IND
void *DM_AMPM_READ_DATA_BLOCK_SIZE_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_AMPM_READ_DATA_BLOCK_SIZE_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_AMPM_READ_DATA_BLOCK_SIZE_IND_T *) CsrPmemZalloc(sizeof(DM_AMPM_READ_DATA_BLOCK_SIZE_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->local_amp_id, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_AMPM_READ_DATA_BLOCK_SIZE_IND */

#ifndef EXCLUDE_DM_SM_USER_CONFIRMATION_REQUEST_NEG_RSP
CsrSize DM_SM_USER_CONFIRMATION_REQUEST_NEG_RSP_TSizeof(void *msg)
{
    DM_SM_USER_CONFIRMATION_REQUEST_NEG_RSP_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_USER_CONFIRMATION_REQUEST_NEG_RSP */

#ifndef EXCLUDE_DM_SM_USER_CONFIRMATION_REQUEST_NEG_RSP
CsrUint8 *DM_SM_USER_CONFIRMATION_REQUEST_NEG_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_USER_CONFIRMATION_REQUEST_NEG_RSP_T *primitive;

    primitive = (DM_SM_USER_CONFIRMATION_REQUEST_NEG_RSP_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_USER_CONFIRMATION_REQUEST_NEG_RSP */

#ifndef EXCLUDE_DM_SM_USER_CONFIRMATION_REQUEST_NEG_RSP
void *DM_SM_USER_CONFIRMATION_REQUEST_NEG_RSP_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_USER_CONFIRMATION_REQUEST_NEG_RSP_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_USER_CONFIRMATION_REQUEST_NEG_RSP_T *) CsrPmemZalloc(sizeof(DM_SM_USER_CONFIRMATION_REQUEST_NEG_RSP_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_USER_CONFIRMATION_REQUEST_NEG_RSP */

#ifndef EXCLUDE_DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_REQ
CsrSize DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_REQ_TSizeof(void *msg)
{
    DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_REQ */

#ifndef EXCLUDE_DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_REQ
CsrUint8 *DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_REQ_T *primitive;

    primitive = (DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_REQ */

#ifndef EXCLUDE_DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_REQ
void *DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_CFM
CsrSize DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_CFM_TSizeof(void *msg)
{
    DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_CFM
CsrUint8 *DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_CFM_T *primitive;

    primitive = (DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_CFM
void *DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_CFM */

#ifndef EXCLUDE_DM_HCI_ENHANCED_FLUSH_CFM
CsrSize DM_HCI_ENHANCED_FLUSH_CFM_TSizeof(void *msg)
{
    DM_HCI_ENHANCED_FLUSH_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ENHANCED_FLUSH_CFM */

#ifndef EXCLUDE_DM_HCI_ENHANCED_FLUSH_CFM
CsrUint8 *DM_HCI_ENHANCED_FLUSH_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ENHANCED_FLUSH_CFM_T *primitive;

    primitive = (DM_HCI_ENHANCED_FLUSH_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ENHANCED_FLUSH_CFM */

#ifndef EXCLUDE_DM_HCI_ENHANCED_FLUSH_CFM
void *DM_HCI_ENHANCED_FLUSH_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ENHANCED_FLUSH_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ENHANCED_FLUSH_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_ENHANCED_FLUSH_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ENHANCED_FLUSH_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_PAGESCAN_MODE_CFM
CsrSize DM_HCI_WRITE_PAGESCAN_MODE_CFM_TSizeof(void *msg)
{
    DM_HCI_WRITE_PAGESCAN_MODE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_PAGESCAN_MODE_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_PAGESCAN_MODE_CFM
CsrUint8 *DM_HCI_WRITE_PAGESCAN_MODE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_PAGESCAN_MODE_CFM_T *primitive;

    primitive = (DM_HCI_WRITE_PAGESCAN_MODE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_PAGESCAN_MODE_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_PAGESCAN_MODE_CFM
void *DM_HCI_WRITE_PAGESCAN_MODE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_PAGESCAN_MODE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_PAGESCAN_MODE_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_PAGESCAN_MODE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_PAGESCAN_MODE_CFM */

#ifndef EXCLUDE_DM_SM_LINK_KEY_IND
CsrSize DM_SM_LINK_KEY_IND_TSizeof(void *msg)
{
    DM_SM_LINK_KEY_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_LINK_KEY_IND */

#ifndef EXCLUDE_DM_SM_LINK_KEY_IND
CsrUint8 *DM_SM_LINK_KEY_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_LINK_KEY_IND_T *primitive;

    primitive = (DM_SM_LINK_KEY_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->key_type);
    CsrMemCpySer(buffer, length, ((const void *) primitive->key), ((CsrSize) (16)));

    return buffer;
}
#endif /* EXCLUDE_DM_SM_LINK_KEY_IND */

#ifndef EXCLUDE_DM_SM_LINK_KEY_IND
void *DM_SM_LINK_KEY_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_LINK_KEY_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_LINK_KEY_IND_T *) CsrPmemZalloc(sizeof(DM_SM_LINK_KEY_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->key_type, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->key), buffer, &offset, ((CsrSize) (16)));

    return primitive;
}
#endif /* EXCLUDE_DM_SM_LINK_KEY_IND */

#ifndef EXCLUDE_DM_SM_UNREGISTER_OUTGOING_CFM
CsrSize DM_SM_UNREGISTER_OUTGOING_CFM_TSizeof(void *msg)
{
    DM_SM_UNREGISTER_OUTGOING_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_UNREGISTER_OUTGOING_CFM */

#ifndef EXCLUDE_DM_SM_UNREGISTER_OUTGOING_CFM
CsrUint8 *DM_SM_UNREGISTER_OUTGOING_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_UNREGISTER_OUTGOING_CFM_T *primitive;

    primitive = (DM_SM_UNREGISTER_OUTGOING_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->connection.service.protocol_id);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->connection.service.channel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connection.addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connection.addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connection.addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->connection.addrt.addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_UNREGISTER_OUTGOING_CFM */

#ifndef EXCLUDE_DM_SM_UNREGISTER_OUTGOING_CFM
void *DM_SM_UNREGISTER_OUTGOING_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_UNREGISTER_OUTGOING_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_UNREGISTER_OUTGOING_CFM_T *) CsrPmemZalloc(sizeof(DM_SM_UNREGISTER_OUTGOING_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->connection.service.protocol_id, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->connection.service.channel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connection.addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connection.addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connection.addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->connection.addrt.addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_UNREGISTER_OUTGOING_CFM */

#ifndef EXCLUDE_DM_HCI_READ_COUNTRY_CODE_CFM
CsrSize DM_HCI_READ_COUNTRY_CODE_CFM_TSizeof(void *msg)
{
    DM_HCI_READ_COUNTRY_CODE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_COUNTRY_CODE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_COUNTRY_CODE_CFM
CsrUint8 *DM_HCI_READ_COUNTRY_CODE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_COUNTRY_CODE_CFM_T *primitive;

    primitive = (DM_HCI_READ_COUNTRY_CODE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->country_code);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_COUNTRY_CODE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_COUNTRY_CODE_CFM
void *DM_HCI_READ_COUNTRY_CODE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_COUNTRY_CODE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_COUNTRY_CODE_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_COUNTRY_CODE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->country_code, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_COUNTRY_CODE_CFM */

#ifndef EXCLUDE_DM_SM_REGISTER_OUTGOING_REQ
CsrSize DM_SM_REGISTER_OUTGOING_REQ_TSizeof(void *msg)
{
    DM_SM_REGISTER_OUTGOING_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_REGISTER_OUTGOING_REQ */

#ifndef EXCLUDE_DM_SM_REGISTER_OUTGOING_REQ
CsrUint8 *DM_SM_REGISTER_OUTGOING_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_REGISTER_OUTGOING_REQ_T *primitive;

    primitive = (DM_SM_REGISTER_OUTGOING_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->connection.service.protocol_id);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->connection.service.channel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connection.addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connection.addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connection.addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->connection.addrt.addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->security_level);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_REGISTER_OUTGOING_REQ */

#ifndef EXCLUDE_DM_SM_REGISTER_OUTGOING_REQ
void *DM_SM_REGISTER_OUTGOING_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_REGISTER_OUTGOING_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_REGISTER_OUTGOING_REQ_T *) CsrPmemZalloc(sizeof(DM_SM_REGISTER_OUTGOING_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->connection.service.protocol_id, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->connection.service.channel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connection.addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connection.addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connection.addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->connection.addrt.addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->security_level, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_REGISTER_OUTGOING_REQ */

#ifndef EXCLUDE_DM_HCI_GET_LINK_QUALITY_CFM
CsrSize DM_HCI_GET_LINK_QUALITY_CFM_TSizeof(void *msg)
{
    DM_HCI_GET_LINK_QUALITY_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_GET_LINK_QUALITY_CFM */

#ifndef EXCLUDE_DM_HCI_GET_LINK_QUALITY_CFM
CsrUint8 *DM_HCI_GET_LINK_QUALITY_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_GET_LINK_QUALITY_CFM_T *primitive;

    primitive = (DM_HCI_GET_LINK_QUALITY_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->link_quality);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_GET_LINK_QUALITY_CFM */

#ifndef EXCLUDE_DM_HCI_GET_LINK_QUALITY_CFM
void *DM_HCI_GET_LINK_QUALITY_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_GET_LINK_QUALITY_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_GET_LINK_QUALITY_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_GET_LINK_QUALITY_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->link_quality, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_GET_LINK_QUALITY_CFM */

#ifndef EXCLUDE_DM_HCI_READ_LMP_HANDLE_REQ
CsrSize DM_HCI_READ_LMP_HANDLE_REQ_TSizeof(void *msg)
{
    DM_HCI_READ_LMP_HANDLE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_LMP_HANDLE_REQ */

#ifndef EXCLUDE_DM_HCI_READ_LMP_HANDLE_REQ
CsrUint8 *DM_HCI_READ_LMP_HANDLE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_LMP_HANDLE_REQ_T *primitive;

    primitive = (DM_HCI_READ_LMP_HANDLE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_LMP_HANDLE_REQ */

#ifndef EXCLUDE_DM_HCI_READ_LMP_HANDLE_REQ
void *DM_HCI_READ_LMP_HANDLE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_LMP_HANDLE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_LMP_HANDLE_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_LMP_HANDLE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_LMP_HANDLE_REQ */

#ifndef EXCLUDE_DM_AMPM_SIMPLE_PAIRING_ENABLED_IND
CsrSize DM_AMPM_SIMPLE_PAIRING_ENABLED_IND_TSizeof(void *msg)
{
    DM_AMPM_SIMPLE_PAIRING_ENABLED_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_AMPM_SIMPLE_PAIRING_ENABLED_IND */

#ifndef EXCLUDE_DM_AMPM_SIMPLE_PAIRING_ENABLED_IND
CsrUint8 *DM_AMPM_SIMPLE_PAIRING_ENABLED_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_AMPM_SIMPLE_PAIRING_ENABLED_IND_T *primitive;

    primitive = (DM_AMPM_SIMPLE_PAIRING_ENABLED_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);

    return buffer;
}
#endif /* EXCLUDE_DM_AMPM_SIMPLE_PAIRING_ENABLED_IND */

#ifndef EXCLUDE_DM_AMPM_SIMPLE_PAIRING_ENABLED_IND
void *DM_AMPM_SIMPLE_PAIRING_ENABLED_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_AMPM_SIMPLE_PAIRING_ENABLED_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_AMPM_SIMPLE_PAIRING_ENABLED_IND_T *) CsrPmemZalloc(sizeof(DM_AMPM_SIMPLE_PAIRING_ENABLED_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_AMPM_SIMPLE_PAIRING_ENABLED_IND */

#ifndef EXCLUDE_DM_BAD_MESSAGE_IND
CsrSize DM_BAD_MESSAGE_IND_TSizeof(void *msg)
{
    DM_BAD_MESSAGE_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->message_length * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_BAD_MESSAGE_IND */

#ifndef EXCLUDE_DM_BAD_MESSAGE_IND
CsrUint8 *DM_BAD_MESSAGE_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_BAD_MESSAGE_IND_T *primitive;

    primitive = (DM_BAD_MESSAGE_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reason);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->protocol);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->request_type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->message_length);
    CsrMemCpySer(buffer, length, &primitive->message, sizeof(primitive->message));
    if(primitive->message)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->message), ((CsrSize) (primitive->message_length * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_DM_BAD_MESSAGE_IND */

#ifndef EXCLUDE_DM_BAD_MESSAGE_IND
void *DM_BAD_MESSAGE_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_BAD_MESSAGE_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_BAD_MESSAGE_IND_T *) CsrPmemZalloc(sizeof(DM_BAD_MESSAGE_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reason, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->protocol, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->request_type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->message_length, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->message), buffer, &offset, ((CsrSize) sizeof(primitive->message)));
    if (primitive->message_length)
    {
        primitive->message = CsrPmemZalloc(((CsrUint32) (primitive->message_length * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->message), buffer, &offset, ((CsrSize) (primitive->message_length * sizeof(CsrUint8))));
    }
    else
    {
        primitive->message = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_DM_BAD_MESSAGE_IND */

#ifndef EXCLUDE_DM_BAD_MESSAGE_IND
void DM_BAD_MESSAGE_IND_TSerFree(void *msg)
{
    DM_BAD_MESSAGE_IND_T *primitive;

    primitive = (DM_BAD_MESSAGE_IND_T *) msg;

    if(primitive->message != NULL)
    {
        CsrPmemFree(primitive->message);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_DM_BAD_MESSAGE_IND */

#ifndef EXCLUDE_DM_HCI_ULP_RECEIVER_TEST_REQ
CsrSize DM_HCI_ULP_RECEIVER_TEST_REQ_TSizeof(void *msg)
{
    DM_HCI_ULP_RECEIVER_TEST_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_RECEIVER_TEST_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_RECEIVER_TEST_REQ
CsrUint8 *DM_HCI_ULP_RECEIVER_TEST_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_RECEIVER_TEST_REQ_T *primitive;

    primitive = (DM_HCI_ULP_RECEIVER_TEST_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->rx_channel);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_RECEIVER_TEST_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_RECEIVER_TEST_REQ
void *DM_HCI_ULP_RECEIVER_TEST_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_RECEIVER_TEST_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_RECEIVER_TEST_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_RECEIVER_TEST_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->rx_channel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_RECEIVER_TEST_REQ */

#ifndef EXCLUDE_DM_HCI_READ_INQUIRYSCAN_ACTIVITY_REQ
CsrSize DM_HCI_READ_INQUIRYSCAN_ACTIVITY_REQ_TSizeof(void *msg)
{
    DM_HCI_READ_INQUIRYSCAN_ACTIVITY_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_INQUIRYSCAN_ACTIVITY_REQ */

#ifndef EXCLUDE_DM_HCI_READ_INQUIRYSCAN_ACTIVITY_REQ
CsrUint8 *DM_HCI_READ_INQUIRYSCAN_ACTIVITY_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_INQUIRYSCAN_ACTIVITY_REQ_T *primitive;

    primitive = (DM_HCI_READ_INQUIRYSCAN_ACTIVITY_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_INQUIRYSCAN_ACTIVITY_REQ */

#ifndef EXCLUDE_DM_HCI_READ_INQUIRYSCAN_ACTIVITY_REQ
void *DM_HCI_READ_INQUIRYSCAN_ACTIVITY_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_INQUIRYSCAN_ACTIVITY_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_INQUIRYSCAN_ACTIVITY_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_INQUIRYSCAN_ACTIVITY_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_INQUIRYSCAN_ACTIVITY_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_PAGE_TIMEOUT_CFM
CsrSize DM_HCI_WRITE_PAGE_TIMEOUT_CFM_TSizeof(void *msg)
{
    DM_HCI_WRITE_PAGE_TIMEOUT_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_PAGE_TIMEOUT_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_PAGE_TIMEOUT_CFM
CsrUint8 *DM_HCI_WRITE_PAGE_TIMEOUT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_PAGE_TIMEOUT_CFM_T *primitive;

    primitive = (DM_HCI_WRITE_PAGE_TIMEOUT_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_PAGE_TIMEOUT_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_PAGE_TIMEOUT_CFM
void *DM_HCI_WRITE_PAGE_TIMEOUT_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_PAGE_TIMEOUT_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_PAGE_TIMEOUT_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_PAGE_TIMEOUT_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_PAGE_TIMEOUT_CFM */

#ifndef EXCLUDE_DM_HCI_READ_CLOCK_REQ
CsrSize DM_HCI_READ_CLOCK_REQ_TSizeof(void *msg)
{
    DM_HCI_READ_CLOCK_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_CLOCK_REQ */

#ifndef EXCLUDE_DM_HCI_READ_CLOCK_REQ
CsrUint8 *DM_HCI_READ_CLOCK_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_CLOCK_REQ_T *primitive;

    primitive = (DM_HCI_READ_CLOCK_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->which_clock);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_CLOCK_REQ */

#ifndef EXCLUDE_DM_HCI_READ_CLOCK_REQ
void *DM_HCI_READ_CLOCK_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_CLOCK_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_CLOCK_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_CLOCK_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->which_clock, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_CLOCK_REQ */

#ifndef EXCLUDE_DM_WRITE_CACHED_CLOCK_OFFSET_REQ
CsrSize DM_WRITE_CACHED_CLOCK_OFFSET_REQ_TSizeof(void *msg)
{
    DM_WRITE_CACHED_CLOCK_OFFSET_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_WRITE_CACHED_CLOCK_OFFSET_REQ */

#ifndef EXCLUDE_DM_WRITE_CACHED_CLOCK_OFFSET_REQ
CsrUint8 *DM_WRITE_CACHED_CLOCK_OFFSET_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_WRITE_CACHED_CLOCK_OFFSET_REQ_T *primitive;

    primitive = (DM_WRITE_CACHED_CLOCK_OFFSET_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->clock_offset);

    return buffer;
}
#endif /* EXCLUDE_DM_WRITE_CACHED_CLOCK_OFFSET_REQ */

#ifndef EXCLUDE_DM_WRITE_CACHED_CLOCK_OFFSET_REQ
void *DM_WRITE_CACHED_CLOCK_OFFSET_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_WRITE_CACHED_CLOCK_OFFSET_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_WRITE_CACHED_CLOCK_OFFSET_REQ_T *) CsrPmemZalloc(sizeof(DM_WRITE_CACHED_CLOCK_OFFSET_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->clock_offset, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_WRITE_CACHED_CLOCK_OFFSET_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_VOICE_SETTING_REQ
CsrSize DM_HCI_WRITE_VOICE_SETTING_REQ_TSizeof(void *msg)
{
    DM_HCI_WRITE_VOICE_SETTING_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_VOICE_SETTING_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_VOICE_SETTING_REQ
CsrUint8 *DM_HCI_WRITE_VOICE_SETTING_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_VOICE_SETTING_REQ_T *primitive;

    primitive = (DM_HCI_WRITE_VOICE_SETTING_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->voice_setting);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_VOICE_SETTING_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_VOICE_SETTING_REQ
void *DM_HCI_WRITE_VOICE_SETTING_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_VOICE_SETTING_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_VOICE_SETTING_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_VOICE_SETTING_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->voice_setting, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_VOICE_SETTING_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_CFM
CsrSize DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_CFM_TSizeof(void *msg)
{
    DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_CFM
CsrUint8 *DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_CFM_T *primitive;

    primitive = (DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_CFM
void *DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_REQ
CsrSize DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_REQ_TSizeof(void *msg)
{
    DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_REQ
CsrUint8 *DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_REQ_T *primitive;

    primitive = (DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->inqscan_interval);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->inqscan_window);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_REQ
void *DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->inqscan_interval, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->inqscan_window, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_REQ */

#ifndef EXCLUDE_DM_SM_ENCRYPTION_CHANGE_IND
CsrSize DM_SM_ENCRYPTION_CHANGE_IND_TSizeof(void *msg)
{
    DM_SM_ENCRYPTION_CHANGE_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_ENCRYPTION_CHANGE_IND */

#ifndef EXCLUDE_DM_SM_ENCRYPTION_CHANGE_IND
CsrUint8 *DM_SM_ENCRYPTION_CHANGE_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_ENCRYPTION_CHANGE_IND_T *primitive;

    primitive = (DM_SM_ENCRYPTION_CHANGE_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->encrypted);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_ENCRYPTION_CHANGE_IND */

#ifndef EXCLUDE_DM_SM_ENCRYPTION_CHANGE_IND
void *DM_SM_ENCRYPTION_CHANGE_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_ENCRYPTION_CHANGE_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_ENCRYPTION_CHANGE_IND_T *) CsrPmemZalloc(sizeof(DM_SM_ENCRYPTION_CHANGE_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->encrypted, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_ENCRYPTION_CHANGE_IND */

#ifndef EXCLUDE_DM_SM_DATA_SIGN_CFM
CsrSize DM_SM_DATA_SIGN_CFM_TSizeof(void *msg)
{
    DM_SM_DATA_SIGN_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->length * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_DATA_SIGN_CFM */

#ifndef EXCLUDE_DM_SM_DATA_SIGN_CFM
CsrUint8 *DM_SM_DATA_SIGN_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_DATA_SIGN_CFM_T *primitive;

    primitive = (DM_SM_DATA_SIGN_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->verify);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->length);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    if(primitive->data)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->data), ((CsrSize) (primitive->length * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_DM_SM_DATA_SIGN_CFM */

#ifndef EXCLUDE_DM_SM_DATA_SIGN_CFM
void *DM_SM_DATA_SIGN_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_DATA_SIGN_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_DATA_SIGN_CFM_T *) CsrPmemZalloc(sizeof(DM_SM_DATA_SIGN_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->verify, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->length, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    if (primitive->length)
    {
        primitive->data = CsrPmemZalloc(((CsrUint32) (primitive->length * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->data), buffer, &offset, ((CsrSize) (primitive->length * sizeof(CsrUint8))));
    }
    else
    {
        primitive->data = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_DM_SM_DATA_SIGN_CFM */

#ifndef EXCLUDE_DM_SM_DATA_SIGN_CFM
void DM_SM_DATA_SIGN_CFM_TSerFree(void *msg)
{
    DM_SM_DATA_SIGN_CFM_T *primitive;

    primitive = (DM_SM_DATA_SIGN_CFM_T *) msg;

    if(primitive->data != NULL)
    {
        CsrPmemFree(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_DM_SM_DATA_SIGN_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_TRANSMITTER_TEST_CFM
CsrSize DM_HCI_ULP_TRANSMITTER_TEST_CFM_TSizeof(void *msg)
{
    DM_HCI_ULP_TRANSMITTER_TEST_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_TRANSMITTER_TEST_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_TRANSMITTER_TEST_CFM
CsrUint8 *DM_HCI_ULP_TRANSMITTER_TEST_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_TRANSMITTER_TEST_CFM_T *primitive;

    primitive = (DM_HCI_ULP_TRANSMITTER_TEST_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_TRANSMITTER_TEST_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_TRANSMITTER_TEST_CFM
void *DM_HCI_ULP_TRANSMITTER_TEST_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_TRANSMITTER_TEST_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_TRANSMITTER_TEST_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_TRANSMITTER_TEST_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_TRANSMITTER_TEST_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_CONN_ACCEPT_TIMEOUT_CFM
CsrSize DM_HCI_WRITE_CONN_ACCEPT_TIMEOUT_CFM_TSizeof(void *msg)
{
    DM_HCI_WRITE_CONN_ACCEPT_TIMEOUT_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_CONN_ACCEPT_TIMEOUT_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_CONN_ACCEPT_TIMEOUT_CFM
CsrUint8 *DM_HCI_WRITE_CONN_ACCEPT_TIMEOUT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_CONN_ACCEPT_TIMEOUT_CFM_T *primitive;

    primitive = (DM_HCI_WRITE_CONN_ACCEPT_TIMEOUT_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_CONN_ACCEPT_TIMEOUT_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_CONN_ACCEPT_TIMEOUT_CFM
void *DM_HCI_WRITE_CONN_ACCEPT_TIMEOUT_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_CONN_ACCEPT_TIMEOUT_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_CONN_ACCEPT_TIMEOUT_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_CONN_ACCEPT_TIMEOUT_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_CONN_ACCEPT_TIMEOUT_CFM */

#ifndef EXCLUDE_DM_HCI_READ_FAILED_CONTACT_COUNT_REQ
CsrSize DM_HCI_READ_FAILED_CONTACT_COUNT_REQ_TSizeof(void *msg)
{
    DM_HCI_READ_FAILED_CONTACT_COUNT_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_FAILED_CONTACT_COUNT_REQ */

#ifndef EXCLUDE_DM_HCI_READ_FAILED_CONTACT_COUNT_REQ
CsrUint8 *DM_HCI_READ_FAILED_CONTACT_COUNT_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_FAILED_CONTACT_COUNT_REQ_T *primitive;

    primitive = (DM_HCI_READ_FAILED_CONTACT_COUNT_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_FAILED_CONTACT_COUNT_REQ */

#ifndef EXCLUDE_DM_HCI_READ_FAILED_CONTACT_COUNT_REQ
void *DM_HCI_READ_FAILED_CONTACT_COUNT_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_FAILED_CONTACT_COUNT_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_FAILED_CONTACT_COUNT_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_FAILED_CONTACT_COUNT_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_FAILED_CONTACT_COUNT_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_TEST_END_REQ
CsrSize DM_HCI_ULP_TEST_END_REQ_TSizeof(void *msg)
{
    DM_HCI_ULP_TEST_END_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_TEST_END_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_TEST_END_REQ
CsrUint8 *DM_HCI_ULP_TEST_END_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_TEST_END_REQ_T *primitive;

    primitive = (DM_HCI_ULP_TEST_END_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_TEST_END_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_TEST_END_REQ
void *DM_HCI_ULP_TEST_END_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_TEST_END_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_TEST_END_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_TEST_END_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_TEST_END_REQ */

#ifndef EXCLUDE_DM_DATA_TO_HCI_IND
CsrSize DM_DATA_TO_HCI_IND_TSizeof(void *msg)
{
    DM_DATA_TO_HCI_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrMblkGetLength(primitive->data) + sizeof(CsrUint16));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_DATA_TO_HCI_IND */

#ifndef EXCLUDE_DM_DATA_TO_HCI_IND
CsrUint8 *DM_DATA_TO_HCI_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_DATA_TO_HCI_IND_T *primitive;

    primitive = (DM_DATA_TO_HCI_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->controller);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->physical_handle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->logical_handle);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    if (primitive->data)
    {
        CsrUint16 mblkLength = CsrMblkGetLength(primitive->data);
        CsrUint16Ser(buffer, length, mblkLength);
        CsrMblkCopyToMemory(primitive->data, 0, mblkLength, &buffer[*length]);
        *length += mblkLength;
    }
    else
    {
        buffer[*length] = 0;
        buffer[*length + 1] = 0;
        *length += 2;
    }

    return buffer;
}
#endif /* EXCLUDE_DM_DATA_TO_HCI_IND */

#ifndef EXCLUDE_DM_DATA_TO_HCI_IND
void *DM_DATA_TO_HCI_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_DATA_TO_HCI_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_DATA_TO_HCI_IND_T *) CsrPmemZalloc(sizeof(DM_DATA_TO_HCI_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->controller, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->physical_handle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->logical_handle, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
    {
        CsrUint8 *mblkData;
        CsrUint16 mblkLength;
        CsrUint16Des((CsrUint16 *) &mblkLength, buffer, &offset);
        primitive->data = CsrMblkMallocCreate((void **) &mblkData, mblkLength);
        CsrMemCpy(mblkData,&buffer[offset], mblkLength);
        offset += mblkLength;
    }

    return primitive;
}
#endif /* EXCLUDE_DM_DATA_TO_HCI_IND */

#ifndef EXCLUDE_DM_DATA_TO_HCI_IND
void DM_DATA_TO_HCI_IND_TSerFree(void *msg)
{
    DM_DATA_TO_HCI_IND_T *primitive;

    primitive = (DM_DATA_TO_HCI_IND_T *) msg;

    if(primitive->data != NULL)
    {
        CsrMblkDestroy(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_DM_DATA_TO_HCI_IND */

#ifndef EXCLUDE_DM_HCI_SWITCH_ROLE_REQ
CsrSize DM_HCI_SWITCH_ROLE_REQ_TSizeof(void *msg)
{
    DM_HCI_SWITCH_ROLE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_SWITCH_ROLE_REQ */

#ifndef EXCLUDE_DM_HCI_SWITCH_ROLE_REQ
CsrUint8 *DM_HCI_SWITCH_ROLE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_SWITCH_ROLE_REQ_T *primitive;

    primitive = (DM_HCI_SWITCH_ROLE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->role);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_SWITCH_ROLE_REQ */

#ifndef EXCLUDE_DM_HCI_SWITCH_ROLE_REQ
void *DM_HCI_SWITCH_ROLE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_SWITCH_ROLE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_SWITCH_ROLE_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_SWITCH_ROLE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->role, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_SWITCH_ROLE_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_PIN_TYPE_CFM
CsrSize DM_HCI_WRITE_PIN_TYPE_CFM_TSizeof(void *msg)
{
    DM_HCI_WRITE_PIN_TYPE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_PIN_TYPE_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_PIN_TYPE_CFM
CsrUint8 *DM_HCI_WRITE_PIN_TYPE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_PIN_TYPE_CFM_T *primitive;

    primitive = (DM_HCI_WRITE_PIN_TYPE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_PIN_TYPE_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_PIN_TYPE_CFM
void *DM_HCI_WRITE_PIN_TYPE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_PIN_TYPE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_PIN_TYPE_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_PIN_TYPE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_PIN_TYPE_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_REQ
CsrSize DM_HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_REQ_TSizeof(void *msg)
{
    DM_HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_REQ
CsrUint8 *DM_HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_REQ_T *primitive;

    primitive = (DM_HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address_type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->address.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->address.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_REQ
void *DM_HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address_type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->address.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->address.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_REQ */

#ifndef EXCLUDE_DM_HCI_READ_ENCRYPTION_KEY_SIZE_REQ
CsrSize DM_HCI_READ_ENCRYPTION_KEY_SIZE_REQ_TSizeof(void *msg)
{
    DM_HCI_READ_ENCRYPTION_KEY_SIZE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_ENCRYPTION_KEY_SIZE_REQ */

#ifndef EXCLUDE_DM_HCI_READ_ENCRYPTION_KEY_SIZE_REQ
CsrUint8 *DM_HCI_READ_ENCRYPTION_KEY_SIZE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_ENCRYPTION_KEY_SIZE_REQ_T *primitive;

    primitive = (DM_HCI_READ_ENCRYPTION_KEY_SIZE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_ENCRYPTION_KEY_SIZE_REQ */

#ifndef EXCLUDE_DM_HCI_READ_ENCRYPTION_KEY_SIZE_REQ
void *DM_HCI_READ_ENCRYPTION_KEY_SIZE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_ENCRYPTION_KEY_SIZE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_ENCRYPTION_KEY_SIZE_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_ENCRYPTION_KEY_SIZE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_ENCRYPTION_KEY_SIZE_REQ */

#ifndef EXCLUDE_DM_HCI_READ_LOCAL_EXT_FEATURES_REQ
CsrSize DM_HCI_READ_LOCAL_EXT_FEATURES_REQ_TSizeof(void *msg)
{
    DM_HCI_READ_LOCAL_EXT_FEATURES_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_LOCAL_EXT_FEATURES_REQ */

#ifndef EXCLUDE_DM_HCI_READ_LOCAL_EXT_FEATURES_REQ
CsrUint8 *DM_HCI_READ_LOCAL_EXT_FEATURES_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_LOCAL_EXT_FEATURES_REQ_T *primitive;

    primitive = (DM_HCI_READ_LOCAL_EXT_FEATURES_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->page_num);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_LOCAL_EXT_FEATURES_REQ */

#ifndef EXCLUDE_DM_HCI_READ_LOCAL_EXT_FEATURES_REQ
void *DM_HCI_READ_LOCAL_EXT_FEATURES_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_LOCAL_EXT_FEATURES_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_LOCAL_EXT_FEATURES_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_LOCAL_EXT_FEATURES_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->page_num, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_LOCAL_EXT_FEATURES_REQ */

#ifndef EXCLUDE_DM_HCI_PERIODIC_INQUIRY_MODE_CFM
CsrSize DM_HCI_PERIODIC_INQUIRY_MODE_CFM_TSizeof(void *msg)
{
    DM_HCI_PERIODIC_INQUIRY_MODE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_PERIODIC_INQUIRY_MODE_CFM */

#ifndef EXCLUDE_DM_HCI_PERIODIC_INQUIRY_MODE_CFM
CsrUint8 *DM_HCI_PERIODIC_INQUIRY_MODE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_PERIODIC_INQUIRY_MODE_CFM_T *primitive;

    primitive = (DM_HCI_PERIODIC_INQUIRY_MODE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_PERIODIC_INQUIRY_MODE_CFM */

#ifndef EXCLUDE_DM_HCI_PERIODIC_INQUIRY_MODE_CFM
void *DM_HCI_PERIODIC_INQUIRY_MODE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_PERIODIC_INQUIRY_MODE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_PERIODIC_INQUIRY_MODE_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_PERIODIC_INQUIRY_MODE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_PERIODIC_INQUIRY_MODE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_VOICE_SETTING_REQ
CsrSize DM_HCI_READ_VOICE_SETTING_REQ_TSizeof(void *msg)
{
    DM_HCI_READ_VOICE_SETTING_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_VOICE_SETTING_REQ */

#ifndef EXCLUDE_DM_HCI_READ_VOICE_SETTING_REQ
CsrUint8 *DM_HCI_READ_VOICE_SETTING_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_VOICE_SETTING_REQ_T *primitive;

    primitive = (DM_HCI_READ_VOICE_SETTING_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_VOICE_SETTING_REQ */

#ifndef EXCLUDE_DM_HCI_READ_VOICE_SETTING_REQ
void *DM_HCI_READ_VOICE_SETTING_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_VOICE_SETTING_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_VOICE_SETTING_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_VOICE_SETTING_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_VOICE_SETTING_REQ */

#ifndef EXCLUDE_DM_HCI_READ_CONN_ACCEPT_TIMEOUT_CFM
CsrSize DM_HCI_READ_CONN_ACCEPT_TIMEOUT_CFM_TSizeof(void *msg)
{
    DM_HCI_READ_CONN_ACCEPT_TIMEOUT_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_CONN_ACCEPT_TIMEOUT_CFM */

#ifndef EXCLUDE_DM_HCI_READ_CONN_ACCEPT_TIMEOUT_CFM
CsrUint8 *DM_HCI_READ_CONN_ACCEPT_TIMEOUT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_CONN_ACCEPT_TIMEOUT_CFM_T *primitive;

    primitive = (DM_HCI_READ_CONN_ACCEPT_TIMEOUT_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_accept_timeout);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_CONN_ACCEPT_TIMEOUT_CFM */

#ifndef EXCLUDE_DM_HCI_READ_CONN_ACCEPT_TIMEOUT_CFM
void *DM_HCI_READ_CONN_ACCEPT_TIMEOUT_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_CONN_ACCEPT_TIMEOUT_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_CONN_ACCEPT_TIMEOUT_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_CONN_ACCEPT_TIMEOUT_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_accept_timeout, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_CONN_ACCEPT_TIMEOUT_CFM */

#ifndef EXCLUDE_DM_HCI_RESET_FAILED_CONTACT_COUNT_REQ
CsrSize DM_HCI_RESET_FAILED_CONTACT_COUNT_REQ_TSizeof(void *msg)
{
    DM_HCI_RESET_FAILED_CONTACT_COUNT_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_RESET_FAILED_CONTACT_COUNT_REQ */

#ifndef EXCLUDE_DM_HCI_RESET_FAILED_CONTACT_COUNT_REQ
CsrUint8 *DM_HCI_RESET_FAILED_CONTACT_COUNT_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_RESET_FAILED_CONTACT_COUNT_REQ_T *primitive;

    primitive = (DM_HCI_RESET_FAILED_CONTACT_COUNT_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_RESET_FAILED_CONTACT_COUNT_REQ */

#ifndef EXCLUDE_DM_HCI_RESET_FAILED_CONTACT_COUNT_REQ
void *DM_HCI_RESET_FAILED_CONTACT_COUNT_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_RESET_FAILED_CONTACT_COUNT_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_RESET_FAILED_CONTACT_COUNT_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_RESET_FAILED_CONTACT_COUNT_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_RESET_FAILED_CONTACT_COUNT_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_READ_SUPPORTED_STATES_REQ
CsrSize DM_HCI_ULP_READ_SUPPORTED_STATES_REQ_TSizeof(void *msg)
{
    DM_HCI_ULP_READ_SUPPORTED_STATES_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_READ_SUPPORTED_STATES_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_READ_SUPPORTED_STATES_REQ
CsrUint8 *DM_HCI_ULP_READ_SUPPORTED_STATES_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_READ_SUPPORTED_STATES_REQ_T *primitive;

    primitive = (DM_HCI_ULP_READ_SUPPORTED_STATES_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_READ_SUPPORTED_STATES_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_READ_SUPPORTED_STATES_REQ
void *DM_HCI_ULP_READ_SUPPORTED_STATES_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_READ_SUPPORTED_STATES_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_READ_SUPPORTED_STATES_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_READ_SUPPORTED_STATES_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_READ_SUPPORTED_STATES_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_SCO_FLOW_CON_ENABLE_REQ
CsrSize DM_HCI_WRITE_SCO_FLOW_CON_ENABLE_REQ_TSizeof(void *msg)
{
    DM_HCI_WRITE_SCO_FLOW_CON_ENABLE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_SCO_FLOW_CON_ENABLE_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_SCO_FLOW_CON_ENABLE_REQ
CsrUint8 *DM_HCI_WRITE_SCO_FLOW_CON_ENABLE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_SCO_FLOW_CON_ENABLE_REQ_T *primitive;

    primitive = (DM_HCI_WRITE_SCO_FLOW_CON_ENABLE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->sco_flow_control_enable);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_SCO_FLOW_CON_ENABLE_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_SCO_FLOW_CON_ENABLE_REQ
void *DM_HCI_WRITE_SCO_FLOW_CON_ENABLE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_SCO_FLOW_CON_ENABLE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_SCO_FLOW_CON_ENABLE_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_SCO_FLOW_CON_ENABLE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->sco_flow_control_enable, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_SCO_FLOW_CON_ENABLE_REQ */

#ifndef EXCLUDE_DM_HCI_CHANGE_CONN_PKT_TYPE_REQ
CsrSize DM_HCI_CHANGE_CONN_PKT_TYPE_REQ_TSizeof(void *msg)
{
    DM_HCI_CHANGE_CONN_PKT_TYPE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_CHANGE_CONN_PKT_TYPE_REQ */

#ifndef EXCLUDE_DM_HCI_CHANGE_CONN_PKT_TYPE_REQ
CsrUint8 *DM_HCI_CHANGE_CONN_PKT_TYPE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_CHANGE_CONN_PKT_TYPE_REQ_T *primitive;

    primitive = (DM_HCI_CHANGE_CONN_PKT_TYPE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pkt_type);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_CHANGE_CONN_PKT_TYPE_REQ */

#ifndef EXCLUDE_DM_HCI_CHANGE_CONN_PKT_TYPE_REQ
void *DM_HCI_CHANGE_CONN_PKT_TYPE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_CHANGE_CONN_PKT_TYPE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_CHANGE_CONN_PKT_TYPE_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_CHANGE_CONN_PKT_TYPE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pkt_type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_CHANGE_CONN_PKT_TYPE_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_CFM
CsrSize DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_CFM_TSizeof(void *msg)
{
    DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_CFM
CsrUint8 *DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_CFM_T *primitive;

    primitive = (DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_CFM
void *DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_CFM */

#ifndef EXCLUDE_DM_HCI_READ_SCO_FLOW_CON_ENABLE_REQ
CsrSize DM_HCI_READ_SCO_FLOW_CON_ENABLE_REQ_TSizeof(void *msg)
{
    DM_HCI_READ_SCO_FLOW_CON_ENABLE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_SCO_FLOW_CON_ENABLE_REQ */

#ifndef EXCLUDE_DM_HCI_READ_SCO_FLOW_CON_ENABLE_REQ
CsrUint8 *DM_HCI_READ_SCO_FLOW_CON_ENABLE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_SCO_FLOW_CON_ENABLE_REQ_T *primitive;

    primitive = (DM_HCI_READ_SCO_FLOW_CON_ENABLE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_SCO_FLOW_CON_ENABLE_REQ */

#ifndef EXCLUDE_DM_HCI_READ_SCO_FLOW_CON_ENABLE_REQ
void *DM_HCI_READ_SCO_FLOW_CON_ENABLE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_SCO_FLOW_CON_ENABLE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_SCO_FLOW_CON_ENABLE_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_SCO_FLOW_CON_ENABLE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_SCO_FLOW_CON_ENABLE_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_NUM_BCAST_RETXS_REQ
CsrSize DM_HCI_WRITE_NUM_BCAST_RETXS_REQ_TSizeof(void *msg)
{
    DM_HCI_WRITE_NUM_BCAST_RETXS_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_NUM_BCAST_RETXS_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_NUM_BCAST_RETXS_REQ
CsrUint8 *DM_HCI_WRITE_NUM_BCAST_RETXS_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_NUM_BCAST_RETXS_REQ_T *primitive;

    primitive = (DM_HCI_WRITE_NUM_BCAST_RETXS_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->num);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_NUM_BCAST_RETXS_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_NUM_BCAST_RETXS_REQ
void *DM_HCI_WRITE_NUM_BCAST_RETXS_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_NUM_BCAST_RETXS_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_NUM_BCAST_RETXS_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_NUM_BCAST_RETXS_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->num, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_NUM_BCAST_RETXS_REQ */

#ifndef EXCLUDE_DM_SYNC_RENEGOTIATE_CFM
CsrSize DM_SYNC_RENEGOTIATE_CFM_TSizeof(void *msg)
{
    DM_SYNC_RENEGOTIATE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SYNC_RENEGOTIATE_CFM */

#ifndef EXCLUDE_DM_SYNC_RENEGOTIATE_CFM
CsrUint8 *DM_SYNC_RENEGOTIATE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SYNC_RENEGOTIATE_CFM_T *primitive;

    primitive = (DM_SYNC_RENEGOTIATE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pv_cbarg);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tx_interval);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->wesco);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->rx_packet_length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->tx_packet_length);

    return buffer;
}
#endif /* EXCLUDE_DM_SYNC_RENEGOTIATE_CFM */

#ifndef EXCLUDE_DM_SYNC_RENEGOTIATE_CFM
void *DM_SYNC_RENEGOTIATE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SYNC_RENEGOTIATE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SYNC_RENEGOTIATE_CFM_T *) CsrPmemZalloc(sizeof(DM_SYNC_RENEGOTIATE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pv_cbarg, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tx_interval, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->wesco, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->rx_packet_length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->tx_packet_length, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SYNC_RENEGOTIATE_CFM */

#ifndef EXCLUDE_DM_SM_UNREGISTER_CFM
CsrSize DM_SM_UNREGISTER_CFM_TSizeof(void *msg)
{
    DM_SM_UNREGISTER_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_UNREGISTER_CFM */

#ifndef EXCLUDE_DM_SM_UNREGISTER_CFM
CsrUint8 *DM_SM_UNREGISTER_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_UNREGISTER_CFM_T *primitive;

    primitive = (DM_SM_UNREGISTER_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->service.protocol_id);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->service.channel);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_UNREGISTER_CFM */

#ifndef EXCLUDE_DM_SM_UNREGISTER_CFM
void *DM_SM_UNREGISTER_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_UNREGISTER_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_UNREGISTER_CFM_T *) CsrPmemZalloc(sizeof(DM_SM_UNREGISTER_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->service.protocol_id, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->service.channel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_UNREGISTER_CFM */

#ifndef EXCLUDE_DM_HCI_MASTER_LINK_KEY_REQ
CsrSize DM_HCI_MASTER_LINK_KEY_REQ_TSizeof(void *msg)
{
    DM_HCI_MASTER_LINK_KEY_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_MASTER_LINK_KEY_REQ */

#ifndef EXCLUDE_DM_HCI_MASTER_LINK_KEY_REQ
CsrUint8 *DM_HCI_MASTER_LINK_KEY_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_MASTER_LINK_KEY_REQ_T *primitive;

    primitive = (DM_HCI_MASTER_LINK_KEY_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->link_key_type);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_MASTER_LINK_KEY_REQ */

#ifndef EXCLUDE_DM_HCI_MASTER_LINK_KEY_REQ
void *DM_HCI_MASTER_LINK_KEY_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_MASTER_LINK_KEY_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_MASTER_LINK_KEY_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_MASTER_LINK_KEY_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->link_key_type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_MASTER_LINK_KEY_REQ */

#ifndef EXCLUDE_DM_HCI_READ_PAGE_TIMEOUT_CFM
CsrSize DM_HCI_READ_PAGE_TIMEOUT_CFM_TSizeof(void *msg)
{
    DM_HCI_READ_PAGE_TIMEOUT_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_PAGE_TIMEOUT_CFM */

#ifndef EXCLUDE_DM_HCI_READ_PAGE_TIMEOUT_CFM
CsrUint8 *DM_HCI_READ_PAGE_TIMEOUT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_PAGE_TIMEOUT_CFM_T *primitive;

    primitive = (DM_HCI_READ_PAGE_TIMEOUT_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->page_timeout);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_PAGE_TIMEOUT_CFM */

#ifndef EXCLUDE_DM_HCI_READ_PAGE_TIMEOUT_CFM
void *DM_HCI_READ_PAGE_TIMEOUT_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_PAGE_TIMEOUT_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_PAGE_TIMEOUT_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_PAGE_TIMEOUT_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->page_timeout, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_PAGE_TIMEOUT_CFM */

#ifndef EXCLUDE_DM_AMPM_DISCONNECT_CHANNEL_IND
CsrSize DM_AMPM_DISCONNECT_CHANNEL_IND_TSizeof(void *msg)
{
    DM_AMPM_DISCONNECT_CHANNEL_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_AMPM_DISCONNECT_CHANNEL_IND */

#ifndef EXCLUDE_DM_AMPM_DISCONNECT_CHANNEL_IND
CsrUint8 *DM_AMPM_DISCONNECT_CHANNEL_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_AMPM_DISCONNECT_CHANNEL_IND_T *primitive;

    primitive = (DM_AMPM_DISCONNECT_CHANNEL_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->local_amp_id);

    return buffer;
}
#endif /* EXCLUDE_DM_AMPM_DISCONNECT_CHANNEL_IND */

#ifndef EXCLUDE_DM_AMPM_DISCONNECT_CHANNEL_IND
void *DM_AMPM_DISCONNECT_CHANNEL_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_AMPM_DISCONNECT_CHANNEL_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_AMPM_DISCONNECT_CHANNEL_IND_T *) CsrPmemZalloc(sizeof(DM_AMPM_DISCONNECT_CHANNEL_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->local_amp_id, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_AMPM_DISCONNECT_CHANNEL_IND */

#ifndef EXCLUDE_DM_HCI_READ_LINK_POLICY_SETTINGS_CFM
CsrSize DM_HCI_READ_LINK_POLICY_SETTINGS_CFM_TSizeof(void *msg)
{
    DM_HCI_READ_LINK_POLICY_SETTINGS_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_LINK_POLICY_SETTINGS_CFM */

#ifndef EXCLUDE_DM_HCI_READ_LINK_POLICY_SETTINGS_CFM
CsrUint8 *DM_HCI_READ_LINK_POLICY_SETTINGS_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_LINK_POLICY_SETTINGS_CFM_T *primitive;

    primitive = (DM_HCI_READ_LINK_POLICY_SETTINGS_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->link_policy);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_LINK_POLICY_SETTINGS_CFM */

#ifndef EXCLUDE_DM_HCI_READ_LINK_POLICY_SETTINGS_CFM
void *DM_HCI_READ_LINK_POLICY_SETTINGS_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_LINK_POLICY_SETTINGS_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_LINK_POLICY_SETTINGS_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_LINK_POLICY_SETTINGS_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->link_policy, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_LINK_POLICY_SETTINGS_CFM */

#ifndef EXCLUDE_DM_HCI_READ_PAGE_TIMEOUT_REQ
CsrSize DM_HCI_READ_PAGE_TIMEOUT_REQ_TSizeof(void *msg)
{
    DM_HCI_READ_PAGE_TIMEOUT_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_PAGE_TIMEOUT_REQ */

#ifndef EXCLUDE_DM_HCI_READ_PAGE_TIMEOUT_REQ
CsrUint8 *DM_HCI_READ_PAGE_TIMEOUT_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_PAGE_TIMEOUT_REQ_T *primitive;

    primitive = (DM_HCI_READ_PAGE_TIMEOUT_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_PAGE_TIMEOUT_REQ */

#ifndef EXCLUDE_DM_HCI_READ_PAGE_TIMEOUT_REQ
void *DM_HCI_READ_PAGE_TIMEOUT_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_PAGE_TIMEOUT_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_PAGE_TIMEOUT_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_PAGE_TIMEOUT_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_PAGE_TIMEOUT_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_INQUIRY_MODE_CFM
CsrSize DM_HCI_WRITE_INQUIRY_MODE_CFM_TSizeof(void *msg)
{
    DM_HCI_WRITE_INQUIRY_MODE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_INQUIRY_MODE_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_INQUIRY_MODE_CFM
CsrUint8 *DM_HCI_WRITE_INQUIRY_MODE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_INQUIRY_MODE_CFM_T *primitive;

    primitive = (DM_HCI_WRITE_INQUIRY_MODE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_INQUIRY_MODE_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_INQUIRY_MODE_CFM
void *DM_HCI_WRITE_INQUIRY_MODE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_INQUIRY_MODE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_INQUIRY_MODE_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_INQUIRY_MODE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_INQUIRY_MODE_CFM */

#ifndef EXCLUDE_DM_HCI_SNIFF_MODE_REQ
CsrSize DM_HCI_SNIFF_MODE_REQ_TSizeof(void *msg)
{
    DM_HCI_SNIFF_MODE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_SNIFF_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_SNIFF_MODE_REQ
CsrUint8 *DM_HCI_SNIFF_MODE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_SNIFF_MODE_REQ_T *primitive;

    primitive = (DM_HCI_SNIFF_MODE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->max_interval);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->min_interval);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->attempt);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->timeout);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_SNIFF_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_SNIFF_MODE_REQ
void *DM_HCI_SNIFF_MODE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_SNIFF_MODE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_SNIFF_MODE_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_SNIFF_MODE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->max_interval, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->min_interval, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->attempt, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->timeout, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_SNIFF_MODE_REQ */

#ifndef EXCLUDE_DM_SYNC_REGISTER_CFM
CsrSize DM_SYNC_REGISTER_CFM_TSizeof(void *msg)
{
    DM_SYNC_REGISTER_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SYNC_REGISTER_CFM */

#ifndef EXCLUDE_DM_SYNC_REGISTER_CFM
CsrUint8 *DM_SYNC_REGISTER_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SYNC_REGISTER_CFM_T *primitive;

    primitive = (DM_SYNC_REGISTER_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pv_cbarg);

    return buffer;
}
#endif /* EXCLUDE_DM_SYNC_REGISTER_CFM */

#ifndef EXCLUDE_DM_SYNC_REGISTER_CFM
void *DM_SYNC_REGISTER_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SYNC_REGISTER_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SYNC_REGISTER_CFM_T *) CsrPmemZalloc(sizeof(DM_SYNC_REGISTER_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pv_cbarg, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SYNC_REGISTER_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_CFM
CsrSize DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_CFM_TSizeof(void *msg)
{
    DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_CFM
CsrUint8 *DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_CFM_T *primitive;

    primitive = (DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_CFM
void *DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_CFM */

#ifndef EXCLUDE_DM_SM_USER_PASSKEY_REQUEST_IND
CsrSize DM_SM_USER_PASSKEY_REQUEST_IND_TSizeof(void *msg)
{
    DM_SM_USER_PASSKEY_REQUEST_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_USER_PASSKEY_REQUEST_IND */

#ifndef EXCLUDE_DM_SM_USER_PASSKEY_REQUEST_IND
CsrUint8 *DM_SM_USER_PASSKEY_REQUEST_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_USER_PASSKEY_REQUEST_IND_T *primitive;

    primitive = (DM_SM_USER_PASSKEY_REQUEST_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->dev_class);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_USER_PASSKEY_REQUEST_IND */

#ifndef EXCLUDE_DM_SM_USER_PASSKEY_REQUEST_IND
void *DM_SM_USER_PASSKEY_REQUEST_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_USER_PASSKEY_REQUEST_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_USER_PASSKEY_REQUEST_IND_T *) CsrPmemZalloc(sizeof(DM_SM_USER_PASSKEY_REQUEST_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->dev_class, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_USER_PASSKEY_REQUEST_IND */

#ifndef EXCLUDE_DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_REQ
CsrSize DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_REQ_TSizeof(void *msg)
{
    DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_REQ
CsrUint8 *DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_REQ_T *primitive;

    primitive = (DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->timeout);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_REQ
void *DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->timeout, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_CFM
CsrSize DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_CFM_TSizeof(void *msg)
{
    DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_CFM
CsrUint8 *DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_CFM_T *primitive;

    primitive = (DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_CFM
void *DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_CFM */

#ifndef EXCLUDE_DM_SYNC_UNREGISTER_REQ
CsrSize DM_SYNC_UNREGISTER_REQ_TSizeof(void *msg)
{
    DM_SYNC_UNREGISTER_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SYNC_UNREGISTER_REQ */

#ifndef EXCLUDE_DM_SYNC_UNREGISTER_REQ
CsrUint8 *DM_SYNC_UNREGISTER_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SYNC_UNREGISTER_REQ_T *primitive;

    primitive = (DM_SYNC_UNREGISTER_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pv_cbarg);

    return buffer;
}
#endif /* EXCLUDE_DM_SYNC_UNREGISTER_REQ */

#ifndef EXCLUDE_DM_SYNC_UNREGISTER_REQ
void *DM_SYNC_UNREGISTER_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SYNC_UNREGISTER_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SYNC_UNREGISTER_REQ_T *) CsrPmemZalloc(sizeof(DM_SYNC_UNREGISTER_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pv_cbarg, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SYNC_UNREGISTER_REQ */

#ifndef EXCLUDE_DM_HCI_READ_ENC_MODE_CFM
CsrSize DM_HCI_READ_ENC_MODE_CFM_TSizeof(void *msg)
{
    DM_HCI_READ_ENC_MODE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_ENC_MODE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_ENC_MODE_CFM
CsrUint8 *DM_HCI_READ_ENC_MODE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_ENC_MODE_CFM_T *primitive;

    primitive = (DM_HCI_READ_ENC_MODE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mode);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_ENC_MODE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_ENC_MODE_CFM
void *DM_HCI_READ_ENC_MODE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_ENC_MODE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_ENC_MODE_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_ENC_MODE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_ENC_MODE_CFM */

#ifndef EXCLUDE_DM_SM_ACCESS_REQ
CsrSize DM_SM_ACCESS_REQ_TSizeof(void *msg)
{
    DM_SM_ACCESS_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_ACCESS_REQ */

#ifndef EXCLUDE_DM_SM_ACCESS_REQ
CsrUint8 *DM_SM_ACCESS_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_ACCESS_REQ_T *primitive;

    primitive = (DM_SM_ACCESS_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_setup.connection.service.protocol_id);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_setup.connection.service.channel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->conn_setup.connection.addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->conn_setup.connection.addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->conn_setup.connection.addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_setup.connection.addrt.addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->conn_setup.incoming);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->context);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_ACCESS_REQ */

#ifndef EXCLUDE_DM_SM_ACCESS_REQ
void *DM_SM_ACCESS_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_ACCESS_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_ACCESS_REQ_T *) CsrPmemZalloc(sizeof(DM_SM_ACCESS_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_setup.connection.service.protocol_id, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_setup.connection.service.channel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->conn_setup.connection.addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->conn_setup.connection.addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->conn_setup.connection.addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_setup.connection.addrt.addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->conn_setup.incoming, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->context, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_ACCESS_REQ */

#ifndef EXCLUDE_DM_HCI_DELETE_STORED_LINK_KEY_CFM
CsrSize DM_HCI_DELETE_STORED_LINK_KEY_CFM_TSizeof(void *msg)
{
    DM_HCI_DELETE_STORED_LINK_KEY_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_DELETE_STORED_LINK_KEY_CFM */

#ifndef EXCLUDE_DM_HCI_DELETE_STORED_LINK_KEY_CFM
CsrUint8 *DM_HCI_DELETE_STORED_LINK_KEY_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_DELETE_STORED_LINK_KEY_CFM_T *primitive;

    primitive = (DM_HCI_DELETE_STORED_LINK_KEY_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->num_keys_deleted);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_DELETE_STORED_LINK_KEY_CFM */

#ifndef EXCLUDE_DM_HCI_DELETE_STORED_LINK_KEY_CFM
void *DM_HCI_DELETE_STORED_LINK_KEY_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_DELETE_STORED_LINK_KEY_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_DELETE_STORED_LINK_KEY_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_DELETE_STORED_LINK_KEY_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->num_keys_deleted, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_DELETE_STORED_LINK_KEY_CFM */

#ifndef EXCLUDE_DM_HCI_ENABLE_DUT_MODE_CFM
CsrSize DM_HCI_ENABLE_DUT_MODE_CFM_TSizeof(void *msg)
{
    DM_HCI_ENABLE_DUT_MODE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ENABLE_DUT_MODE_CFM */

#ifndef EXCLUDE_DM_HCI_ENABLE_DUT_MODE_CFM
CsrUint8 *DM_HCI_ENABLE_DUT_MODE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ENABLE_DUT_MODE_CFM_T *primitive;

    primitive = (DM_HCI_ENABLE_DUT_MODE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ENABLE_DUT_MODE_CFM */

#ifndef EXCLUDE_DM_HCI_ENABLE_DUT_MODE_CFM
void *DM_HCI_ENABLE_DUT_MODE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ENABLE_DUT_MODE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ENABLE_DUT_MODE_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_ENABLE_DUT_MODE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ENABLE_DUT_MODE_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_CFM
CsrSize DM_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_CFM_TSizeof(void *msg)
{
    DM_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_CFM
CsrUint8 *DM_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_CFM_T *primitive;

    primitive = (DM_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_CFM
void *DM_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_CFM */

#ifndef EXCLUDE_DM_HCI_READ_BD_ADDR_CFM
CsrSize DM_HCI_READ_BD_ADDR_CFM_TSizeof(void *msg)
{
    DM_HCI_READ_BD_ADDR_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_BD_ADDR_CFM */

#ifndef EXCLUDE_DM_HCI_READ_BD_ADDR_CFM
CsrUint8 *DM_HCI_READ_BD_ADDR_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_BD_ADDR_CFM_T *primitive;

    primitive = (DM_HCI_READ_BD_ADDR_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_BD_ADDR_CFM */

#ifndef EXCLUDE_DM_HCI_READ_BD_ADDR_CFM
void *DM_HCI_READ_BD_ADDR_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_BD_ADDR_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_BD_ADDR_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_BD_ADDR_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_BD_ADDR_CFM */

#ifndef EXCLUDE_DM_HCI_READ_NUM_BCAST_RETXS_REQ
CsrSize DM_HCI_READ_NUM_BCAST_RETXS_REQ_TSizeof(void *msg)
{
    DM_HCI_READ_NUM_BCAST_RETXS_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_NUM_BCAST_RETXS_REQ */

#ifndef EXCLUDE_DM_HCI_READ_NUM_BCAST_RETXS_REQ
CsrUint8 *DM_HCI_READ_NUM_BCAST_RETXS_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_NUM_BCAST_RETXS_REQ_T *primitive;

    primitive = (DM_HCI_READ_NUM_BCAST_RETXS_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_NUM_BCAST_RETXS_REQ */

#ifndef EXCLUDE_DM_HCI_READ_NUM_BCAST_RETXS_REQ
void *DM_HCI_READ_NUM_BCAST_RETXS_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_NUM_BCAST_RETXS_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_NUM_BCAST_RETXS_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_NUM_BCAST_RETXS_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_NUM_BCAST_RETXS_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_VOICE_SETTING_CFM
CsrSize DM_HCI_WRITE_VOICE_SETTING_CFM_TSizeof(void *msg)
{
    DM_HCI_WRITE_VOICE_SETTING_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_VOICE_SETTING_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_VOICE_SETTING_CFM
CsrUint8 *DM_HCI_WRITE_VOICE_SETTING_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_VOICE_SETTING_CFM_T *primitive;

    primitive = (DM_HCI_WRITE_VOICE_SETTING_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_VOICE_SETTING_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_VOICE_SETTING_CFM
void *DM_HCI_WRITE_VOICE_SETTING_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_VOICE_SETTING_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_VOICE_SETTING_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_VOICE_SETTING_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_VOICE_SETTING_CFM */

#ifndef EXCLUDE_DM_SM_UNREGISTER_OUTGOING_REQ
CsrSize DM_SM_UNREGISTER_OUTGOING_REQ_TSizeof(void *msg)
{
    DM_SM_UNREGISTER_OUTGOING_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_UNREGISTER_OUTGOING_REQ */

#ifndef EXCLUDE_DM_SM_UNREGISTER_OUTGOING_REQ
CsrUint8 *DM_SM_UNREGISTER_OUTGOING_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_UNREGISTER_OUTGOING_REQ_T *primitive;

    primitive = (DM_SM_UNREGISTER_OUTGOING_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->connection.service.protocol_id);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->connection.service.channel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connection.addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connection.addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connection.addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->connection.addrt.addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_UNREGISTER_OUTGOING_REQ */

#ifndef EXCLUDE_DM_SM_UNREGISTER_OUTGOING_REQ
void *DM_SM_UNREGISTER_OUTGOING_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_UNREGISTER_OUTGOING_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_UNREGISTER_OUTGOING_REQ_T *) CsrPmemZalloc(sizeof(DM_SM_UNREGISTER_OUTGOING_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->connection.service.protocol_id, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->connection.service.channel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connection.addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connection.addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connection.addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->connection.addrt.addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_UNREGISTER_OUTGOING_REQ */

#ifndef EXCLUDE_DM_SM_USER_CONFIRMATION_REQUEST_IND
CsrSize DM_SM_USER_CONFIRMATION_REQUEST_IND_TSizeof(void *msg)
{
    DM_SM_USER_CONFIRMATION_REQUEST_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_USER_CONFIRMATION_REQUEST_IND */

#ifndef EXCLUDE_DM_SM_USER_CONFIRMATION_REQUEST_IND
CsrUint8 *DM_SM_USER_CONFIRMATION_REQUEST_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_USER_CONFIRMATION_REQUEST_IND_T *primitive;

    primitive = (DM_SM_USER_CONFIRMATION_REQUEST_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->dev_class);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->numeric_value);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->io_cap_local);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->io_cap_remote);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_USER_CONFIRMATION_REQUEST_IND */

#ifndef EXCLUDE_DM_SM_USER_CONFIRMATION_REQUEST_IND
void *DM_SM_USER_CONFIRMATION_REQUEST_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_USER_CONFIRMATION_REQUEST_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_USER_CONFIRMATION_REQUEST_IND_T *) CsrPmemZalloc(sizeof(DM_SM_USER_CONFIRMATION_REQUEST_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->dev_class, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->numeric_value, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->io_cap_local, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->io_cap_remote, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_USER_CONFIRMATION_REQUEST_IND */

#ifndef EXCLUDE_DM_HCI_EXIT_PERIODIC_INQUIRY_MODE_CFM
CsrSize DM_HCI_EXIT_PERIODIC_INQUIRY_MODE_CFM_TSizeof(void *msg)
{
    DM_HCI_EXIT_PERIODIC_INQUIRY_MODE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_EXIT_PERIODIC_INQUIRY_MODE_CFM */

#ifndef EXCLUDE_DM_HCI_EXIT_PERIODIC_INQUIRY_MODE_CFM
CsrUint8 *DM_HCI_EXIT_PERIODIC_INQUIRY_MODE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_EXIT_PERIODIC_INQUIRY_MODE_CFM_T *primitive;

    primitive = (DM_HCI_EXIT_PERIODIC_INQUIRY_MODE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_EXIT_PERIODIC_INQUIRY_MODE_CFM */

#ifndef EXCLUDE_DM_HCI_EXIT_PERIODIC_INQUIRY_MODE_CFM
void *DM_HCI_EXIT_PERIODIC_INQUIRY_MODE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_EXIT_PERIODIC_INQUIRY_MODE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_EXIT_PERIODIC_INQUIRY_MODE_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_EXIT_PERIODIC_INQUIRY_MODE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_EXIT_PERIODIC_INQUIRY_MODE_CFM */

#ifndef EXCLUDE_DM_ACL_CLOSED_IND
CsrSize DM_ACL_CLOSED_IND_TSizeof(void *msg)
{
    DM_ACL_CLOSED_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_ACL_CLOSED_IND */

#ifndef EXCLUDE_DM_ACL_CLOSED_IND
CsrUint8 *DM_ACL_CLOSED_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_ACL_CLOSED_IND_T *primitive;

    primitive = (DM_ACL_CLOSED_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->reason);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);

    return buffer;
}
#endif /* EXCLUDE_DM_ACL_CLOSED_IND */

#ifndef EXCLUDE_DM_ACL_CLOSED_IND
void *DM_ACL_CLOSED_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_ACL_CLOSED_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_ACL_CLOSED_IND_T *) CsrPmemZalloc(sizeof(DM_ACL_CLOSED_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->reason, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_ACL_CLOSED_IND */

#ifndef EXCLUDE_DM_AMPM_DISCONNECT_CHANNEL_RSP
CsrSize DM_AMPM_DISCONNECT_CHANNEL_RSP_TSizeof(void *msg)
{
    DM_AMPM_DISCONNECT_CHANNEL_RSP_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_AMPM_DISCONNECT_CHANNEL_RSP */

#ifndef EXCLUDE_DM_AMPM_DISCONNECT_CHANNEL_RSP
CsrUint8 *DM_AMPM_DISCONNECT_CHANNEL_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_AMPM_DISCONNECT_CHANNEL_RSP_T *primitive;

    primitive = (DM_AMPM_DISCONNECT_CHANNEL_RSP_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->local_amp_id);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_AMPM_DISCONNECT_CHANNEL_RSP */

#ifndef EXCLUDE_DM_AMPM_DISCONNECT_CHANNEL_RSP
void *DM_AMPM_DISCONNECT_CHANNEL_RSP_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_AMPM_DISCONNECT_CHANNEL_RSP_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_AMPM_DISCONNECT_CHANNEL_RSP_T *) CsrPmemZalloc(sizeof(DM_AMPM_DISCONNECT_CHANNEL_RSP_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->local_amp_id, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_AMPM_DISCONNECT_CHANNEL_RSP */

#ifndef EXCLUDE_DM_HCI_WRITE_STORED_LINK_KEY_CFM
CsrSize DM_HCI_WRITE_STORED_LINK_KEY_CFM_TSizeof(void *msg)
{
    DM_HCI_WRITE_STORED_LINK_KEY_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_STORED_LINK_KEY_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_STORED_LINK_KEY_CFM
CsrUint8 *DM_HCI_WRITE_STORED_LINK_KEY_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_STORED_LINK_KEY_CFM_T *primitive;

    primitive = (DM_HCI_WRITE_STORED_LINK_KEY_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->num_keys_written);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_STORED_LINK_KEY_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_STORED_LINK_KEY_CFM
void *DM_HCI_WRITE_STORED_LINK_KEY_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_STORED_LINK_KEY_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_STORED_LINK_KEY_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_STORED_LINK_KEY_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->num_keys_written, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_STORED_LINK_KEY_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_SCO_FLOW_CON_ENABLE_CFM
CsrSize DM_HCI_WRITE_SCO_FLOW_CON_ENABLE_CFM_TSizeof(void *msg)
{
    DM_HCI_WRITE_SCO_FLOW_CON_ENABLE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_SCO_FLOW_CON_ENABLE_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_SCO_FLOW_CON_ENABLE_CFM
CsrUint8 *DM_HCI_WRITE_SCO_FLOW_CON_ENABLE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_SCO_FLOW_CON_ENABLE_CFM_T *primitive;

    primitive = (DM_HCI_WRITE_SCO_FLOW_CON_ENABLE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_SCO_FLOW_CON_ENABLE_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_SCO_FLOW_CON_ENABLE_CFM
void *DM_HCI_WRITE_SCO_FLOW_CON_ENABLE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_SCO_FLOW_CON_ENABLE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_SCO_FLOW_CON_ENABLE_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_SCO_FLOW_CON_ENABLE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_SCO_FLOW_CON_ENABLE_CFM */

#ifndef EXCLUDE_DM_SET_BLE_CONNECTION_PARAMETERS_REQ
CsrSize DM_SET_BLE_CONNECTION_PARAMETERS_REQ_TSizeof(void *msg)
{
    DM_SET_BLE_CONNECTION_PARAMETERS_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SET_BLE_CONNECTION_PARAMETERS_REQ */

#ifndef EXCLUDE_DM_SET_BLE_CONNECTION_PARAMETERS_REQ
CsrUint8 *DM_SET_BLE_CONNECTION_PARAMETERS_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SET_BLE_CONNECTION_PARAMETERS_REQ_T *primitive;

    primitive = (DM_SET_BLE_CONNECTION_PARAMETERS_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->scan_interval);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->scan_window);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_interval_min);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_interval_max);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_latency);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->supervision_timeout);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_attempt_timeout);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->adv_interval_min);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->adv_interval_max);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_latency_max);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->supervision_timeout_min);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->supervision_timeout_max);

    return buffer;
}
#endif /* EXCLUDE_DM_SET_BLE_CONNECTION_PARAMETERS_REQ */

#ifndef EXCLUDE_DM_SET_BLE_CONNECTION_PARAMETERS_REQ
void *DM_SET_BLE_CONNECTION_PARAMETERS_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SET_BLE_CONNECTION_PARAMETERS_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SET_BLE_CONNECTION_PARAMETERS_REQ_T *) CsrPmemZalloc(sizeof(DM_SET_BLE_CONNECTION_PARAMETERS_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->scan_interval, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->scan_window, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_interval_min, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_interval_max, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_latency, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->supervision_timeout, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_attempt_timeout, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->adv_interval_min, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->adv_interval_max, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_latency_max, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->supervision_timeout_min, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->supervision_timeout_max, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SET_BLE_CONNECTION_PARAMETERS_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_REQ
CsrSize DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_REQ_TSizeof(void *msg)
{
    DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_REQ
CsrUint8 *DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_REQ_T *primitive;

    primitive = (DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mode);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_REQ
void *DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_REQ */

#ifndef EXCLUDE_DM_WRITE_CACHED_PAGE_MODE_REQ
CsrSize DM_WRITE_CACHED_PAGE_MODE_REQ_TSizeof(void *msg)
{
    DM_WRITE_CACHED_PAGE_MODE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_WRITE_CACHED_PAGE_MODE_REQ */

#ifndef EXCLUDE_DM_WRITE_CACHED_PAGE_MODE_REQ
CsrUint8 *DM_WRITE_CACHED_PAGE_MODE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_WRITE_CACHED_PAGE_MODE_REQ_T *primitive;

    primitive = (DM_WRITE_CACHED_PAGE_MODE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->page_scan_mode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->page_scan_rep_mode);

    return buffer;
}
#endif /* EXCLUDE_DM_WRITE_CACHED_PAGE_MODE_REQ */

#ifndef EXCLUDE_DM_WRITE_CACHED_PAGE_MODE_REQ
void *DM_WRITE_CACHED_PAGE_MODE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_WRITE_CACHED_PAGE_MODE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_WRITE_CACHED_PAGE_MODE_REQ_T *) CsrPmemZalloc(sizeof(DM_WRITE_CACHED_PAGE_MODE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->page_scan_mode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->page_scan_rep_mode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_WRITE_CACHED_PAGE_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_EXIT_PARK_MODE_REQ
CsrSize DM_HCI_EXIT_PARK_MODE_REQ_TSizeof(void *msg)
{
    DM_HCI_EXIT_PARK_MODE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_EXIT_PARK_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_EXIT_PARK_MODE_REQ
CsrUint8 *DM_HCI_EXIT_PARK_MODE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_EXIT_PARK_MODE_REQ_T *primitive;

    primitive = (DM_HCI_EXIT_PARK_MODE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_EXIT_PARK_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_EXIT_PARK_MODE_REQ
void *DM_HCI_EXIT_PARK_MODE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_EXIT_PARK_MODE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_EXIT_PARK_MODE_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_EXIT_PARK_MODE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_EXIT_PARK_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_REMOTE_NAME_REQUEST_REQ
CsrSize DM_HCI_REMOTE_NAME_REQUEST_REQ_TSizeof(void *msg)
{
    DM_HCI_REMOTE_NAME_REQUEST_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_REMOTE_NAME_REQUEST_REQ */

#ifndef EXCLUDE_DM_HCI_REMOTE_NAME_REQUEST_REQ
CsrUint8 *DM_HCI_REMOTE_NAME_REQUEST_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_REMOTE_NAME_REQUEST_REQ_T *primitive;

    primitive = (DM_HCI_REMOTE_NAME_REQUEST_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->page_scan_rep_mode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->page_scan_mode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->clock_offset);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_REMOTE_NAME_REQUEST_REQ */

#ifndef EXCLUDE_DM_HCI_REMOTE_NAME_REQUEST_REQ
void *DM_HCI_REMOTE_NAME_REQUEST_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_REMOTE_NAME_REQUEST_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_REMOTE_NAME_REQUEST_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_REMOTE_NAME_REQUEST_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->page_scan_rep_mode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->page_scan_mode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->clock_offset, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_REMOTE_NAME_REQUEST_REQ */

#ifndef EXCLUDE_DM_SM_REGISTER_OUTGOING_CFM
CsrSize DM_SM_REGISTER_OUTGOING_CFM_TSizeof(void *msg)
{
    DM_SM_REGISTER_OUTGOING_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_REGISTER_OUTGOING_CFM */

#ifndef EXCLUDE_DM_SM_REGISTER_OUTGOING_CFM
CsrUint8 *DM_SM_REGISTER_OUTGOING_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_REGISTER_OUTGOING_CFM_T *primitive;

    primitive = (DM_SM_REGISTER_OUTGOING_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->connection.service.protocol_id);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->connection.service.channel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connection.addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connection.addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connection.addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->connection.addrt.addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->security_level);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_REGISTER_OUTGOING_CFM */

#ifndef EXCLUDE_DM_SM_REGISTER_OUTGOING_CFM
void *DM_SM_REGISTER_OUTGOING_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_REGISTER_OUTGOING_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_REGISTER_OUTGOING_CFM_T *) CsrPmemZalloc(sizeof(DM_SM_REGISTER_OUTGOING_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->connection.service.protocol_id, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->connection.service.channel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connection.addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connection.addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connection.addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->connection.addrt.addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->security_level, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_REGISTER_OUTGOING_CFM */

#ifndef EXCLUDE_DM_SM_ENCRYPT_CFM
CsrSize DM_SM_ENCRYPT_CFM_TSizeof(void *msg)
{
    DM_SM_ENCRYPT_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_ENCRYPT_CFM */

#ifndef EXCLUDE_DM_SM_ENCRYPT_CFM
CsrUint8 *DM_SM_ENCRYPT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_ENCRYPT_CFM_T *primitive;

    primitive = (DM_SM_ENCRYPT_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->success);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->encrypted);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_ENCRYPT_CFM */

#ifndef EXCLUDE_DM_SM_ENCRYPT_CFM
void *DM_SM_ENCRYPT_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_ENCRYPT_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_ENCRYPT_CFM_T *) CsrPmemZalloc(sizeof(DM_SM_ENCRYPT_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->success, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->encrypted, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_ENCRYPT_CFM */

#ifndef EXCLUDE_DM_ACL_OPENED_IND
CsrSize DM_ACL_OPENED_IND_TSizeof(void *msg)
{
    DM_ACL_OPENED_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_ACL_OPENED_IND */

#ifndef EXCLUDE_DM_ACL_OPENED_IND
CsrUint8 *DM_ACL_OPENED_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_ACL_OPENED_IND_T *primitive;

    primitive = (DM_ACL_OPENED_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->dev_class);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ble_conn_params.conn_interval);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ble_conn_params.conn_latency);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ble_conn_params.supervision_timeout);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->ble_conn_params.clock_accuracy);

    return buffer;
}
#endif /* EXCLUDE_DM_ACL_OPENED_IND */

#ifndef EXCLUDE_DM_ACL_OPENED_IND
void *DM_ACL_OPENED_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_ACL_OPENED_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_ACL_OPENED_IND_T *) CsrPmemZalloc(sizeof(DM_ACL_OPENED_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->dev_class, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->ble_conn_params.conn_interval, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->ble_conn_params.conn_latency, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->ble_conn_params.supervision_timeout, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->ble_conn_params.clock_accuracy, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_ACL_OPENED_IND */

#ifndef EXCLUDE_DM_HCI_FLOW_SPEC_CFM
CsrSize DM_HCI_FLOW_SPEC_CFM_TSizeof(void *msg)
{
    DM_HCI_FLOW_SPEC_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_FLOW_SPEC_CFM */

#ifndef EXCLUDE_DM_HCI_FLOW_SPEC_CFM
CsrUint8 *DM_HCI_FLOW_SPEC_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_FLOW_SPEC_CFM_T *primitive;

    primitive = (DM_HCI_FLOW_SPEC_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->flags);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->flow_direction);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->service_type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->token_rate);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->token_bucket_size);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->peak_bandwidth);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->access_latency);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_FLOW_SPEC_CFM */

#ifndef EXCLUDE_DM_HCI_FLOW_SPEC_CFM
void *DM_HCI_FLOW_SPEC_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_FLOW_SPEC_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_FLOW_SPEC_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_FLOW_SPEC_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->flags, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->flow_direction, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->service_type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->token_rate, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->token_bucket_size, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->peak_bandwidth, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->access_latency, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_FLOW_SPEC_CFM */

#ifndef EXCLUDE_DM_HCI_ENABLE_DUT_MODE_REQ
CsrSize DM_HCI_ENABLE_DUT_MODE_REQ_TSizeof(void *msg)
{
    DM_HCI_ENABLE_DUT_MODE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ENABLE_DUT_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_ENABLE_DUT_MODE_REQ
CsrUint8 *DM_HCI_ENABLE_DUT_MODE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ENABLE_DUT_MODE_REQ_T *primitive;

    primitive = (DM_HCI_ENABLE_DUT_MODE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ENABLE_DUT_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_ENABLE_DUT_MODE_REQ
void *DM_HCI_ENABLE_DUT_MODE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ENABLE_DUT_MODE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ENABLE_DUT_MODE_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_ENABLE_DUT_MODE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ENABLE_DUT_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_CREATE_CONNECTION_CANCEL_CFM
CsrSize DM_HCI_ULP_CREATE_CONNECTION_CANCEL_CFM_TSizeof(void *msg)
{
    DM_HCI_ULP_CREATE_CONNECTION_CANCEL_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_CREATE_CONNECTION_CANCEL_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_CREATE_CONNECTION_CANCEL_CFM
CsrUint8 *DM_HCI_ULP_CREATE_CONNECTION_CANCEL_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_CREATE_CONNECTION_CANCEL_CFM_T *primitive;

    primitive = (DM_HCI_ULP_CREATE_CONNECTION_CANCEL_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_CREATE_CONNECTION_CANCEL_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_CREATE_CONNECTION_CANCEL_CFM
void *DM_HCI_ULP_CREATE_CONNECTION_CANCEL_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_CREATE_CONNECTION_CANCEL_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_CREATE_CONNECTION_CANCEL_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_CREATE_CONNECTION_CANCEL_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_CREATE_CONNECTION_CANCEL_CFM */

#ifndef EXCLUDE_DM_HCI_READ_INQUIRY_SCAN_TYPE_CFM
CsrSize DM_HCI_READ_INQUIRY_SCAN_TYPE_CFM_TSizeof(void *msg)
{
    DM_HCI_READ_INQUIRY_SCAN_TYPE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_INQUIRY_SCAN_TYPE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_INQUIRY_SCAN_TYPE_CFM
CsrUint8 *DM_HCI_READ_INQUIRY_SCAN_TYPE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_INQUIRY_SCAN_TYPE_CFM_T *primitive;

    primitive = (DM_HCI_READ_INQUIRY_SCAN_TYPE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mode);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_INQUIRY_SCAN_TYPE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_INQUIRY_SCAN_TYPE_CFM
void *DM_HCI_READ_INQUIRY_SCAN_TYPE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_INQUIRY_SCAN_TYPE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_INQUIRY_SCAN_TYPE_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_INQUIRY_SCAN_TYPE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_INQUIRY_SCAN_TYPE_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_REQ
CsrSize DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_REQ_TSizeof(void *msg)
{
    DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_REQ
CsrUint8 *DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_REQ_T *primitive;

    primitive = (DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->adv_interval_min);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->adv_interval_max);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->advertising_type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->own_address_type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->direct_address_type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->direct_address.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->direct_address.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->direct_address.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->advertising_channel_map);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->advertising_filter_policy);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_REQ
void *DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->adv_interval_min, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->adv_interval_max, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->advertising_type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->own_address_type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->direct_address_type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->direct_address.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->direct_address.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->direct_address.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->advertising_channel_map, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->advertising_filter_policy, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_CFM
CsrSize DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_CFM_TSizeof(void *msg)
{
    DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_CFM
CsrUint8 *DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_CFM_T *primitive;

    primitive = (DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_CFM
void *DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_CFM */

#ifndef EXCLUDE_DM_HCI_READ_REMOTE_VER_INFO_CFM
CsrSize DM_HCI_READ_REMOTE_VER_INFO_CFM_TSizeof(void *msg)
{
    DM_HCI_READ_REMOTE_VER_INFO_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_REMOTE_VER_INFO_CFM */

#ifndef EXCLUDE_DM_HCI_READ_REMOTE_VER_INFO_CFM
CsrUint8 *DM_HCI_READ_REMOTE_VER_INFO_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_REMOTE_VER_INFO_CFM_T *primitive;

    primitive = (DM_HCI_READ_REMOTE_VER_INFO_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->LMP_version);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->manufacturer_name);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->LMP_subversion);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_REMOTE_VER_INFO_CFM */

#ifndef EXCLUDE_DM_HCI_READ_REMOTE_VER_INFO_CFM
void *DM_HCI_READ_REMOTE_VER_INFO_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_REMOTE_VER_INFO_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_REMOTE_VER_INFO_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_REMOTE_VER_INFO_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->LMP_version, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->manufacturer_name, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->LMP_subversion, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_REMOTE_VER_INFO_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_HOLD_MODE_ACTIVITY_CFM
CsrSize DM_HCI_WRITE_HOLD_MODE_ACTIVITY_CFM_TSizeof(void *msg)
{
    DM_HCI_WRITE_HOLD_MODE_ACTIVITY_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_HOLD_MODE_ACTIVITY_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_HOLD_MODE_ACTIVITY_CFM
CsrUint8 *DM_HCI_WRITE_HOLD_MODE_ACTIVITY_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_HOLD_MODE_ACTIVITY_CFM_T *primitive;

    primitive = (DM_HCI_WRITE_HOLD_MODE_ACTIVITY_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_HOLD_MODE_ACTIVITY_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_HOLD_MODE_ACTIVITY_CFM
void *DM_HCI_WRITE_HOLD_MODE_ACTIVITY_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_HOLD_MODE_ACTIVITY_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_HOLD_MODE_ACTIVITY_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_HOLD_MODE_ACTIVITY_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_HOLD_MODE_ACTIVITY_CFM */

#ifndef EXCLUDE_DM_SM_BONDING_CFM
CsrSize DM_SM_BONDING_CFM_TSizeof(void *msg)
{
    DM_SM_BONDING_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_BONDING_CFM */

#ifndef EXCLUDE_DM_SM_BONDING_CFM
CsrUint8 *DM_SM_BONDING_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_BONDING_CFM_T *primitive;

    primitive = (DM_SM_BONDING_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_BONDING_CFM */

#ifndef EXCLUDE_DM_SM_BONDING_CFM
void *DM_SM_BONDING_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_BONDING_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_BONDING_CFM_T *) CsrPmemZalloc(sizeof(DM_SM_BONDING_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_BONDING_CFM */

#ifndef EXCLUDE_DM_HCI_READ_AUTO_FLUSH_TIMEOUT_REQ
CsrSize DM_HCI_READ_AUTO_FLUSH_TIMEOUT_REQ_TSizeof(void *msg)
{
    DM_HCI_READ_AUTO_FLUSH_TIMEOUT_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_AUTO_FLUSH_TIMEOUT_REQ */

#ifndef EXCLUDE_DM_HCI_READ_AUTO_FLUSH_TIMEOUT_REQ
CsrUint8 *DM_HCI_READ_AUTO_FLUSH_TIMEOUT_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_AUTO_FLUSH_TIMEOUT_REQ_T *primitive;

    primitive = (DM_HCI_READ_AUTO_FLUSH_TIMEOUT_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_AUTO_FLUSH_TIMEOUT_REQ */

#ifndef EXCLUDE_DM_HCI_READ_AUTO_FLUSH_TIMEOUT_REQ
void *DM_HCI_READ_AUTO_FLUSH_TIMEOUT_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_AUTO_FLUSH_TIMEOUT_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_AUTO_FLUSH_TIMEOUT_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_AUTO_FLUSH_TIMEOUT_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_AUTO_FLUSH_TIMEOUT_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_READ_WHITE_LIST_SIZE_CFM
CsrSize DM_HCI_ULP_READ_WHITE_LIST_SIZE_CFM_TSizeof(void *msg)
{
    DM_HCI_ULP_READ_WHITE_LIST_SIZE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_READ_WHITE_LIST_SIZE_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_READ_WHITE_LIST_SIZE_CFM
CsrUint8 *DM_HCI_ULP_READ_WHITE_LIST_SIZE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_READ_WHITE_LIST_SIZE_CFM_T *primitive;

    primitive = (DM_HCI_ULP_READ_WHITE_LIST_SIZE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->white_list_size);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_READ_WHITE_LIST_SIZE_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_READ_WHITE_LIST_SIZE_CFM
void *DM_HCI_ULP_READ_WHITE_LIST_SIZE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_READ_WHITE_LIST_SIZE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_READ_WHITE_LIST_SIZE_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_READ_WHITE_LIST_SIZE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->white_list_size, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_READ_WHITE_LIST_SIZE_CFM */

#ifndef EXCLUDE_DM_SM_CONFIGURE_LOCAL_ADDRESS_REQ
CsrSize DM_SM_CONFIGURE_LOCAL_ADDRESS_REQ_TSizeof(void *msg)
{
    DM_SM_CONFIGURE_LOCAL_ADDRESS_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_CONFIGURE_LOCAL_ADDRESS_REQ */

#ifndef EXCLUDE_DM_SM_CONFIGURE_LOCAL_ADDRESS_REQ
CsrUint8 *DM_SM_CONFIGURE_LOCAL_ADDRESS_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_CONFIGURE_LOCAL_ADDRESS_REQ_T *primitive;

    primitive = (DM_SM_CONFIGURE_LOCAL_ADDRESS_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->set_permanent_address_type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->static_addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->static_addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->static_addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->static_addrt.addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->privacy);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_CONFIGURE_LOCAL_ADDRESS_REQ */

#ifndef EXCLUDE_DM_SM_CONFIGURE_LOCAL_ADDRESS_REQ
void *DM_SM_CONFIGURE_LOCAL_ADDRESS_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_CONFIGURE_LOCAL_ADDRESS_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_CONFIGURE_LOCAL_ADDRESS_REQ_T *) CsrPmemZalloc(sizeof(DM_SM_CONFIGURE_LOCAL_ADDRESS_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    {
        CsrUint16 data;
        CsrUint16Des((CsrUint16 *) &data, buffer, &offset);
        primitive->set_permanent_address_type = (DM_SM_PERMANENT_ADDRESS_T) data;
    }
    CsrUint8Des((CsrUint8 *) &primitive->static_addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->static_addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->static_addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->static_addrt.addr.nap, buffer, &offset);
    {
        CsrUint16 data;
        CsrUint16Des((CsrUint16 *) &data, buffer, &offset);
        primitive->privacy = (DM_SM_PRIVACY_T) data;
    }

    return primitive;
}
#endif /* EXCLUDE_DM_SM_CONFIGURE_LOCAL_ADDRESS_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_INQUIRY_SCAN_TYPE_CFM
CsrSize DM_HCI_WRITE_INQUIRY_SCAN_TYPE_CFM_TSizeof(void *msg)
{
    DM_HCI_WRITE_INQUIRY_SCAN_TYPE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_WRITE_INQUIRY_SCAN_TYPE_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_INQUIRY_SCAN_TYPE_CFM
CsrUint8 *DM_HCI_WRITE_INQUIRY_SCAN_TYPE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_WRITE_INQUIRY_SCAN_TYPE_CFM_T *primitive;

    primitive = (DM_HCI_WRITE_INQUIRY_SCAN_TYPE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_WRITE_INQUIRY_SCAN_TYPE_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_INQUIRY_SCAN_TYPE_CFM
void *DM_HCI_WRITE_INQUIRY_SCAN_TYPE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_INQUIRY_SCAN_TYPE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_INQUIRY_SCAN_TYPE_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_INQUIRY_SCAN_TYPE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_WRITE_INQUIRY_SCAN_TYPE_CFM */

#ifndef EXCLUDE_DM_SM_BONDING_CANCEL_REQ
CsrSize DM_SM_BONDING_CANCEL_REQ_TSizeof(void *msg)
{
    DM_SM_BONDING_CANCEL_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_SM_BONDING_CANCEL_REQ */

#ifndef EXCLUDE_DM_SM_BONDING_CANCEL_REQ
CsrUint8 *DM_SM_BONDING_CANCEL_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_SM_BONDING_CANCEL_REQ_T *primitive;

    primitive = (DM_SM_BONDING_CANCEL_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);

    return buffer;
}
#endif /* EXCLUDE_DM_SM_BONDING_CANCEL_REQ */

#ifndef EXCLUDE_DM_SM_BONDING_CANCEL_REQ
void *DM_SM_BONDING_CANCEL_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_BONDING_CANCEL_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_BONDING_CANCEL_REQ_T *) CsrPmemZalloc(sizeof(DM_SM_BONDING_CANCEL_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_SM_BONDING_CANCEL_REQ */

#ifndef EXCLUDE_DM_HCI_DELETE_STORED_LINK_KEY_REQ
CsrSize DM_HCI_DELETE_STORED_LINK_KEY_REQ_TSizeof(void *msg)
{
    DM_HCI_DELETE_STORED_LINK_KEY_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_DELETE_STORED_LINK_KEY_REQ */

#ifndef EXCLUDE_DM_HCI_DELETE_STORED_LINK_KEY_REQ
CsrUint8 *DM_HCI_DELETE_STORED_LINK_KEY_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_DELETE_STORED_LINK_KEY_REQ_T *primitive;

    primitive = (DM_HCI_DELETE_STORED_LINK_KEY_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->flag);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_DELETE_STORED_LINK_KEY_REQ */

#ifndef EXCLUDE_DM_HCI_DELETE_STORED_LINK_KEY_REQ
void *DM_HCI_DELETE_STORED_LINK_KEY_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_DELETE_STORED_LINK_KEY_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_DELETE_STORED_LINK_KEY_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_DELETE_STORED_LINK_KEY_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->flag, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_DELETE_STORED_LINK_KEY_REQ */

#ifndef EXCLUDE_DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_REQ
CsrSize DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_REQ_TSizeof(void *msg)
{
    DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_REQ */

#ifndef EXCLUDE_DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_REQ
CsrUint8 *DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_REQ_T *primitive;

    primitive = (DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_REQ */

#ifndef EXCLUDE_DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_REQ
void *DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_READ_REMOTE_USED_FEATURES_CFM
CsrSize DM_HCI_ULP_READ_REMOTE_USED_FEATURES_CFM_TSizeof(void *msg)
{
    DM_HCI_ULP_READ_REMOTE_USED_FEATURES_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_READ_REMOTE_USED_FEATURES_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_READ_REMOTE_USED_FEATURES_CFM
CsrUint8 *DM_HCI_ULP_READ_REMOTE_USED_FEATURES_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_READ_REMOTE_USED_FEATURES_CFM_T *primitive;

    primitive = (DM_HCI_ULP_READ_REMOTE_USED_FEATURES_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrMemCpySer(buffer, length, ((const void *) primitive->feature_set), ((CsrSize) (8)));

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_READ_REMOTE_USED_FEATURES_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_READ_REMOTE_USED_FEATURES_CFM
void *DM_HCI_ULP_READ_REMOTE_USED_FEATURES_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_READ_REMOTE_USED_FEATURES_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_READ_REMOTE_USED_FEATURES_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_READ_REMOTE_USED_FEATURES_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->feature_set), buffer, &offset, ((CsrSize) (8)));

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_READ_REMOTE_USED_FEATURES_CFM */

#ifndef EXCLUDE_DM_HCI_READ_HOLD_MODE_ACTIVITY_CFM
CsrSize DM_HCI_READ_HOLD_MODE_ACTIVITY_CFM_TSizeof(void *msg)
{
    DM_HCI_READ_HOLD_MODE_ACTIVITY_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_HOLD_MODE_ACTIVITY_CFM */

#ifndef EXCLUDE_DM_HCI_READ_HOLD_MODE_ACTIVITY_CFM
CsrUint8 *DM_HCI_READ_HOLD_MODE_ACTIVITY_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_HOLD_MODE_ACTIVITY_CFM_T *primitive;

    primitive = (DM_HCI_READ_HOLD_MODE_ACTIVITY_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->activity);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_HOLD_MODE_ACTIVITY_CFM */

#ifndef EXCLUDE_DM_HCI_READ_HOLD_MODE_ACTIVITY_CFM
void *DM_HCI_READ_HOLD_MODE_ACTIVITY_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_HOLD_MODE_ACTIVITY_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_HOLD_MODE_ACTIVITY_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_HOLD_MODE_ACTIVITY_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->activity, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_HOLD_MODE_ACTIVITY_CFM */

#ifndef EXCLUDE_DM_HCI_READ_PAGESCAN_PERIOD_MODE_REQ
CsrSize DM_HCI_READ_PAGESCAN_PERIOD_MODE_REQ_TSizeof(void *msg)
{
    DM_HCI_READ_PAGESCAN_PERIOD_MODE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_PAGESCAN_PERIOD_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_READ_PAGESCAN_PERIOD_MODE_REQ
CsrUint8 *DM_HCI_READ_PAGESCAN_PERIOD_MODE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_PAGESCAN_PERIOD_MODE_REQ_T *primitive;

    primitive = (DM_HCI_READ_PAGESCAN_PERIOD_MODE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_PAGESCAN_PERIOD_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_READ_PAGESCAN_PERIOD_MODE_REQ
void *DM_HCI_READ_PAGESCAN_PERIOD_MODE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_PAGESCAN_PERIOD_MODE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_PAGESCAN_PERIOD_MODE_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_PAGESCAN_PERIOD_MODE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_PAGESCAN_PERIOD_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_FLOW_SPEC_REQ
CsrSize DM_HCI_FLOW_SPEC_REQ_TSizeof(void *msg)
{
    DM_HCI_FLOW_SPEC_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_FLOW_SPEC_REQ */

#ifndef EXCLUDE_DM_HCI_FLOW_SPEC_REQ
CsrUint8 *DM_HCI_FLOW_SPEC_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_FLOW_SPEC_REQ_T *primitive;

    primitive = (DM_HCI_FLOW_SPEC_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->common.length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->flags);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->flow_direction);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->service_type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->token_rate);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->token_bucket_size);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->peak_bandwidth);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->access_latency);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_FLOW_SPEC_REQ */

#ifndef EXCLUDE_DM_HCI_FLOW_SPEC_REQ
void *DM_HCI_FLOW_SPEC_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_FLOW_SPEC_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_FLOW_SPEC_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_FLOW_SPEC_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->flags, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->flow_direction, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->service_type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->token_rate, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->token_bucket_size, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->peak_bandwidth, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->access_latency, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_FLOW_SPEC_REQ */

#ifndef EXCLUDE_DM_HCI_READ_AFH_CHANNEL_CLASS_M_CFM
CsrSize DM_HCI_READ_AFH_CHANNEL_CLASS_M_CFM_TSizeof(void *msg)
{
    DM_HCI_READ_AFH_CHANNEL_CLASS_M_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_AFH_CHANNEL_CLASS_M_CFM */

#ifndef EXCLUDE_DM_HCI_READ_AFH_CHANNEL_CLASS_M_CFM
CsrUint8 *DM_HCI_READ_AFH_CHANNEL_CLASS_M_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_AFH_CHANNEL_CLASS_M_CFM_T *primitive;

    primitive = (DM_HCI_READ_AFH_CHANNEL_CLASS_M_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->class_mode);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_AFH_CHANNEL_CLASS_M_CFM */

#ifndef EXCLUDE_DM_HCI_READ_AFH_CHANNEL_CLASS_M_CFM
void *DM_HCI_READ_AFH_CHANNEL_CLASS_M_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_AFH_CHANNEL_CLASS_M_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_AFH_CHANNEL_CLASS_M_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_AFH_CHANNEL_CLASS_M_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->class_mode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_AFH_CHANNEL_CLASS_M_CFM */

#ifndef EXCLUDE_DM_ACL_OPEN_REQ
CsrSize DM_ACL_OPEN_REQ_TSizeof(void *msg)
{
    DM_ACL_OPEN_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_ACL_OPEN_REQ */

#ifndef EXCLUDE_DM_ACL_OPEN_REQ
CsrUint8 *DM_ACL_OPEN_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_ACL_OPEN_REQ_T *primitive;

    primitive = (DM_ACL_OPEN_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);

    return buffer;
}
#endif /* EXCLUDE_DM_ACL_OPEN_REQ */

#ifndef EXCLUDE_DM_ACL_OPEN_REQ
void *DM_ACL_OPEN_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_ACL_OPEN_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_ACL_OPEN_REQ_T *) CsrPmemZalloc(sizeof(DM_ACL_OPEN_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_ACL_OPEN_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_SET_SCAN_PARAMETERS_CFM
CsrSize DM_HCI_ULP_SET_SCAN_PARAMETERS_CFM_TSizeof(void *msg)
{
    DM_HCI_ULP_SET_SCAN_PARAMETERS_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_SCAN_PARAMETERS_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_SET_SCAN_PARAMETERS_CFM
CsrUint8 *DM_HCI_ULP_SET_SCAN_PARAMETERS_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_SET_SCAN_PARAMETERS_CFM_T *primitive;

    primitive = (DM_HCI_ULP_SET_SCAN_PARAMETERS_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_SCAN_PARAMETERS_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_SET_SCAN_PARAMETERS_CFM
void *DM_HCI_ULP_SET_SCAN_PARAMETERS_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_SET_SCAN_PARAMETERS_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_SET_SCAN_PARAMETERS_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_SET_SCAN_PARAMETERS_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_ULP_SET_SCAN_PARAMETERS_CFM */

#ifndef EXCLUDE_DM_HCI_READ_LMP_HANDLE_CFM
CsrSize DM_HCI_READ_LMP_HANDLE_CFM_TSizeof(void *msg)
{
    DM_HCI_READ_LMP_HANDLE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_DM_HCI_READ_LMP_HANDLE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_LMP_HANDLE_CFM
CsrUint8 *DM_HCI_READ_LMP_HANDLE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_READ_LMP_HANDLE_CFM_T *primitive;

    primitive = (DM_HCI_READ_LMP_HANDLE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->lmp_handle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reserved);

    return buffer;
}
#endif /* EXCLUDE_DM_HCI_READ_LMP_HANDLE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_LMP_HANDLE_CFM
void *DM_HCI_READ_LMP_HANDLE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_LMP_HANDLE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_READ_LMP_HANDLE_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_LMP_HANDLE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->lmp_handle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reserved, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_DM_HCI_READ_LMP_HANDLE_CFM */

#endif /* EXCLUDE_DM_MODULE */

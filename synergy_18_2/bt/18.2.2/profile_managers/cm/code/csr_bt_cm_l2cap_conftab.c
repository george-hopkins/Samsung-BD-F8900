/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE

#include "csr_bt_cm_l2cap.h"
#include "csr_bt_cm_l2cap_conftab.h"
#include "csr_bt_cm_dm.h"
#include "csr_bt_cm_util.h"
#include "csr_bt_sc_private_lib.h"
#include "bkeyval.h"

static void csrBtCmL2caAutoBkvDyn16Exact(CsrUint16 *pConfTab, CsrUint16 *conftabCount, CsrUint16 key, CsrUint16 value)
{
    BKVD_16_EXACT(pConfTab, *conftabCount, key, value);
    *conftabCount += 2;
}

static void csrBtCmL2caAutoBkvDyn16Range(CsrUint16 *pConfTab, CsrUint16 *conftabCount, CsrUint16 key,
                                         CsrUint16 best, CsrUint16 worst)
{
    BKVD_16_RANGE(pConfTab, *conftabCount, key, best, worst);
    *conftabCount += 3;
}

static void csrBtCmL2caAutoBkvDyn32Exact(CsrUint16 *pConfTab, CsrUint16 *conftabCount, CsrUint16 key, CsrUint32 value)
{
    BKVD_32_EXACT(pConfTab, *conftabCount, key, value);
    *conftabCount += 3;
}

static void csrBtCmL2caAutoBkvDyn32Range(CsrUint16 *pConfTab, CsrUint16 *conftabCount, CsrUint16 key,
                                         CsrUint32 best, CsrUint32 worst)
{
    BKVD_32_RANGE(pConfTab, *conftabCount, key, best, worst);
    *conftabCount += 5;
}

void CsrBtCmL2caConftabInit(CsrUint16 **pConfTab, CsrUint16 *conftabCount)
{
    CsrUint16 *confTab;

    *conftabCount = 0;
    CsrPmemFree(*pConfTab);

    *pConfTab = CsrPmemZalloc(CSR_BT_CM_L2CA_AUTOPT_MAX_SIZE);
    confTab = *pConfTab;
    confTab[(*conftabCount)++] = BKV_SEP(0);
}

void CsrBtCmL2caConftabCull(CsrUint16 **pConfTab, CsrUint16 *pConftabCount)
{
    /* Cull size of conftab buffer to match exactly what we've got */
    CsrUint16 *new;
    new = (CsrUint16*)CsrMemDup(*pConfTab, *pConftabCount* sizeof(CsrUint16));
    CsrPmemFree(*pConfTab);
    *pConfTab = new;
}

void CsrBtCmL2caConftabEnlarge(CsrUint16 **pConfTab, CsrUint16 *pConftabCount)
{
    CsrUint16 *newtab = NULL;
    CsrUint16 newlen = 0;

    if(*pConfTab == NULL)
    {
        /* Already emptym so reinit */
        CsrBtCmL2caConftabInit(pConfTab, pConftabCount);
        return;
    }

    CsrBtCmL2caConftabInit(&newtab, &newlen);
    newlen = *pConftabCount;
    CsrMemCpy(newtab, *pConfTab, newlen * sizeof(CsrUint16));

    /* Erase the end barrier */
    if(newtab[newlen - 1] == BKV_SEP_END(0))
    {
        newlen--;
    }

    CsrPmemFree(*pConfTab);
    *pConfTab = newtab;
    *pConftabCount = newlen;
}


void CsrBtCmL2caConftabFinalize(CsrUint16 *confTab, CsrUint16 *conftabCount)
{
    confTab[(*conftabCount)++] = BKV_SEP_END(0);
}

void CsrBtCmL2caConftabNoReconf(CsrUint16 *confTab, CsrUint16 *conftabCount)
{
    csrBtCmL2caAutoBkvDyn16Exact(confTab, conftabCount, L2CA_AUTOPT_DISABLE_RECONF, L2CA_RECONF_DISABLE_ALL);
}

void CsrBtCmL2caConftabMtu(CsrUint16 *confTab, CsrUint16 *conftabCount, CsrBool incoming, l2ca_mtu_t mtu)
{
    if (mtu != CSR_BT_AUTO_SELECT_MAX_FRAME_SIZE)
    {
        if (incoming)
        {
            csrBtCmL2caAutoBkvDyn16Exact(confTab, conftabCount, L2CA_AUTOPT_MTU_IN, mtu);
        }
        else
        {
            csrBtCmL2caAutoBkvDyn16Exact(confTab, conftabCount, L2CA_AUTOPT_MTU_OUT, mtu);
        }
    }
}

void CsrBtCmL2caConftabMps(CsrUint16 *confTab, CsrUint16 *conftabCount, CsrBool incoming, l2ca_mtu_t min, l2ca_mtu_t max)
{
    if((min != CSR_BT_AUTO_SELECT_MAX_FRAME_SIZE) ||
       (max != CSR_BT_AUTO_SELECT_MAX_FRAME_SIZE))
    {
        if(incoming)
        {
            csrBtCmL2caAutoBkvDyn16Range(confTab, conftabCount, L2CA_AUTOPT_FLOW_MAX_PDU_IN,
                                         (CsrUint16)CSRMAX(min, max), /* best */
                                         (CsrUint16)CSRMIN(min, max)); /* worst */
        }
        else
        {
            csrBtCmL2caAutoBkvDyn16Range(confTab, conftabCount, L2CA_AUTOPT_FLOW_MAX_PDU_OUT,
                                         (CsrUint16)CSRMAX(min, max), /* best */
                                         (CsrUint16)CSRMIN(min, max)); /* worst */
        }
    }
}

void CsrBtCmL2caConftabFlushTo(CsrUint16 *confTab, CsrUint16 *conftabCount, CsrBool incoming, CsrUint16 flushTo)
{
    if ((flushTo != 0) &&
        (flushTo != L2CA_FLUSH_TO_DEFAULT))
    {
        if (incoming)
        {
            csrBtCmL2caAutoBkvDyn32Range(confTab, conftabCount, L2CA_AUTOPT_FLUSH_IN,
                                         flushTo * 1000 /* us */,
                                         flushTo * 1000 /* us */);
        }
        else
        {
            csrBtCmL2caAutoBkvDyn32Range(confTab, conftabCount, L2CA_AUTOPT_FLUSH_OUT,
                                         flushTo * 1000 /* us */,
                                         flushTo * 1000 /* us */);
        }
    }
}

void CsrBtCmL2caConftabFlushToAllowAnyPeer(CsrUint16 *confTab, CsrUint16 *conftabCount)
{
    csrBtCmL2caAutoBkvDyn32Range(confTab, conftabCount, L2CA_AUTOPT_FLUSH_IN,
                                 L2CA_FLOWSPEC_FLUSH_TO_INFINITE, /* best */
                                 L2CA_FLUSH_TO_IMMEDIATE); /* worst */
}

void CsrBtCmL2caConftabTxwinAllowAnyPeer(CsrUint16 *confTab, CsrUint16 *conftabCount)
{
    csrBtCmL2caAutoBkvDyn16Range(confTab, conftabCount, L2CA_AUTOPT_FLOW_WINDOW_OUT,
                                 MAX_L2CAP_TX_WINDOW, /* best */
                                 MIN_L2CAP_TX_WINDOW); /* worst */
}

void CsrBtCmL2caConftabExtendedFeatures(CsrUint16 *confTab, CsrUint16 *conftabCount, CsrUint32 extFeat)
{
    csrBtCmL2caAutoBkvDyn32Exact(confTab, conftabCount, L2CA_AUTOPT_EXT_FEATS, extFeat);
}

void CsrBtCmL2caConftabFlow(CsrUint16 *confTab, CsrUint16 *conftabCount, L2CA_FLOW_T *flow, CsrBool allowFallback)
{
    if (flow != NULL)
    {
        CsrUint16 mode = ((CsrUint16) flow->mode) << 8;

        if (allowFallback)
        {
            mode |= L2CA_MODE_MASK_BASIC;
        }

        csrBtCmL2caAutoBkvDyn16Exact(confTab, conftabCount, L2CA_AUTOPT_FLOW_MODE, mode);
        csrBtCmL2caAutoBkvDyn16Range(confTab, conftabCount, L2CA_AUTOPT_FLOW_WINDOW_IN,
                                     flow->tx_window, flow->tx_window);
        csrBtCmL2caAutoBkvDyn16Range(confTab, conftabCount, L2CA_AUTOPT_FLOW_MAX_RETX_IN,
                                     255, 0); /* best, worst */

        /* Auto value doesn't fill in entry */
        if(flow->maximum_pdu == CSR_BT_AUTO_SELECT_MAX_FRAME_SIZE)
        {
            CsrBtCmL2caConftabMps(confTab, conftabCount, TRUE, flow->maximum_pdu, flow->maximum_pdu);
        }
    }
    else
    {
        /* Set ext. features to 0 to fake that peer doesn't support
         * anything. This will work around BT1.0, defunct and
         * non-compliant systems that doesn't respond to the
         * get-info */
        CsrBtCmL2caConftabExtendedFeatures(confTab, conftabCount, 0x0);
    }
}

#ifdef CSR_AMP_ENABLE
void CsrBtCmL2caConftabFcs(CsrUint16 *confTab, CsrUint16 *conftabCount, CsrBool useFcs)
{
    csrBtCmL2caAutoBkvDyn16Exact(confTab, conftabCount, L2CA_AUTOPT_FCS,
                                 (CsrUint16)(useFcs ? L2CA_FCS_ON : L2CA_FCS_OFF));
}
#endif

void CsrBtCmL2caConftabHighDataPriority(CsrUint16 *confTab, CsrUint16 *conftabCount, CsrUint16 dataPriority)
{
    if(dataPriority != CSR_BT_CM_PRIORITY_NORMAL)
    {
        csrBtCmL2caAutoBkvDyn16Exact(confTab, conftabCount,
                                     L2CA_AUTOPT_CHANNEL_PRIORITY,
                                     (CsrUint16)CSRMIN(dataPriority, CSR_BT_CM_PRIORITY_LOW));
    }
}
#endif



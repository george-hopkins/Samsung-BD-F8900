/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_env_prim.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_util.h"
#include "csr_exceptionhandler.h"
#include "csr_bt_hfg_prim.h"
#include "csr_bt_hfg_main.h"
#include "csr_bt_hfg_proto.h"
#include "csr_formatted_io.h"

/* Length of temporary string buffer */
#define CSR_BT_HFG_TMP_STRING 200

/* Local: String append, no formatting */
static void csrBtHfgStr(CsrUint8 *str, CsrUint16 *idx, CsrUint8 *app)
{
    CsrUint16 srcIdx;
    CsrUint16 srcLen;

    if(app == NULL)
    {
        return;
    }

    srcIdx = 0;
    srcLen = (CsrUint16)CsrStrLen((char*)app);
    while(srcIdx < srcLen)
    {
        str[*idx] = app[srcIdx];
        (*idx)++;
        srcIdx++;
    }
}

/* Local: Number append, no formatting */
static void csrBtHfgInt(CsrUint8 *str, CsrUint16 *idx, CsrUint32 num)
{
    CsrUint8 conv[20];
    CsrSnprintf((CsrCharString *) conv, CSR_ARRAY_SIZE(conv), "%u", num);
    csrBtHfgStr(str, idx, conv);
}

/* Local: CR and LF append */
static void csrBtHfgCrlf(CsrUint8 *str, CsrUint16 *idx)
{
    csrBtHfgStr(str, idx, (CsrUint8*)"\r\n");
}

/* Local: Append string with quotation if not already quoted */
static void csrBtHfgStrQ(CsrUint8 *str, CsrUint16 *idx, CsrCharString *app)
{
    if((app == NULL) || (CsrStrLen((char*)app) < 2))
    {
        return;
    }

    if(app[0] == '"')
    {
        csrBtHfgStr(str, idx, (CsrUint8*)app);
    }
    else
    {
        csrBtHfgStr(str, idx, (CsrUint8*)"\"");
        csrBtHfgStr(str, idx, (CsrUint8*)app);
        csrBtHfgStr(str, idx, (CsrUint8*)"\"");
    }
}

static void csrBtHfgCom(CsrUint8 *str, CsrUint16 *idx, CsrBool add)
{
    if(add)
    {
        csrBtHfgStr(str, idx, (CsrUint8*)",");
    }
}

/* From this point and down follows the actual AT result generator and
 * sender functions */

/* Send OK */
void CsrBtHfgSendAtOk(HfgInstance_t *inst)
{

    CsrUint8 *str;
    CsrUint16 idx = 7;

    str = CsrPmemAlloc(idx);

    CsrMemSet(str, 0, idx);
    idx = 0;
    csrBtHfgCrlf(str, &idx);
    csrBtHfgStr(str, &idx, CSR_BT_HFG_STR_OK);

    csrBtHfgCrlf(str, &idx);

    CsrBtHfgSendCmDataReq(inst, idx, str);
}

/* Send CMEE error */
void CsrBtHfgSendAtError(HfgInstance_t *inst)
{
    CsrUint8 *str;
    CsrUint16 idx = 20;

    str = CsrPmemAlloc(idx);
    CsrMemSet(str, 0, idx);
    idx = 0;
    csrBtHfgCrlf(str, &idx);

    csrBtHfgStr(str, &idx, CSR_BT_HFG_STR_ERROR);
    csrBtHfgCrlf(str, &idx);

    CsrBtHfgSendCmDataReq(inst, idx, str);
}

/* Send response/error-code based on CMEE and code */
void CsrBtHfgSendAtResponse(HfgInstance_t *inst, CsrUint16 cme)
{
    if(cme == CSR_BT_CME_SUCCESS)
    {
        CsrBtHfgSendAtOk(inst);
    }
    else if(inst->ind.other[CSR_BT_HFG_SET_CMEE] != 0)
    {
        CsrBtHfgSendAtCmee(inst, cme);
    }
    else
    {
        CsrBtHfgSendAtError(inst);
    }
}

/* Send CMEE error */
void CsrBtHfgSendAtCmee(HfgInstance_t *inst, CsrUint16 cmee)
{
    CsrUint8 *str;
    CsrUint16 idx = 50;

    str = CsrPmemAlloc(idx);
    CsrMemSet(str, 0, idx);
    idx = 0;
    csrBtHfgCrlf(str, &idx);

    csrBtHfgStr(str, &idx, CSR_BT_HFG_STR_CMEE_ERROR);
    csrBtHfgInt(str, &idx, cmee);

    csrBtHfgCrlf(str, &idx);
    CsrBtHfgSendCmDataReq(inst, idx, str);
}

/* Send RING */
void CsrBtHfgSendAtRing(HfgInstance_t *inst)
{
    CsrUint8 *str;
    CsrUint16 idx = 10;

    str = CsrPmemAlloc(idx);
    CsrMemSet(str, 0, idx);
    idx = 0;
    csrBtHfgCrlf(str, &idx);

    csrBtHfgStr(str, &idx, CSR_BT_HFG_STR_RING);
    csrBtHfgCrlf(str, &idx);

    CsrBtHfgSendCmDataReq(inst, idx, str);
}

/* Send CIND supported attributes */
void CsrBtHfgSendAtCindSupport(HfgInstance_t *inst)
{
    CsrUint8 *str;
    CsrUint16 idx = CSR_BT_HFG_TMP_STRING;

    str = CsrPmemAlloc(idx);
    CsrMemSet(str, 0, idx);
    idx = 0;
    csrBtHfgCrlf(str, &idx);

    csrBtHfgStr(str, &idx, CSR_BT_HFG_STR_CIND);
    csrBtHfgStr(str, &idx, CSR_BT_HFG_STR_CIND_SUPPORT_1);
    csrBtHfgStr(str, &idx, CSR_BT_HFG_STR_CIND_SUPPORT_2);
    csrBtHfgCrlf(str, &idx);

    CsrBtHfgSendCmDataReq(inst, idx, str);
}

/* Send CIND status update */
void CsrBtHfgSendAtCindStatus(HfgInstance_t *inst)
{
    CsrUint8 *str;
    CsrUint16 idx = CSR_BT_HFG_TMP_STRING;

    str = CsrPmemAlloc(idx);
    CsrMemSet(str, 0, idx);
    idx = 0;
    csrBtHfgCrlf(str, &idx);

    csrBtHfgStr(str, &idx, CSR_BT_HFG_STR_CIND);

    csrBtHfgInt(str, &idx, inst->ind.ciev[CSR_BT_SERVICE_INDICATOR]);
    csrBtHfgCom(str, &idx, TRUE);

    csrBtHfgInt(str, &idx, inst->ind.ciev[CSR_BT_CALL_STATUS_INDICATOR]);
    csrBtHfgCom(str, &idx, TRUE);

    csrBtHfgInt(str, &idx, inst->ind.ciev[CSR_BT_CALL_SETUP_STATUS_INDICATOR]);
    csrBtHfgCom(str, &idx, TRUE);

    csrBtHfgInt(str, &idx, inst->ind.ciev[CSR_BT_CALL_HELD_INDICATOR]);
    csrBtHfgCom(str, &idx, TRUE);

    csrBtHfgInt(str, &idx, inst->ind.ciev[CSR_BT_SIGNAL_STRENGTH_INDICATOR]);
    csrBtHfgCom(str, &idx, TRUE);

    csrBtHfgInt(str, &idx, inst->ind.ciev[CSR_BT_ROAM_INDICATOR]);
    csrBtHfgCom(str, &idx, TRUE);

    csrBtHfgInt(str, &idx, inst->ind.ciev[CSR_BT_BATTERY_CHARGE_INDICATOR]);

    csrBtHfgCrlf(str, &idx);
    CsrBtHfgSendCmDataReq(inst, idx, str);
}

/* Send CHLD supported attributes */
void CsrBtHfgSendAtChldSupport(HfgInstance_t *inst)
{
    HfgMainInstance_t *mainInst;
    CsrBool comma;
    CsrUint8 *str;
    CsrUint16 idx = CSR_BT_HFG_TMP_STRING;

    str = CsrPmemAlloc(idx);
    CsrMemSet(str, 0, idx);
    idx = 0;
    csrBtHfgCrlf(str, &idx);

    mainInst = CsrBtHfgGetMainInstance(inst);
    comma = FALSE;

    csrBtHfgStr(str, &idx, CSR_BT_HFG_STR_CHLD);
    csrBtHfgStr(str, &idx, (CsrUint8*)"(");

    if(!(mainInst->callConfig & CSR_BT_HFG_CRH_DISABLE_CHLD_0))
    {
        csrBtHfgStr(str, &idx, (CsrUint8*)"0");
        comma = TRUE;
    }

    if(!(mainInst->callConfig & CSR_BT_HFG_CRH_DISABLE_CHLD_1))
    {
        csrBtHfgCom(str, &idx, comma);
        csrBtHfgStr(str, &idx, (CsrUint8*)"1");
        comma = TRUE;
    }

    if(!(mainInst->callConfig & CSR_BT_HFG_CRH_DISABLE_CHLD_1X))
    {
        csrBtHfgCom(str, &idx, comma);
        csrBtHfgStr(str, &idx, (CsrUint8*)"1x");
        comma = TRUE;
    }

    if(!(mainInst->callConfig & CSR_BT_HFG_CRH_DISABLE_CHLD_2))
    {
        csrBtHfgCom(str, &idx, comma);
        csrBtHfgStr(str, &idx, (CsrUint8*)"2");
        comma = TRUE;
    }

    if(!(mainInst->callConfig & CSR_BT_HFG_CRH_DISABLE_CHLD_2X))
    {
        csrBtHfgCom(str, &idx, comma);
        csrBtHfgStr(str, &idx, (CsrUint8*)"2x");
        comma = TRUE;
    }

    if(!(mainInst->callConfig & CSR_BT_HFG_CRH_DISABLE_CHLD_3))
    {
        csrBtHfgCom(str, &idx, comma);
        csrBtHfgStr(str, &idx, (CsrUint8*)"3");
        comma = TRUE;
    }

    if(!(mainInst->callConfig & CSR_BT_HFG_CRH_DISABLE_CHLD_4))
    {
        csrBtHfgCom(str, &idx, comma);
        csrBtHfgStr(str, &idx, (CsrUint8*)"4");
    }

    csrBtHfgStr(str, &idx, (CsrUint8*)")");
    csrBtHfgCrlf(str, &idx);

    CsrBtHfgSendCmDataReq(inst, idx, str);
}

/* Send BRSF and bitmap */
void CsrBtHfgSendAtBrsf(HfgInstance_t *inst)
{
    CsrUint8 *str;
    CsrUint16 idx = 20;

    str = CsrPmemAlloc(idx);
    CsrMemSet(str, 0, idx);
    idx = 0;
    csrBtHfgCrlf(str, &idx);

    csrBtHfgStr(str, &idx, CSR_BT_HFG_STR_BRSF);
    csrBtHfgInt(str, &idx, CsrBtHfgGetMainInstance(inst)->locSupFeatures);

    csrBtHfgCrlf(str, &idx);
    CsrBtHfgSendCmDataReq(inst, idx, str);
}

/* Send CIEV update */
void CsrBtHfgSendAtCiev(HfgInstance_t *inst, CsrUint8 ind, CsrUint8 value)
{
    CsrUint8 *str;
    CsrUint16 idx = 20;

    str = CsrPmemAlloc(idx);
    CsrMemSet(str, 0, idx);
    idx = 0;
    csrBtHfgCrlf(str, &idx);

    csrBtHfgStr(str, &idx, CSR_BT_HFG_STR_CIEV);
    csrBtHfgInt(str, &idx, ind);
    csrBtHfgCom(str, &idx, TRUE);
    csrBtHfgInt(str, &idx, value);

    csrBtHfgCrlf(str, &idx);
    CsrBtHfgSendCmDataReq(inst, idx, str);
}

/* Send CIEV update: several indicators at a time */
void CsrBtHfgSendCombinedAtCiev(HfgInstance_t *inst, CsrUint8 indicatorMask)
{
    if (indicatorMask > 0)
    {
        CsrUint8 i;
        CsrBool addStartMark = FALSE;
        CsrUint8 *str;
        CsrUint16 idx = CSR_BT_HFG_TMP_STRING;

        str = CsrPmemAlloc(idx);
        CsrMemSet(str, 0, idx);
        idx = 0;
        csrBtHfgCrlf(str, &idx);

        for (i=0;i<(CSR_BT_CIEV_NUMBER_OF_INDICATORS-1);i++)
        {
            if (indicatorMask & (1<<i))
            {
                if (addStartMark)
                {/* <CR><LF> needed to separate the AT commands */
                    csrBtHfgCrlf(str, &idx);
                }
                csrBtHfgStr(str, &idx, CSR_BT_HFG_STR_CIEV);
                csrBtHfgInt(str, &idx, i+1);
                csrBtHfgCom(str, &idx, TRUE);
                csrBtHfgInt(str, &idx, inst->ind.ciev[i+1]);
                csrBtHfgCrlf(str, &idx);
                addStartMark = TRUE;
            }
        }
        CsrBtHfgSendCmDataReq(inst, idx, str);
    }
}

/* Start non-Auristream codec negotiation */
void CsrBtHfgSendCodecNegMsg(HfgInstance_t *inst)
{
    CsrUint8 *str;
    CsrUint16 idx = 20;
    CsrUint8 codec = CSR_BT_WBS_CVSD_CODEC;

    str = CsrPmemAlloc(idx);
    CsrMemSet(str, 0, idx);
    idx = 0;
    csrBtHfgCrlf(str, &idx);

    csrBtHfgStr(str, &idx, CSR_BT_HFG_STR_BCS);
    if ((inst->remSupportedCodecs & CSR_BT_WBS_MSBC_CODEC_MASK)
        && (inst->lastCodecUsed != CSR_BT_WBS_MSBC_CODEC))
    {
        codec = CSR_BT_WBS_MSBC_CODEC; 
    }
    else
    {
        codec = CSR_BT_WBS_CVSD_CODEC;
    }
    /* Keep track of the last trial */
    inst->lastCodecUsed = codec;
    inst->pendingCodecNegotiation = TRUE;
    csrBtHfgInt(str, &idx, codec);
    
    csrBtHfgCrlf(str, &idx);
    CsrBtHfgSendCmDataReq(inst, idx, str);
}

/* Send speaker gain */
void CsrBtHfgSendAtSpeakerGain(HfgInstance_t *inst, CsrUint8 gain)
{
    CsrUint8 *opcode;
    CsrUint8 *str;
    CsrUint16 idx = 20;

    str = CsrPmemAlloc(idx);
    CsrMemSet(str, 0, idx);
    idx = 0;
    csrBtHfgCrlf(str, &idx);

    opcode = (inst->linkType == CSR_BT_HFG_CONNECTION_HFG
              ? CSR_BT_HFG_STR_HF_SPEAKER_GAIN
              : CSR_BT_HFG_STR_HS_SPEAKER_GAIN);

    csrBtHfgStr(str, &idx, opcode);
    csrBtHfgInt(str, &idx, gain);

    csrBtHfgCrlf(str, &idx);
    CsrBtHfgSendCmDataReq(inst, idx, str);
}

/* Send microphone gain */
void CsrBtHfgSendAtMicGain(HfgInstance_t *inst, CsrUint8 gain)
{
    CsrUint8 *opcode;
    CsrUint8 *str;
    CsrUint16 idx = 20;

    str = CsrPmemAlloc(idx);
    CsrMemSet(str, 0, idx);
    idx = 0;
    csrBtHfgCrlf(str, &idx);

    opcode = (inst->linkType == CSR_BT_HFG_CONNECTION_HFG
              ? CSR_BT_HFG_STR_HF_MIC_GAIN
              : CSR_BT_HFG_STR_HS_MIC_GAIN);

    csrBtHfgStr(str, &idx, opcode);
    csrBtHfgInt(str, &idx, gain);

    csrBtHfgCrlf(str, &idx);
    CsrBtHfgSendCmDataReq(inst, idx, str);
}

/* Send call waiting notification without name */
void CsrBtHfgSendAtCcwa(HfgInstance_t *inst,
                   CsrCharString *number,
                   CsrCharString *name,
                   CsrUint8 numType)
{
    CsrUint8 *str;
    CsrUint16 idx = CSR_BT_HFG_TMP_STRING;

    str = CsrPmemAlloc(idx);
    CsrMemSet(str, 0, idx);
    idx = 0;
    csrBtHfgCrlf(str, &idx);

    csrBtHfgStr(str, &idx, CSR_BT_HFG_STR_CCWA);
    csrBtHfgStrQ(str, &idx, number);
    csrBtHfgCom(str, &idx, TRUE);
    csrBtHfgInt(str, &idx, numType);

    if(name != NULL)
    {
        csrBtHfgStr(str, &idx, (CsrUint8*)",");
        csrBtHfgStrQ(str, &idx, name);
    }

    csrBtHfgCrlf(str, &idx);
    CsrBtHfgSendCmDataReq(inst, idx, str);
}

/* Send response-and-hold status */
void CsrBtHfgSendAtBtrh(HfgInstance_t *inst, CsrUint8 btrh)
{
    CsrUint8 *str;
    CsrUint16 idx = 20;

    str = CsrPmemAlloc(idx);
    CsrMemSet(str, 0, idx);
    idx = 0;
    csrBtHfgCrlf(str, &idx);

    csrBtHfgStr(str, &idx, CSR_BT_HFG_STR_BTRH);
    csrBtHfgInt(str, &idx, btrh);

    csrBtHfgCrlf(str, &idx);
    CsrBtHfgSendCmDataReq(inst, idx, str);
}

/* Send call list entry (clcc) */
void CsrBtHfgSendAtClcc(HfgInstance_t *inst,
                   CsrUint8 cidx,
                   CsrUint8 dir,
                   CsrUint8 state,
                   CsrUint8 mode,
                   CsrUint8 mpy,
                   CsrCharString *number,
                   CsrUint8 numType)
{
    CsrUint8 *str;
    CsrUint16 idx = CSR_BT_HFG_TMP_STRING;

    str = CsrPmemAlloc(idx);
    CsrMemSet(str, 0, idx);
    idx = 0;
    csrBtHfgCrlf(str, &idx);

    csrBtHfgStr(str, &idx, CSR_BT_HFG_STR_CLCC);

    csrBtHfgInt(str, &idx, cidx);
    csrBtHfgCom(str, &idx, TRUE);

    csrBtHfgInt(str, &idx, dir);
    csrBtHfgCom(str, &idx, TRUE);

    csrBtHfgInt(str, &idx, state);
    csrBtHfgCom(str, &idx, TRUE);

    csrBtHfgInt(str, &idx, mode);
    csrBtHfgCom(str, &idx, TRUE);

    csrBtHfgInt(str, &idx, mpy);
    csrBtHfgCom(str, &idx, TRUE);

    csrBtHfgStrQ(str, &idx, number);
    csrBtHfgCom(str, &idx, TRUE);

    csrBtHfgInt(str, &idx, numType);

    csrBtHfgCrlf(str, &idx);
    CsrBtHfgSendCmDataReq(inst, idx, str);
}

/* Send subscriber number entry (cnum) */
void CsrBtHfgSendAtCnum(HfgInstance_t *inst,
                   CsrCharString      *number,
                   CsrUint8        numType,
                   CsrUint8        service)
{
    CsrUint8 *str;
    CsrUint16 idx = CSR_BT_HFG_TMP_STRING;

    str = CsrPmemAlloc(idx);
    CsrMemSet(str, 0, idx);
    idx = 0;
    csrBtHfgCrlf(str, &idx);

    csrBtHfgStr(str, &idx, CSR_BT_HFG_STR_CNUM);

    /* alpha is blank */
    csrBtHfgCom(str, &idx, TRUE);

    /* number */
    csrBtHfgStrQ(str, &idx, number);
    csrBtHfgCom (str, &idx, TRUE);

    /* numbertype */
    csrBtHfgInt(str, &idx, numType);
    csrBtHfgCom(str, &idx, TRUE);

    /* speed (is blank) */
    csrBtHfgCom(str, &idx, TRUE);

    /* service */
    csrBtHfgInt(str, &idx, service);

    csrBtHfgCrlf(str, &idx);
    CsrBtHfgSendCmDataReq(inst, idx, str);
}

/* Send inband ringing status update (bsir) */
void CsrBtHfgSendAtBsir(HfgInstance_t *inst, CsrBool inband)
{
    CsrUint8 *str;
    CsrUint16 idx = 20;

    str = CsrPmemAlloc(idx);
    CsrMemSet(str, 0, idx);
    idx = 0;
    csrBtHfgCrlf(str, &idx);

    csrBtHfgStr(str, &idx, CSR_BT_HFG_STR_BSIR);
    csrBtHfgInt(str, &idx, inband);

    csrBtHfgCrlf(str, &idx);
    CsrBtHfgSendCmDataReq(inst, idx, str);
}

/* Send Bluetooth input response (binp) */
void CsrBtHfgSendAtBinp(HfgInstance_t *inst, CsrCharString *response)
{
    CsrUint8 *str;
    CsrUint16 idx = 20;
    if (response != NULL)
    {
        idx = (CsrUint16)(idx + CsrStrLen((char *)response) + 1);
    }

    str = CsrPmemAlloc(idx);

    CsrMemSet(str, 0, idx);
    idx = 0;
    csrBtHfgCrlf(str, &idx);

    csrBtHfgStr(str, &idx, CSR_BT_HFG_STR_BINP);
    csrBtHfgStrQ(str, &idx, response);

    csrBtHfgCrlf(str, &idx);
    CsrBtHfgSendCmDataReq(inst, idx, str);
}

#ifdef CSR_BT_INSTALL_HFG_C2C
/* Send unsolicited CSR-2-CSR text */
void CsrBtHfgSendAtCsrTxt(HfgInstance_t *inst, CsrCharString *txt)
{
    if(!(CsrBtHfgGetMainInstance(inst)->hfgConfig & CSR_BT_HFG_CNF_DISABLE_C2C))
    {
        CsrUint8 *str;
        CsrUint16 idx = (CsrUint16)(50 + CsrStrLen((char*)txt));

        str = CsrPmemAlloc(idx);

        CsrMemSet(str, 0, idx);
        idx = 0;
        csrBtHfgCrlf(str, &idx);

        csrBtHfgStr(str, &idx, CSR_BT_HFG_STR_C2C_TXT);
        csrBtHfgStrQ(str, &idx, txt);

        csrBtHfgCrlf(str, &idx);
        CsrBtHfgSendCmDataReq(inst, idx, str);

    }
}

/* Send SMS arrival notification */
void CsrBtHfgSendAtCsrSms(HfgInstance_t *inst,
                     CsrUint16 index,
                     CsrCharString *number,
                     CsrCharString *name)
{
   if(!(CsrBtHfgGetMainInstance(inst)->hfgConfig & CSR_BT_HFG_CNF_DISABLE_C2C))
   {
        CsrUint8 *str;
        CsrUint16 idx = CSR_BT_HFG_TMP_STRING;

        str = CsrPmemAlloc(idx);
        CsrMemSet(str, 0, idx);
        idx = 0;
        csrBtHfgCrlf(str, &idx);

        csrBtHfgStr(str, &idx, CSR_BT_HFG_STR_C2C_SMS_ARRIVE);

        csrBtHfgInt(str, &idx, index);
        csrBtHfgCom(str, &idx, TRUE);

        csrBtHfgStrQ(str, &idx, number);
        csrBtHfgCom(str, &idx, TRUE);

        csrBtHfgStrQ(str, &idx, name);

        csrBtHfgCrlf(str, &idx);
        CsrBtHfgSendCmDataReq(inst, idx, str);
   }
}

/* Send unsolicited CSR-2-CSR SMS text */
void CsrBtHfgSendAtCsrGetSms(HfgInstance_t *inst, CsrCharString *txt)
{
    if(!(CsrBtHfgGetMainInstance(inst)->hfgConfig & CSR_BT_HFG_CNF_DISABLE_C2C))
    {
        CsrUint8 *str;
        CsrUint16 idx = (CsrUint16)(50 + CsrStrLen((char*)txt));

        /* We need a much bigger AT-command for SMS'es */
        str = CsrPmemAlloc(idx);

        CsrMemSet(str, 0, idx);
        csrBtHfgCrlf(str, &idx);

        csrBtHfgStr(str, &idx, CSR_BT_HFG_STR_C2C_SMS_TXT);
        csrBtHfgStrQ(str, &idx, txt);

        csrBtHfgCrlf(str, &idx);
        CsrBtHfgSendCmDataReq(inst, idx, str);
    }
}
#endif /* CSR_BT_INSTALL_HFG_C2C */

/* Send clip without name */
void CsrBtHfgSendAtClip(HfgInstance_t *inst,
                   CsrCharString *number,
                   CsrCharString *name,
                   CsrUint8 numType)
{
    CsrUint8 *str;
    CsrUint16 idx = CSR_BT_HFG_TMP_STRING;

    str = CsrPmemAlloc(idx);
    CsrMemSet(str, 0, idx);
    idx = 0;
    csrBtHfgCrlf(str, &idx);

    csrBtHfgStr(str, &idx, CSR_BT_HFG_STR_CLIP);

    csrBtHfgStrQ(str, &idx, number);
    csrBtHfgCom(str, &idx, TRUE);

    csrBtHfgInt(str, &idx, numType);

    if(name != NULL)
    {
        /* subaddr is blank */
        csrBtHfgCom(str, &idx, TRUE);
        /* satype is blank */
        csrBtHfgCom(str, &idx, TRUE);
        /* remember a ',' before the name */
        csrBtHfgCom(str, &idx, TRUE);

        csrBtHfgStrQ(str, &idx, name);
    }

    csrBtHfgCrlf(str, &idx);
    CsrBtHfgSendCmDataReq(inst, idx, str);
}

/* Send voice recognition activation */
void CsrBtHfgSendAtBvra(HfgInstance_t *inst, CsrUint8 bvra)
{
    CsrUint8 *str;
    CsrUint16 idx = 20;

    str = CsrPmemAlloc(idx);
    CsrMemSet(str, 0, idx);
    idx = 0;
    csrBtHfgCrlf(str, &idx);

    csrBtHfgStr(str, &idx, CSR_BT_HFG_STR_BVRA);
    csrBtHfgInt(str, &idx, bvra);

    csrBtHfgCrlf(str, &idx);
    CsrBtHfgSendCmDataReq(inst, idx, str);
}

/* Send network operator without string */
void CsrBtHfgSendAtCops(HfgInstance_t *inst,
                   CsrUint8 mode,
                   CsrCharString *name)
{
    CsrUint8 *str;
    CsrUint16 idx = CSR_BT_HFG_TMP_STRING;

    str = CsrPmemAlloc(idx);
    CsrMemSet(str, 0, idx);
    idx = 0;
    csrBtHfgCrlf(str, &idx);

    csrBtHfgStr(str, &idx, CSR_BT_HFG_STR_COPS);

    csrBtHfgInt(str, &idx, mode);

    csrBtHfgCom(str, &idx, TRUE);

    if(name != NULL)
    {

        csrBtHfgStr(str, &idx, (CsrUint8*)"0,");

        csrBtHfgStrQ(str, &idx, name);
    }
    else
    {
        csrBtHfgCom(str, &idx, TRUE);
    }

    csrBtHfgCrlf(str, &idx);
    CsrBtHfgSendCmDataReq(inst, idx, str);
}

#ifdef CSR_BT_INSTALL_HFG_C2C
/* Send local/own CSR-2-CSR supported features */
void CsrBtHfgSendAtCsrSf(HfgInstance_t *inst)
{
    if(!(CsrBtHfgGetMainInstance(inst)->hfgConfig & CSR_BT_HFG_CNF_DISABLE_C2C))
    {
        CsrUint8 i;
        CsrUint8 *str;
        CsrUint16 idx = 50;

        str = CsrPmemAlloc(idx);
        CsrMemSet(str, 0, idx);
        idx = 0;
        csrBtHfgCrlf(str, &idx);

        csrBtHfgStr(str, &idx, CSR_BT_HFG_STR_C2C_SF);

        /* Add all indicators. Also add commas as long as there are
         * subsequence indicators */
        for(i=1; i<CSR_BT_C2C_NUMBER_OF_INDICATORS; i++)
        {
            CsrUint8 indNumberLimit = CSR_BT_C2C_NUMBER_OF_INDICATORS-1;
            if (!inst->remoteSampleRateSupport)
            {/* Adjust number of indicators to send depending on whether the remote device
                has support for sample rate indicator */
                indNumberLimit--;
            }
            if ((CSR_BT_C2C_ADPCM_IND == i) &&
                !CsrBtHfgGetMainInstance(inst)->adpcmLocalSupported)
            {
                /* Specifically announce lack of support for OptiStream if
                   not supported by chip */
                csrBtHfgInt(str, &idx, 0);
            }
            else
            {
                if (((CSR_BT_C2C_SAMPLE_RATE_IND == i) && (inst->remoteSampleRateSupport)) ||
                    (CSR_BT_C2C_SAMPLE_RATE_IND != i))
                {/* Do not send sample rate settings if the remote device does not support that feature */
                    csrBtHfgInt(str, &idx, inst->ind.loc2c[i]);
                }
            }

            csrBtHfgCom(str, &idx, (CsrBool)(i < indNumberLimit));
        }

        csrBtHfgCrlf(str, &idx);
        CsrBtHfgSendCmDataReq(inst, idx, str);
    }
}

/* Send C2C SF update */
void CsrBtHfgSendAtCsr(HfgInstance_t *inst, CsrUint8 ind, CsrUint8 value)
{
    if(!(CsrBtHfgGetMainInstance(inst)->hfgConfig & CSR_BT_HFG_CNF_DISABLE_C2C))
    {
        CsrUint8 *str;
        CsrUint16 idx = 50;

        str = CsrPmemAlloc(idx);
        CsrMemSet(str, 0, idx);
        idx = 0;
        csrBtHfgCrlf(str, &idx);

        csrBtHfgStr(str, &idx, CSR_BT_HFG_STR_C2C_SF_UPDATE);
        csrBtHfgStr(str, &idx, (CsrUint8*)"(");
        csrBtHfgInt(str, &idx, ind);
        csrBtHfgCom(str, &idx, TRUE);
        csrBtHfgInt(str, &idx, value);
        csrBtHfgStr(str, &idx, (CsrUint8*)")");

        csrBtHfgCrlf(str, &idx);
        CsrBtHfgSendCmDataReq(inst, idx, str);
    }
}

/* Send C2C FN negotiation */
void CsrBtHfgSendAtCsrFn(HfgInstance_t *inst, CsrUint8 ind, CsrUint8 value)
{
    if(!(CsrBtHfgGetMainInstance(inst)->hfgConfig & CSR_BT_HFG_CNF_DISABLE_C2C))
    {
        CsrUint8 *str;
        CsrUint16 idx = 50;

        str = CsrPmemAlloc(idx);
        CsrMemSet(str, 0, idx);
        idx = 0;
        csrBtHfgCrlf(str, &idx);

        csrBtHfgStr(str, &idx, CSR_BT_HFG_STR_C2C_FN);
        csrBtHfgStr(str, &idx, (CsrUint8*)"(");
        csrBtHfgInt(str, &idx, ind);
        csrBtHfgCom(str, &idx, TRUE);
        if (CSR_BT_C2C_ADPCM_IND == ind)
        {/* Make sure to send a valid value */
            if ((value & inst->ind.c2c[CSR_BT_C2C_ADPCM_IND]) || (value == 0))
            {/* the value is compatible */
                csrBtHfgInt(str, &idx, value);
            }
            else
            {/* Find a compatible value */
                CsrUint8 localValue = inst->ind.loc2c[CSR_BT_C2C_ADPCM_IND] & inst->ind.c2c[CSR_BT_C2C_ADPCM_IND];
                /* make sure only one value is sent, and not a combination of acceptable values... */
                if (localValue & CSR_BT_C2C_ADPCM_4BIT_VALUE)
                {/* try the highest value for starters */
                    localValue = CSR_BT_C2C_ADPCM_4BIT_VALUE;
                }
                else
                {
                    if (localValue & CSR_BT_C2C_ADPCM_2BIT_VALUE)
                    {
                        localValue = CSR_BT_C2C_ADPCM_2BIT_VALUE;
                    }
                    else
                    {
                        localValue = CSR_BT_C2C_ADPCM_OFF_VALUE;
                    }
                }
                csrBtHfgInt(str, &idx, localValue);
            }
        }
        else
        {
            csrBtHfgInt(str, &idx, value);
        }
        csrBtHfgStr(str, &idx, (CsrUint8*)")");

        if ( (ind == CSR_BT_C2C_ADPCM_IND) && (inst->remoteSampleRateSupport) )
        {/* Audio settings and remote supports sample rate: send two indicator values */
            CsrUint8 localValue = inst->ind.loc2c[CSR_BT_C2C_SAMPLE_RATE_IND] & inst->ind.c2c[CSR_BT_C2C_SAMPLE_RATE_IND];

            if ((inst->sampleRateUsed & inst->ind.c2c[CSR_BT_C2C_SAMPLE_RATE_IND]) != 0)
            {
                localValue = inst->sampleRateUsed;
            }
            csrBtHfgCom(str, &idx, TRUE);
            csrBtHfgStr(str, &idx, (CsrUint8*)"(");
            csrBtHfgInt(str, &idx, CSR_BT_C2C_SAMPLE_RATE_IND);
            csrBtHfgCom(str, &idx, TRUE);
            /* make sure only one value is sent, and not a combination of acceptable values... */
            if (localValue & CSR_BT_C2C_SAMPLE_16KHZ_VALUE)
            {/* try the highest value for starters */
                localValue = CSR_BT_C2C_SAMPLE_16KHZ_VALUE;
            }
            /* make sure there is coherence between what is negotiated and what is preferred... */
            inst->sampleRateUsed = localValue;
            csrBtHfgInt(str, &idx, localValue);
            csrBtHfgStr(str, &idx, (CsrUint8*)")");
        }

        csrBtHfgCrlf(str, &idx);
        CsrBtHfgSendCmDataReq(inst, idx, str);
    }
}
#endif /* CSR_BT_INSTALL_HFG_C2C */



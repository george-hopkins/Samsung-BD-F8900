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

/* AT command table structure */
typedef struct
{
    CsrUint8        token;       /* Numeric token */
    char          *at;          /* AT command string */
    HfgAtHandler_t func;        /* AT handler function */
} HfgAtCheck_t;



static const HfgAtCheck_t hfgAtCheck[] =
{
    { CSR_BT_ATA_TOKEN,             "ATA",           CsrBtHfgAtAtaExec      },
    { CSR_BT_VGM_TOKEN,             "AT+VGM=",       CsrBtHfgAtVgmSet       },
    { CSR_BT_VGS_TOKEN,             "AT+VGS=",       CsrBtHfgAtVgsSet       },
    { CSR_BT_CKPD_TOKEN,            "AT+CKPD=",      CsrBtHfgAtCkpdSet      },
    { CSR_BT_CHUP_TOKEN,            "AT+CHUP",       CsrBtHfgAtChupExec     },
    { CSR_BT_CHLD_QUESTION_TOKEN,   "AT+CHLD=?",     CsrBtHfgAtChldTest     },
    { CSR_BT_CHLD_REQUEST_TOKEN,    "AT+CHLD=",      CsrBtHfgAtChldSet      },
    { CSR_BT_ATD_MEMORY_DIAL_TOKEN, "ATD>",          CsrBtHfgAtAtdMemoryExec},
    { CSR_BT_ATD_TOKEN,             "ATD",           CsrBtHfgAtAtdNumberExec},
    { CSR_BT_BLDN_TOKEN,            "AT+BLDN",       CsrBtHfgAtBldnExec     },
    { CSR_BT_BRSF_TOKEN,            "AT+BRSF=",      CsrBtHfgAtBrsfSet      },
    { CSR_BT_BTRH_QUERY_TOKEN,      "AT+BTRH?",      CsrBtHfgAtBtrhRead     },
    { CSR_BT_BTRH_REQUEST_TOKEN,    "AT+BTRH=",      CsrBtHfgAtBtrhSet      },
    { CSR_BT_CIND_STATUS_TOKEN,     "AT+CIND?",      CsrBtHfgAtCindRead     },
    { CSR_BT_CIND_SUPPORT_TOKEN,    "AT+CIND=?",     CsrBtHfgAtCindTest     },
    { CSR_BT_CMER_TOKEN,            "AT+CMER=",      CsrBtHfgAtCmerSet      },
    { CSR_BT_CMEE_REQUEST_TOKEN,    "AT+CMEE=",      CsrBtHfgAtCmeeSet      },
    { CSR_BT_CCWA_TOKEN,            "AT+CCWA=",      CsrBtHfgAtCcwaSet      },
    { CSR_BT_CLIP_TOKEN,            "AT+CLIP=",      CsrBtHfgAtClipSet      },
    { CSR_BT_CLCC_QUERY_TOKEN,      "AT+CLCC",       CsrBtHfgAtClccExec     },
    { CSR_BT_CNUM_QUESTION_TOKEN,   "AT+CNUM",       CsrBtHfgAtCnumExec     },
    { CSR_BT_COPS_QUESTION_TOKEN,   "AT+COPS?",      CsrBtHfgAtCopsRead     },
    { CSR_BT_COPS_REQUEST_TOKEN,    "AT+COPS=",      CsrBtHfgAtCopsSet      },
    { CSR_BT_BVRA_TOKEN,            "AT+BVRA=",      CsrBtHfgAtBvraSet      },
#ifdef CSR_BT_INSTALL_HFG_C2C
    { CSR_BT_CSR_TOKEN,             "AT+CSR=",       CsrBtHfgAtCsrSet       },
    { CSR_BT_CSR_BAT_TOKEN,         "AT+CSRBATT=",   CsrBtHfgAtCsrBatSet    },
    { CSR_BT_CSR_GETSMS_TOKEN,      "AT+CSRGETSMS=", CsrBtHfgAtCsrGetsmsSet },
    { CSR_BT_CSR_PWR_TOKEN,         "AT+CSRPWR=",    CsrBtHfgAtCsrPwrSet    },
    { CSR_BT_CSR_SF_TOKEN,          "AT+CSRSF=",     CsrBtHfgAtCsrSfSet     },
    { CSR_BT_CSR_FN_TOKEN,          "AT+CSRFN=",     CsrBtHfgAtCsrFnSet     },
#endif
    { CSR_BT_BIA_TOKEN,             "AT+BIA=",       CsrBtHfgAtBiaSet       },
    { CSR_BT_NREC_TOKEN,            "AT+NREC=",      CsrBtHfgAtNrecSet      },
    { CSR_BT_VTS_TOKEN,             "AT+VTS=",       CsrBtHfgAtVtsSet       },
    { CSR_BT_BINP_TOKEN,            "AT+BINP=",      CsrBtHfgAtBinpSet      },
    { CSR_BT_BAC_TOKEN,             "AT+BAC=",       CsrBtHfgAtBacSet       },
    { CSR_BT_BCC_TOKEN,             "AT+BCC",        CsrBtHfgAtBccExec      },
    { CSR_BT_BCS_TOKEN,             "AT+BCS=",       CsrBtHfgAtBcsSet       },
    { CSR_BT_OTHER_TOKEN,           NULL,            NULL                   }
};


#define TO_UPPER(x)        ((((x) >= 'a') && ((x) <= 'z')) ? ((x) & 0xDF) : (x))

#ifdef CSR_BT_INSTALL_HFG_CONFIG_ATCMD_HANDLING
/* Utility handler functions. Moved here because they need access to AT
 * cmd table */
/* AT cmd setting - all commands*/
void CsrBtHfgXConfigAtCmdHandling(HfgMainInstance_t *inst)
{
    CsrBtHfgConfigAtcmdHandlingReq *prim;
    CsrUintFast8 i;


    prim = (CsrBtHfgConfigAtcmdHandlingReq*)inst->msg;

    if(inst->atParser ==  CSR_BT_HFG_AT_MODE_USER_CONFIG)
    {
        for (i = 0; i<CSR_BT_HFG_NUMBER_AT_CMD; i++)
        {/* 1 byte at a time */
            inst->HfgSendAtToApp[i] = prim->bitwiseIndicators[i];
        }

        CsrBtHfgSendHfgConfigAtCmdHandlingCfm(prim->phandle, 0 /*CSR_BT_HFG_AT_CMD_SUCCESS*/, prim->bitwiseIndicators, CSR_BT_HFG_NUMBER_AT_CMD);
        prim->bitwiseIndicators = NULL;
    }
    else
    { /* Wrong AT mode*/
        CsrUint8  *pTemp = CsrPmemAlloc(CSR_BT_HFG_NUMBER_AT_CMD); /* Wil be freed when data received and handled */

        CsrMemCpy(pTemp,(CsrUint8 *)&(inst->HfgSendAtToApp),CSR_BT_HFG_NUMBER_AT_CMD);

        CsrBtHfgSendHfgConfigAtCmdHandlingCfm(prim->phandle, 2 /*CSR_BT_HFG_AT_CMD_WRONG_ATMODE*/,pTemp,CSR_BT_HFG_NUMBER_AT_CMD);
    }
}
#endif

#ifdef CSR_BT_INSTALL_HFG_CONFIG_SINGLE_ATCMD
/* AT cmd setting - single command */
void CsrBtHfgXConfigSingleAtcmd(HfgMainInstance_t *inst)
{
    CsrBtHfgConfigSingleAtcmdReq *prim;

    prim = (CsrBtHfgConfigSingleAtcmdReq*)inst->msg;

    if(inst->atParser ==  CSR_BT_HFG_AT_MODE_USER_CONFIG)
    {
        CsrUint8 numberOfEntries = 0;
        while(hfgAtCheck[numberOfEntries].at != NULL)
            {
                numberOfEntries++;
            }

        if (prim->idx <= numberOfEntries)
        {   /* Remember index 0 means 'unknown AT command' */
            CsrUint8 index = prim->idx/8; /* byte number to update */
            CsrUint8 tmpVal = 1 << (prim->idx % 8); /* bit position in that byte */

            if (prim->sendToApp)
            {/* Set the bit */
                inst->HfgSendAtToApp[index] |= tmpVal;
            }
            else
            { /* Reset that single bit */
                inst->HfgSendAtToApp[index] &= ~tmpVal;
            }

            CsrBtHfgSendHfgConfigSingleAtCmdCfm(prim->phandle, 0 /*CSR_BT_HFG_AT_CMD_SUCCESS*/);
        }
        else
        { /* Out of bounds */
            CsrBtHfgSendHfgConfigSingleAtCmdCfm(prim->phandle, 1 /*CSR_BT_HFG_AT_CMD_OUT_OF_BOUNDS*/);
        }
    }
    else
    { /* Wrong AT mode*/
        CsrBtHfgSendHfgConfigSingleAtCmdCfm(prim->phandle, 2 /*CSR_BT_HFG_AT_CMD_WRONG_ATMODE*/);
    }
}
#endif

/* Local: AT compare function. Perform a very safe comparison of the
 * atStr against maStr, with full range checking etc. Set 'index' to
 * the first occurrence of any trailing non-whitespace
 * (ie. values). */
static CsrBool csrBtHfgCompare(char *atStr, CsrUint16 atLen, char *maStr, CsrUint16 *index)
{
    CsrUintFast16 atIdx;
    CsrUintFast16 maIdx;
    CsrUintFast16 maLen;

    /* Dummy checks*/
    if((atStr == NULL) ||
       (maStr == NULL) ||
       (index == NULL) ||
       (atLen < 3) ||
       (atLen < CsrStrLen(maStr)))
    {
        return FALSE;
    }

    atIdx = 0;
    maIdx = 0;
    *index = 0;
    maLen = (CsrUint16)CsrStrLen(maStr);

    /* Scan characters */
    while((maIdx < maLen) &&
          (atIdx < atLen))
    {
        /* Skip whitespace */
        if((atStr[atIdx] == ' ') ||
           (atStr[atIdx] == '\n') ||
           (atStr[atIdx] == '\r') ||
           (atStr[atIdx] == '\t'))
        {
            atIdx++;
            continue;
        }

        /* Does AT command match? */
        if(TO_UPPER(atStr[atIdx]) != maStr[maIdx])
        {
            /* Mismatch, break out now */
            return FALSE;
        }

        /* Next set of character */
        atIdx++;
        maIdx++;
    }

    /* Skip any last whitespaces */
    while((atIdx < atLen) &&
          ( (atStr[atIdx] == ' ') ||
            (atStr[atIdx] == '\n') ||
            (atStr[atIdx] == '\r') ||
            (atStr[atIdx] == '\t')) )
    {
        atIdx++;
    }

    /* If index has reached the end, it's an actual match */
    if(maIdx == maLen)
    {
        *index = (CsrUint16)atIdx;
        return TRUE;
    }
    else
    {
        *index = 0;
        return FALSE;
    }
}

/* Local: Fault-tolerant parsing of raw ATs to get token - 'at' and
 * 'atLen' is the AT input string and length. At exit 'index' is the
 * index of the first parameter (unless OTHER_TOKEN is returned) */
static CsrUint8 csrBtHfgGetIndex(HfgInstance_t *inst,
                      char *at,
                      CsrUint16 atLen,
                      CsrUint16 *index)
{
    CsrUintFast8 i;

    /* Scan the AT command table for a token */
    i = 0;
    while(hfgAtCheck[i].at != NULL)
    {
        if(csrBtHfgCompare(at,
                      atLen,
                      hfgAtCheck[i].at,
                      index))
        {
            break;
        }
        i++;
    }

    /* The 'i' will always be valid at this point */
    return (CsrUint8)i;
}

/* This is the function that receives the actual raw AT command.  Find
 * out if token index and call the appropiate handle if necessary */
void CsrBtHfgAtInterpret(HfgInstance_t *inst)
{
    CsrUint16 voff;
    CsrUint8 index;
    CsrBtHfgAtParserMode atmode;

    atmode = CsrBtHfgGetMainInstance(inst)->atParser;

    if(atmode == CSR_BT_HFG_AT_MODE_TRANSPARENT)
    {
        /* Transparent AT mode - send upstream to app now */
        CsrBtHfgSendHfgAtCmdInd(inst, FALSE);
    }
    else
    {
        /* Full or semi mode - decode */
        CsrUint8 bitValue, byteVal;
        index = csrBtHfgGetIndex(inst,
                            (char*)inst->atCmd,
                            inst->atLen,
                            &voff);
        /* Use 'index + 1' because index 0 is used for 'unknown commands' when deciding what commands to forward to app. */
        byteVal = (index+1)/8;
        bitValue = 1 << ((index +1)% 8);
        if (((CsrBtHfgGetMainInstance(inst)->HfgSendAtToApp[byteVal] & bitValue) == 0) && (atmode == CSR_BT_HFG_AT_MODE_USER_CONFIG) &&
            (!inst->pendingSearch) && (inst->atState != At0Idle_s) &&
            (inst->state != ServiceSearch_s) && (inst->state != Connect_s))
        {/* If SLC established and marked as application specific, do not hanlde;
            just forward the command to the application */
            CsrBtHfgSendHfgAtCmdInd(inst, FALSE);
        }
        else
        {
            /* Execute command if known */
            if(hfgAtCheck[index].token != CSR_BT_OTHER_TOKEN)
            {
                /* Note that "voff" is the char index of the first AT
                 * argument. Pass the address in order to save a lot of
                 * variables in the AT handlers... */
                hfgAtCheck[index].func(inst,
                                       &voff,
                                       (CsrBool)(inst->state == ServiceSearch_s ? TRUE : FALSE));
            }
            else
            {
                CsrBool handleUnknownCmd = (atmode == CSR_BT_HFG_AT_MODE_USER_CONFIG) && ((CsrBtHfgGetMainInstance(inst)->HfgSendAtToApp[0] & 0x01) == 1);

                if ( ((atmode != CSR_BT_HFG_AT_MODE_SEMI) && (atmode != CSR_BT_HFG_AT_MODE_USER_CONFIG)) ||
                     (inst->pendingSearch) || (inst->atState == At0Idle_s) ||
                     (inst->state == ServiceSearch_s) || (inst->state == Connect_s) || (handleUnknownCmd) )
                {
                    /* Full mode or SLC not fully established yet, and unknown AT command,
                       or unknown command and unknown commands marked as not to be forwarded to the
                       application: send error */
                    CsrBtHfgSendAtResponse(inst, CSR_BT_CME_OPERATION_NOT_SUPPORTED);
                }
                else
                {
                    /* Semi mode and SLC already established, send upstream to app */
                    CsrBtHfgSendHfgAtCmdInd(inst,
                                       (CsrBool)(inst->ind.other[CSR_BT_HFG_SET_CMEE] == 1 ? TRUE : FALSE));
                }
            }
        }
    }

    /* Free AT buffer */
    CsrPmemFree(inst->atCmd);
    inst->atCmd = NULL;
    inst->atLen = 0;
}



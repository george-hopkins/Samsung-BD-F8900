/**********************************************************************
* File Name   : Bluetooth_SC.c
* Description : This file contains functions and variables for
*                handling BT profiles
*	author 			:	Kim Jin Cheol	(SW Platform., HYUNDAI MOBIS)
*	Updates			:
***********************************************************************/
#include "csr_bt_declare_tasks.h"
#include "csr_bt_pac_lib.h"
#include "csr_bt_pac_prim.h"
#include "Bluetooth_Common.h"
#include "Bluetooth_PBAC.h"
#include "csr_pmem.h"

/*****************************************************************************
* Function Name : init_sc()
******************************************************************************
* Desription    :
* Parameter     :
* Return        :
******************************************************************************/
void init_pbac(void **gash)
{
	PbacAppInst *inst;

	*gash = CsrPmemAlloc(sizeof(PbacAppInst));
	CsrMemSet(*gash, 0, sizeof(PbacAppInst));
	inst = (PbacAppInst *)*gash;

	inst->qid = TESTQUEUE;

}

/*****************************************************************************
* Function Name : deinit_sc()
******************************************************************************
* Desription    :
* Parameter     :
* Return        :
******************************************************************************/
void deinit_pbac(void **gash)
{
	PbacAppInst *inst;

	inst = (PbacAppInst *)*gash;

	CsrPmemFree(inst);
}

/*****************************************************************************
* Function Name : handlePbacPrim()
******************************************************************************
* Desription    :
* Parameter     :
* Return        :
******************************************************************************/
void handlePbacPrim(PbacAppInst  *inst)
{
    CsrPrim    *primType;
    primType = (CsrPrim *) inst->recvMsgP;

	switch (*primType)
	{

        case CSR_BT_PAC_CONNECT_CFM:
			{
				CsrBtPacConnectCfm *pMsg =(CsrBtPacConnectCfm *) inst->recvMsgP;
        	}
            break;

        case CSR_BT_PAC_AUTHENTICATE_IND:
			{
				CsrBtPacAuthenticateInd *pMsg = (CsrBtPacAuthenticateInd *)inst->recvMsgP;
        	}
            break;

        case CSR_BT_PAC_PULL_PB_CFM:
			{
				CsrBtPacPullPbCfm *pMsg = (CsrBtPacPullPbCfm *)inst->recvMsgP;
        	}
            break;

        case CSR_BT_PAC_PULL_PB_IND:
			{
				CsrBtPacPullPbInd *pMsg = (CsrBtPacPullPbInd *)inst->recvMsgP;
			 	CsrBtPacPullPbResSend(FALSE);
        	}
            break;

        case CSR_BT_PAC_SET_FOLDER_CFM:
            /* Fall through */
        case CSR_BT_PAC_SET_BACK_FOLDER_CFM:
            /* Fall through */
        case CSR_BT_PAC_SET_ROOT_FOLDER_CFM:
			{
				CsrBtPacSetFolderCfm *pMsg = (CsrBtPacSetFolderCfm *) inst->recvMsgP;
        	}
            break;

        case CSR_BT_PAC_PULL_VCARD_LIST_CFM:
			{
				CsrBtPacPullVcardListCfm *pMsg =(CsrBtPacPullVcardListCfm *) inst->recvMsgP;
        	}
            break;

        case CSR_BT_PAC_PULL_VCARD_LIST_IND:
		   {
          		CsrBtPacPullVcardListInd *pMsg = (CsrBtPacPullVcardListInd *)inst->recvMsgP;
        	}
            break;

        case CSR_BT_PAC_PULL_VCARD_ENTRY_CFM:
			{
				CsrBtPacPullVcardEntryCfm *pMsg = (CsrBtPacPullVcardEntryCfm *) inst->recvMsgP;
        	}
            break;

        case CSR_BT_PAC_PULL_VCARD_ENTRY_IND:
			{
				CsrBtPacPullVcardEntryInd *pMsg = (CsrBtPacPullVcardEntryInd *)inst->recvMsgP;
			  	CsrBtPacPullVcardEntryResSend(FALSE);
        	}
            break;

        case CSR_BT_PAC_ABORT_CFM:
			{
				CsrBtPacAbortCfm *pMsg = (CsrBtPacAbortCfm *)inst->recvMsgP;
        	}
			break;

        case CSR_BT_PAC_DISCONNECT_IND:
			{
				CsrBtPacDisconnectInd *pMsg =(CsrBtPacDisconnectInd *) inst->recvMsgP;
        	}
            break;

		default:
		{
			/* Unexpected primitive received */
			CL_API_LOGPrintf("Unhandled SC primitive: 0x%04x\n", *primType);
			CsrBtPacFreeUpstreamMessageContents(CSR_BT_PAC_PRIM, inst->recvMsgP);
			break;
		}
	}

}



//////////////////////////////////////////////



//////////////////////////////


void Pac_connect(CsrBtDeviceAddr   theDestination,CsrBool  theAuthtorize,CsrUint8         *password)
{
   CsrBtPacConnectReqSend(TESTQUEUE,
                           PAC_MAX_OBEX_PACKET_SIZE,
                           theDestination,
                           theAuthtorize,
                           0, NULL,
                           (CsrUint16)CsrStrLen((char *) password),
                           (CsrUint8 *) CsrStrDup((char *) password),
                           NULL,
                           0, /*length*/
                           0, /*count*/
                           0, /*windowSize*/
                           TRUE);
}


void Pac_abort()
{
   CsrBtPacAbortReqSend();
}


void Pac_disconnect()
{
	CsrBtPacDisconnectReqSend(TRUE);


//        CsrBtPacCancelConnectReqSend();

}

#if 0

#define VCARD_VERSION   (1 << 0)
#define VCARD_N  (1 << 2) /* N Structured Presentation of Name) */
#define VCARD_TEL   (1 << 7)
#define VCARD_EMAIL (1 << 8)
#define VCARD_TITLE (1 << 12)
#define VCARD_ROLE  (1 << 13)


CsrBool filterState(CsrUint8 cKey)
{
    CsrUint32    val, tmpval;
    CsrUint8     offset = 0; /* offset into pInst->filter[] */
    char        *s;


    /*
     * When you change this, REMEMBER
     * to sync with filterMenu() below.
     */

    switch (cKey) {
    case 'e':
    case 'E':
        val = VCARD_EMAIL;
        s = "email";
        break;

    case 'n':
    case 'N':
        val = VCARD_N;
        s = "name";
        break;

    case 'p':
    case 'P':
        val = VCARD_TEL;
        s = "phone number";
        break;

    case 'r':
    case 'R':
        val = VCARD_ROLE;
        s = "role";
        break;

    case 't':
    case 'T':
        val = VCARD_TITLE;
        s = "title";
        break;

    case 'v':
    case 'V':
        val = VCARD_VERSION;
        s = "VCARD version";
        break;

    default:
        printf("Invalid filter key.\n");
        return TRUE;
    }

    /*
     * Find offset into pInst->filter[] and flip
     * the appropriate bit in that 8 bit quantity.
     * Find offset by shifting value bitpattern 8
     * positions to the right.  While the result
     * is positive (only one bit set), the
     * pInst->filter offset is too small.
     *
     * Note that while CsrUint32 is really too small
     * to store all bits, bits 29 through 63 are
     * reserved and not used for qualification tests.
     */
    tmpval = val;
    while (tmpval >> 8) {
        tmpval >>= 8;
        offset++;
    }
    val >>= 8 * offset;

    /* Now flip/xor the bit. */
    filter[offset] ^= val;

    return TRUE;
}

#endif

void  Pac_PullPbReqSend(char * name ,CsrBtPacSrcType      src,CsrUint8         filter[8],CsrUint16 nMaxSize)
{

	CsrUcs2String *pName = NULL;
	CsrUint32 nNameLen;
	const unsigned char FILTER[8] = {0x87, 0, 0, 0x1F, 0, 0, 0, 0};//jckim_120130->exclude image

	nNameLen = CsrStrLen(name)*2 + 2;
	if(nNameLen > 2)
	{
		pName = CsrUtf82Ucs2ByteString((CsrUint8*)name);
	}
		
	pName = CsrUtf82Ucs2ByteString((CsrUint8*)name);

    CsrBtPacPullPbReqSend(pName,
                     src,
                     (CsrUint8*)FILTER,
                     CSR_BT_PAC_FORMAT_VCARD2_1,
                     nMaxSize,
                     0,
                     FALSE);
}


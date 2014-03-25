/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* IMPORTANT NOTICE: This file contains generic functions for the CSR
 * Synegy BlueCore bootstrap system. You should NOT touch anything
 * here. Instead use the 'csr_bootstrap_specific.c/h' files to set up
 * the bootstrap for your particular system */

#include "csr_usr_config.h"
#include "csr_bootstrap_generic.h"
#include "csr_bootstrap_specific.h"
#include "csr_file.h"
#include "csr_log_text.h"
#include "csr_macro.h"
#include "csr_bccmd_lib.h"
#include "csr_tm_bluecore_bootstrap.h"

/* Select sane defaults */
#ifndef CSR_BOOTSTRAP_LINE_BUFFER
#define CSR_BOOTSTRAP_LINE_BUFFER 2000
#endif
#ifndef CSR_MAX_FRIENDLY_NAME_LEN
#define CSR_MAX_FRIENDLY_NAME_LEN                       (50)
#endif

/* Misc constants */
#define CSR_BOOTSTRAP_VARID_WARM_RESET            ((CsrUint16) 0x4002)

/* Special ps-keys */
#define CSR_BOOTSTRAP_PSKEY_UART_ACK_TIMEOUT      ((CsrUint16) 0x01c4)
#define CSR_BOOTSTRAP_PSKEY_UART_BITRATE          ((CsrUint16) 0x01EA)
#define CSR_BOOTSTRAP_PSKEY_UART_BAUDRATE         ((CsrUint16) 0x01BE)
#define CSR_BOOTSTRAP_PSKEY_ANA_FREQ              ((CsrUint16) 0x01FE)
#define CSR_BOOTSTRAP_PSKEY_ANA_FTRIM             ((CsrUint16) 0x01F6)
#define CSR_BOOTSTRAP_PSKEY_BDADDR                ((CsrUint16) 0x0001)
#define CSR_BOOTSTRAP_PSKEY_DEVICE_NAME           ((CsrUint16) 0x0108)
#define CSR_BOOTSTRAP_PSKEY_HOST_INTERFACE        ((CsrUint16) 0x01F9)

/* Transport values */
#define CSR_BOOTSTRAP_PSKEY_HOST_INTERFACE_BCSP   ((CsrUint16) 0x0001)
#define CSR_BOOTSTRAP_PSKEY_HOST_INTERFACE_H2_USB ((CsrUint16) 0x0002)
#define CSR_BOOTSTRAP_PSKEY_HOST_INTERFACE_H4DS   ((CsrUint16) 0x0007)
#define CSR_BOOTSTRAP_PSKEY_HOST_INTERFACE_SDIO   ((CsrUint16) 0x0009)

/* Command list */
typedef struct csrBootstrapCmdTag
{
    struct csrBootstrapCmdTag *next;          /* Next key */
    CsrUint16                  key;             /* Numeric PS Key identifier */
    CsrUint16                 *data;            /* Data of the PS Key to be set */
    CsrUint16                  length;          /* length of data in CsrUint16s */
} csrBoostrapCmd;

/* Internal bootstrap instance */
typedef struct
{
    csrBoostrapCmd *ps_head;
    csrBoostrapCmd *ps_tail;
    CsrBool         bc7up;
    CsrUint32       bitRate;
    CsrUint16       freq;
    CsrUint16       freqTrim;
    CsrUint8        address[6];
    CsrUtf8String   name[CSR_MAX_FRIENDLY_NAME_LEN + 1];
    CsrUint16       iface;
} csrBootstrapInst;

/* Get bootstrap context */
static csrBootstrapInst *_csr_bootstrap_inst = NULL;
static csrBootstrapInst *csrBootstrapGetInst(void *bootContext)
{
    if (bootContext != NULL)
    {
        return (csrBootstrapInst *) bootContext;
    }
    else if (_csr_bootstrap_inst != NULL)
    {
        return _csr_bootstrap_inst;
    }
    else
    {
        _csr_bootstrap_inst = (csrBootstrapInst *) CsrPmemZalloc(sizeof(csrBootstrapInst));
        return _csr_bootstrap_inst;
    }
}

/* Scan cached PSkeys and see if key exists */
static csrBoostrapCmd *csrBootstrapGetPskey(void *bootContext, CsrUint16 key)
{
    csrBootstrapInst *inst;
    csrBoostrapCmd *cmd;
    inst = csrBootstrapGetInst(bootContext);

    for (cmd = inst->ps_head; cmd != NULL; cmd = cmd->next)
    {
        if (cmd->key == key)
        {
            return cmd;
        }
    }
    return NULL;
}

/* Allocate new PSkey at end of list */
static csrBoostrapCmd *csrBootstrapAppendPskey(void *bootContext)
{
    csrBootstrapInst *inst;

    inst = csrBootstrapGetInst(bootContext);
    if (inst->ps_head == NULL)
    {
        inst->ps_head = CsrPmemZalloc(sizeof(csrBoostrapCmd));
        inst->ps_tail = inst->ps_head;
    }
    else
    {
        inst->ps_tail->next = CsrPmemZalloc(sizeof(csrBoostrapCmd));
        inst->ps_tail = inst->ps_tail->next;
    }

    return inst->ps_tail;
}

/* Calculate host interface baud-rate PS-Key based on the requested bitRate. */
static CsrUint16 csrBootstrapBitToBaud(CsrUint32 bitRate)
{
    return (CsrUint16) ((((bitRate / 100) * 4096) + 5000) / 10000);
}

/* Apply function based user settings */
#define CSR_BCSP_MAX_PACKAGE_LENGTH (512 + 4) /* 0xfff is the maximum payload af a BCSP package.
                                                  but we use the max ACL data package length as an
                                                  indicator of what will actially be used.
                                                  PSKEY_H_HC_FC_MAX_ACL_PKT_LEN has a MAX of 512*/
#define CSR_BCSP_PROCESS_TIME_MS (8)
#define CSR_BCSP_UART_BIT_PER_BYTE (11)
static void csrBootstrapApplyUserSettings(void *bootContext)
{
    csrBootstrapInst *inst;
    csrBoostrapCmd *cmd;
    inst = csrBootstrapGetInst(bootContext);

    if (inst->bitRate != 0)
    {
        if (inst->bc7up)
        {
            /* BC7 and newer uses bitrate. Bit rate is 32bits, 2 xap words */
            cmd = csrBootstrapGetPskey(inst, CSR_BOOTSTRAP_PSKEY_UART_BITRATE);
            if (cmd == NULL)
            {
                cmd = csrBootstrapAppendPskey(inst);
                cmd->key = CSR_BOOTSTRAP_PSKEY_UART_BITRATE;
                cmd->length = 2; /* num xap words */
                cmd->data = CsrPmemAlloc(cmd->length * sizeof(CsrUint16));
            }
            cmd->data[0] = (CsrUint16) (inst->bitRate >> 16);
            cmd->data[1] = (CsrUint16) (inst->bitRate & 0x0000FFFF);
        }
        else
        {
            /* BC6 and older uses baudrate. Baud rate is 16bits, 1 xap word */
            CsrUint16 baud;
            baud = csrBootstrapBitToBaud(inst->bitRate);
            cmd = csrBootstrapGetPskey(inst, CSR_BOOTSTRAP_PSKEY_UART_BAUDRATE);
            if (cmd == NULL)
            {
                cmd = csrBootstrapAppendPskey(inst);
                cmd->key = CSR_BOOTSTRAP_PSKEY_UART_BAUDRATE;
                cmd->length = 1; /* num xap words */
                cmd->data = CsrPmemAlloc(cmd->length * sizeof(CsrUint16));
            }
            cmd->data[0] = baud;
        }
    }

    if (inst->freq != 0)
    {
        /* Crystal frequency is 16bits, 1 xap word */
        cmd = csrBootstrapGetPskey(inst, CSR_BOOTSTRAP_PSKEY_ANA_FREQ);
        if (cmd == NULL)
        {
            cmd = csrBootstrapAppendPskey(inst);
            cmd->key = CSR_BOOTSTRAP_PSKEY_ANA_FREQ;
            cmd->length = 1; /* num xap words */
            cmd->data = CsrPmemAlloc(cmd->length * sizeof(CsrUint16));
        }
        cmd->data[0] = inst->freq;
    }

    if (inst->freqTrim != 0)
    {
        /* Frequency trim is 16bits, 1 xap word */
        cmd = csrBootstrapGetPskey(inst, CSR_BOOTSTRAP_PSKEY_ANA_FTRIM);
        if (cmd == NULL)
        {
            cmd = csrBootstrapAppendPskey(inst);
            cmd->key = CSR_BOOTSTRAP_PSKEY_ANA_FTRIM;
            cmd->length = 1; /* num xap words */
            cmd->data = CsrPmemAlloc(cmd->length * sizeof(CsrUint16));
        }
        cmd->data[0] = inst->freqTrim;
    }

    if ((inst->address[0] != 0) ||
        (inst->address[1] != 0) ||
        (inst->address[2] != 0) ||
        (inst->address[3] != 0) ||
        (inst->address[4] != 0) ||
        (inst->address[5] != 0))
    {
        /* Device address is 64 bits, 4 xap words */
        cmd = csrBootstrapGetPskey(inst, CSR_BOOTSTRAP_PSKEY_BDADDR);
        if (cmd == NULL)
        {
            cmd = csrBootstrapAppendPskey(inst);
            cmd->key = CSR_BOOTSTRAP_PSKEY_BDADDR;
            cmd->length = 4; /* num xap words */
            cmd->data = CsrPmemAlloc(cmd->length * sizeof(CsrUint16));
        }
        cmd->data[0] = (CsrUint16) (inst->address[3]);
        cmd->data[1] = (CsrUint16) (inst->address[4] << 8 | inst->address[5]);
        cmd->data[2] = (CsrUint16) (inst->address[2]);
        cmd->data[3] = (CsrUint16) (inst->address[0] << 8 | inst->address[1]);
    }

    if (inst->name[0] != '\0')
    {
        /* Device name is variable length with two characters per
         * xap-word. Always do a full free/alloc/patch of the name. */
        CsrUint16 len;
        cmd = csrBootstrapGetPskey(inst, CSR_BOOTSTRAP_PSKEY_DEVICE_NAME);
        if (cmd != NULL)
        {
            CsrPmemFree(cmd->data);
        }
        else
        {
            cmd = csrBootstrapAppendPskey(inst);
            cmd->key = CSR_BOOTSTRAP_PSKEY_DEVICE_NAME;
        }

        /* Two chars per word. If remainder, add modulus to make it even */
        len = CSRMIN((CsrUint16) CsrUtf8StringLengthInBytes(inst->name), CSR_MAX_FRIENDLY_NAME_LEN);
        len = (len / sizeof(CsrUint16)) + (len % sizeof(CsrUint16));
        cmd->length = len;
        cmd->data = CsrPmemZalloc(len * sizeof(CsrUint16));
        CsrMemCpy((CsrCharString *) cmd->data,
            inst->name,
            CSRMIN(CsrUtf8StringLengthInBytes(inst->name), CSR_MAX_FRIENDLY_NAME_LEN));
    }

    if (inst->iface != 0)
    {
        /* Host interface is 16bits, 1 xap word */
        cmd = csrBootstrapGetPskey(inst, CSR_BOOTSTRAP_PSKEY_HOST_INTERFACE);
        if (cmd == NULL)
        {
            cmd = csrBootstrapAppendPskey(inst);
            cmd->key = CSR_BOOTSTRAP_PSKEY_HOST_INTERFACE;
            cmd->length = 1; /* num xap words */
            cmd->data = CsrPmemAlloc(cmd->length * sizeof(CsrUint16));
        }
        cmd->data[0] = inst->iface;
    }
}

/* Convert PSkey table to BCCMD commands */
static CsrUint8 **csrBootstrapConvertTable(void *bootContext, CsrUint16 *entries)
{
    csrBootstrapInst *inst;
    CsrUint16 i;
    csrBoostrapCmd *psk;
    CsrUint8 **table;

    inst = csrBootstrapGetInst(bootContext);

    for (i = 0, psk = inst->ps_head; psk != NULL; psk = psk->next)
    {
        i++;
    }
    i++; /* Make room for the warm reset */
    *entries = i;
    table = (CsrUint8 **) CsrPmemZalloc(sizeof(CsrUint8 * *) * i);

    for (i = 0; inst->ps_head; i++)
    {
        psk = inst->ps_head->next;
        table[i] = CsrTmBlueCoreBuildBccmdPsSetMsg(inst->ps_head->key,
            CSR_BCCMD_STORES_PSRAM,
            inst->ps_head->length,
            inst->ps_head->data);
        CsrPmemFree(inst->ps_head->data);
        CsrPmemFree(inst->ps_head);
        inst->ps_head = psk;
    }
    inst->ps_head = NULL;
    inst->ps_tail = NULL;

    table[i] = CsrTmBlueCoreBuildBccmdSetMsg(CSR_BOOTSTRAP_VARID_WARM_RESET,
        0, NULL);

    return table;
}

static CsrBool csrBootstrapIsFill(CsrCharString ch)
{
    if ((ch == ' ') || (ch == '\f') || (ch == '\r') || (ch == '\n') || (ch == '\t'))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/* Parse PSR line. Format is "&kkkk=pppp...0", where k is key, p is
 * payload and 0 is the zero-terminator. Whitespace may appear
 * anywhere. Anything after a '#' is ignored. Returns TRUE if line is
 * valid, FALSE otherwise */
static CsrBool csrBootstrapParsePsrLine(CsrCharString *line, CsrUint16 *key,
    CsrUint16 *data, CsrUint16 *count)
{
    CsrUint32 idx;
    CsrUint16 elm;
    CsrUint8 wi;

    *count = 0;
    *key = 0;
    elm = 0;
    wi = 0;

    /* Sanity */
    if (line == NULL)
    {
        return FALSE;
    }

    for (idx = 0; /* use breaks in code */; idx++)
    {
        if (csrBootstrapIsFill(line[idx]))
        {
            /* Whitespace. Ignore and next. */
            continue;
        }
        else if ((line[idx] == '#') ||
                 (line[idx] == '\0') ||
                 (line[idx] == '/'))
        {
            /* Comment or end of line. Stop. */
            break;
        }
        else if ((line[idx] >= '0') && (line[idx] <= '9') && (wi < 4))
        {
            elm = (CsrUint16) ((elm << 4) | (line[idx] - '0'));
            wi++;
        }
        else if ((line[idx] >= 'a') && (line[idx] <= 'f') && (wi < 4))
        {
            elm = (CsrUint16) ((elm << 4) | (line[idx] - 'a' + 0xa));
            wi++;
        }
        else if ((line[idx] >= 'A') && (line[idx] <= 'F') && (wi < 4))
        {
            elm = (CsrUint16) ((elm << 4) | (line[idx] - 'A' + 0xa));
            wi++;
        }
        else if ((line[idx] == '&') && (wi == 0) && (*key == 0))
        {
            /* Key start token. Ignore and next. */
            continue;
        }
        else if ((line[idx] == '=') && (wi == 4) && (*key == 0))
        {
            /* Payload start token and key complete. Store and fetch
             * next */
            *key = elm;
            wi = 0;
            elm = 0;
            continue;
        }
        else
        {
            /* Invalid character. Stop. */
            break;
        }

        /* Complete payload word */
        if ((*key != 0) && (wi == 4))
        {
            data[*count] = elm;
            (*count)++;
            wi = 0;
            elm = 0;
        }
    }

    /* Entry only valid if key and payload existed and terminated
     * after a full word */
    if ((*key != 0) && (*count > 0) && (wi == 0))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/* Add PSKEY */
void CsrBootstrapAddPskey(void *bootContext, CsrUint16 key, CsrUint16 *data, CsrUint16 length)
{
    csrBootstrapInst *inst;
    csrBoostrapCmd *cmd;

    inst = csrBootstrapGetInst(bootContext);
    cmd = csrBootstrapAppendPskey(inst);
    cmd->next = NULL;
    cmd->key = key;
    cmd->data = data;
    cmd->length = length;
}

/* High-level constant-string PSR parser */
void CsrBootstrapParsePsrConstant(void *bootContext, const CsrCharString *string)
{
    CsrCharString *line;
    CsrUint32 index;
    CsrUint32 outdex;
    CsrUint16 *data;
    CsrUint16 length;
    CsrUint16 key;

    if (string == NULL)
    {
        return;
    }

    index = 0;
    outdex = 0;
    line = CsrPmemZalloc(CSR_BOOTSTRAP_LINE_BUFFER);
    while (string[index] != '\0')
    {
        line[outdex] = string[index];
        if ((string[index] == '\n') ||
            (string[index] == '\r'))
        {
            data = CsrPmemZalloc(outdex / 2);
            if (csrBootstrapParsePsrLine(line, &key, data, &length))
            {
                CsrBootstrapAddPskey(bootContext, key, data, length);
            }
            else
            {
                CsrPmemFree(data);
            }
            outdex = 0;
            CsrMemSet(line, 0, CSR_BOOTSTRAP_LINE_BUFFER);
        }
        else
        {
            outdex++;
        }
        index++;
    }
    CsrPmemFree(line);
}

/* High-level PSR file parser */
void CsrBootstrapParsePsrFile(void *bootContext, const CsrCharString *filename)
{
    CsrFileHandle *fh;
    CsrResult res;
    CsrCharString *line;
    CsrUint32 index;
    CsrSize got;
    CsrUint16 *data;
    CsrUint16 length;
    CsrUint16 key;

    if (filename == NULL)
    {
        return;
    }

    res = CsrFileOpen(&fh, (const CsrUtf8String *) filename,
        CSR_FILE_OPEN_FLAGS_READ_ONLY,
        CSR_FILE_PERMS_USER_READ);
    /* Scan file */
    if (res == CSR_RESULT_SUCCESS)
    {
        line = CsrPmemZalloc(CSR_BOOTSTRAP_LINE_BUFFER);
        while (res == CSR_RESULT_SUCCESS)
        {
            index = 0;
            CsrMemSet(line, 0, CSR_BOOTSTRAP_LINE_BUFFER);

            /* Scan line */
            while (res == CSR_RESULT_SUCCESS
                   && index < (CSR_BOOTSTRAP_LINE_BUFFER - 1))
            {
                res = CsrFileRead(&line[index], (CsrSize) 1, fh, &got);
                if ((res == CSR_RESULT_SUCCESS) && (got == 1))
                {
                    index++;
                    if ((line[index - 1] == '\n') ||
                        (line[index - 1] == '\r'))
                    {
                        break;
                    }
                }
            }

            data = CsrPmemZalloc(index / 2);
            if (csrBootstrapParsePsrLine(line, &key, data, &length))
            {
                CsrBootstrapAddPskey(bootContext, key, data, length);
            }
            else
            {
                CsrPmemFree(data);
            }
        }
        CsrPmemFree(line);
        CsrFileClose(fh);
    }
}

/* Set host interface transport */
void CsrBootstrapSetInterface(void *bootContext, CsrUint16 hostInterface)
{
    csrBootstrapInst *inst;
    inst = csrBootstrapGetInst(bootContext);

    switch (hostInterface)
    {
        case TRANSPORT_TYPE_BCSP:
            inst->iface = CSR_BOOTSTRAP_PSKEY_HOST_INTERFACE_BCSP;
            break;

        case TRANSPORT_TYPE_USB:
            inst->iface = CSR_BOOTSTRAP_PSKEY_HOST_INTERFACE_H2_USB;
            break;

        case TRANSPORT_TYPE_H4_DS:
            inst->iface = CSR_BOOTSTRAP_PSKEY_HOST_INTERFACE_H4DS;
            break;

        case TRANSPORT_TYPE_TYPE_A:
            inst->iface = CSR_BOOTSTRAP_PSKEY_HOST_INTERFACE_SDIO;
            break;

        default:
            break;
    }
}

void CsrBootstrapSetBitRate(void *bootContext, CsrUint32 bitRate)
{
    csrBootstrapInst *inst;
    inst = csrBootstrapGetInst(bootContext);
    inst->bitRate = bitRate;
}

/* Set BlueCore friendly name */
void CsrBootstrapSetFriendlyName(void *bootContext, const CsrUtf8String *name)
{
    csrBootstrapInst *inst;
    inst = csrBootstrapGetInst(bootContext);
    CsrMemSet(inst->name, 0, CSR_MAX_FRIENDLY_NAME_LEN + 1);
    if (name != NULL)
    {
        CsrUtf8StrNCpy(inst->name, name, CSR_MAX_FRIENDLY_NAME_LEN + 1);
    }
}

/* Set Bluetooth address */
void CsrBootstrapSetLocalBtAddress(void *bootContext, const CsrUint8 address[6])
{
    csrBootstrapInst *inst;
    inst = csrBootstrapGetInst(bootContext);
    CsrMemCpy(&inst->address, address, 6);
}

/* Set crystal frequency */
void CsrBootstrapSetFrequency(void *bootContext, CsrUint16 frequency)
{
    csrBootstrapInst *inst;
    inst = csrBootstrapGetInst(bootContext);
    inst->freq = frequency;
}

/* Set crystal frequency trim */
void CsrBootstrapSetFreqTrim(void *bootContext, CsrUint16 freqTrim)
{
    csrBootstrapInst *inst;
    inst = csrBootstrapGetInst(bootContext);
    inst->freqTrim = freqTrim;
}

/* Framework callback function. This is the main bootstrap
 * function. We are expected to return a byte array of BCCMDs with the
 * combined bootstrap */
CsrUint8 **CsrTmBlueCoreGetBootstrap(CsrUint16 buildId, CsrUint16 *entries)
{
    csrBootstrapInst *inst;
    CsrUint8 **table;
    inst = csrBootstrapGetInst(NULL);

    /* Set default value to support most <bc7 without adding them to the table of known build ids. */
    if (buildId >= 6817)
    {
        inst->bc7up = TRUE;
    }
    else
    {
        inst->bc7up = FALSE;
    }
    CsrBootstrapSpecific((void *) inst, buildId, &(inst->bc7up));
    csrBootstrapApplyUserSettings((void *) inst);
    table = csrBootstrapConvertTable((void *) inst, entries);

    /* done with the data unless having chip manager reset support,
       in which case the framework might call us again */
#if !defined(CSR_CHIP_MANAGER_ENABLE) && !defined(CSR_BLUECORE_ONOFF)
    CsrPmemFree(inst);
    _csr_bootstrap_inst = NULL;
#endif

    return table;
}

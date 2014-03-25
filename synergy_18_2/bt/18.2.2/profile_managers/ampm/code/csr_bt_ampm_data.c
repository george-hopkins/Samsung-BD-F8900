/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_ampm_main.h"
#include "csr_log_text.h"

/* Constant HMAC keys */
static const CsrUint8 key_id_gamp[4] = { 0x67, 0x61, 0x6d, 0x70 };
static const CsrUint8 key_id_802b[4] = { 0x38, 0x30, 0x32, 0x62 };
static const CsrUint8 key_id_ecma[4] = { 0x65, 0x63, 0x6d, 0x61 };

/* Log helpers */
#ifdef CSR_LOG_ENABLE
static const CsrLogSubOrigin csrBtAmpmSubOrigins[] =
{
    {0, "CSR_BT_AMPM_DATA"}
};
#endif

/* Log an AMP/GAMP key */
#ifdef CSR_LOG_ENABLE
static void csrBtAmpmKeyLogText(CsrUint8 *key, CsrUint16 keyLength)
{
    CsrCharString *txt;
    CsrUint16 i = 0;
    CsrUint16 j;

    /* Each octet in the key takes up 3 chars ("xx ") */
    txt = (CsrCharString*)CsrPmemAlloc((keyLength * 3) + 2);

    /* Unroll key */
    for(j=0; j<keyLength; j++)
    {
        CsrUint8 cv;
        cv = (key[j] & 0xF0) >> 4;
        txt[i++] = (CsrUint8)(cv > 9
                              ? cv - 10 + 'A'
                              : cv + '0');
        cv = key[j] & 0x0F;
        txt[i++] = (CsrUint8)(cv > 9
                              ? cv - 10 + 'A'
                              : cv + '0');

        txt[i++] = ' ';
    }
    txt[i++] = '\0';

    CSR_LOG_TEXT_DEBUG((CSR_BT_AMPM_IFACEQUEUE, 0,
                        "(0x%04x) %s", keyLength, txt));
    CsrPmemFree(txt);
}
#endif

static CsrBool csrBtAmpmDataKeysMatchesBtAddress(const void *pCriteria,
                                                 const void *pListEntry)
{
    BD_ADDR_T bd_addr;
    const CsrBtAmpmDataDeviceKeys *pBtDeviceKeys;

    CSR_BT_AMPM_ASSERT(pCriteria != NULL);
    CSR_BT_AMPM_ASSERT(pListEntry != NULL);

    bd_addr = *((const BD_ADDR_T *) pCriteria);
    pBtDeviceKeys = (const CsrBtAmpmDataDeviceKeys *) pListEntry;

    return (bd_addr_eq((const BD_ADDR_T *) &(pBtDeviceKeys->bd_addr),
                       (const BD_ADDR_T *) &bd_addr));
}


static CsrBtAmpmDataDeviceKeys *csrBtAmpmDataKeysGetByBdaddr(CsrBtAmpmInstance * inst,
                                                             BD_ADDR_T bd_addr)
{
    return (CsrBtAmpmDataDeviceKeys *)CsrBtAmpmListFindMatchingMember(
        inst->pBTDeviceKeysAnchor,
        csrBtAmpmDataKeysMatchesBtAddress,
        (const void *) &bd_addr);
}


static void csrBtAmpmDataScFlushKeys(CsrBtAmpmInstance *inst,
                                     BD_ADDR_T *addr)
{
    static const CsrUint8 ampTypes[] =
        { /* Do not write SSP key */
            CSR_BT_SC_DB_ENTRY_AMP_KEY_GAMP,
            CSR_BT_SC_DB_ENTRY_AMP_KEY_DOT11,
            CSR_BT_SC_DB_ENTRY_AMP_KEY_UWB
        };
    CsrUint8 i;

    for(i=0; i<sizeof(ampTypes); i++)
    {
        CsrBtScDatabaseReqSend(CSR_BT_AMPM_IFACEQUEUE,
                               addr,
                               CSR_BT_SC_DB_OP_WRITE,
                               ampTypes[i],
                               HCI_KEY_TYPE_UNKNOWN,
                               0, /* length */
                               NULL); /* data */
    }
}

static void csrBtAmpmDataClearAmpKey(CsrBtAmpmDataAmpKey *key)
{
    CSR_BT_AMPM_ASSERT(key != NULL);

    CsrPmemFree(key->key);
    key->key = NULL;
    key->keySize = 0;
    key->keyType = HCI_KEY_TYPE_UNKNOWN;
}

static void csrBtAmpmDataScStoreKey(CsrBtAmpmInstance *inst,
                                    BD_ADDR_T *addr,
                                    CsrUint8 entry,
                                    CsrUint8 keyType,
                                    CsrUint8 length,
                                    CsrUint8 *key,
                                    CsrBool valid)
{
    CsrBtAmpmDataDeviceKeys *deviceKeys;

    /* Will overwrite. No need to check validity */
    deviceKeys = csrBtAmpmDataKeysGetByBdaddr(inst, *addr);
    if(deviceKeys == NULL)
    {
        /* This is a new entry - create and store in local db */
        deviceKeys = CsrPmemZalloc(sizeof(CsrBtAmpmDataDeviceKeys));
        deviceKeys->bd_addr = *addr;
        CsrBtAmpmListAddHead(inst->pBTDeviceKeysAnchor,
                             (CsrBtAmpmGenListMember *) deviceKeys);
    }

    switch(entry)
    {
        case CSR_BT_SC_DB_ENTRY_AMP_KEY_SSP:
            if(valid && (length == SIZE_LINK_KEY))
            {
                deviceKeys->sspKeyType = keyType;
                CsrMemCpy(deviceKeys->sspKey, key, length);
            }
            else
            {
                CsrMemSet(deviceKeys->sspKey, 0, SIZE_LINK_KEY);
                deviceKeys->sspKeyType = HCI_KEY_TYPE_UNKNOWN;
            }
            break;

        case CSR_BT_SC_DB_ENTRY_AMP_KEY_GAMP:
            if(valid && (length == CSR_BT_AMP_LINK_KEY_SIZE_GAMP))
            {
                deviceKeys->gampKeyType = keyType;
                CsrMemCpy(deviceKeys->gampKey, key, length);
            }
            else
            {
                CsrMemSet(deviceKeys->gampKey, 0, CSR_BT_AMP_LINK_KEY_SIZE_GAMP);
                deviceKeys->gampKeyType = HCI_KEY_TYPE_UNKNOWN;
            }
            break;

        case CSR_BT_SC_DB_ENTRY_AMP_KEY_DOT11:
            csrBtAmpmDataClearAmpKey(&(deviceKeys->ampKeys[CSR_BT_AMP_AMP_CONTROLLER_TYPE_802_11 - 1]));

            if(valid && (length == CSR_BT_AMP_LINK_KEY_SIZE_802_11))
            {
                deviceKeys->ampKeys[CSR_BT_AMP_AMP_CONTROLLER_TYPE_802_11 - 1].key = (CsrUint8*)CsrPmemAlloc(length);
                CsrMemCpy(deviceKeys->ampKeys[CSR_BT_AMP_AMP_CONTROLLER_TYPE_802_11 - 1].key,
                          key,
                          length);
                deviceKeys->ampKeys[CSR_BT_AMP_AMP_CONTROLLER_TYPE_802_11 - 1].keyType = keyType;
                deviceKeys->ampKeys[CSR_BT_AMP_AMP_CONTROLLER_TYPE_802_11 - 1].keySize = length;
            }
            break;

        case CSR_BT_SC_DB_ENTRY_AMP_KEY_UWB:
            csrBtAmpmDataClearAmpKey(&(deviceKeys->ampKeys[CSR_BT_AMP_AMP_CONTROLLER_TYPE_ECMA_368 - 1]));

            if(valid && (length == CSR_BT_AMP_LINK_KEY_SIZE_ECMA_368))
            {
                deviceKeys->ampKeys[CSR_BT_AMP_AMP_CONTROLLER_TYPE_ECMA_368 - 1].key = (CsrUint8*)CsrPmemAlloc(length);
                CsrMemCpy(deviceKeys->ampKeys[CSR_BT_AMP_AMP_CONTROLLER_TYPE_ECMA_368 - 1].key,
                          key,
                          length);
                deviceKeys->ampKeys[CSR_BT_AMP_AMP_CONTROLLER_TYPE_ECMA_368 - 1].keyType = keyType;
                deviceKeys->ampKeys[CSR_BT_AMP_AMP_CONTROLLER_TYPE_ECMA_368 - 1].keySize = length;
            }
            break;
    }
}

void CsrBtAmpmDataScTrigger(CsrBtAmpmInstance *inst,
                            BD_ADDR_T *addr)
{
    static const CsrUint8 ampTypes[] =
        { CSR_BT_SC_DB_ENTRY_AMP_KEY_SSP,
          CSR_BT_SC_DB_ENTRY_AMP_KEY_GAMP,
          CSR_BT_SC_DB_ENTRY_AMP_KEY_DOT11,
          CSR_BT_SC_DB_ENTRY_AMP_KEY_UWB };
    CsrUint8 i;

    /* If our local database contains the entry for this device we
     * need not read it from SC_DB as the two are guaranteed to be in
     * sync. No need to check validity as that will be in sync too */
    if(csrBtAmpmDataKeysGetByBdaddr(inst, *addr) != NULL)
    {
        return;
    }

    /* Force SC to read ALL database entries for us. This operation is
     * stateless, so fire off read request for each type of key we
     * want to know about */
    for(i=0; i<sizeof(ampTypes); i++)
    {
        CsrBtScDatabaseReqSend(CSR_BT_AMPM_IFACEQUEUE,
                               addr,
                               CSR_BT_SC_DB_OP_READ,
                               ampTypes[i],
                               HCI_KEY_TYPE_UNKNOWN,
                               0, /* length */
                               NULL); /* data */
    }
}

void CsrBtAmpmDataInit(CsrBtAmpmInstance * inst)
{
    CSR_BT_AMPM_ASSERT(inst->pBTDeviceKeysAnchor == NULL);

#ifdef CSR_LOG_ENABLE
    CsrLogTextRegister(CsrSchedTaskQueueGet(),
                       "CSR_BT_AMPM",
                       CSR_ARRAY_SIZE(csrBtAmpmSubOrigins),
                       csrBtAmpmSubOrigins);
#endif

    /* Perform any initialisation required */
    inst->pBTDeviceKeysAnchor = CsrBtAmpmListCreateAnchor();
}


void CsrBtAmpmDataDeinit(CsrBtAmpmInstance * inst)
{
    if(inst->pBTDeviceKeysAnchor != NULL)
    {
        CSR_BT_AMPM_ASSERT(inst->pBTDeviceKeysAnchor != NULL);
        while (!CsrBtAmpmListIsEmpty(inst->pBTDeviceKeysAnchor))
        {
            /* This function will automatically remove the item from
             * the list, so don't do it here */
            CsrBtAmpmDataKeysDelete(inst,
                                     (CsrBtAmpmDataDeviceKeys*)inst->pBTDeviceKeysAnchor->head);
        }
        CsrBtAmpmListDestroyAnchor(&(inst->pBTDeviceKeysAnchor));
    }
}

void CsrBtAmpmDataSetTimersEnableState(
    CsrBtAmpmInstance * inst,
    CsrBool enabledState)
{
    inst->ampmTimersEnabled = enabledState;
}


void CsrBtAmpmDataKeysDelete(CsrBtAmpmInstance * inst,
                             CsrBtAmpmDataDeviceKeys *devKeys)
{
    CsrUint8 i = 0;

    CSR_BT_AMPM_ASSERT(devKeys != NULL);

    /* Free any specific amp type keys */
    for (i = 0; i <= (CSR_BT_AMPM_MAX_LOCAL_AMPS - 1); i++)
    {
        CsrPmemFree(devKeys->ampKeys[i].key);
    }
    (void)CsrBtAmpmListRemoveMatchingMember(inst->pBTDeviceKeysAnchor,
                                            CsrBtAmpmListEntryMatchesPtr,
                                            devKeys);
    CsrPmemFree(devKeys);
}

/* New link key from the DM. */
void CsrBtAmpmDataDmAmpmKeyNotificationInd(CsrBtAmpmInstance * inst,
                                           DM_AMPM_LINK_KEY_NOTIFICATION_IND_T * prim)
{
    CsrBtAmpmDataDeviceKeys *devKeys;
    CsrUint8 i;

    CSR_BT_AMPM_ASSERT(prim != NULL);

#ifdef CSR_LOG_ENABLE
    /* Log the BR/EDR SSP link key */
    CSR_LOG_TEXT_DEBUG((CSR_BT_AMPM_IFACEQUEUE, 0, "SSP key"));
    csrBtAmpmKeyLogText(prim->key, SIZE_LINK_KEY);
#endif

    /* Will overwrite, so no need to check validity */
    devKeys = csrBtAmpmDataKeysGetByBdaddr(inst, prim->bd_addr);
    if (NULL != devKeys)
    {
        /* If we've got an existing set of keys then this new key must
         * be as a result of a "change connection link key" so we also
         * need to disconnect any physical links to the peer device as
         * the key is now out of date. */
        CsrBtAmpmPhyLink *pPhyLink = CsrBtAmpmPhyGetByBdaddr(inst,
                                                             prim->bd_addr);
        if (NULL != pPhyLink)
        {
            CsrBtAmpmPhyDisconnect(inst, pPhyLink);
        }
        CsrBtAmpmDataKeysDelete(inst, devKeys);
    }

    /* Flush all AMP keys from the SC_DB */
    csrBtAmpmDataScFlushKeys(inst, (BD_ADDR_T*)&prim->bd_addr);

    /* Store the SSP key */
    devKeys = CsrPmemZalloc(sizeof(CsrBtAmpmDataDeviceKeys));
    devKeys->bd_addr = prim->bd_addr;
    devKeys->sspKeyType = prim->key_type;
    CsrMemCpy(devKeys->sspKey, prim->key, SIZE_LINK_KEY);

    /* Initialise the specific and generic AMP keys (invalid) */
    devKeys->gampKeyType = HCI_KEY_TYPE_UNKNOWN;
    for (i = 0; i <= (CSR_BT_AMPM_MAX_LOCAL_AMPS - 1); i++)
    {
        devKeys->ampKeys[i].keyType = HCI_KEY_TYPE_UNKNOWN;
        devKeys->ampKeys[i].key = NULL;
        devKeys->ampKeys[i].keySize = 0;
    }

    /* Store keys in local AMPM database */
    CsrBtAmpmListAddHead(inst->pBTDeviceKeysAnchor,
                         (CsrBtAmpmGenListMember *) devKeys);
}

/* When an AMP connection is first set up over a PAL, we need to
 * either generate -- or regenerate -- the GAMP key. This function
 * will do so, and store the key in the SC_DB */
static void CsrBtAmpmDataGenerateGampKey(CsrBtAmpmInstance * inst,
                                         CsrBtAmpmDataDeviceKeys *devKeys)
{
    CsrBool h2;
    CsrBtAmpmHmacSha256ctx *pCtx;
    CsrUint8 W[CSR_BT_AMP_LINK_KEY_SIZE_GAMP];

    /* The initial GAMP key is the "SSP SSP" concatenation -- but only
     * use initial key, if no key exists already */
    if(devKeys->gampKeyType == HCI_KEY_TYPE_UNKNOWN)
    {
        (void)CsrMemCpy(W, devKeys->sspKey, SIZE_LINK_KEY);
        (void)CsrMemCpy(&(W[SIZE_LINK_KEY]), devKeys->sspKey, SIZE_LINK_KEY);
        devKeys->gampKeyType = devKeys->sspKeyType;

        /* Initial generation of GAMP key */
        h2 = TRUE;
    }
    else if(devKeys->gampKeyType == HCI_DEBUG_COMBINATION_KEY)
    {
        /* Debug keys are never regenerated */
        h2 = FALSE;
    }
    else
    {
        /* Regenerate GAMP key for all other purposes */
        h2 = TRUE;
    }

    /* Possibly Run the key through the h2 function */
    if(h2)
    {
        pCtx = CsrPmemAlloc(sizeof(*pCtx));
        CsrBtAmpmHmacSha256Init(pCtx, (unsigned char *) W, CSR_BT_AMP_LINK_KEY_SIZE_GAMP);
        CsrBtAmpmHmacSha256Update(pCtx, (const void *) key_id_gamp, 4);
        CsrBtAmpmHmacSha256Final((unsigned char *) (devKeys->gampKey), pCtx);
        CsrPmemFree(pCtx);

#ifdef CSR_LOG_ENABLE
        /* Log the GAMP key */
        CSR_LOG_TEXT_DEBUG((CSR_BT_AMPM_IFACEQUEUE, 0,  "Generate G-AMP key"));
        csrBtAmpmKeyLogText(devKeys->gampKey, CSR_BT_AMP_LINK_KEY_SIZE_GAMP);
#endif

        /* Flush keys in SC_DB and store new GAMP key */
        CsrBtScDatabaseReqSend(CSR_BT_AMPM_IFACEQUEUE,
                               (BD_ADDR_T*)&devKeys->bd_addr,
                               CSR_BT_SC_DB_OP_WRITE,
                               CSR_BT_SC_DB_ENTRY_AMP_KEY_GAMP,
                               devKeys->gampKeyType,
                               CSR_BT_AMP_LINK_KEY_SIZE_GAMP, /* length */
                               devKeys->gampKey); /* data */
    }
}

static CsrBtAmpmDataAmpKey *csrBtAmpmDataDuplicateKey(CsrBtAmpmDataAmpKey *key)
{
    CsrBtAmpmDataAmpKey *new;
    new = (CsrBtAmpmDataAmpKey*)CsrPmemAlloc(sizeof(CsrBtAmpmDataAmpKey));
    new->key = (CsrUint8*)CsrPmemAlloc(key->keySize);
    new->keySize = key->keySize;
    new->keyType = key->keyType;
    CsrMemCpy(new->key, key->key, key->keySize);

    return new;
}

CsrBtAmpmDataAmpKey *CsrBtAmpmDataKeysGetKeyForAmp(CsrBtAmpmInstance *inst,
                                                   BD_ADDR_T bd_addr,
                                                   CsrBtAmpmType ampType)
{
    CsrBtAmpmDataAmpKey *newKey;
    CsrBtAmpmDataAmpKey *returnKey;
    CsrBtAmpmDataDeviceKeys *devKeys;
    CsrUint8 dbEntry;
    CsrUint8 index;
    const CsrUint8 *seed;

    /* As there is no string defined for the Fake PAL we'll use the 802.11
     * as the default, so that a key is generated for a fake pal.
     * This is ok as nothing actually happens with the key in the PAL */
    index = (CsrUint8)((CSR_BT_AMP_AMP_CONTROLLER_TYPE_FAKE == ampType)
                       ? CSR_BT_AMPM_MAX_LOCAL_AMPS - 1
                       : ampType - 1);

    devKeys = csrBtAmpmDataKeysGetByBdaddr(inst, bd_addr);

    if(NULL != devKeys)
    {
        /* Check for invalid SSP key */
        if(devKeys->sspKeyType == HCI_KEY_TYPE_UNKNOWN)
        {
            return NULL;
        }

        /* If dedicated AMP key already exists, reuse it */
        if((devKeys->ampKeys[index].keyType != HCI_KEY_TYPE_UNKNOWN) &&
           (devKeys->ampKeys[index].keySize > 0) &&
           (devKeys->ampKeys[index].key != NULL))
        {
#ifdef CSR_LOG_ENABLE
            CSR_LOG_TEXT_DEBUG((CSR_BT_AMPM_IFACEQUEUE, 0, "Reuse existing specific AMP key"));
#endif
            return csrBtAmpmDataDuplicateKey(&(devKeys->ampKeys[index]));
        }

        /* Request GAMP key to be generated -- or regenerated if it
         * already exists. We do this because a _new_ AMP connection
         * is being set up */
        CsrBtAmpmDataGenerateGampKey(inst, devKeys);

        /* Prepare to generate specific AMP key */
        newKey = &(devKeys->ampKeys[index]);
        csrBtAmpmDataClearAmpKey(newKey);
        newKey->keyType = devKeys->gampKeyType;
        switch(ampType)
        {
            case CSR_BT_AMP_AMP_CONTROLLER_TYPE_802_11:
                newKey->keySize = CSR_BT_AMP_LINK_KEY_SIZE_802_11;
                dbEntry = CSR_BT_SC_DB_ENTRY_AMP_KEY_DOT11;
                seed = key_id_802b;
                break;

            case CSR_BT_AMP_AMP_CONTROLLER_TYPE_ECMA_368:
                newKey->keySize = CSR_BT_AMP_LINK_KEY_SIZE_ECMA_368;
                dbEntry = CSR_BT_SC_DB_ENTRY_AMP_KEY_UWB;
                seed = key_id_ecma;
                break;

            default:
                /* Fake PAL doesn't need a key, but to excersie the
                 * code, we borrow bits from 802.11 -- but don't
                 * save it in the database */
                newKey->keySize = CSR_BT_AMP_LINK_KEY_SIZE_802_11;
                dbEntry = CSR_BT_AMPM_SC_NO_DB_WRITE;
                seed = key_id_802b;
                break;
        }
        newKey->key = CsrPmemAlloc(newKey->keySize);

        if (newKey->keyType == CSR_BT_AMP_LINK_KEY_TYPE_DEBUG_COMBINATION_KEY)
        {
            /* Generate the specific key - debugging */
#ifdef CSR_LOG_ENABLE
            CSR_LOG_TEXT_DEBUG((CSR_BT_AMPM_IFACEQUEUE, 0, "Debug combination link key detected"));
#endif
            (void)CsrMemCpy(newKey->key, devKeys->gampKey, newKey->keySize);
        }
        else
        {
            /* Generate the specific, non-debug AMP key */
            CsrBtAmpmHmacSha256ctx *pCtx;
            CsrUint8 tempKey[CSR_BT_AMP_LINK_KEY_SIZE_GAMP];

#ifdef CSR_LOG_ENABLE
            CSR_LOG_TEXT_DEBUG((CSR_BT_AMPM_IFACEQUEUE, 0, "Generate new dedicated AMP key"));
#endif

            pCtx = (CsrBtAmpmHmacSha256ctx*)CsrPmemAlloc(sizeof(CsrBtAmpmHmacSha256ctx));
            CsrBtAmpmHmacSha256Init(pCtx, (unsigned char *)(devKeys->gampKey), CSR_BT_AMP_LINK_KEY_SIZE_GAMP);
            CsrBtAmpmHmacSha256Update(pCtx, (const void *)seed , 4);
            CsrBtAmpmHmacSha256Final((unsigned char *) tempKey, pCtx);
            CsrPmemFree(pCtx);

            (void)CsrMemCpy(newKey->key, tempKey, newKey->keySize);
        }

#ifdef CSR_LOG_ENABLE
        /* Log specific AMP type key */
        CSR_LOG_TEXT_DEBUG((CSR_BT_AMPM_IFACEQUEUE, 0, "Generate dedicated AMP key"));
        csrBtAmpmKeyLogText(newKey->key, newKey->keySize);
#endif
        returnKey = csrBtAmpmDataDuplicateKey(newKey);

        /* Test support for key invalidation: All keys are at least 16
         * bytes, so alter the 9th byte to change the key */
        if (CsrBtAmpmDataChangeAmpKey(inst, ampType))
        {
            CSR_LOG_TEXT_DEBUG((CSR_BT_AMPM_IFACEQUEUE, 0, "AMP key corruption will mangle dedicated key"));
            returnKey->key[8] += inst->ampmChangeAmpKeyCount;
        }

        /* Store non-corrupted key in SC_DB (if for a real PAL) */
        if(dbEntry != CSR_BT_AMPM_SC_NO_DB_WRITE)
        {
            CsrBtScDatabaseReqSend(CSR_BT_AMPM_IFACEQUEUE,
                                   &bd_addr,
                                   CSR_BT_SC_DB_OP_WRITE,
                                   dbEntry,
                                   newKey->keyType,
                                   newKey->keySize, /* length */
                                   newKey->key); /* data */
        }

        /* Return copy of possibly corrupted key */
        return returnKey;
    }

    return NULL;
}

void CsrBtAmpmDataSetLocalBtAddr(CsrBtAmpmInstance * inst,
                                 BD_ADDR_T bd_addr)
{
    bd_addr_copy(&(inst->local_bd_addr), &bd_addr);
}


BD_ADDR_T CsrBtAmpmDataGetLocalBtAddr(CsrBtAmpmInstance * inst)
{
    return (inst->local_bd_addr);
}

void CsrBtAmpmDataSetIgnoreSduOptions(CsrBtAmpmInstance * inst,
                                      CsrBool enableState,
                                      CsrUint8 sduId,
                                      CsrUint8 count)
{
    inst->ampmIgnoreSduEnabled = enableState;
    if (enableState)
    {
        inst->ampmIgnoreSduId = sduId;
        inst->ampmIgnoreSduTimes = count;
        inst->ampmIgnoreSduCount = 0;
    }
    else
    {
        inst->ampmIgnoreSduId = 0;
        inst->ampmIgnoreSduTimes = 0;
        inst->ampmIgnoreSduCount = 0;
    }
}

CsrBool CsrBtAmpmDataIgnoreSdu(CsrBtAmpmInstance * inst,
                               CsrUint8 sduId)
{
    if (inst->ampmIgnoreSduEnabled && inst->ampmIgnoreSduId == sduId &&
        (0 == inst->ampmIgnoreSduTimes ||
         inst->ampmIgnoreSduTimes > inst->ampmIgnoreSduCount))
    {
        inst->ampmIgnoreSduCount++;
        return (TRUE);
    }
    return (FALSE);
}


void CsrBtAmpmDataSetChangeAmpKeyOptions(CsrBtAmpmInstance * inst,
                                         CsrBool enableState,
                                         CsrBtAmpmType ampType,
                                         CsrUint8 count)
{
    inst->ampmChangeAmpKeyEnabled = enableState;
    if (enableState)
    {
        inst->ampmChangeAmpKeyType = ampType;
        inst->ampmChangeAmpKeyTimes = count;
        inst->ampmChangeAmpKeyCount = 0;
    }
    else
    {
        inst->ampmChangeAmpKeyType = 0;
        inst->ampmChangeAmpKeyTimes = 0;
        inst->ampmChangeAmpKeyCount = 0;
    }
}


CsrBool CsrBtAmpmDataChangeAmpKey(CsrBtAmpmInstance * inst,
                                  CsrBtAmpmType ampType)
{
    CSR_BT_AMPM_ASSERT(CSR_BT_AMPM_IS_VALID_TYPE(ampType));

    if (inst->ampmChangeAmpKeyEnabled && inst->ampmChangeAmpKeyType == ampType
        && (0 == inst->ampmChangeAmpKeyTimes ||
            inst->ampmChangeAmpKeyTimes > inst->ampmChangeAmpKeyCount))
    {
        inst->ampmChangeAmpKeyCount++;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void CsrBtAmpmDataSetRxSduControlOption(CsrBtAmpmInstance * inst,
                                        CsrBtAmpmControl rxSduControl,
                                        CsrSchedQid qid)
{
    inst->ampmRxSduQid = qid;
    inst->ampmRxSduControl = rxSduControl;
}

CsrBool CsrBtAmpmL2capCopyRxSduToAmpmSap(CsrBtAmpmInstance * inst)
{
    return ((CSR_BT_AMPM_CONTROL_RX_SDU_SEND_COPY & inst->ampmRxSduControl)
            == CSR_BT_AMPM_CONTROL_RX_SDU_SEND_COPY);
}


CsrBool CsrBtAmpmSendRxSduToAmpmSap(CsrBtAmpmInstance * inst)
{
    return ((CSR_BT_AMPM_CONTROL_RX_SDU_SEND_ORIGINAL & inst->ampmRxSduControl)
            == CSR_BT_AMPM_CONTROL_RX_SDU_SEND_ORIGINAL);
}

CsrBool CsrBtAmpmSendRxHciToTester(CsrBtAmpmInstance * inst)
{
    return ((CSR_BT_AMPM_CONTROL_RX_HCI_SEND_ORIGINAL & inst->ampmRxSduControl)
            == CSR_BT_AMPM_CONTROL_RX_HCI_SEND_ORIGINAL);
}

void CsrBtAmpmSendTestGetAssocReq(CsrBtAmpmInstance *inst,
                                  CsrBtDeviceAddr *addr,
                                  CsrUint8 signalId,
                                  CsrBtAmpmId ampId)
{
    CsrBtAmpmBtdev *pBtDevice;
    CsrMblk *mblk;
    pBtDevice = CsrBtAmpmBtdevGetByBdaddr(inst, *addr);

    if(pBtDevice != NULL)
    {
        mblk = CsrBtAmpmSduCreateAmpGetAmpAssocRequest(signalId, ampId);
        L2CA_DataWriteReq((l2ca_cid_t)pBtDevice->cid, 0, mblk);
    }
}

void CsrBtAmpmDataScHandler(CsrBtAmpmInstance *inst)
{
    switch(*(CsrUint16*)inst->msg)
    {
        case CSR_BT_SC_DATABASE_CFM:
            {
                CsrBtScDatabaseCfm *prim = (CsrBtScDatabaseCfm*)inst->msg;
                if(prim->opcode == CSR_BT_SC_DB_OP_READ)
                {
                    csrBtAmpmDataScStoreKey(inst,
                                            (BD_ADDR_T*)&prim->deviceAddr,
                                            prim->entry,
                                            prim->keyType,
                                            prim->length,
                                            prim->data,
                                            (CsrBool)(((prim->resultCode == CSR_BT_RESULT_CODE_SC_SUCCESS) &&
                                                       (prim->resultSupplier == CSR_BT_SUPPLIER_SC) &&
                                                       (prim->keyType != HCI_KEY_TYPE_UNKNOWN))
                                                      ? TRUE : FALSE));
                }
            }
            break;

        default:
            /* ignore */
            break;
    }
}

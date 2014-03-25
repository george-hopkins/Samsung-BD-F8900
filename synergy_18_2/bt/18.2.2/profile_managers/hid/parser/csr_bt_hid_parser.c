/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_pmem.h"
#include "csr_bt_util.h"
#include "csr_bt_hid_parser.h"

#define GETU(x,y) \
if ((x)->itemSize == 1) (y) = (CsrUint8)(x)->itemData; \
else if ((x)->itemSize == 2) (y) = (CsrUint16)(x)->itemData; \
else if ((x)->itemSize == 4) (y) = (CsrUint32)(x)->itemData; \
else (y) = 0;

#define GETS(x,y) \
if ((x)->itemSize == 1) (y) = (CsrInt8)(x)->itemData; \
else if ((x)->itemSize == 2) (y) = (CsrInt16)(x)->itemData; \
else if ((x)->itemSize == 4) (y) = (CsrInt32)(x)->itemData; \
else (y) = 0;

const CsrUint8 itemSize[4] = {0,1,2,4};

static void hidStackPush(struct CsrBtHidStack **stackPtr, struct CsrBtHidStack *elmPtr)
{
    elmPtr->next = *stackPtr;
    *stackPtr = elmPtr;
}

static struct CsrBtHidStack *hidStackPop(struct CsrBtHidStack **stackPtr)
{
    struct CsrBtHidStack *elmPtr = *stackPtr;

    if (NULL != *stackPtr)
    {
        *stackPtr = (*stackPtr)->next;
    }
    return elmPtr;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      getValue
 *
 *  DESCRIPTION
 *      Read the length of a SDP record fields.
 *
 *  PARAMETERS
 *        data:    Data (HID descriptor)
 *        idx:    Data index
 *
 *    RETURNS
 *        Unsigned value
 *
 *----------------------------------------------------------------------------*/
static CsrUint32 getValue(CsrUint8 *data, CsrUint16 *idx)
{
    CsrUint8 len;
    CsrUint32 v;

    len = (CsrUint8)(data[(*idx)++] & 0x07);

    switch(len)
    {
        case 0:
        case 5:
            {
                v = (CsrUint32)data[(*idx)++];
                break;
            }
        case 1:
        case 6:
            {
                v  = data[(*idx)++]<< 8;
                v |= data[(*idx)++];
                break;
            }
        case 2:
        case 7:
            {
                v  = data[(*idx)++]<<24;
                v |= data[(*idx)++]<<16;
                v |= data[(*idx)++]<<8;
                v |= data[(*idx)++];
                break;
            }
        default:
            {
                v = 0;
                break;
            }
    }

    return(v);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      hidSearchCollection
 *
 *  DESCRIPTION
 *      Climb up the stack, search for the specified collection type
 *        and return the usage.
 *
 *  PARAMETERS
 *      parser:    Parser (administration) data
 *        type:    collection type to lookup
 *
 *    RETURNS
 *        usage, 0 if usage information not found
 *
 *----------------------------------------------------------------------------*/
static CsrUint8 hidSearchCollection(struct CsrBtHidParser *parser, unsigned type)
{
    struct CsrBtHidCollection *stackPtr = (struct CsrBtHidCollection *)parser->collectionStack;

    while (stackPtr != NULL)
    {
        if (stackPtr->type == type)
        {
            return (CsrUint8)(stackPtr->usage);
        }
        stackPtr = (struct CsrBtHidCollection*)stackPtr->next;
    }

    return 0;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      hidAddUsage
 *
 *  DESCRIPTION
 *      Add a usage to the parser table.
 *
 *  PARAMETERS
 *      parser:    Parser (administration) data
 *        usage:
 *
 *    RETURNS
 *        TRUE on success, otherwise FALSE
 *
 *----------------------------------------------------------------------------*/
static CsrBool hidAddUsage(struct CsrBtHidParser *parser, CsrUint32 data)
{
    CsrUint32 i, start;

    if( parser->local.delimiterBranch > 1)
    {
        return TRUE;
    }

    if( parser->itemSize <= 2)
    {
        data = (parser->global.usagePage << 16) + data;
    }

    if ( parser->itemTag == CSR_BT_HID_ITEM_USAGE_MIN)
    {
        parser->local.usageMin = data;
        return TRUE;
    }

    start = (parser->itemTag == CSR_BT_HID_ITEM_USAGE_MAX) ? parser->local.usageMin : data;

    for (i = start; i <= data; i++)
    {
        if (parser->local.usageIndex >= CSR_BT_HID_MAX_USAGES)
        {
            return FALSE;
        }

        parser->local.usage[parser->local.usageIndex] = i;
        parser->local.usageIndex++;
    }

    return TRUE;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      hidAllocReport
 *
 *  DESCRIPTION
 *      Allocate a new report for a device.
 *
 *  PARAMETERS
 *      device:    device to be allocated a new report
 *        type:    Report type (input/output/feature)
 *        id:        Report id
 *
 *    RETURNS
 *        Pointer to allocated report, NULL on failure
 *
 *----------------------------------------------------------------------------*/
static struct CsrBtHidReport *hidAllocReport(struct CsrBtHidDevice *device, CsrUint8 type, CsrUint8 id)
{
    struct CsrBtHidReport            *report;
    struct CsrBtHidReportType    *reportType;
    CsrUint8 offset;

    reportType = &device->reportType[type];
    offset = reportType->reportIdTable[id];

    report = reportType->reportIndex[offset];
    if(report && (report->id == id))
    {
        return reportType->reportIndex[offset];
    }

    if( reportType->maxReport >= CSR_BT_HID_MAX_REP)
    {
        return NULL;
    }

    if ( id != 0)
    {
        reportType->usesIds = TRUE;
    }

    /* make actual allocation & initialize values */
    report = CsrPmemZalloc(sizeof(struct CsrBtHidReport));

    report->id = id;
    report->type = type;
    report->size = 0;
    report->device = device;

    reportType->reportIdTable[id] = reportType->maxReport;
    reportType->reportIndex[reportType->maxReport] = report;

    reportType->maxReport++;

    return report;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      hidCreateEntry
 *
 *  DESCRIPTION
 *      Add a new report entry.
 *
 *  PARAMETERS
 *      parser:            Parser (administration) data
 *        reportType:        Report type (input/output/feature)
 *        flags:            flags
 *
 *    RETURNS
 *        TRUE on success, otherwise FALSE
 *
 *----------------------------------------------------------------------------*/
static CsrBool hidCreateEntry(struct CsrBtHidParser *parser, CsrUint8 reportType, CsrUint32 flags)
{
    struct CsrBtHidReport *report;
    struct CsrBtHidEntry *entry;
    CsrUint32 usages;
    CsrUint16 i;

    report = hidAllocReport(parser->device, reportType, parser->global.reportId);
    if(!report)
    {
        return FALSE;
    }

    if (report->maxEntry == CSR_BT_HID_MAX_ENTRYS)
    {
        return FALSE;
    }

    if( parser->global.logicalMax < parser->global.logicalMin)
    {
        return FALSE;
    }

    usages = parser->local.usageIndex;

    report->size += parser->global.reportSize * parser->global.reportCount;

    if( usages < parser->global.reportCount)
    {
        usages = parser->global.reportCount;
    }

    if( usages == 0)
    {
        return TRUE; /* ignore padding */
    }

    entry = CsrPmemZalloc(sizeof(struct CsrBtHidEntry));
    entry->usage = (struct CsrBtHidUsage *)CsrPmemZalloc(usages * sizeof(struct CsrBtHidUsage));
    entry->value = (CsrInt32 *)CsrPmemZalloc(parser->global.reportCount * sizeof(unsigned));
    entry->report = report;

    report->entry[report->maxEntry++] = entry;

    if (parser->local.usageIndex) /* to avoid accessing an invalid element in parser->local.usage[] */
    {
        for (i = 0; i < usages; i++)
        {
            entry->usage[i].usage = (i < parser->local.usageIndex) ? parser->local.usage[i] :
                parser->local.usage[parser->local.usageIndex - 1];
        }
    }

    for (i = 0; i < CSR_BT_HID_COLLECTION_TYPES; i++)
    {
        entry->collectionType[i] = hidSearchCollection(parser, i);
    }

    entry->maxusage = usages;
    entry->flags = flags;
    entry->reportType = reportType;
    entry->reportSize = parser->global.reportSize;
    entry->reportCount = parser->global.reportCount;
    entry->logicalMin = parser->global.logicalMin;
    entry->logicalMax = parser->global.logicalMax;
    entry->physicalMin = parser->global.physicalMin;
    entry->physicalMax = parser->global.physicalMax;
    entry->unitExp = parser->global.unitExp;
    entry->unit = parser->global.unit;

    return TRUE;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      hidParseItem
 *
 *  DESCRIPTION
 *      Handle an item.
 *
 *  PARAMETERS
 *      parser:    Parser (administration) data
 *
 *    RETURNS
 *        TRUE on success, otherwise FALSE
 *
 *----------------------------------------------------------------------------*/
static CsrBool hidParseItem(struct CsrBtHidParser *parser)
{
    CsrUint32 data;
    CsrBool ret = TRUE;

    if ((parser->itemType == CSR_BT_HID_TYPE_LOCAL) && (parser->itemSize == 0))
    {
        return FALSE;
    }

    GETU(parser, data);

    switch (parser->itemTag)
    {
        /* Main items */
        case CSR_BT_HID_ITEM_BEGIN_COLLECTION:
            {
                struct CsrBtHidCollection *collection = CsrPmemZalloc(sizeof(struct CsrBtHidCollection));
                collection->type = (data & 0xff);
                collection->usage = (CsrUint16)parser->local.usage[0];
                hidStackPush(&parser->collectionStack, (struct CsrBtHidStack*)collection);
                break;
            }
        case CSR_BT_HID_ITEM_END_COLLECTION:
            {
                struct CsrBtHidCollection *collection = (struct CsrBtHidCollection*)hidStackPop(&parser->collectionStack);

                if (collection != NULL)
                {
                    CsrPmemFree(collection);
                }
                else
                {
                    ret = FALSE;
                }
                break;
            }
        case CSR_BT_HID_ITEM_INPUT:
        case CSR_BT_HID_ITEM_OUTPUT:
        case CSR_BT_HID_ITEM_FEATURE:
            {
                CsrUint8 reportType = (CsrUint8)((parser->itemTag & 0x30) >> 4);

                if (reportType == 3)
                    reportType--;

                ret = hidCreateEntry(parser, reportType, data);
                break;
            }
            /* Global items */
        case CSR_BT_HID_ITEM_PUSH:
            {
                struct CsrBtHidGlobal *global = CsrPmemZalloc(sizeof(struct CsrBtHidGlobal));
                CsrMemCpy(global, &parser->global, sizeof(struct CsrBtHidGlobal));
                hidStackPush(&parser->globalStack, (struct CsrBtHidStack*)global);
                break;
            }
        case CSR_BT_HID_ITEM_POP:
            {
                struct CsrBtHidGlobal *global = (struct CsrBtHidGlobal*)hidStackPop(&parser->globalStack);

                if (global != NULL)
                {
                    CsrMemCpy(&parser->global, global, sizeof(struct CsrBtHidGlobal));
                    CsrPmemFree(global);
                }
                else
                {
                    ret = FALSE;
                }
                break;
            }
        case CSR_BT_HID_ITEM_USAGE_PAGE:
            {
                GETU(parser, parser->global.usagePage);
                break;
            }
        case CSR_BT_HID_ITEM_LOG_MIN:
            {
                GETS(parser, parser->global.logicalMin);
                break;
            }
        case CSR_BT_HID_ITEM_LOG_MAX:
            {
                if (parser->global.logicalMin >= 0)
                {
                    GETU(parser, parser->global.logicalMax);
                }
                else
                {
                    GETS(parser, parser->global.logicalMax);
                }
                break;
            }
        case CSR_BT_HID_ITEM_PHYS_MIN:
            {
                GETS(parser, parser->global.physicalMin);
                break;
            }
        case CSR_BT_HID_ITEM_PHYS_MAX:
            {
                if (parser->global.physicalMin >= 0)
                {
                    GETU(parser, parser->global.physicalMax);
                }
                else
                {
                    GETS(parser, parser->global.physicalMax);
                }
                break;
            }
        case CSR_BT_HID_ITEM_UNIT_EXPONENT:
            {
                GETS(parser, parser->global.unitExp);
                break;
            }
        case CSR_BT_HID_ITEM_UNIT:
            {
                GETU(parser, parser->global.unit);
                break;
            }
        case CSR_BT_HID_ITEM_REP_SIZE:
            {
                GETU(parser, parser->global.reportSize);
                if( parser->global.reportSize > 32)
                {
                    ret = FALSE;
                }
                break;
            }
        case CSR_BT_HID_ITEM_REP_COUNT:
            {
                GETU(parser, parser->global.reportCount);
                if ( parser->global.reportCount > CSR_BT_HID_MAX_USAGES) {
                    ret = FALSE;
                }
                break;
            }
        case CSR_BT_HID_ITEM_REP_ID:
            {
                parser->global.reportId = (CsrUint8)parser->itemData;
                if ( parser->global.reportId  == 0) {
                    ret = FALSE;
                }
                break;
            }
            /* Local items */
        case CSR_BT_HID_ITEM_DELIMITER:
            {
                if (data)
                {
                    if (parser->local.delimiterDepth == 0)
                    {
                        parser->local.delimiterDepth++;
                        parser->local.delimiterBranch++;
                    }
                }
                else
                {
                    if (parser->local.delimiterDepth >= 1)
                    {
                        parser->local.delimiterDepth--;
                    }
                }
                ret = FALSE; /*TODO check*/
                break;
            }
        case CSR_BT_HID_ITEM_USAGE_MIN:
        case CSR_BT_HID_ITEM_USAGE_MAX:
        case CSR_BT_HID_ITEM_USAGE:
            {
                ret = hidAddUsage(parser, data);
                break;
            }
        default:
            {
                ret = (CsrBool)((parser->itemType == CSR_BT_HID_TYPE_GLOBAL) ? (FALSE) : (TRUE));
                break;
            }
    }

    if (parser->itemType == CSR_BT_HID_TYPE_MAIN)
    {
        CsrMemSet(&parser->local, 0, sizeof(parser->local));
    }

    return (ret);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      hidParserSetItemData
 *
 *  DESCRIPTION
 *      Sets parser item data
 *
 *  PARAMETERS
 *      parser:    Parser (administration) data
 *        item:    Item to have data set
 *
 *    RETURNS
 *        TRUE on success, otherwise FALSE
 *
 *----------------------------------------------------------------------------*/
static CsrBool hidParserSetItemData(struct CsrBtHidParser *parser)
{
    CsrUint8 *data = parser->descPos;
    CsrUint8 size = parser->itemSize;

    /* No handling of long items */
    if (parser->itemTag == CSR_BT_HID_ITEM_LONG)
    {
        return FALSE;
    }

    if (size == 1)
    {
        parser->itemData = *data;
    }
    else if (size == 2)
    {
        parser->itemData = *data + ((CsrUint16)(*(data + 1)) << 8);
    }
    else if (size == 4)
    {
        parser->itemData = *data + ((CsrUint32)(*(data + 1)) << 8) +
            ((CsrUint32)(*(data + 2)) << 16) + ((CsrUint32)(*(data + 3)) << 24);
    }
    else
    {
        parser->itemData = 0;
    }

    return TRUE;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      hidInitiateParser
 *
 *  DESCRIPTION
 *      Initiates parser - sets descriptor position and size
 *
 *  PARAMETERS
 *      parser:    Parser (administration) data
 *        item:    Item to have data set
 *
 *    RETURNS
 *        TRUE on success, otherwise FALSE
 *
 *----------------------------------------------------------------------------*/
static CsrBool hidInitiateParser(struct CsrBtHidParser *parser, CsrUint8 *rawHid, CsrUint16 rawHidSize)
{
    CsrUint16 idx = 0;
    CsrBool ret;

    if( (*rawHid & 0xF8) == 0x30)
    {
        /* if wrapped into data element sequence, adjust the start of actual descriptor */
        getValue(rawHid,&idx);
        getValue(rawHid,&idx);
        getValue(rawHid,&idx);
        getValue(rawHid,&idx);
    }

    if (rawHidSize > idx)
    {
        parser->descPos = rawHid + idx;
        parser->descEnd = rawHid + rawHidSize;
        parser->globalStack = NULL;
        ret = TRUE;
    }
    else
    {
        ret = FALSE;
    }

    return ret;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidFree
 *
 *  DESCRIPTION
 *      Free a device structure and everything below it.
 *
 *  PARAMETERS
 *      device:    A hid device structure to be freed
 *
 *    RETURNS
 *        -
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidFree(struct CsrBtHidDevice *device)
{
    CsrUint8 i,j;

    if (device)
    {
        for (i = 0; i < CSR_BT_HID_REP_TYPES; i++)
        {
            struct CsrBtHidReportType *reportType = device->reportType + i;

            for (j = 0; j < reportType->maxReport; j++)
            {
                struct CsrBtHidReport *report = reportType->reportIndex[j];

                if (report)
                {
                    CsrUint8 n;

                    for (n = 0; n < report->maxEntry; n++)
                    {
                        CsrPmemFree(report->entry[n]->usage);
                        CsrPmemFree(report->entry[n]->value);
                        CsrPmemFree(report->entry[n]);
                    }
                    CsrPmemFree(report);
                }
            }
        }

        CsrPmemFree(device);
    }
    else
    {
        /* Do nothing */
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidParse
 *
 *  DESCRIPTION
 *      Parse a raw HID descriptor into a hidDevice structure.
 *
 *  PARAMETERS
 *      rawHid:        Pointer to a raw HID descriptor byte sequence
 *    rawHidSize:    Size of byte sequence
 *
 *    RETURNS
 *        Pointer to a HID device description
 *
 *----------------------------------------------------------------------------*/
struct CsrBtHidDevice *CsrBtHidParse(CsrUint8 *rawHid, CsrUint16 rawHidSize)
{
    struct CsrBtHidParser *parser;
    struct CsrBtHidDevice *device;
    CsrBool fail = FALSE;

    parser = CsrPmemZalloc(sizeof(struct CsrBtHidParser));

    if (!hidInitiateParser(parser, rawHid, rawHidSize))
    {
        CsrPmemFree(parser);
        return NULL;
    }

    device = CsrPmemZalloc(sizeof(struct CsrBtHidDevice));
    parser->device = device;

    while (parser->descPos != parser->descEnd)
    {
        parser->itemSize = itemSize[(CsrUint8)(*parser->descPos & CSR_BT_HID_ITEM_SIZE_MASK)];
        parser->itemType = (CsrUint8)(*parser->descPos & CSR_BT_HID_ITEM_TYPE_MASK);
        parser->itemTag = (CsrUint8)(*parser->descPos & CSR_BT_HID_ITEM_TAG_MASK);
        parser->descPos++;

        if (!hidParserSetItemData(parser))
        {
            fail = TRUE;
            break;
        }

        if (!hidParseItem(parser))
        {
            fail = TRUE;
            break;
        }

        parser->descPos += parser->itemSize;
    }

    if (parser->local.delimiterDepth)
    {
        fail = TRUE;
    }

    if (fail)
    {
        CsrBtHidFree(device);
        device = NULL;
    }
    /* The local variable "parser" shall be freed always*/
    CsrPmemFree(parser);
    parser = NULL;
    
    return (device);
}


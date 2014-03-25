#ifndef CSR_BT_AV2_UTIL_H__
#define CSR_BT_AV2_UTIL_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include "csr_bt_av_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

void setMediaPayloadType(CsrUint8 mpt);

void setTimestamp(CsrUint32 ts);


void getSampleBufferSize(CsrUint16 *bs);


void setSampleBufferSize(CsrUint16 bs);


CsrUint16 getMaxFrameSize(void);

void setMaxFrameSize(CsrUint16 mfs);

void getPcmSettings(CsrUint8 *channels, CsrUint8 *bits, CsrUint16 *freq);

void setPcmSettings(CsrUint8 channels, CsrUint8 bits, CsrUint16 freq);


CsrBool getPcmLocked();


void setPcmLocked(CsrBool locked);


/* Filter functions */
void filtersKill(Av2AppInst *instData);
void filtersBackgroundInterrupt(void *);
void filtersSetup(Av2AppInst *instData);
#ifdef EXCLUDE_CSR_BT_SS_CODEC	
void filtersInstall(Av2AppInst *instData);
#else
void filtersInstall(Av2AppInst *instData, CsrUint8 installedCodec);
#endif
CsrBool filtersRun(Av2AppInst *instData,
                  CsrUint8 op);
CsrBool filtersCheck(Av2AppInst *instData);
CsrUint8 *filtersGetConfig(Av2AppInst *instData,
                          CsrUint8 *length);
CsrBtAvResult filtersSetConfig(Av2AppInst *instData,
                             CsrUint8 *conf,
                             CsrUint8 conf_len);
CsrUint8 *filtersGetCaps(Av2AppInst *instData,
                        CsrUint8 *length);
CsrUint8 *filtersRemoteCaps(Av2AppInst *instData,
                           CsrUint8 *caps,
                           CsrUint8 caps_len,
                           CsrUint8 *conf_len);
void filtersQosUpdate(Av2AppInst *instData,
                      CsrUint16 qosLevel);



#ifdef __cplusplus
}
#endif

#endif


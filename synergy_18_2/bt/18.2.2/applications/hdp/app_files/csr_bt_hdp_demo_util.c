/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#include "csr_pmem.h"
#include "csr_bt_util.h"
#include "csr_bt_hdp_lib.h"
#include "csr_bt_hdp_prim.h"
#include "csr_sched_init.h"
#include "csr_bt_hdp_demo_util.h"
#if 0
CsrUint8 AarqApdu[] = {
                        0xE2,0x00,
                        0x00,0x32,
                        0x80,0x00,0x00,0x00,
                        0x00,0x01,0x00,0x2A,
                        0x50,0x79,
                        0x00,0x26,
                        0x80,0x00,0x00,0x00,
                        0xA0,0x00,
                        0x80,0x00,0x00,0x00,
                        0x00,0x00,0x00,0x00,
                        0x00,0x80,0x00,0x00,
                        0x00,0x08,
                        0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x07,
                        0x40,0x00,
                        0x00,0x01
                     };
#endif
CsrUint8 AarqApdu[] = {
                        0xE2,0x00,
                        0x00,0x32,
                        0x80,0x00,0x00,0x00,
                        0x00,0x01,0x00,0x2A,
                        0x50,0x79,
                        0x00,0x26,
                        0x80,0x00,0x00,0x00,
                        0x80,0x00,
                        0x80,0x00,0x00,0x00,
                        0x00,0x00,0x00,0x00,
                        0x00,0x80,0x00,0x00,
                        0x00,0x08,
                        0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x07,
                        0x02, 0xBC,
                        0x00,0x01,
                        0x01,0x00,
                        0x00,0x00,0x00,0x00
                     };

void  hdpdemoSendAarqApdu(hdpDemoGlobalInstanceData_t *hdpData)
{
    CsrUint16 len = AarqApdu[3]+4;
    CsrUint8 *data = CsrPmemAlloc(len);
    CsrMemCpy(data,AarqApdu, len);

    printf("\nSending Aarq Apdu");

    CsrBtHdpDataReqSend(hdpData->phandle, hdpData->assocChId[hdpData->idxAssocChId],data,len);
}

CsrUint8 AareApdu[] = {
                        0xE3,0x00,
                        0x00,0x2C,
                        0x00,0x03,
                        0x50,0x79,
                        0x00,0x26,
                        0x80,0x00,0x00,0x00,
                        0x80,0x00,
                        0x80,0x00,0x00,0x00,
                        0x00,0x00,0x00,0x00,
                        0x80,0x00,0x00,0x00,
                        0x00,0x08,
                        0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,
                        0x00,0x00,
                        0x00,0x00,
                        0x00,0x00,
                        0x00,0x00,0x00,0x00,
                    };

void  hdpdemoSendAareApdu(hdpDemoGlobalInstanceData_t *hdpData,CsrBool result )
{
    CsrUint16 len = AareApdu[3]+4;
    CsrUint8 *data = CsrPmemAlloc(len);

    if (result == TRUE )
    {
        AareApdu[5] = 0x00;
    }

    CsrMemCpy(data,AareApdu, len);
    printf("\nSending AareApdu");
    CsrBtHdpDataReqSend(hdpData->phandle, hdpData->assocChId[hdpData->idxAssocChId],data,len);
}


CsrUint8 PrstApduConfig[] = {
                        0xE7,0x00,
                        0x00,0x94,
                        0x00,0x92,
                        0x00,0x01, /*invoke id mirrod from request */
                        0x01,0x01,
                        0x00,0x8C,
                        0x00,0x00,
                        0xFF,0xFF,0xFF,0xFF,
                        0x0D,0x1C,
                        0x00,0x82,
                        0x02, 0xBC,
                        0x00,0x02,
                        0x00,0x7C,
                        0x00,0x06,
                        0x00,0x01,
                        0x00,0x07,
                        0x00,0x40,
                        0x09,0x2F,
                        0x00,0x04,
                        0x00,0x02,0x4A,0x04,
                        0x0A,0x46,
                        0x00,0x02,
                        0xF0,0x40,
                        0x0A,0x73,
                        0x00,0x02,
                        0x03,0x03,
                        0x0A,0x76,
                        0x00,0x0A,
                        0x00,0x03,
                        0x00,0x06,
                        0x4A,0x05,
                        0x4A,0x06,
                        0x4A,0x07,
                        0x09,0x96,
                        0x00,0x02,
                        0x0F,0x20,
                        0x09,0x4A,
                        0x00,0x04,
                        0xFA,0x1E,0x84,0x80,
                        0x0A,0x55,
                        0x00,0x0C,
                        0x00,0x02,
                        0x00,0x08,
                        0x0A,0x75,0x00,0x0A,
                        0x09,0x90,0x00,0x08,
                        0x00,0x06,
                        0x00,0x02,
                        0x00,0x05,
                        0x00,0x2C,
                        0x09,0x2F,
                        0x00,0x04,
                        0x00,0x02,0x48,0x2A,
                        0x0A,0x46,
                        0x00,0x02,
                        0xF0,0x40,
                        0x09,0x96,
                        0x00,0x02,
                        0x0A,0xA0,
                        0x09,0x4A,
                        0x00,0x04,
                        0xFA,0x0F,0x42,0x40,
                        0x0A,0x55,
                        0x00,0x0C,
                        0x00,0x02,
                        0x00,0x08,
                        0x0A,0x4C,0x00,0x02,
                        0x09,0x90,0x00,0x08
                    };

void  hdpdemoSendApduConfig(hdpDemoGlobalInstanceData_t *hdpData)
{
    CsrUint16 len = PrstApduConfig[3]+4;
    CsrUint8 *data = CsrPmemAlloc(len);
    CsrMemCpy(data,PrstApduConfig, len);

    printf("\nSending ApduConfig");
    CsrBtHdpDataReqSend(hdpData->phandle, hdpData->assocChId[hdpData->idxAssocChId],data,len);
}

CsrUint8 PrstApduConfigRsp[] = {
                                    0xE7,0x00,
                                    0x00,0x16,
                                    0x00,0x14,
                                    0x00,0x01,
                                    0x02,0x01,
                                    0x00,0x0E,
                                    0x00,0x00,
                                    0x00,0x00,0x00,0x00,
                                    0x0D,0x1C,
                                    0x00,0x04,
                                    0x40,0x00,
                                    0x00,0x00
                                };


void  hdpdemoSendApduConfigRsp(hdpDemoGlobalInstanceData_t *hdpData)
{
    CsrUint16 len = PrstApduConfigRsp[3]+4;
    CsrUint8 *data = CsrPmemAlloc(len);
    CsrMemCpy(data,PrstApduConfigRsp, len);

    printf("\nSending ApduConfigRsp");
    CsrBtHdpDataReqSend(hdpData->phandle, hdpData->assocChId[hdpData->idxAssocChId],data,len);
}

CsrUint8 DataMDSGet[] = {
                            0xE7,0x00,
                            0x00,0x0E,
                            0x00,0x0C,
                            0x00,0x01, /*invoke id mirrod from request */
                            0x01,0x03,
                            0x00,0x06,
                            0x00,0x00,
                            0x00,0x00,
                            0x00,0x00
                        };

void  hdpdemoSendDataMDSGet(hdpDemoGlobalInstanceData_t *hdpData)
{
    CsrUint16 len = DataMDSGet[3]+4;
    CsrUint8 *data = CsrPmemAlloc(len);
    CsrMemCpy(data,DataMDSGet, len);

    printf("\nSending MDS Get handle = 0");
    CsrBtHdpDataReqSend(hdpData->phandle, hdpData->assocChId[hdpData->idxAssocChId],data,len);
}


CsrUint8 DataMDSAttrib[] = {
                                0xE7,0x00,
                                0x00,0x6C,
                                0x00,0x6A,
                                0x00,0x01, /*invoke id mirrod from request */
                                0x02,0x03,
                                0x00,0x64,
                                0x00,0x00,
                                0x00,0x06,
                                0x00,0x5E,
                                0x0A,0x5A,
                                0x00,0x08,
                                0x00,0x01,
                                0x00,0x04,
                                0x10,0x07,
                                0x00,0x01,
                                0x09,0x28,
                                0x00,0x18,
                                0x00,0x0A,0x54,0x68,
                                0x65,0x43,0x6F,0x6D,
                                0x70,0x61,0x6E,0x79,
                                0x00,0x0A,0x54,0x68,
                                0x65,0x42,0x50,0x4D,
                                0x20,0x41,0x42,0x43,
                                0x09,0x84,
                                0x00,0x0A,
                                0x00,0x08,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x07,
                                0x0a,0x44,
                                0x00,0x02,
                                0x40,0x00,
                                0x09,0x2D,
                                0x00,0x12,
                                0x00,0x01,
                                0x00,0x0E,
                                0x00,0x01,
                                0x00,0x00,
                                0x00,0x08,0x44,0x45,
                                0x31,0x32,0x34,0x35,
                                0x36,0x37,
                                0x09,0x87,
                                0x00,0x08,
                                0x20,0x07,0x02,0x01,
                                0x12,0x05,0x00,0x00
                            };

void  hdpdemoSendDataMDSAttrib(hdpDemoGlobalInstanceData_t *hdpData)
{
    CsrUint16 len = DataMDSAttrib[3]+4;
    CsrUint8 *data = CsrPmemAlloc(len);
    CsrMemCpy(data,DataMDSAttrib, len);

    printf("\nSending MDS Attributes");
    CsrBtHdpDataReqSend(hdpData->phandle, hdpData->assocChId[hdpData->idxAssocChId],data,len);
}

CsrUint8 DataMeasurement[] = {
                                0xE7,0x00,
                                0x00,0x3E,
                                0x00,0x3C,
                                0x00,0x01,/*invoke id mirrod from request */
                                0x01,0x01,
                                0x00,0x36,
                                0x00,0x00,
                                0xFF,0xFF,0xFF,0xFF,
                                0x0D,0x1D,
                                0x00,0x2C,
                                0xF0,0x00,
                                0x00,0x00,
                                0x00,0x02,
                                0x00,0x24,
                                0x00,0x01,
                                0x00,0x12,
                                0x00,0x03,
                                0x00,0x06,
                                0x00,0x78,
                                0x00,0x50,
                                0x00,0x64,
                                0x20,0x07,0x12,0x06,
                                0x12,0x10,0x00,0x00,
                                0x00,0x02,
                                0x00,0x0A,
                                0xF2,0x58,
                                0x20,0x07,0x12,0x06,
                                0x12,0x10,0x00,0x00
                            };

void  hdpdemoDataMeasurement(hdpDemoGlobalInstanceData_t *hdpData)
{
    CsrUint16 len = DataMeasurement[3]+4;
    CsrUint8 *data = CsrPmemAlloc(len);
    CsrMemCpy(data,DataMeasurement, len);

    printf("\nSending data measurement");
    CsrBtHdpDataReqSend(hdpData->phandle, hdpData->assocChId[hdpData->idxAssocChId],data,len);
}

CsrUint8 DataMeasurementCfm[] = {
                                    0xE7,0x00,
                                    0x00,0x12,
                                    0x00,0x10,
                                    0x00,0x01,
                                    0x02,0x01,
                                    0x00,0x0A,
                                    0x00,0x00,
                                    0x00,0x00,0x00,0x00,
                                    0x0D,0x1D,
                                    0x00,0x00
                                };

void  hdpdemoDataMeasurementCfm(hdpDemoGlobalInstanceData_t *hdpData)
{
    CsrUint16 len = DataMeasurementCfm[3]+4;
    CsrUint8 *data = CsrPmemAlloc(len);
    CsrMemCpy(data,DataMeasurementCfm, len);

    printf("\nSending Data{Response-confirmed event report}");
    CsrBtHdpDataReqSend(hdpData->phandle, hdpData->assocChId[hdpData->idxAssocChId],data,len);
}


CsrUint8 RlrqApdu[] = {
                        0xE4,0x00,
                        0x00,0x02,
                        0x00,0x00
                      };

void  hdpdemoRlrqApdu(hdpDemoGlobalInstanceData_t *hdpData)
{
    CsrUint16 len = RlrqApdu[3]+4;
    CsrUint8 *data = CsrPmemAlloc(len);
    CsrMemCpy(data,RlrqApdu, len);

    printf("\nSending RlrqApdu");
    CsrBtHdpDataReqSend(hdpData->phandle, hdpData->assocChId[hdpData->idxAssocChId],data,len);
}

CsrUint8 RlreApdu[] = {
                        0xE5,0x00,
                        0x00,0x02,
                        0x00,0x00
                      };
void  hdpdemoRlreApdu(hdpDemoGlobalInstanceData_t *hdpData)
{
    CsrUint16 len = RlreApdu[3]+4;
    CsrUint8 *data = CsrPmemAlloc(len);
    CsrMemCpy(data,RlreApdu, len);

    printf("\nSending RlreApdu");

    CsrBtHdpDataReqSend(hdpData->phandle, hdpData->assocChId[hdpData->idxAssocChId],data,len);
}


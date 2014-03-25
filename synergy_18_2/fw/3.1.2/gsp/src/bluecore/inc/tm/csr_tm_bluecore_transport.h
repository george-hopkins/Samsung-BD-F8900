#ifndef CSR_TM_BLUECORE_TRANSPORT_H__
#define CSR_TM_BLUECORE_TRANSPORT_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Generic transport structure */
struct CsrTmBlueCoreTransport
{
    CsrBool   (*start)(void);
    CsrBool   (*stop)(void);
    CsrUint16 (*query)(void);
    void      (*msgtx)(void *msg);
    void      (*msgrx)(void *msg);
    void      (*msgclean)(void *msg);
    void      (*scotx)(void *scoData);
    CsrBool   (*driverRestart)(CsrUint8 reset);
    void      (*restart)(void);
    void      (*close)(void);
    CsrBool   started;
};

/** Check if the transport is started
 *
 * @param hdl Transport handle
 *
 * @return TRUE or FALSE
 */
#define CSR_TM_BLUECORE_TRANSPORT_STARTED(hdl) (((struct CsrTmBlueCoreTransport *) (hdl))->started)

/** Delete a transport ** Currently does nothing **
 *
 * @param hdl Transport handle
 */
void CsrTmBlueCoreTransportDelete(void *hdl);

/** Start a transport
 *
 * @param hdl Transport handle
 *
 * @return TRUE on success; FALSE otherwise
 */
CsrBool CsrTmBlueCoreTransportStart(void *hdl);

/** Stop a transport
 *
 * @param hdl Transport handle
 *
 * @return TRUE on success; FALSE otherwise
 */
CsrBool CsrTmBlueCoreTransportStop(void *hdl);

/** Query a transport type
 *
 * @param hdl Transport handle
 *
 * @return Type of transport
 */
CsrUint16 CsrTmBlueCoreTransportQuery(void *hdl);

/** Send a message
 *
 * @param hdl Transport handle
 *
 * @param msg Message to send
 */
void CsrTmBlueCoreTransportMsgTx(void *hdl, void *msg);

/** Receive a message (upper layer call back)
 *
 * @param hdl Transport handle
 *
 * @param msg Message received by transport
 */
void CsrTmBlueCoreTransportMsgRx(void *hdl, void *msg);

/** Clean/destroy a message
 *
 * @param hdl Transport handle
 *
 * @param msg Message to clean
 */
void CsrTmBlueCoreTransportMsgClean(void *hdl, void *msg);

/** Send sco data
 *
 * @param hdl Transport handle
 *
 * @param scoData Sco data to send
 */
void CsrTmBlueCoreTransportScoTx(void *hdl, void *scoData);

void CsrTmBlueCoreTransportRegisterMsgRx(void *hdl, void (*msgrx)(void *msg));

CsrBool CsrTmBlueCoreTransportDriverRestart(void *hdl, CsrUint8 reset);

void CsrTmBlueCoreTransportRestart(void *hdl);

void CsrTmBlueCoreTransportClose(void *hdl);

void CsrTmBlueCoreTransportInit(void **gash, void *blueCoreTransportDescriptor);

#ifdef __cplusplus
}
#endif

#endif /* CSR_TM_BLUECORE_TRANSPORT_H__ */

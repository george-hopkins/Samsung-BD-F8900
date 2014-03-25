/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2cap_cidme.c

\brief  This file defines L2CAP channel managment interface.
*/

#ifndef _L2CAP_CIDME_H
#define _L2CAP_CIDME_H

#include "csr_synergy.h"

#include "csr_bt_common.h"
#include "l2cap_cid.h"
#include "l2cap_con_handle.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void CIDME_FreeCidcb(CIDCB_T *cidcb);
extern void CIDME_Unroute(CIDCB_T *cidcb);
extern struct cidtag *CIDME_GetCidcb(l2ca_cid_t cid);
extern struct cidtag *CIDME_GetCidcbWithHandle(const L2CAP_CHCB_T *chcb, l2ca_cid_t cid);
extern struct cidtag *CIDME_GetCidcbRemoteWithHandle(const L2CAP_CHCB_T *chcb, l2ca_cid_t remote_cid);
extern struct cidtag *CIDME_GetCLCidcbRemote(const L2CAP_CHCB_T *chcb, l2ca_cid_t remote_cid, psm_t cl_psm);
extern struct cidtag *CIDME_GetCidcbWithContext(CsrUint16 context);
extern CIDCB_T *CIDME_NewCidcb(l2ca_cid_t local_cid);
extern void CIDME_InitCidcb(CIDCB_T *cidcb,
                            L2CAP_CHCB_T *chcb,
                            l2ca_cid_t remote_cid,
                            struct psm_tag_t *psm_reg,
                            psm_t remote_psm,
                            CsrUint16 context);
extern CsrUint16 CIDME_RegistrationContext(const CIDCB_T *cidcb);
extern psm_t CIDME_LocalPsm(const CIDCB_T *cidcb);

#ifdef __cplusplus
}
#endif
#endif

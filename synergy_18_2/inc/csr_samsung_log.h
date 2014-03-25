#ifndef _CSR_SAMSUNG_LOG_H
#define _CSR_SAMSUNG_LOG_H

/****************************************************************************

                         © Cambridge Silicon Radio Ltd, 2004
 	 			
                                   All rights reserved

FILE: peer_com_linux_u2u.h

DESCRIPTION:   Definitions used by the user <> user peer communication

REVISION:  $Revision: #1 $ by $Author: jk05 $

****************************************************************************/



#ifdef __cplusplus
extern "C" {
#endif

#include "csr_log_text_2.h"

extern CsrSchedQid TESTQUEUE;
#define CSR_SS_LOG_LEVEL_TEXT_OFF        0x0000
	
#define CSR_SS_LOG_LEVEL_TEXT_CRITICAL   0x01
#define CSR_SS_LOG_LEVEL_TEXT_ERROR      0x02
#define CSR_SS_LOG_LEVEL_TEXT_WARNING    0x03
#define CSR_SS_LOG_LEVEL_TEXT_INFO       0x04
#define CSR_SS_LOG_LEVEL_TEXT_DEBUG      0x05
	
#define CSR_SS_LOG_LEVEL_TEXT_ALL        0xFF


#define CSR_SS_LOG_COMMON                          0x00
#define CSR_SS_LOG_USB                             0x01
#define CSR_SS_LOG_GAP                             0x02
#define CSR_SS_LOG_AP                              0x03
#define CSR_SS_LOG_AVRCP                           0x04


//CsrSSApto
#define CSR_SS_AP_LOG_STR "[CSR_DBG]"
//#define CSR_SS_AP_LOG_ID 0x15FFFFF
CSR_LOG_TEXT_HANDLE_DECLARE(CSR_SS_AP_LOG_ID);
#if defined(CSR_LOG_ENABLE)
extern void csr_ss_client_dbg_printf(int level, const char *fm, ...);
#define DBG_SS_PRINT(t)         csr_ss_client_dbg_printf(CSR_LOG_LEVEL_TEXT_INFO, "[CSR_DBG]: %s\n",t);
#define DBG_SS_PRINT_INT1(t, i) csr_ss_client_dbg_printf(CSR_LOG_LEVEL_TEXT_INFO, "[CSR_DBG]: %s, %d\n", t, i);
#define DBG_SS_PRINT_ADDR(t,n,u,l) csr_ss_client_dbg_printf(CSR_LOG_LEVEL_TEXT_INFO, "[CSR_DBG]: %s,[%04X:%02X:%06X]\n",t, n, u,l);
#else 
#define DBG_SS_PRINT(t)
#define DBG_SS_PRINT_INT1(t, i)
#define DBG_SS_PRINT_ADDR(t,n,u,l)
#endif

void csr_ss_log_set_level(int level);
#ifdef __cplusplus
}
#endif 

#endif//_CSR_SAMSUNG_LOG_H
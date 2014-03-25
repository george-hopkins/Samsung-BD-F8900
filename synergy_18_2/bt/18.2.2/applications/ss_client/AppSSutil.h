#ifndef CSR_BT_AVRCP_APP_UTIL_H__
#define CSR_BT_AVRCP_APP_UTIL_H__
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif


void ss_bd_addr_copy(BD_ADDR_T *p_bd_addr_dest, const BD_ADDR_T *p_bd_addr_src);

/*! \brief Set the BD Address to all zeroes.

    \param p_bd_addr Pointer to Bluetooth address to be zeroed.
*/
void ss_bd_addr_zero(BD_ADDR_T *p_bd_addr);

/*! \brief Compare two BD Addresses for equality.

    \param p_bd_addr_1 Pointer to first Bluetooth address.
    \param p_bd_addr_2 Pointer to second Bluetooth address.
    \returns TRUE if equal, FALSE if different.
*/
CsrBool ss_bd_addr_eq(const BD_ADDR_T *p_bd_addr_1,const BD_ADDR_T *p_bd_addr_2);





#ifdef __cplusplus
}
#endif


#endif /* CSR_BT_AVRCP_APP_UTIL_H__ */

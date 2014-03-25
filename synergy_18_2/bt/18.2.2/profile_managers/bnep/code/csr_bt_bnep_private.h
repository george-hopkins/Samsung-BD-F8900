#ifndef BNEP_PRIVATE_H
#define BNEP_PRIVATE_H

#include "csr_synergy.h"
/******************************************************************************

                (c) CSR plc 2008

                All rights reserved

FILE:            csr_bt_bnep_private.h  --  _bluetooth _network _encapsulation _protocol

DESCRIPTION:    $Log: csr_bt_bnep_private.h,v $
                Revision 1.2  2003/07/31 12:39:47  ab02
                First compilable version

                Revision 1.4  2002/10/18 16:48:14  mgr
                Implement new NAP/GN-supporting BNEP API.

                Revision 1.3  2002/09/10 15:47:22  bs01
                Merge changes between jbs-pan-branch-start and jbs-pan-branch-mark-a onto the main line. Move external pan headers to interface/app/pan directory.

                Revision 1.2.18.1  2002/09/02 12:37:24  bs01
                Move pan headers and support from interface/app/bluestack and dev/src/csr/pan into interface/app/pan. Update dev side to cope.

                Revision 1.2  2002/07/18 14:05:24  mgr
                Merge from chef-1-branch.

                Revision 1.1.2.5  2002/06/28 19:07:34  mgr
                Split out UDP layer.
                Add mDNS server.
                Make radioio cope with odd-length packed pmalloc ('isIN') mblks.
                Add ability to perform ping and mDNS tests, and cause link-local jams.

                Revision 1.1.2.4  2002/04/15 18:03:02  mgr
                Add IPv4 support.
                Add QUOTE server.
                Add sanity-checking for addresses received in ARP/ND.

                Revision 1.1.2.3  2002/04/05 18:39:33  mgr
                Make the TCP primitives public.
                Improve the public/private header split.
                Some lint pacification.

                Revision 1.1.2.2  2002/03/28 19:26:36  mgr
                Tighten up state checking.
                Implement T_crt (and T_setup for the responder).
                Add BNEP_DISCONNECT_REQ and implement BNEP_DOWN_IND.
                Set the default link policy to allow switch, hold, sniff and park.
                Fix maximum tohost packet size to cope with maximum-size Ethernet packets.
                Improve config process (pay attention to more_flag and handle collisions).
                Don't ignore Ethernet packets whose source is apparently us (for DAD).

                Revision 1.1.2.1  2002/03/18 15:19:08  mgr
                Initial revision.

REVISION:        $Revision: #1 $

******************************************************************************/

/* Synergy BT change
   #include "app/pan/csr_bt_bnep_prim.h"
*/
#include "csr_bt_bnep_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************

    variable bnep_local_addr  --  local Ethernet address

******************************************************************************/

    extern ETHER_ADDR bnep_local_addr;

/******************************************************************************

    function bnep_get_addr  --  extract an Ethernet address from a packet

******************************************************************************/

    void bnep_get_addr (ETHER_ADDR *addr, const CsrUint8 *raw_addr);


/******************************************************************************

    macro bnep_set_addr  --  write an Ethernet address into a header

******************************************************************************/

#define bnep_set_addr(d,s) (*(d) = s.w[0], *(d + 1) = s.w[1], *(d + 2) = s.w[2])


/******************************************************************************

    macro bnep_set_broadcast_addr  --  set a broadcast Ethernet address

******************************************************************************/

#define bnep_set_broadcast_addr(d) ((d)->w[0] = (d)->w[1] = (d)->w[2] = ETHER_BROADCAST_IPV4)

#ifdef __cplusplus
}
#endif

#endif /* BNEP_PRIVATE_H */

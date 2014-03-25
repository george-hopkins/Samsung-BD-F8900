#ifndef IN_PRIVATE_H
#define IN_PRIVATE_H

#include "csr_synergy.h"
/******************************************************************************

                (c) CSR plc 2008

                All rights reserved

FILE:            csr_bt_in_private.h  --  generic _internet support

DESCRIPTION:    $Log: csr_bt_in_private.h,v $
                Revision 1.3  2003/09/25 12:42:16  ab02
                no message

                Revision 1.2  2003/07/31 12:39:47  ab02
                First compilable version

                Revision 1.6  2002/11/21 15:47:51  bs01
                Merge from gccsdk-2-62-branch onto mainline. Adjust for the fact that csr_bt_in.h
                and ipv4.h have been moved across to interface/app/pan.

                Revision 1.5.2.1  2002/11/19 15:10:47  bs01
                Move some headers into app/pan on gccsdk-2-62-branch.

                Revision 1.5  2002/11/12 19:31:15  mgr
                Add ZZZCOUNTER macro.

                Revision 1.4  2002/10/18 16:48:14  mgr
                Implement new NAP/GN-supporting BNEP API.

                Revision 1.3  2002/09/10 15:47:22  bs01
                Merge changes between jbs-pan-branch-start and jbs-pan-branch-mark-a onto
                the main line. Move external pan headers to interface/app/pan directory.

                Revision 1.2.18.1  2002/09/02 12:37:24  bs01
                Move pan headers and support from interface/app/bluestack and dev/src/csr/pan
                into interface/app/pan. Update dev side to cope.

                Revision 1.2  2002/07/18 14:05:24  mgr
                Merge from chef-1-branch.

                Revision 1.1.2.7  2002/07/05 14:12:21  mgr
                Add limited support for extension headers (just skip them).
                Add ability to send options extension headers as part of ping test.
                Pull FFS code out into its own directory and
                consequently make tcp_inject a bit less parochial.
                Disconnect on L2CAP configuration failure rather than panicking.
                Panic on failure to send BNEP_CONNECT_CFM rather than ignoring.

                Revision 1.1.2.5  2002/07/02 16:21:46  mgr
                Add ability to perform IPv6 mDNS tests (to ff02::1) and
                get the remote IPv4 link-local address.
                Fix UDP checksums (+0 sent as -0 as required by RFC768).
                Support the No Next Header extension header.  Oh, yeah, baby.

                Revision 1.1.2.4  2002/06/28 19:07:34  mgr
                Split out UDP layer.
                Add mDNS server.
                Make radioio cope with odd-length packed pmalloc ('isIN') mblks.
                Add ability to perform ping and mDNS tests, and cause link-local jams.

                Revision 1.1.2.3  2002/06/05 17:49:08  mgr
                Add HTTP server and DVD demo code.
                Manually apply patches from trunk for hostio_usr.
                Add beta TCP injection helper code.
                Add beta flash filing system.
                Improve BNEP compression if destined for remote device.
                Add support for acting as tester in BNEP conformance tests.
                Add support for acting as single-user GN.
                Tweak BNEP API.
                Add BNEP (L2CAP) channel flow control mechanism.
                Add support for BNEP extension headers.
                Hack TCP send to try to work around delayed ack/small tx buffer infelicity.
                Fix TCP CsrTime-WAIT recycling when inbound not logically empty.
                Suppress useless TCP window updates and expedite them when becoming not-full.
                Add support for IPv4 link-local addresses.
                Add support for IPv4 default router.

                Revision 1.1.2.2  2002/04/15 18:03:02  mgr
                Add IPv4 support.
                Add QUOTE server.
                Add sanity-checking for addresses received in ARP/ND.

                Revision 1.1.2.1  2002/04/05 18:39:33  mgr
                Make the TCP primitives public.
                Improve the public/private header split.
                Some lint pacification.

REVISION:        $Revision: #1 $

******************************************************************************/



/* Synergy BT change
#include "app/pan/csr_bt_in.h"
#include "bt.h"
#include "app/bluestack/types.h"
#include "csr_mblk.h"
#include "app/pan/csr_bt_ip.h"
*/

#include "csr_bt_in.h"
/*#include "buffer_emulator/bt.h" */
#include "csr_types.h"
#include "csr_mblk.h"
#include "csr_bt_ip.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************

    enum IN_ERROR  --  error-logging codes

******************************************************************************/

enum IN_ERROR
{
    IN_ERROR_NOCONN_BNEP_DATA_IND,
    IN_ERROR_NOMAP_BNEP_DATA_IND,
    IN_ERROR_HDRTOOSHORT_BNEP_DATA_IND,
    IN_ERROR_UNKNOWNTYPE_BNEP_DATA_IND,
    IN_ERROR_LOOPBACK_BNEP_DATA_IND,
    IN_ERROR_EXTHDRTOOSHORT_BNEP_DATA_IND,
    IN_ERROR_BADEXTHDRLEN_BNEP_DATA_IND,
    IN_ERROR_UNKNOWNEXTTYPE_BNEP_DATA_IND,
    IN_ERROR_CONTROLCRUD_BNEP_DATA_IND,
    IN_ERROR_PREMATURE_BNEP_DATA_IND,
    IN_ERROR_NOMEMRECORDED_BNEP_DATA_IND,
    IN_ERROR_NOMEMMSG_BNEP_DATA_IND,
    IN_ERROR_NOLISTENER_BNEP_DATA_IND,

    IN_ERROR_BADSIZE_BNEP_HANDLE_CONTROL,
    IN_ERROR_UNEXPECTEDBAD_BNEP_HANDLE_CONTROL,
    IN_ERROR_NOMEM_BNEP_HANDLE_CONTROL,
    IN_ERROR_UNEXPECTEDREQ_BNEP_HANDLE_CONTROL,
    IN_ERROR_UNEXPECTEDRSP_BNEP_HANDLE_CONTROL,
    IN_ERROR_NOMEMMSG_BNEP_HANDLE_CONTROL,

    IN_ERROR_NOMEM_BNEP_UP_IND,
    IN_ERROR_NOMEMMSG_BNEP_UP_IND,

    IN_ERROR_NOMAP_IPV4_DATA_IND,
    IN_ERROR_HDRTOOSHORT_IPV4_DATA_IND,
    IN_ERROR_NOTIPV4_IPV4_DATA_IND,
    IN_ERROR_TOOSHORT_IPV4_DATA_IND,
    IN_ERROR_BADHDRLEN_IPV4_DATA_IND,
    IN_ERROR_BADCHECKSUM_IPV4_DATA_IND,
    IN_ERROR_FRAGMENTED_IPV4_DATA_IND,
    IN_ERROR_BADCAST_IPV4_DATA_IND,
    IN_ERROR_BADSRC_IPV4_DATA_IND,
    IN_ERROR_OPTIONS_IPV4_DATA_IND,
    IN_ERROR_BADDST_IPV4_DATA_IND,
    IN_ERROR_BADDSTNOREPLY_IPV4_DATA_IND,
    IN_ERROR_UNKNOWNPROTOCOL_IPV4_DATA_IND,
    IN_ERROR_UNKNOWNPROTOCOLNOREPLY_IPV4_DATA_IND,

    IN_ERROR_NOMAP_IPV6_DATA_IND,
    IN_ERROR_HDRTOOSHORT_IPV6_DATA_IND,
    IN_ERROR_NOTIPV6_IPV6_DATA_IND,
    IN_ERROR_TOOSHORT_IPV6_DATA_IND,
    IN_ERROR_BADCAST_IPV6_DATA_IND,
    IN_ERROR_BADSRC_IPV6_DATA_IND,
    IN_ERROR_BADDST_IPV6_DATA_IND,
    IN_ERROR_BADDSTNOREPLY_IPV6_DATA_IND,
    IN_ERROR_BADOPTHDRLEN_IPV6_DATA_IND,
    IN_ERROR_BADOPTHDRLENNOREPLY_IPV6_DATA_IND,
    IN_ERROR_UNKNOWNNEXTHDR_IPV6_DATA_IND,
    IN_ERROR_UNKNOWNNEXTHDRNOREPLY_IPV6_DATA_IND,
    IN_ERROR_NONEXTHDR_IPV6_DATA_IND, /* not really an error, but unexpected */

    IN_ERROR_NOMAP_ARP_DATA_IND,
    IN_ERROR_TOOSHORT_ARP_DATA_IND,
    IN_ERROR_NOTETHERNET_ARP_DATA_IND,
    IN_ERROR_NOTIPV4_ARP_DATA_IND,
    IN_ERROR_BADLEN_ARP_DATA_IND,
    IN_ERROR_NOMEMMSG_ARP_DATA_IND,

    IN_ERROR_HDRTOOSHORT_ICMPV4_DATA_IND,
    IN_ERROR_BADCHECKSUM_ICMPV4_DATA_IND,
    IN_ERROR_ERRORTOOSHORT_ICMPV4_DATA_IND,
    IN_ERROR_ERRORNOTIPV4_ICMPV4_DATA_IND,
    IN_ERROR_UNKNOWNPROTOCOL_ICMPV4_DATA_IND,
    IN_ERROR_UNKNOWNTYPE_ICMPV4_DATA_IND,
    IN_ERROR_ECHOTOOSHORT_ICMPV4_DATA_IND,
    IN_ERROR_ECHONOREPLY_ICMPV4_DATA_IND,

    IN_ERROR_HDRTOOSHORT_ICMPV6_DATA_IND,
    IN_ERROR_BADCHECKSUM_ICMPV6_DATA_IND,
    IN_ERROR_ERRORTOOSHORT_ICMPV6_DATA_IND,
    IN_ERROR_ERRORNOTIPV6_ICMPV6_DATA_IND,
    IN_ERROR_UNKNOWNNEXTHDR_ICMPV6_DATA_IND,
    IN_ERROR_UNKNOWNTYPE_ICMPV6_DATA_IND,
    IN_ERROR_ECHOTOOSHORT_ICMPV6_DATA_IND,
    IN_ERROR_ECHONOREPLY_ICMPV6_DATA_IND,

    IN_ERROR_NSBADHOPLIMIT_ICMPV6_DATA_IND,
    IN_ERROR_NSTOOSHORT_ICMPV6_DATA_IND,
    IN_ERROR_NSNOTCODE0_ICMPV6_DATA_IND,
    IN_ERROR_NSTRGMC_ICMPV6_DATA_IND,
    IN_ERROR_NSBADOPT_ICMPV6_DATA_IND,
    IN_ERROR_NSBADUNSPEC_ICMPV6_DATA_IND,

    IN_ERROR_NANOMEMPAY_ICMPV6_DATA_IND,
    IN_ERROR_NABADHOPLIMIT_ICMPV6_DATA_IND,
    IN_ERROR_NATOOSHORT_ICMPV6_DATA_IND,
    IN_ERROR_NANOTCODE0_ICMPV6_DATA_IND,
    IN_ERROR_NATRGMC_ICMPV6_DATA_IND,
    IN_ERROR_NABADOPT_ICMPV6_DATA_IND,
    IN_ERROR_NABADMCSOL_ICMPV6_DATA_IND,
    IN_ERROR_NABADTRG_ICMPV6_DATA_IND,

    IN_ERROR_HDRTOOSHORT_UDP_DATA_IND,
    IN_ERROR_BADSIZE_UDP_DATA_IND,
    IN_ERROR_ZEROCHECKSUM_UDP_DATA_IND,
    IN_ERROR_BADCHECKSUM_UDP_DATA_IND,

    IN_ERROR_BADREQUEST_MDNSD_DATA_IND,

    IN_ERROR_BADDST_TCP_DATA_IND,
    IN_ERROR_BADSRC_TCP_DATA_IND,
    IN_ERROR_HDRTOOSHORT_TCP_DATA_IND,
    IN_ERROR_BADCHECKSUM_TCP_DATA_IND,
    IN_ERROR_BADDATAOFFSET_TCP_DATA_IND,
    IN_ERROR_BADSRCPORT_TCP_DATA_IND,

    IN_ERROR_UNLOVED_BNEP_DATA_REQ,
    IN_ERROR_PREMATURE_BNEP_DATA_REQ,
    IN_ERROR_NOMEMHDR_BNEP_DATA_REQ,
    IN_ERROR_NOMEMMSG_BNEP_DATA_REQ,

    IN_ERROR_UNLOVED_BNEP_EXTENDED_DATA_REQ,
    IN_ERROR_PREMATURE_BNEP_EXTENDED_DATA_REQ,
    IN_ERROR_NOMEMHDR_BNEP_EXTENDED_DATA_REQ,
    IN_ERROR_NOMEMMSG_BNEP_EXTENDED_DATA_REQ,

    IN_ERROR_NOMEMHDR_BNEP_EXTENDED_MULTICAST_DATA_REQ,
    IN_ERROR_NOMEMCOPY_BNEP_EXTENDED_MULTICAST_DATA_REQ,
    IN_ERROR_NOMEMMSG_BNEP_EXTENDED_MULTICAST_DATA_REQ,
    IN_ERROR_NOMEMRECORDED_BNEP_EXTENDED_MULTICAST_DATA_REQ,

    IN_ERROR_NOMEM_BNEP_SEND_L2CA_DATA_REQ,
    IN_ERROR_BACKLOG_BNEP_SEND_L2CA_DATA_REQ,
    IN_ERROR_NOMEM_BNEP_SEND_NEXT_L2CA_DATA_REQ,

    IN_ERROR_NOMEMPROTOCOLMBLK_IPV4_DATA_REQ,
    IN_ERROR_NOMEMHDR_IPV4_DATA_REQ,
    IN_ERROR_NOMEMMSG_IPV4_DATA_REQ,

    IN_ERROR_NOMEMNEXTHDRMBLK_IPV6_DATA_REQ,
    IN_ERROR_NOMEMHDR_IPV6_DATA_REQ,
    IN_ERROR_NOMEMMSG_IPV6_DATA_REQ,

    IN_ERROR_RATELIMITED_ICMPV4_DATA_REQ,
    IN_ERROR_NOMEMHDR_ICMPV4_DATA_REQ,

    IN_ERROR_RATELIMITED_ICMPV6_DATA_REQ,
    IN_ERROR_NOMEMHDR_ICMPV6_DATA_REQ,

    IN_ERROR_NOMEMPAY_ARP_SEND_REQ,
    IN_ERROR_NOMEMMSG_ARP_SEND_REQ,

    IN_ERROR_NSNOMEMPAY_ICMPV6_SEND_NS,

    IN_ERROR_NOMEMHDR_UDP_DATA_REQ,

    IN_ERROR_NOMEMHDR_TCP_OUTPUT,
    IN_ERROR_NOMEMHDR_TCP_SEND_CONTROL
};


/******************************************************************************

    macro IN_ERROR_LOG  --  log an error

******************************************************************************/

#ifndef STRUCT_PARSE_IGNORE
#ifdef PCTESTBUILD
#define IN_ERROR_LOG(s,n) printf ("*** %s (%u)\n", s, n)
#else
#if 0 /* def BNEP_DEBUG -- this no longer works for multiple connections */
#include "csr_bt_bnep_private.h"
#include "app/bluestack/l2cap_prim.h"
#include "csr_bt_tasks.h"
#define IN_ERROR_LOG(s,n) \
if (bnep_send_db) \
{ \
    L2CA_DATAWRITE_REQ_T *l2dr = pmalloc (sizeof (*l2dr)); \
    CsrMblk *mblk = mblk_new_in (3, TRUE); \
    mblk->pData[0] = 0xdb; \
    mblk->pData[1] = n; \
    l2dr->type = L2CA_DATAWRITE_REQ; \
    l2dr->cid = bnep_local_cid; \
    l2dr->length = 0; \
    l2dr->data = mblk; \
    CsrSchedMessagePut(L2CAP_IFACEQUEUE, L2CAP_PRIM, l2dr); \
} else /* Semicolon deliberately missing */
#else
#define IN_ERROR_LOG(s,n)
#endif /* (!)0 */
#ifdef BNEP_DEBUG
#define ZZZCOUNTER(x) if (1) { static unsigned zzz ## x; ++zzz ## x; } else
#else
#define ZZZCOUNTER(x)
#endif
#endif /* (!)PCTESTBUILD */
#endif /* !STRUCT_PARSE_IGNORE */


/******************************************************************************

    struct IN_SOCKET  --  a TCP/UDP socket
    #define for wildcarded ports
    macro for passive opens

******************************************************************************/

typedef struct IN_SOCKET
{
    struct IN_SOCKET *next;
    IP_ADDR loc_addr;
    IP_ADDR rem_addr;
    CsrUint16 loc_port;
    CsrUint16 rem_port;
    unsigned int state : 8;
    unsigned int is_wild_rem_addr : 1;
    unsigned int is_wild_rem_port : 1;
    CsrSchedQid phandle;
    void *extra;
} IN_SOCKET;
#define IN_IS_PASSIVE_OPEN(s) ((s)->is_wild_rem_addr || (s)->is_wild_rem_port)


/******************************************************************************

    function in_find_socket  --  find socket in socket list

Any fully-specified match wins over a partially-specified match irrespective of
their order in the list.  Otherwise, which of multiple partially-specified
matches wins is undefined.

******************************************************************************/

IN_SOCKET *in_find_socket (IN_SOCKET *head,
                           const IP_ADDR *loc_addr,
                           const IP_ADDR *rem_addr,
                           CsrUint16 loc_port,
                           CsrUint16 rem_port);


/******************************************************************************

    function in_compute_checksum_with_pseudoheader  --  compute/check checksum
    #defines for Internet checksum

To compute the checksum, set the checksum field to IN_CHECKSUM_INIT and
then write it from the result of calling this function (may be zero).
To check the checksum, verify that the result of calling this function
is IN_CHECKSUM_VALID.

If hdr_len is odd then the last header octet should be in the MSB of
the last word and the LSB of the last word should be zero.

hdr_len may be zero (in this case hdr may be NULL).  mblk may be NULL.

******************************************************************************/

CsrUint16 in_compute_checksum_with_pseudoheader (
                      const IP_ADDR *src_addr, const IP_ADDR *dst_addr,
                      CsrUint8 protocol,
                      const CsrUint16 *hdr, CsrUint16 hdr_len,
                      const CsrMblk *mblk);
#define IN_CHECKSUM_INIT 0
#define IN_CHECKSUM_VALID 0


#ifdef INSTALL_IPV4_MODULE

/******************************************************************************

    function in_compute_checksum  --  compute/check checksum

To compute the checksum, set the checksum field to IN_CHECKSUM_INIT and
then write it from the result of calling this function (may be zero).
To check the checksum, verify that the result of calling this function
is IN_CHECKSUM_VALID.

If hdr_len is odd then the last header octet should be in the MSB of
the last word and the LSB of the last word should be zero.

hdr_len may be zero (in this case hdr may be NULL).  mblk may be NULL.

******************************************************************************/

CsrUint16 in_compute_checksum (
                      const CsrUint16 *hdr, CsrUint16 hdr_len,
                      const CsrMblk *mblk);


/******************************************************************************

    function in_compute_ipv4_checksum  --  compute IPv4 header checksum

To compute the checksum, write it from the result of calling this function
(may be zero).  There is no need to zero the checksum field before calling
this function.

The header is assumed to be minimum-size (i.e. 20 octets).

******************************************************************************/

CsrUint16 in_compute_ipv4_checksum (const CsrUint16 *hdr);

#endif

#ifdef __cplusplus
}
#endif

#endif /* IN_PRIVATE_H */

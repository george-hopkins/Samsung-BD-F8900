/******************************************************************************

                (c) CSR plc 2008

                All rights reserved

FILE:           csr_bt_bnep.c  --  _bluetooth _network _encapsulation _protocol

DESCRIPTION:    $Log: csr_bt_bnep.c,v $
                Revision 1.21  2004/01/07 07:20:21  ab02
                Handling bnep msgfrags

                Revision 1.20  2003/10/14 17:20:17  ca01
                Added PANU patch (same subnet).

                Revision 1.19  2003/10/14 10:29:14  ab02
                Removed l2cap register from init and moved to bnep extended register req

                Revision 1.18  2003/10/08 11:41:22  mp02
                no message

                Revision 1.17  2003/10/08 11:22:43  kb01
                Update for exclude_bnep_module og warnings

                Revision 1.16  2003/10/02 10:53:25  ab02
                no message

                Revision 1.15  2003/10/02 08:54:57  mp02
                bdAddr to etherAddr function changed

                Revision 1.14  2003/10/02 07:40:10  mp02
                Updated to new version from Mark d. 15-9-03

                Revision 1.14  2003/09/12 17:39:43  mgr
                Allow for listening to IPv4 limited broadcasts and IPv6 mDNS multicasts.
                Support remotely-initiated BNEP role change requests.
                B-622: Fix spurious disconnects and baseband role violations when
                paged device switches baseband roles before establishment.
                B-623: Prevent ETHER_UNKNOWN addresses from leaking out.
                B-824: Disallow connections without PANUs.
                Add support for xL2CA_PutMsg().
                Fix call to non-existent l2cap_put_message().

                Revision 1.13  2003/05/12 10:37:05  npm
                B-468 - bootmode now implemented as layer over psbc
                B-469 - wrong ifdef used in tasks.in

                Revision 1.6.28.3  2003/05/08 12:51:41  npm
                B-468 Implement bootmode as layer over psbc.

                This involved:-

                 Replacing calls to bootmode(BM_ONCHIP_HCI_CLIENT) by onchip_hci_client.

                 Introducing the concept of a 'raw' store for direct access to the ps.
                 (see psbc_bootmode.h for full description).

                 Implementing a translation layer between bootmode and 'base' keys.

                 Modifications to the existing bootmode mechanism.

                 Removing the bootmode check in hostio since this is now handled by the key
                 translation.

                Revision 1.12  2003/03/20 10:52:21  npm
                Implement bootmode support.
                This involves:
                 - adding a bootmode subsystem
                 - replacing all traces of #ifdef BUILD_XXX with calls to bootmode
                 - rejigging LM message routing
                 - checking all task initialisation against bootmode.
                 - adding dummy fields to certain HCI primitives/events for hci_convert

                This submission also includes SMS's link-key snooping mechanism and a fix for
                B-43.

                Revision 1.11  2003/02/24 16:39:55  bs01
                Merge from gccsdk-2-7-branch.
                T580: Fix bridging when remote device could have been a NAP/GN.
                Disallow all BNEP role changes (avoid duplicate BNEP_CONNECT_INDs).
                802.1p is actually 802.1Q.

                Revision 1.10  2003/02/24 15:43:52  sms
                H18.4: Major CsrSchedTidy-up of buffer subsystem interface.

                * Removed buf_copy_buffer_to_buffer(). Added buf_copy_msgfrag(),
                  buf_copy_msgfrag_at() and buf_copy_partial_msgfrag_at(). New
                  functions to free space check so explicit check could be removed at
                  various places.

                  * buf_get_capacity(), buf_add_indices(), buf_sub_indices() and
                  buf_free_buffer_memory() now take a BUFFER * instead of a bufhandle.

                  * Added buf_get_next_msg_len().

                  * Added buf_split_msgfrag().

                  * Added new macros to synchronise software and hardware buffer
                  indices, to be used in preference to buf_set_bufhandle_offset().

                  * Hid definition of MSGSET - now not needed outside buffer subsystem.

                Various other subsystems were changed to use the new interface: mblk,
                hci_convert, buffer, hostio (and hostio_*), lc, pan, pcm, radiotest
                and vm.

                Intenal changes to buffer subsystem:

                * buf_clear_msgfrag_out_of_order() CsrSchedTidied up.

                * buf_map_msgfrag_prewrite() now calls buf_map_index().

                Other related changes:

                * mblk: Added CsrMblkCopy_msgfrag_to_buffer_at() to copy an entire
                  single MSGFRAG mblk to a buffer, and added codes to call this
                  function.

                  * nonhci_convert: Prefer to allocate MSGFRAGs on stack where possible.

                  * vm: Some dead code eliminated (old versions of vmbuff_slack()).

                Revision 1.9  2003/01/08 15:57:11  mgr
                Require that connection requests have at least one side be a PANU.
                Prevent races due to simultaneous connection establishment.
                Allow passive connections to specify multiple remote roles.

                Revision 1.8  2002/11/12 19:31:44  mgr
                Add BNEP_HCI_SWITCH_ROLE_RSP.
                Improve flood defences.
                Decrease memory use (lazy mblk copy and unionised mblk queue link).
                Implement disconnect reason codes.
                Implement forwarding of unknown extensions (and of 802.1Q headers).

                Revision 1.7  2002/10/18 16:48:14  mgr
                Implement new NAP/GN-supporting BNEP API.

                Revision 1.6  2002/08/30 18:01:00  mgr
                Rename IP_ADDRs' s6_addr[] to w[].
                Rename PSKEY_IPVn_FIXED_* to PSKEY_IPVn_STATIC_*.
                Rename PSKEY_IPVn_ROUTER_ADDR to PSKEY_IPVn_STATIC_ROUTER_ADDR.

                Revision 1.5  2002/08/30 18:01:00  mgr
                Rename IP_ADDRs' s6_addr[] to w[].
                Rename PSKEY_IPVn_FIXED_* to PSKEY_IPVn_STATIC_*.
                Rename PSKEY_IPVn_ROUTER_ADDR to PSKEY_IPVn_STATIC_ROUTER_ADDR.

                Revision 1.4  2002/08/30 14:49:20  mgr
                E2462 (messages received before setup complete).
                Improve L2CAP config handling.
                Improve L2CAP duplicate handling (grr\!).
                Fix blindingly obvious -- NOT! -- memory leak caused by L2CAP API (grr\!).

                Revision 1.3  2002/08/20 17:39:23  mgr
                E2394 (PANU-PANU connections).
                E2405 (SETUP_RESPONSE codes).

                Revision 1.2  2002/07/18 14:05:24  mgr
                Merge from chef-1-branch.

                Revision 1.1.2.11  2002/07/05 14:12:20  mgr
                Add limited support for extension headers (just skip them).
                Add ability to send options extension headers as part of ping test.
                Pull FFS code out into its own directory and
                consequently make tcp_inject a bit less parochial.
                Disconnect on L2CAP configuration failure rather than panicking.
                Panic on failure to send BNEP_CONNECT_CFM rather than ignoring.

                Revision 1.1.2.9  2002/07/02 16:21:46  mgr
                Add ability to perform IPv6 mDNS tests (to ff02::1) and
                get the remote IPv4 link-local address.
                Fix UDP checksums (+0 sent as -0 as required by RFC768).
                Support the No Next Header extension header.  Oh, yeah, baby.

                Revision 1.1.2.8  2002/06/28 19:07:34  mgr
                Split out UDP layer.
                Add mDNS server.
                Make radioio cope with odd-length packed CsrPmemAlloc ('isIN') mblks.
                Add ability to perform ping and mDNS tests, and cause link-local jams.

                Revision 1.1.2.7  2002/06/21 10:15:58  mgr
                Changes made at PANF2F 2002-06.

                Revision 1.1.2.6  2002/06/05 17:49:08  mgr
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

                Revision 1.1.2.5  2002/04/15 18:03:02  mgr
                Add IPv4 support.
                Add QUOTE server.
                Add sanity-checking for addresses received in ARP/ND.

                Revision 1.1.2.4  2002/04/05 18:39:33  mgr
                Make the TCP primitives public.
                Improve the public/private header split.
                Some lint pacification.

                Revision 1.1.2.3  2002/03/28 19:26:36  mgr
                Tighten up state checking.
                Implement T_crt (and T_setup for the responder).
                Add BNEP_DISCONNECT_REQ and implement BNEP_DOWN_IND.
                Set the default link policy to allow switch, hold, sniff and park.
                Fix maximum tohost packet size to cope with maximum-size Ethernet packets.
                Improve config process (pay attention to more_flag and handle collisions).
                Don't ignore Ethernet packets whose source is apparently us (for DAD).

                Revision 1.1.2.2  2002/03/22 13:03:08  mgr
                Changes made at UPF7,5.

                Revision 1.1.2.1  2002/03/18 15:19:07  mgr
                Initial revision.

REVISION:        $Revision: #2 $

******************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE

#include "csr_sched.h"
#include "csr_types.h"
#include "csr_env_prim.h"
#include "csr_bt_bnep_private.h"
#include "bluestacklib.h"
#include "csr_bt_in_private.h"

#include "csr_sched.h"
#include "csr_panic.h"
#include "csr_bt_tasks.h"
#include "csr_pmem.h"
#include "sdputil.h"
#include "l2caplib.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "l2cap_prim.h"
#include "dm_prim.h"
#include "csr_bt_bnep_prim.h"
#include "csr_bt_usr_config.h"
#include "csr_bt_profiles.h"

#include "csr_bt_util.h"

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

/******************************************************************************

    #defines for BNEP magic numbers

******************************************************************************/
/*bchs change */
#define NO_TID 0

#define BNEP_PSM 0x000f /* www.bluetooth.org/assigned-numbers/l2cap.htm */
#define BNEP_MTU 1691 /* BNEP0.95a/2.2 */
#define BNEP_NO_CID 0 /* BT1.1/D/2.1 */
#define BNEP_SIGNALLING_CID 1 /* BT1.1/D/2.1 */
#define BNEP_CONNECTIONLESS_CID 2 /* BT1.1/D/2.1 */
#define BNEP_NO_UUID 0
#define BNEP_NO_PHANDLE 0
#define BNEP_RETX_COUNTER_BITS 2
#define BNEP_T_SETUP 30 /* In seconds, chosen based on BNEP0.96e/2.6.3 */
#define BNEP_T_CRT 10 /* In seconds, as recommended in BNEP0.96e/2.6.3 */
#define BNEP_T_CRT_N_TX (BNEP_T_SETUP / BNEP_T_CRT)
#if BNEP_T_CRT_N_TX >= 1 << BNEP_RETX_COUNTER_BITS
#error BNEP_T_CRT_N_TX too big for BNEP_RETX_COUNTER_BITS
#endif
#define BNEP_ID_LOCAL BNEP_NO_CID /* HERETODO move these to csr_bt_bnep_prim.h */
#define BNEP_ID_DST BNEP_SIGNALLING_CID

/* 16 bit numbers write */
#define PUT_UINT16(ptr, offset, data)          \
{                                              \
    /* output is big-endian */                 \
    ptr[offset] = (CsrUint8)(data >> 8) & 0xFF;\
    ptr[offset + 1] = (CsrUint8)(data & 0xFF); \
}

/* max number of outstanding L2CAP frames */
#define BNEP_MAX_L2_BUSY_COUNT            (2)
/* circular list of recorded outstanding frames */
#define BNEP_MAX_RECORDED_COUNT         (0x01 << 2) /* defined so mask can be used as modulus */
#define BNEP_MAX_RECORDED_COUNT_MASK     (BNEP_MAX_RECORDED_COUNT - 1)


/******************************************************************************

    #defines for BNEP packet header

******************************************************************************/

#define BNEP_OFFSETOF_TYPE 0
#define BNEP_TYPE_GENERAL_ETHER 0x00
#define BNEP_TYPE_CONTROL 0x01
#define BNEP_TYPE_COMPRESSED_ETHER 0x02
#define BNEP_TYPE_COMPRESSED_ETHER_SRC_ONLY 0x03
#define BNEP_TYPE_COMPRESSED_ETHER_DST_ONLY 0x04
#define BNEP_TYPE_8022_LLC 0x7f
#define BNEP_FLG_EXTENSION 0x80
#define BNEP_MIN_HDR_LEN 2 /* Control with no payload */
/* Offsets from here are from the control type octet */
#define BNEP_OFFSETOF_CONTROL_TYPE 0
#define BNEP_CONTROL_TYPE_CONTROL_COMMAND_NOT_UNDERSTOOD 0x00
#define BNEP_OFFSETOF_UNKNOWN_CONTROL_TYPE 1
#define BNEP_CONTROL_TYPE_SETUP_CONNECTION_REQUEST_MSG 0x01
#define BNEP_OFFSETOF_UUID_SIZE 1
#define BNEP_OFFSETOF_DST_UUID 2
#define BNEP_CONTROL_TYPE_SETUP_CONNECTION_RESPONSE_MSG 0x02
#define BNEP_OFFSETOF_RESPONSE_MESSAGE 1
#define BNEP_CONTROL_TYPE_SETUP_RESPONSE_SUCCESSFUL 0x0000
#define BNEP_CONTROL_TYPE_SETUP_RESPONSE_INVALID_DST_UUID 0x0001
#define BNEP_CONTROL_TYPE_SETUP_RESPONSE_INVALID_SRC_UUID 0x0002
#define BNEP_CONTROL_TYPE_SETUP_RESPONSE_INVALID_UUID_SIZE 0x0003
#define BNEP_CONTROL_TYPE_SETUP_RESPONSE_CONNECTION_NOT_ALLOWED 0x0004
#define BNEP_CONTROL_TYPE_FILTER_NET_TYPE_SET_MSG 0x03
#define BNEP_OFFSETOF_LIST_LEN 1
#define BNEP_CONTROL_TYPE_FILTER_NET_TYPE_RESPONSE_MSG 0x04
#define BNEP_CONTROL_TYPE_FILTER_MULTI_ADDR_SET_MSG 0x05
#define BNEP_CONTROL_TYPE_FILTER_MULTI_ADDR_RESPONSE_MSG 0x06
#define BNEP_CONTROL_TYPE_FILTER_RESPONSE_SUCCESSFUL 0x0000
#define BNEP_CONTROL_TYPE_FILTER_RESPONSE_UNSUPPORTED 0x0001
/* Offsets from here are from the extension type octet */
#define BNEP_OFFSETOF_EXTENSION_TYPE 0
#define BNEP_OFFSETOF_EXTENSION_LEN 1
#define BNEP_MIN_EXTENSION_HDR_LEN 2 /* Extension with no payload */
#define BNEP_EXTENSION_TYPE_CONTROL 0x00


/******************************************************************************

    #defines for flood defences

BNEP_MAX_UNICAST_PERCENT_MEMORY defines the maximum percentage of blocks
of size between BNEP_UNICAST_LO and BNEP_UNICAST_HI which can be taken
before outgoing unicast packets are dropped.
BNEP_MAX_MULTICAST_PERCENT_MEMORY defines the maximum percentage of blocks
of size between BNEP_MULTICAST_LO and BNEP_MULTICAST_HI which can be taken
before outgoing multicast packets are dropped.
BNEP_MAX_QUEUE_LENGTH defines the maximum number of (unicast or multicast)
packets which can be queued for transmission to a particular remote device.
BNEP_MAX_MULTICAST_QUEUE_LENGTH defines the maximum number of remotely-
originated multicast packets which can be queued for transmission to a
particular remote device.
BNEP_MAX_LOCAL_MULTICAST_QUEUE_LENGTH defines the maximum number of locally-
originated multicast packets which can be queued for transmission to a
particular remote device (this has to be sufficient for the flurry of
multicasts the IPv4/IPv6/ARP layers send on link establishment).
Any packet currently being transmitted is not included in these three
thresholds.
BNEP_MAX_QUEUE_LENGTH >= BNEP_BNEP_MAX_LOCAL_MULTICAST_QUEUE_LENGTH >=
BNEP_MAX_MULTICAST_QUEUE_LENGTH >= 1 should hold.

All this is a bit like the throttle thresholds in the LC: it should really
be adjusted following experience gained in the field, but won't be.

******************************************************************************/

#define BNEP_MAX_UNICAST_PERCENT_MEMORY 80
#define BNEP_UNICAST_LO (sizeof (MSGFRAG))
#define BNEP_UNICAST_HI (sizeof (CsrMblk))
#define BNEP_MAX_MULTICAST_PERCENT_MEMORY 65
#define BNEP_MULTICAST_LO (sizeof (CsrMblk))
#define BNEP_MULTICAST_HI (sizeof (BNEP_RECORDED))
/* bchs change  allow setting this value from the usrconfig.h file*/
#define BNEP_MAX_QUEUE_LENGTH  CSR_BT_MAX_BSL_QUEUE_LENGTH
#define BNEP_MAX_LOCAL_MULTICAST_QUEUE_LENGTH  CSR_BT_MAX_BSL_LOCAL_MULTICAST_QUEUE_LENGTH
#define BNEP_MAX_MULTICAST_QUEUE_LENGTH 2


/******************************************************************************

    variable bnep_local_addr  --  local Ethernet address

******************************************************************************/

ETHER_ADDR bnep_local_addr;


/******************************************************************************

    variable bnep_local_phandle  --  phandle of connection manager client
    struct BNEP_CONNECTION  --  a BNEP connection
    variable bnep_connections  --  list of BNEP connections
    variable bnep_passive_connection_flags  --  flags for BNEP listener
    struct BNEP_RECORDED  --  recorded delivery of BNEP messages

******************************************************************************/

CsrMblk *CsrBtBnepMblkCreateDup(const void *block, CsrMblkSize block_size)
{
    return CsrMblkDataCreate(CsrMemDup(block, block_size),
                             block_size,
                             TRUE); /* allow free */
}


static CsrSchedQid bnep_local_phandle;
enum BNEP_STATE
{
    BNEP_CONNECTING,
    BNEP_CONFIGURING,
    BNEP_ESTABLISHING,
    BNEP_ESTABLISHED
};
#define from_bnep_client reserved1
#define disable_extended_cuckoo reserved5
#define manual_bridge_cuckoo reserved6
#define disable_stack_cuckoo reserved7
#define lax_roles reserved15
#define BNEP_FLAGS_TO_REM_UUID(f) ((f).rnap ? BNEP_UUID_NAP : ((f).rgn ? BNEP_UUID_GN : BNEP_UUID_PANU))
#define BNEP_FLAGS_TO_LOC_UUID(f) ((f).lnap ? BNEP_UUID_NAP : ((f).lgn ? BNEP_UUID_GN : BNEP_UUID_PANU))
#define BNEP_IS_COMPATIBLE(f,l,r) (((l) == BNEP_UUID_PANU ? (f).lpanu : ((l) == BNEP_UUID_NAP ? (f).lnap : (f).lgn)) && ((r) == BNEP_UUID_PANU ? (f).rpanu : ((r) == BNEP_UUID_NAP ? (f).rnap : (f).rgn)))
static CsrUint16 bnep_local_role, bnep_remote_role; /* HERETODO this needs to be done properly (lots of race hazards at the moment) */

typedef struct BNEP_RECORDED
{
    CsrMblk padding;
    CsrUint8 isARecorded;

    l2ca_cid_t id_last, id_not;
    CsrUint16 ether_type;
    ETHER_ADDR dst_addr;
    ETHER_ADDR src_addr;
    CsrMblk *mblk;
    CsrUint16 unk_extn_len;

    CsrBool reserved1;
} BNEP_RECORDED;

#define BNEP_IS_A_RECORDED(p) (*((CsrBool *) (CsrMblkGetMetadata(p))) == TRUE)

typedef struct MBLK_QUEUE
{
    struct MBLK_QUEUE *next;
    void *mblk_or_recorded; /* depending on MSB of first word... */
} MBLK_QUEUE;

typedef struct BNEP_CONNECTION
{
    struct BNEP_CONNECTION *next;
    MBLK_QUEUE *mblks;
    l2ca_cid_t local_cid;
    BNEP_CONNECT_REQ_FLAGS flags;
    unsigned int state : 3; /* 2 would be enough */
    unsigned int switch_failure : 1;
    unsigned int l2_busy : 1;
    unsigned int initiator : 1;
    unsigned int half_config : 1; /* initiator only */
    unsigned int mtu_recd : 1;
    unsigned int config_bad : 1;
    unsigned int client_knows : 1;
    unsigned int client_requested_disconnection : 1;
    unsigned int retx_counter : BNEP_RETX_COUNTER_BITS;
    ETHER_ADDR rem_addr;
    CsrSchedTid t_setup;
    BNEP_RECORDED *recordedList[BNEP_MAX_RECORDED_COUNT];
    CsrUint16 recordedListWrIdx;
    CsrUint16 recordedListRdIdx;
    CsrUint16 l2_busyCount;
} BNEP_CONNECTION;

static BNEP_CONNECTION *bnep_connections;
static BNEP_CONNECT_REQ_FLAGS bnep_passive_connection_flags;


/******************************************************************************

    macro PUT_BNEP_MESSAGE  --  send a message to a (possibly off-chip) queue

******************************************************************************/

static void PUT_BNEP_MESSAGE(CsrSchedQid phandle, void *msg)
{
    CsrSchedMessagePut(phandle, CSR_BT_BNEP_PRIM, msg);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *  mblk_xnew_in
 *
 *  DESCRIPTION
 *  Create a new Internet message block with a CsrPmemAlloc of a given size.
 *
 *  RETURNS/MODIFIES
 *  Pointer to a new MBLK or NULL if failed.
 *----------------------------------------------------------------------------*/
static CsrMblk *mblk_xnew_in (void **block_ptr, CsrMblkSize data_len, CsrBool special)
{
    CsrMblk *mblk;
    const CsrUint8 metadata[] = { 0 };
    CsrUint16 metasize = sizeof(metadata);

    /*data_len += (CsrUint16) (1 + (special != FALSE));*/
    mblk = CsrMblkMallocCreateMeta(block_ptr, data_len, metadata, metasize);

    return mblk;
}

/******************************************************************************

    function bnep_to_ether_addr  --  convert a BD_ADDR_T to an ETHER_ADDR

******************************************************************************/


static ETHER_ADDR bnep_to_ether_addr (const BD_ADDR_T *ba)
{
    ETHER_ADDR ea;

    ea.w[0] = ba->nap;
    ea.w[1] = ba->uap << 8 | (CsrUint16) (ba->lap >> 16);
    ea.w[2] = (CsrUint16) ba->lap;

    return ea;
}

/******************************************************************************

    function bnep_from_ether_addr  --  convert  an ETHER_ADDR to a BD_ADDR_T

******************************************************************************/

static BD_ADDR_T bnep_from_ether_addr (const ETHER_ADDR *ea)
{
    BD_ADDR_T ba;

    ba.nap = ea->w[0];
    ba.uap = ea->w[1] >> 8;
    ba.lap = (CsrUint32) (ea->w[1] & 0xff) << 16 | ea->w[2];

    return ba;
}


/******************************************************************************

    function bnep_cid_to_connection  --  map a l2ca_cid_t to a BNEP_CONNECTION

******************************************************************************/

static BNEP_CONNECTION *bnep_cid_to_connection (l2ca_cid_t cid)
{
    BNEP_CONNECTION *bnep_connection;

    for (bnep_connection = bnep_connections;
         bnep_connection != NULL;
         bnep_connection = bnep_connection->next)
    {
        if (bnep_connection->local_cid == cid)
        {
            return bnep_connection;
        }
    }

    return NULL;
}


/******************************************************************************

    function bnep_rem_addr_to_connection  --  map an ETHER_ADDR to a BNEP_CONNECTION

******************************************************************************/

static BNEP_CONNECTION *bnep_rem_addr_to_connection (const ETHER_ADDR *rem_addr)
{
    BNEP_CONNECTION *bnep_connection;

    for (bnep_connection = bnep_connections;
         bnep_connection != NULL;
         bnep_connection = bnep_connection->next)
    {
        if (ETHER_IS_SAME_ADDR (&bnep_connection->rem_addr, rem_addr))
        {
            return bnep_connection;
        }
    }

    return NULL;
}


/******************************************************************************

    function bnep_connection_in_limbo  --  is any connection being established?

******************************************************************************/

static CsrBool bnep_connection_in_limbo (void)
{
    BNEP_CONNECTION *bnep_connection;

    for (bnep_connection = bnep_connections;
         bnep_connection != NULL;
         bnep_connection = bnep_connection->next)
    {
        if (bnep_connection->state != BNEP_ESTABLISHED)
        {
            return TRUE;
        }
    }

    return FALSE;
}


/******************************************************************************

    function bnep_create_connection  --  create a BNEP_CONNECTION

All fields other than next are zeroed.

******************************************************************************/

static BNEP_CONNECTION *bnep_create_connection (void)
{
    BNEP_CONNECTION *bnep_connection;

    bnep_connection = CsrPmemZalloc (sizeof (*bnep_connection));
    if (bnep_connection != NULL)
    {
        bnep_connection->next = bnep_connections;
        bnep_connections = bnep_connection;
    }

    return bnep_connection;
}


/******************************************************************************

    function bnep_destroy_connection  --  destroy a BNEP_CONNECTION

******************************************************************************/

static void bnep_destroy_connection (BNEP_CONNECTION *bnep_connection_wanted)
{
    BNEP_CONNECTION **pbnep_connection, *bnep_connection;
    MBLK_QUEUE *link, *next_link;

    for (pbnep_connection = &bnep_connections;
         (bnep_connection = *pbnep_connection) != NULL;
         pbnep_connection = &bnep_connection->next)
    {
        if (bnep_connection == bnep_connection_wanted)
        {
            /*
            * Remove doomed connection from list.
            */
            *pbnep_connection = bnep_connection->next;

            /*
            * Make sure no timers are left running.
            */
            CsrSchedTimerCancel (bnep_connection->t_setup, NULL, NULL);
            bnep_connection->t_setup = 0;

            /*
            * If we were in the middle of sending something,
            * free any associated record and its mblk.
            */
            while (bnep_connection->recordedListRdIdx != bnep_connection->recordedListWrIdx) 
            {
                CsrMblkDestroy( ((BNEP_RECORDED *) bnep_connection->recordedList[bnep_connection->recordedListRdIdx])->mblk );
                CsrPmemFree( bnep_connection->recordedList[bnep_connection->recordedListRdIdx] );
                bnep_connection->recordedListRdIdx++;
                bnep_connection->recordedListRdIdx &= BNEP_MAX_RECORDED_COUNT_MASK;
            }

            /*
            * Free the mblk queue, including any mblks in any recordeds.
            */
            for (link = bnep_connection->mblks;
                link != NULL;
                link = next_link)
            {
                next_link = link->next;
                if (BNEP_IS_A_RECORDED (link->mblk_or_recorded))
                {
                    CsrMblkDestroy (((BNEP_RECORDED *) link->mblk_or_recorded)->mblk);
                    CsrPmemFree (link->mblk_or_recorded);
                }
                else
                {
                    CsrMblkDestroy (link->mblk_or_recorded);
                }
                CsrPmemFree (link);
            }

            /*
            * Hope I haven't forgotten anything!
            */
            CsrPmemFree (bnep_connection);

            break;
        }
    }

    if (bnep_connections == NULL) /* HERETODO simple hack */
    {
        bnep_local_role = 0;
    }
}


/******************************************************************************

    function bnep_last_link_next  --  find the "next" pointer of the last link

If there are more than max_depth links in the chain, NULL is returned.
If there are no links in the chain, a pointer to the base of the chain
is returned.  max_depth may be 0.

******************************************************************************/

static MBLK_QUEUE **bnep_last_link_next (unsigned max_depth, BNEP_CONNECTION *bnep_connection)
{
    MBLK_QUEUE *link;

    if (bnep_connection->mblks == NULL)
    {
        return &bnep_connection->mblks;
    }

    if (max_depth == 0)
    {
        return NULL;
    }

    for (link = bnep_connection->mblks;
         link->next != NULL;
         link = link->next)
    {
        if (--max_depth == 0)
        {
            break;
        }
    }
    if (max_depth == 0)
    {
        return NULL;
    }

    return &link->next;
}


/******************************************************************************

    function bnep_send_l2ca_data_req  --  send/queue a L2CA_DATAWRITE_REQ

******************************************************************************/
#define pmalloc_percentage_taken_blocks(x,y)  0

static CsrBool bnep_send_l2ca_data_req (void *mblk_or_recorded, BNEP_CONNECTION *bnep_connection)
{
    CsrBool isRecorded = (CsrBool)BNEP_IS_A_RECORDED (mblk_or_recorded);

    if (!bnep_connection->l2_busy)
    {
        CsrMblk *mblk;

        /*
        * Assume the worst until proved otherwise.
        */
        bnep_connection->recordedList[bnep_connection->recordedListWrIdx] = NULL;

        /*
        * Is the mblk hidden in a bnep_recorded?
        * If so, we need to make an exquisite copy.
        */
        if (isRecorded)
        {
            BNEP_RECORDED *bnep_recorded = mblk_or_recorded;
            CsrMblk *mblk_hdr;
            void *mblk_data;
            CsrUint8 *hdr;
            CsrBool uncompressed_src;
            CsrUint8 bnep_type = 0;

            CsrMblkIncRefcount(bnep_recorded->mblk, 1);

            /*
            * Prepend the BNEP header to a copy of the raw mblk;
            * if we can't create either then abort.
            */
            uncompressed_src = !ETHER_IS_SAME_ADDR (&bnep_recorded->src_addr, &bnep_local_addr);
            mblk_hdr = mblk_xnew_in(&mblk_data, (CsrUint16) (1 + ETHER_ADDR_LEN * (1 + (CsrUint8)uncompressed_src) + 2), TRUE);

            hdr = (CsrUint8 *) mblk_data;

            if (bnep_recorded->unk_extn_len != 0)
            {
                bnep_type |= BNEP_FLG_EXTENSION;
            }

            if (uncompressed_src)
            {
                bnep_type |= BNEP_TYPE_GENERAL_ETHER;
                CsrBtWriteUint8Val(&hdr, bnep_type );
                CsrBtWriteBnepAddr(&hdr, &bnep_recorded->dst_addr);
                CsrBtWriteBnepAddr(&hdr, &bnep_recorded->src_addr);
            }
            else
            {
                bnep_type |= BNEP_TYPE_COMPRESSED_ETHER_DST_ONLY;
                CsrBtWriteUint8Val(&hdr, bnep_type );
                CsrBtWriteBnepAddr(&hdr, &bnep_recorded->dst_addr);
            }
            CsrBtWriteUint16Val(&hdr, bnep_recorded->ether_type);

            mblk = CsrMblkAddHead(mblk_hdr, bnep_recorded->mblk);
        }
        /*
        * It's just a plain old boring mblk.
        */
        else
        {
            mblk = mblk_or_recorded;
        }

        /*
        * Try to send the mblk off on its way.
        */
        L2CA_DataWriteReq(bnep_connection->local_cid, 0, mblk);
        /*
        * All went well, so update internal state.
        */
        bnep_connection->l2_busyCount++;

        if (isRecorded)
        {
            bnep_connection->recordedList[bnep_connection->recordedListWrIdx++] = mblk_or_recorded;
            bnep_connection->recordedListWrIdx &= BNEP_MAX_RECORDED_COUNT_MASK;
        }
    }
    else
    {
        MBLK_QUEUE *link_new, **next;

        /*
        * Try to create the link.
        */
        link_new = CsrPmemAlloc (sizeof (*link_new));
        link_new->next = NULL;
        link_new->mblk_or_recorded = mblk_or_recorded;

        /*
        * Add the link to the end of the chain,
        * as long as there's enough memory and the chain isn't too long.
        */
        if ((isRecorded ?
             pmalloc_percentage_taken_blocks (BNEP_MULTICAST_LO, BNEP_MULTICAST_HI) <= BNEP_MAX_MULTICAST_PERCENT_MEMORY :
             pmalloc_percentage_taken_blocks (BNEP_UNICAST_LO, BNEP_UNICAST_HI) <= BNEP_MAX_UNICAST_PERCENT_MEMORY) &&
            (next = bnep_last_link_next (BNEP_MAX_QUEUE_LENGTH - 1, bnep_connection)) != NULL)
        {
            *next = link_new;
        }
        else
        {
#define tcp_log(x) 
            tcp_log (("[too many links]\n"));
            IN_ERROR_LOG ("Too many links",
                          IN_ERROR_BACKLOG_BNEP_SEND_L2CA_DATA_REQ);
            ZZZCOUNTER (xmdrbacklogdropped);
            CsrPmemFree (link_new);
            return FALSE;
        }
    }

    return TRUE;
}


/******************************************************************************

    function bnep_get_addr  --  extract an Ethernet address from a packet

******************************************************************************/

void bnep_get_addr (ETHER_ADDR *addr, const CsrUint8 *raw_addr)
{
    addr->w[0] = raw_addr[0] << 8 | raw_addr[1];
    addr->w[1] = raw_addr[2] << 8 | raw_addr[3];
    addr->w[2] = raw_addr[4] << 8 | raw_addr[5];
}


/******************************************************************************

    function bnep_send_connect_cfm  --  send BNEP_CONNECT_CFM

******************************************************************************/

static void bnep_send_connect_cfm (enum BNEP_CONNECT_CFM_STATUS status)
{
    BNEP_CONNECT_CFM_T *abcct;

    /*
    * If we can't send the confirmation, we might as well panic.
    */
    abcct = CsrPmemAlloc(sizeof (*abcct));
    abcct->type = BNEP_CONNECT_CFM;
    abcct->status = status;
    PUT_BNEP_MESSAGE (bnep_local_phandle, abcct);
}


/******************************************************************************

    function bnep_send_connect_ind  --  send BNEP_CONNECT_IND

If status is zero, the type of success appropriate to the connection direction
is used.  bnep_connection may be NULL iff status is non-zero.

******************************************************************************/

static void bnep_send_connect_ind (enum BNEP_CONNECT_IND_STATUS status, BNEP_CONNECTION *bnep_connection)
{
    BNEP_CONNECT_IND_T *abcit;

    /*
    * If we can't send the indication, we might as well panic.
    */
    abcit = CsrPmemZalloc (sizeof (*abcit));
    abcit->type = BNEP_CONNECT_IND;
    if (status != 0)
    {
        abcit->status = status;
    }
    else
    {
        abcit->status = bnep_connection->initiator ?
            BNEP_CONNECT_IND_STATUS_SUCCESSFUL_OUT :
            BNEP_CONNECT_IND_STATUS_SUCCESSFUL_IN;
    }
    if (bnep_connection != NULL)
    {
        abcit->id = bnep_connection->local_cid;
        abcit->rem_addr = bnep_connection->rem_addr;
        if (status == 0)
        {
            abcit->rem_uuid = bnep_remote_role;
            abcit->loc_uuid = bnep_local_role;
        }
    }
    PUT_BNEP_MESSAGE (bnep_local_phandle, abcit);
}


/******************************************************************************

    function bnep_send_disconnect_ind  --  send BNEP_DISCONNECT_IND

******************************************************************************/

static void bnep_send_disconnect_ind (enum BNEP_DISCONNECT_IND_REASON reason, BNEP_CONNECTION *bnep_connection)
{
    BNEP_DISCONNECT_IND_T *abdit;

    /*
    * If we can't send the indication, we might as well panic.
    */
    abdit = CsrPmemAlloc(sizeof (*abdit));
    abdit->type = BNEP_DISCONNECT_IND;
    abdit->id = bnep_connection->local_cid;
    abdit->reason = reason;
    PUT_BNEP_MESSAGE (bnep_local_phandle, abdit);
}


/******************************************************************************

    function bnep_register_req  --  register as the BNEP client

******************************************************************************/

static void bnep_register_req (BNEP_REGISTER_REQ_T *brrt)
{
    bnep_passive_connection_flags.disable_extended_cuckoo = brrt->disable_extended;
    bnep_passive_connection_flags.manual_bridge_cuckoo = brrt->manual_bridge;
    bnep_passive_connection_flags.disable_stack_cuckoo = brrt->disable_stack;
    bnep_local_phandle = brrt->phandle;
}

/******************************************************************************

    same as bnep_register_req but with own device addr included in signal

******************************************************************************/
static void bnep_extended_register_req (BNEP_EXTENDED_REGISTER_REQ_T *brrt)
{
    bnep_passive_connection_flags.disable_extended_cuckoo = brrt->disable_extended;
    bnep_passive_connection_flags.manual_bridge_cuckoo = brrt->manual_bridge;
    bnep_passive_connection_flags.disable_stack_cuckoo = brrt->disable_stack;
    bnep_local_addr = bnep_to_ether_addr ( &(brrt->addr) );
    bnep_local_phandle = brrt->phandle;

    /*
    * Register with the L2CAP layer.
    */
    L2CA_RegisterReq(BNEP_PSM,
                     CSR_BT_BNEP_IFACEQUEUE,
                     L2CA_MODE_MASK_BASIC,
                     0x8000, /* Internal L2CAP flag: L2CA_REGISTER_NOAUTO */
                     0); /* reg_ctx */
}
/******************************************************************************

    function bnep_connect_req  --  connect (possibly passively) to a NAP/GN

******************************************************************************/

static void bnep_connect_req (BNEP_CONNECT_REQ_T *bcrt)
{
    /*
    * Some sanity-checking.
    */
    unsigned nrem = bcrt->flags.rnap + bcrt->flags.rgn + bcrt->flags.rpanu;
    unsigned nloc = bcrt->flags.lnap + bcrt->flags.lgn + bcrt->flags.lpanu;
    unsigned npanu = bcrt->flags.lpanu + bcrt->flags.rpanu;
    CsrBool badnap = bcrt->flags.lnap && (!bnep_passive_connection_flags.disable_stack_cuckoo || bnep_passive_connection_flags.disable_extended_cuckoo);

    /*
    * Fudge to allow illegal connections to be requested
    * (e.g. GN-GN).
    */
    if (bcrt->flags.lax_roles)
    {
        npanu = 1;
    }

    /*
    * 1. If it's an active connection request, then
    * there must be exactly one local and one remote role,
    * and at least one of these must be PANU.
    * HERETODO: In the future one might want to allow
    * more flexibility (e.g. try GN, and then NAP if this
    * doesn't work), but it's not clear this would be of
    * great use (one typically knows, from SDP or other,
    * what roles are to be used).
    * Also, if the local role is NAP, then the local
    * stack must be disabled and extendeds must be enabled.
    * (HERETODO rejecting local NAPs if stack not disabled because
    * don't currently support sending ARP requests both to the
    * on-chip task and to the foreign network.)
    * Also, we must not be in the process of connecting.
    * HERETODO could try to relax this in the future (e.g.
    * if connecting multiple times as GN, or if passive connection
    * in progress is guaranteed to be compatible).
    * Also, if we're already connected then the roles must match,
    * and we must not already be a PANU, and we must be in multi-user mode.
    */
    if (!ETHER_IS_UNKNOWN (&bcrt->rem_addr))
    {
        if (nrem != 1 || nloc != 1 || npanu == 0 || badnap)
        {
            bnep_send_connect_cfm (BNEP_CONNECT_CFM_STATUS_BADARG);
            return;
        }
        if (bnep_connection_in_limbo ())
        {
            bnep_send_connect_cfm (BNEP_CONNECT_CFM_STATUS_TRYLATER);
            return;
        }
        if (bnep_local_role != 0 &&
            (!BNEP_IS_COMPATIBLE (bcrt->flags, bnep_local_role, bnep_remote_role) ||
             bnep_local_role == BNEP_UUID_PANU ||
             bnep_connections->flags.single_user)) /* HERETODO have function to check for single-userness (combine passive and active) */
        {
            bnep_send_connect_cfm (BNEP_CONNECT_CFM_STATUS_CONFLICT);
            return;
        }
    }

    /* 2. If it's a pasive connection enabling request, then
    * there must be at least one local and one remote role,
    * and at least one of these must be PANU.
    * Also, if the local role allows NAP, then the local
    * stack must be disabled and extendeds must be enabled.
    */
    if (ETHER_IS_UNKNOWN (&bcrt->rem_addr) && bcrt->flags.accept_in &&
        (nrem == 0 || nloc == 0 || npanu == 0 || badnap))
    {
        bnep_send_connect_cfm (BNEP_CONNECT_CFM_STATUS_BADARG);
        return;
    }
    /* 3. If it's a passive connection disabling request, then
    * reject it if we're in the process of connecting (to avoid
    * race hazards).
    */
    if (ETHER_IS_UNKNOWN (&bcrt->rem_addr) && !bcrt->flags.accept_in &&
        bnep_connection_in_limbo ())
    {
        bnep_send_connect_cfm (BNEP_CONNECT_CFM_STATUS_TRYLATER);
        return;
    }

    /*
    * If this is an active open, then try to connect.
    */
    if (!ETHER_IS_UNKNOWN (&bcrt->rem_addr))
    {
        BNEP_CONNECTION *bnep_connection;
        BD_ADDR_T rem_addr;

        /*
         * Make sure we can cope.
 */
        if ((bnep_connection = bnep_create_connection ()) == NULL)
        {
            bnep_send_connect_cfm (BNEP_CONNECT_CFM_STATUS_NOMEM);
            return;
        }

#if BNEP_NO_CID != 0
        bnep_connection->local_cid = BNEP_NO_CID;
#endif
        bnep_connection->flags = bcrt->flags;
        bnep_connection->state = BNEP_CONNECTING;
        bnep_connection->initiator = TRUE;
        /* bnep_connection->mtu_recd = FALSE; */
        /* bnep_connection->config_bad = FALSE; */
        bnep_connection->rem_addr = bcrt->rem_addr;

        rem_addr = bnep_from_ether_addr (&bnep_connection->rem_addr);;
        L2CA_ConnectReq(&rem_addr, BNEP_PSM, BNEP_PSM,
                        0, /* con_ctx */
                        NULL); /* security substitution */
    }
    /*
    * If this is a passive open, then squirrel the information away
    * (need to preserve the cuckoo flags).
    */
    else
    {
        CsrBool disable_extended_cuckoo = (CsrBool)bnep_passive_connection_flags.disable_extended_cuckoo;
        CsrBool manual_bridge_cuckoo = (CsrBool)bnep_passive_connection_flags.manual_bridge_cuckoo;
        CsrBool disable_stack_cuckoo = (CsrBool)bnep_passive_connection_flags.disable_stack_cuckoo;
        bnep_passive_connection_flags = bcrt->flags;
        bnep_passive_connection_flags.disable_extended_cuckoo = disable_extended_cuckoo;
        bnep_passive_connection_flags.manual_bridge_cuckoo = manual_bridge_cuckoo;
        bnep_passive_connection_flags.disable_stack_cuckoo = disable_stack_cuckoo;
    }

    /*
    * Oh joy
    */
    bnep_send_connect_cfm (BNEP_CONNECT_CFM_STATUS_SUCCESS);
}


/******************************************************************************

    function bnep_send_l2ca_disconnect_req  --  send L2CA_DISCONNECT_REQ

******************************************************************************/

static void bnep_send_l2ca_disconnect_req (BNEP_CONNECTION *bnep_connection)
{
    L2CA_DisconnectReq(bnep_connection->local_cid);
}


/******************************************************************************

    function bnep_disconnect_req  --  disconnect from a NAP/GN

******************************************************************************/

static void bnep_disconnect_req (BNEP_DISCONNECT_REQ_T *bdrt)
{
    BNEP_CONNECTION *bnep_connection = bnep_cid_to_connection (bdrt->id);

    /*
    * Make sure we're established.
    */
    if (bnep_connection && bnep_connection->state == BNEP_ESTABLISHED)
    {
        bnep_connection->client_requested_disconnection = TRUE;
        bnep_send_l2ca_disconnect_req (bnep_connection);
    }
}


/******************************************************************************

    function bnep_hci_switch_role_rsp  --  check our baseband role

******************************************************************************/

static void bnep_hci_switch_role_rsp (BNEP_HCI_SWITCH_ROLE_RSP_T *bhsrrt)
{
    ETHER_ADDR rem_addr = bnep_to_ether_addr (&bhsrrt->bd_addr);
    BNEP_CONNECTION *bnep_connection = bnep_rem_addr_to_connection (&rem_addr);

    /*
    * If we're not a PANU, we're established, and are not in single-user mode,
    * then we insist on being the master, unless (for experimental purposes)
    * neither side is a PANU.
    */
    if (bnep_local_role != BNEP_UUID_PANU &&
        bnep_remote_role == BNEP_UUID_PANU &&
        bnep_connection != NULL &&
        bnep_connection->state == BNEP_ESTABLISHED &&
        !bnep_connection->flags.single_user &&
        bhsrrt->role != HCI_MASTER)
    {
        bnep_connection->switch_failure = TRUE;
        bnep_send_l2ca_disconnect_req (bnep_connection);
    }
}

/******************************************************************************

    function bnep_timeout_setup  --  setup timeout has fired

******************************************************************************/

static void bnep_timeout_setup (CsrUint16 local_cid, void *unused2)
{
    BNEP_CONNECTION *bnep_connection = bnep_cid_to_connection (local_cid);
    void *mblk_data;

    if (bnep_connection == NULL)
    {
        /*
        * This shouldn't happen, so just whistle nonchalantly.
        */
        return;
    }

    if (bnep_connection->retx_counter-- == 0)
    {
        /*
        * If this was an active open, tell the app about the timeout.
        */
        if (bnep_connection->initiator)
        {
            bnep_send_connect_ind (BNEP_CONNECT_IND_STATUS_TIMEOUT, bnep_connection);
        }

        /*
        * Give up.
        */
        bnep_connection->t_setup = NO_TID;
        bnep_connection->client_knows = TRUE;
        bnep_send_l2ca_disconnect_req (bnep_connection);
    }
    else
    {
        CsrMblk *mblk;
        CsrUint8 *hdr;

        /*
        * Rearm the T_crt timeout.
        */
        bnep_connection->t_setup = CsrSchedTimerSet (BNEP_T_CRT * CSR_SCHED_SECOND,
                                                   bnep_timeout_setup,
                                                   local_cid, NULL);
        /*
        * BNEP has a timeout mechanism, so
        * if we can't send the control message we can silently discard.
        */
        mblk = mblk_xnew_in (&mblk_data, 1 + 1 + 1 + 2 * 2, TRUE);
        hdr = (CsrUint8 *) mblk_data;
        CsrBtWriteUint8Val(&hdr, BNEP_TYPE_CONTROL);
        CsrBtWriteUint8Val(&hdr, BNEP_CONTROL_TYPE_SETUP_CONNECTION_REQUEST_MSG);
        CsrBtWriteUint8Val(&hdr, 2 /* UUID size */);
        bnep_remote_role = BNEP_FLAGS_TO_REM_UUID (bnep_connection->flags);
        bnep_local_role = BNEP_FLAGS_TO_LOC_UUID (bnep_connection->flags);
        CsrBtWriteUint16Val(&hdr, bnep_remote_role);
        CsrBtWriteUint16Val(&hdr, bnep_local_role);
        /*
        * Ask L2CAP to send the BNEP packet;
        * if we can't create the message then silently discard.
        */
        if (!bnep_send_l2ca_data_req (mblk, bnep_connection))
        {
            IN_ERROR_LOG ("No memory for message",
                          IN_ERROR_NOMEMMSG_BNEP_UP_IND);
            CsrMblkDestroy (mblk);
        }
    }
}


/******************************************************************************

    struct BNEP_LISTENER  --  an Ethertype/address pair to listen to
    variable bnep_listeners  --  array for listeners

Use a fixed-sized array to keep things simple.  The worst case is:
- Six IPv6 listeners (one unicast, one all-nodes multicast,
  one mDNS, and three (link-local, autoconfigured, static) solicited)
- Three IPv4 listeners (one unicast, one broadcast and one mDNS)
- Two ARP listeners (one unicast and one broadcast)

******************************************************************************/

typedef struct BNEP_LISTENER
{
    CsrSchedQid phandle;
    CsrUint16 ether_type;
    ETHER_ADDR addr;
} BNEP_LISTENER;

/*bchs change define ipv4 to define at least two listeneres*/
#define INSTALL_IPV4_MODULE
static BNEP_LISTENER bnep_listeners[0
#ifdef INSTALL_IPV6_MODULE
                                    + 6
#endif
#ifdef INSTALL_IPV4_MODULE
                                    + 3 + 2
#endif
];
#define BNEP_NUM_LISTENERS (sizeof (bnep_listeners) / sizeof (bnep_listeners[0]))


/******************************************************************************

    function bnep_listen_req  --  listen for a given Ethertype/address pair

******************************************************************************/

static void bnep_listen_req (BNEP_LISTEN_REQ_T *blrt)
{
    CsrSize i;

    /*
    * Look for a free slot, and fill it in.
    */
    for (i = 0; i < BNEP_NUM_LISTENERS; ++i)
    {
        if (ETHER_IS_UNKNOWN (&bnep_listeners[i].addr))
        {
            bnep_listeners[i].phandle = blrt->phandle;
            bnep_listeners[i].ether_type = blrt->ether_type;
            bnep_listeners[i].addr = blrt->addr;
            /*HERETODO if this is the first listen for this phandle, and we're up, we should BNEP_UP_IND */
            return;
        }
    }

    /*
    * If there was no free slot, bnep_listeners[] needs to be increased!
    */
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
    CsrGeneralException("BNEP", CSR_BT_BNEP_PRIM, BNEP_LISTEN_REQ, "BNEP_NUM_LISTENERS exceeded");
#endif
}


/******************************************************************************

    function bnep_ignore_req  --  ignore a given Ethertype/address pair

******************************************************************************/

static void bnep_ignore_req (BNEP_IGNORE_REQ_T *birt)
{
    CsrSize i;

    /*
    * Try to find Ethertype/address pair in the listener list, and delete it.
    */
    for (i = 0; i < BNEP_NUM_LISTENERS; ++i)
    {
        if (bnep_listeners[i].ether_type == birt->ether_type &&
            ETHER_IS_SAME_ADDR (&bnep_listeners[i].addr, &birt->addr))
        {
            bnep_listeners[i].addr.w[0] = ETHER_UNKNOWN;
            bnep_listeners[i].phandle = BNEP_NO_PHANDLE;
        }
    }
}


/******************************************************************************

    function bnep_send_updown_ind  --  send BNEP_UP/DOWN_IND to all listeners
                                       and BNEP_CONNECT_IND to client, if up

******************************************************************************/

static void bnep_send_updown_ind (CsrUint16 type, BNEP_CONNECTION *bnep_connection)
{
    CsrSize i, j;
    CsrSchedQid prevspam, spam;
    BNEP_UP_IND_T *abuit; /* Compatible with BNEP_DOWN_IND_T */

    prevspam = BNEP_NO_PHANDLE;
    for (i = 0; i < BNEP_NUM_LISTENERS; ++i)
    {
        /*
        * Do a quick check to exclude the last spammee,
        * before checking for a valid address.
        */
        if (prevspam != bnep_listeners[i].phandle &&
            !ETHER_IS_UNKNOWN (&bnep_listeners[i].addr))
        {
            /*
            * Check whether the spammee has already
            * been spammed.
            */
            spam = bnep_listeners[i].phandle;
            for (j = 0; j < i; ++j)
            {
                if (bnep_listeners[j].phandle == spam)
                {
                    break;
                }
            }
            if (i == j)
            {
                /*
                * Spam spam spam spam spam spam spam...
                */
                prevspam = spam;
                /*
                * If we can't send the indication, we might as well panic.
                */
                abuit = CsrPmemAlloc(sizeof (*abuit));
                if (type == BNEP_UP_IND)
                {
                    abuit->rem_uuid = bnep_remote_role;
                    abuit->loc_uuid = bnep_local_role;
                }
                else
                {
                    CsrMemSet(abuit,0,sizeof (*abuit));
                }
                abuit->phandle = spam;
                abuit->type = type;
                PUT_BNEP_MESSAGE (abuit->phandle, abuit);
            }
        }
    }
}


/******************************************************************************

    function bnep_send_data_ind  --  find listeners for a packet

Returns TRUE iff a listener was found.
The mblk needs to be freed iff FALSE is returned.

******************************************************************************/

static CsrBool bnep_send_data_ind (CsrUint16 ether_type, ETHER_ADDR *dst_addr, CsrMblk *mblk)
{
    CsrSize i;

    for (i = 0; i < BNEP_NUM_LISTENERS; ++i)
    {
        if (bnep_listeners[i].ether_type == ether_type &&
            ETHER_IS_SAME_ADDR (&bnep_listeners[i].addr, dst_addr))
        {
            BNEP_DATA_IND_T *abdit; /* Compatible with BNEP_MULTICAST_DATA_IND_T */

            /*
            * If we can't create the task message,
            * then silently discard.
            */
            abdit = CsrPmemAlloc(sizeof (*abdit));
            abdit->type = ETHER_IS_MULTICAST (dst_addr) ?
                BNEP_MULTICAST_DATA_IND :
                BNEP_DATA_IND;
            abdit->phandle = bnep_listeners[i].phandle;
            abdit->mblk = mblk;
            PUT_BNEP_MESSAGE (abdit->phandle, abdit);
            return TRUE;
        }
    }

    return FALSE;
}


/******************************************************************************

    function bnep_data_req  --  generate outgoing BNEP Ethernet packet

******************************************************************************/

static CsrBool bnep_extended_multicast_data_req (BNEP_RECORDED *bnep_recorded); /* HERETODO */
static void bnep_data_req (BNEP_DATA_REQ_T *bdrt)
{
    BNEP_CONNECTION *bnep_connection;
    CsrMblk *mblk = bdrt->mblk, *mblk_hdr;
    CsrUint8 *hdr;
    CsrBool compressed_dst;
    void *mblk_data;

    /*
    * HERETODO
    * Check for loopback.
    */
    if (ETHER_IS_LOOPBACK (&bdrt->rem_addr))
    {
        CsrMblkDestroy (mblk);
        return;
    }

    /*
    * Silently discard if stack is disabled.
    */
    if (bnep_passive_connection_flags.disable_stack_cuckoo)
    {
        IN_ERROR_LOG ("BNEP mblk_data no-one's interested in",
                      IN_ERROR_UNLOVED_BNEP_DATA_REQ);
        CsrMblkDestroy (mblk);
        return;
    }

    /*
    * HERETODO do EXTENDED_DATA_IND if not disable_extended, and manual_bridge
    * and not from host
    */

    /*
    * If it's multicast (but not unknown) we need to do some spamming.
    */
    if (ETHER_IS_MULTICAST (&bdrt->rem_addr) && !ETHER_IS_UNKNOWN (&bdrt->rem_addr))
    {
        BNEP_RECORDED *bnep_recorded = CsrPmemAlloc(sizeof (*bnep_recorded));

        bnep_recorded->isARecorded = TRUE;
        bnep_recorded->id_last = BNEP_NO_CID;
        bnep_recorded->id_not = BNEP_ID_LOCAL; /* HERETODO breaks multicasts but makes DAD work -- think of better solution (but note this is used as a test for the flood defences in bnep_extended_multicast_data_req () */
        bnep_recorded->ether_type = bdrt->ether_type;
        bnep_recorded->dst_addr = bdrt->rem_addr;
        bnep_recorded->src_addr = bnep_local_addr;
        bnep_recorded->mblk = mblk;
        bnep_recorded->from_bnep_client = FALSE;
        bnep_recorded->unk_extn_len = 0;

        if (bnep_extended_multicast_data_req (bnep_recorded))
        {
            /*
            * Come back later for the listeners and/or NAP port.
            */
            ZZZCOUNTER (drspam);
            return;
        }
        else
        {
            /*
            * No PANUs were interested, so just pass on to any listeners.
            */
            CsrPmemFree (bnep_recorded);
        }
        /* HERETODO should also pass to listeners -- see id_not comment above */
        /*
        * Send an extended mblk_data indication, if not disabled.
        */
        if (!bnep_passive_connection_flags.disable_extended_cuckoo)
        {
            BNEP_EXTENDED_DATA_IND_T *abedit;

            ZZZCOUNTER (drxdi);
            /*
            * If we can't create the task message,
            * then silently discard.
            */
            abedit = CsrPmemAlloc(sizeof (*abedit));
            abedit->type = BNEP_EXTENDED_DATA_IND;
            abedit->id = BNEP_ID_LOCAL;
            abedit->ether_type = bdrt->ether_type;
            abedit->dst_addr = bdrt->rem_addr;
            abedit->src_addr = bnep_local_addr;
            abedit->mblk = mblk;
            PUT_BNEP_MESSAGE (bnep_local_phandle, abedit);
            return;
        }

        /*
        * Boo-hoo!
        */
        bnep_connection = NULL;
    }
    /*
    * Otherwise find the lucky winner.
    */
    else
    {
        bnep_connection = bnep_rem_addr_to_connection (&bdrt->rem_addr);
        /*
        * If we can't find a winner, but we're a PANU,
        * then send to the remote device (if any).
        */
        if (bnep_connection == NULL &&
            bnep_connections != NULL &&
            bnep_local_role == BNEP_UUID_PANU)
        {
            bnep_connection = bnep_connections;
        }
    }

    /*
    * Silently discard if there are no takers for this packet.
    */
    if (bnep_connection == NULL)
    {
        IN_ERROR_LOG ("BNEP mblk_data no-one's interested in",
                      IN_ERROR_UNLOVED_BNEP_DATA_REQ);
        CsrMblkDestroy (mblk);
        return;
    }

    /*
    * Reject premature attempts.
    */
    if (bnep_connection->state != BNEP_ESTABLISHED)
    {
        IN_ERROR_LOG ("BNEP premature mblk_data",
                      IN_ERROR_PREMATURE_BNEP_DATA_REQ);
        CsrMblkDestroy (mblk);
        return;
    }

    /*
    * Prepend the BNEP header to the mblk;
    * if we can't create the mblk then silently discard.
    * If we don't know whom to send this to, send it to
    * the peer and hope (it might be a router).
    */
    compressed_dst =
        ETHER_IS_SAME_ADDR (&bdrt->rem_addr, &bnep_connection->rem_addr) ||
        ETHER_IS_UNKNOWN (&bdrt->rem_addr);
    mblk_hdr = mblk_xnew_in(&mblk_data, (CsrUint16) (1 + ETHER_ADDR_LEN * !compressed_dst + 2), TRUE);
    hdr = (CsrUint8 *)  mblk_data;
    if (compressed_dst)
    {
        CsrBtWriteUint8Val(&hdr, BNEP_TYPE_COMPRESSED_ETHER);
    }
    else
    {
        CsrBtWriteUint8Val(&hdr, BNEP_TYPE_COMPRESSED_ETHER_DST_ONLY);
        CsrBtWriteBnepAddr(&hdr, &bdrt->rem_addr);
    }
    CsrBtWriteUint16Val(&hdr, bdrt->ether_type);

    mblk = CsrMblkAddHead(mblk_hdr, mblk);

    /*
    * Ask L2CAP to send the BNEP packet;
    * if we can't create the message then silently discard.
    */
    if (!bnep_send_l2ca_data_req (mblk, bnep_connection))
    {
        IN_ERROR_LOG ("No memory for message",
                      IN_ERROR_NOMEMMSG_BNEP_DATA_REQ);
        CsrMblkDestroy (mblk);
    }
    tcp_log (("[bnep sent]\n"));
}


/******************************************************************************

    function bnep_extended_data_req  --  generate outgoing BNEP Ethernet packet

******************************************************************************/

static void bnep_extended_data_req (BNEP_EXTENDED_DATA_REQ_T *bedrt, CsrBool extn)
{
    BNEP_CONNECTION *bnep_connection;
    CsrMblk *mblk = bedrt->mblk, *mblk_hdr;
    CsrUint8 *hdr;
    CsrUint16 id = bedrt->id;
    CsrBool uncompressed_src, uncompressed_dst;
    CsrUint8 bnep_type = 0;
    void *mblk_data;

    /*
    * Look up destination if requested.
    */
    if (id == BNEP_ID_DST)
    {
        /* HERETODO optimise this: going from bnep_connection to id and then back to bnep_connection below */
        if ((bnep_connection = bnep_rem_addr_to_connection (&bedrt->dst_addr)) != NULL)
        {
            id = bnep_connection->local_cid;
        }
        else if (ETHER_IS_SAME_ADDR (&bedrt->dst_addr, &bnep_local_addr))
        {
            id = BNEP_ID_LOCAL;
        }
        /*
        * The destination isn't anyone we know about.
        * If we're a PANU then we might as well try sending the packet
        * to the peer, in the hope it's a NAP or GN who knows about
        * the destination.
        */
        else if (bnep_local_role == BNEP_UUID_PANU)
        {
            id = bnep_connections->local_cid;
        }
        else
        {
            IN_ERROR_LOG ("BNEP mblk_data no-one's interested in",
                          IN_ERROR_UNLOVED_BNEP_EXTENDED_DATA_REQ);
            CsrMblkDestroy (mblk);
            return;
        }
    }

    /*
    * If it's for the local stack, pass on if not disabled.
    */
    if (id == BNEP_ID_LOCAL)
    {
        if (bnep_passive_connection_flags.disable_stack_cuckoo ||
            !bnep_send_data_ind (bedrt->ether_type, &bedrt->dst_addr, mblk))
        {
            IN_ERROR_LOG ("BNEP mblk_data no-one's interested in",
                          IN_ERROR_UNLOVED_BNEP_EXTENDED_DATA_REQ);
            CsrMblkDestroy (mblk);
        }
        return;
    }

    /*
    * Silently discard if there are no takers for this packet.
    */
    if ((bnep_connection = bnep_cid_to_connection (id)) == NULL)
    {
        IN_ERROR_LOG ("BNEP mblk_data no-one's interested in",
                      IN_ERROR_UNLOVED_BNEP_EXTENDED_DATA_REQ);
        CsrMblkDestroy (mblk);
        return;
    }

    /*
    * Reject premature attempts.
    */
    if (bnep_connection->state != BNEP_ESTABLISHED)
    {
        IN_ERROR_LOG ("BNEP premature mblk_data",
                      IN_ERROR_PREMATURE_BNEP_EXTENDED_DATA_REQ);
        CsrMblkDestroy (mblk);
        return;
    }

    /*
    * Prepend the BNEP header to the mblk;
    * if we can't create the mblk then silently discard.
    */
    uncompressed_dst = !ETHER_IS_SAME_ADDR (&bedrt->dst_addr, &bnep_connection->rem_addr);
    uncompressed_src = !ETHER_IS_SAME_ADDR (&bedrt->src_addr, &bnep_local_addr);

    mblk_hdr = mblk_xnew_in(&mblk_data, (CsrUint16) (1 + ETHER_ADDR_LEN * ((CsrUint8)uncompressed_dst + (CsrUint8)uncompressed_src) + 2), TRUE);
    hdr = (CsrUint8 *) mblk_data;

    if (extn)
    {
        bnep_type |= BNEP_FLG_EXTENSION;
    }
    if (uncompressed_dst)
    {
        if (uncompressed_src)
        {
            bnep_type |= BNEP_TYPE_GENERAL_ETHER;
            CsrBtWriteUint8Val(&hdr, bnep_type);
            CsrBtWriteBnepAddr(&hdr, &bedrt->dst_addr);
            CsrBtWriteBnepAddr(&hdr, &bedrt->src_addr);
        }
        else
        {
            bnep_type |= BNEP_TYPE_COMPRESSED_ETHER_DST_ONLY;
            CsrBtWriteUint8Val(&hdr, bnep_type);
            CsrBtWriteBnepAddr(&hdr, &bedrt->dst_addr);
        }
    }
    else if (uncompressed_src)
    {
        bnep_type |= BNEP_TYPE_COMPRESSED_ETHER_SRC_ONLY;
        CsrBtWriteUint8Val(&hdr, bnep_type);
        CsrBtWriteBnepAddr(&hdr, &bedrt->src_addr);
    }
    else /* if (!uncompressed_dst && !uncompressed_src) */
    {
        bnep_type |= BNEP_TYPE_COMPRESSED_ETHER;
        CsrBtWriteUint8Val(&hdr, bnep_type);
    }

    CsrBtWriteUint16Val(&hdr, bedrt->ether_type);

    mblk = CsrMblkAddHead(mblk_hdr, mblk);

    /*
    * Ask L2CAP to send the BNEP packet;
    * if we can't create the message then silently discard.
    */
    if (!bnep_send_l2ca_data_req (mblk, bnep_connection))
    {
        IN_ERROR_LOG ("No memory for message",
                      IN_ERROR_NOMEMMSG_BNEP_EXTENDED_DATA_REQ);
        CsrMblkDestroy (mblk);
    }
}


/******************************************************************************

    function bnep_extended_multicast_data_req  --  generate outgoing multicast BNEP Ethernet packet

Returns TRUE iff a BNEP packet was queued to L2CAP.
The packet is not queued to the local stack.

******************************************************************************/

static CsrBool bnep_extended_multicast_data_req (BNEP_RECORDED *bnep_recorded)
{
    BNEP_CONNECTION *bnep_connection;
    CsrBool found_last;

    /*
    * Find the lucky winner.
    */
    found_last = bnep_recorded->id_last == BNEP_NO_CID;
    for (bnep_connection = bnep_connections;
         bnep_connection != NULL;
         bnep_connection = bnep_connection->next)
    {
        /*
        * Skip the not id.
        */
        if (bnep_connection->local_cid == bnep_recorded->id_not)
        {
            continue;
        }

        /*
        * Look for the connection after the last one.
        */
        if (bnep_connection->local_cid == bnep_recorded->id_last)
        {
            found_last = TRUE;
            continue;
        }

        /*
        * Reject premature attempts.
        */
        if (bnep_connection->state != BNEP_ESTABLISHED)
        {
            continue;
        }

        /*
        * Have we found what we're after?
        */
        if (found_last)
        {
            if (bnep_last_link_next (bnep_recorded->id_not == BNEP_ID_LOCAL || bnep_recorded->from_bnep_client ? BNEP_MAX_LOCAL_MULTICAST_QUEUE_LENGTH - 1 : BNEP_MAX_MULTICAST_QUEUE_LENGTH - 1, bnep_connection) == NULL)
            {
                ZZZCOUNTER (xmdrbacklogskipped);
                IN_ERROR_LOG ("Too many links",
                              IN_ERROR_BACKLOG_BNEP_EXTENDED_MULTICAST_DATA_REQ);
                /*
                * Carry on looking for other less burdened parties.
                */
                continue;
            }

            break;
        }
    }

    /*
    * Abort if we've run out of interested parties.
    */
    if (bnep_connection == NULL)
    {
        ZZZCOUNTER (xmdrdone);
        return FALSE;
    }

    /*
    * Ask L2CAP to send the BNEP packet;
    * if we can't create the message then abort.
    */
    if (!bnep_send_l2ca_data_req (bnep_recorded, bnep_connection))
    {
        ZZZCOUNTER (xmdrnosend);
        return FALSE;
    }

    /*
    * Hooray!
    */
    bnep_recorded->id_last = bnep_connection->local_cid;

    return TRUE;
}


/******************************************************************************

    function bnep_enforce_mastery  --  request role switch to master as needed

******************************************************************************/

static void bnep_enforce_mastery (const BNEP_CONNECTION *bnep_connection)
{
    /*
    * We need to switch roles, unless the client has undertaken
    * to take care of this, or we're in single-user mode,
    * or we're a PANU or (for experimental purposes) neither
    * side is a PANU.
    * Note that we may in fact already be in the right role
    * (e.g. when we're a GN and we're the paging device),
    * but the request will succeed gracefully, and so we can
    * avoid keeping track of the baseband role.
    */
    if (!bnep_connection->flags.no_switch &&
        !bnep_connection->flags.single_user &&
        bnep_local_role != BNEP_UUID_PANU &&
        bnep_remote_role == BNEP_UUID_PANU)
    {
        DM_HCI_SWITCH_ROLE_REQ_T *dhsr;

        dhsr = CsrPmemAlloc(sizeof (*dhsr));
        dhsr->common.op_code = HCI_SWITCH_ROLE;
        dhsr->bd_addr = bnep_from_ether_addr (&bnep_connection->rem_addr);
        dhsr->role = HCI_MASTER;
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, dhsr);
    }
}


/******************************************************************************

    function bnep_handle_control  --  handle incoming BNEP control packet

pkt points at the BNEP control type.
len is the number of octets available at pkt and must not be zero.
If the control packet is broken (too short, unknown type), 0 is returned,
else the number of octets read from pkt is returned.

******************************************************************************/

static CsrUint16 bnep_handle_control (const CsrUint8 *pkt, CsrUint16 len, BNEP_CONNECTION *bnep_connection)
{
    CsrMblk *mblk = NULL;
    CsrUint8 *orig_hdr = NULL, *hdr = NULL;
    CsrUint16 len_var = 0, len_read = 0;
    void *mblk_data;

    /*
    * Note length is known to be enough for control type.
    */
    switch (pkt[BNEP_OFFSETOF_CONTROL_TYPE])
    {
    case BNEP_CONTROL_TYPE_CONTROL_COMMAND_NOT_UNDERSTOOD:
        if (len < 2)
        {
            IN_ERROR_LOG ("BNEP control packet wrong size",
                IN_ERROR_BADSIZE_BNEP_HANDLE_CONTROL);
        }
        else
        {
            len_read = 2;
            /*
            * There's not really much we can do here.
            */
            IN_ERROR_LOG ("BNEP_CONTROL_COMMAND_NOT_UNDERSTOOD packet received",
                IN_ERROR_UNEXPECTEDBAD_BNEP_HANDLE_CONTROL);
        }
        break;

    case BNEP_CONTROL_TYPE_SETUP_CONNECTION_REQUEST_MSG:
        if (len < 2 ||
            len - 2 < (len_var = pkt[BNEP_OFFSETOF_UUID_SIZE]) * 2)
        {
            IN_ERROR_LOG ("BNEP control packet wrong size",
                IN_ERROR_BADSIZE_BNEP_HANDLE_CONTROL);
        }
        else if (len_read = 2 + len_var * 2,
            bnep_connection->state != BNEP_ESTABLISHED && bnep_connection->initiator)
        {
            /*
            * We haven't finished setup, so we shouldn't receive a request!
            * Ignore (BNEP1.0d/2.6.3.1).
            */
            IN_ERROR_LOG ("BNEP_SETUP_REQUEST packet received",
                IN_ERROR_UNEXPECTEDREQ_BNEP_HANDLE_CONTROL);
        }
        else
        {
            CsrUint16 luuid, ruuid;
            /*
            * BNEP has a timeout mechanism, so
            * if we can't send the control message we can silently discard.
            * Don't do as INspecial as odd length.
            */
            mblk = mblk_xnew_in(&mblk_data, 1 + 1 + 2, FALSE);
            orig_hdr = hdr = (CsrUint8 *) mblk_data;
            CsrBtWriteUint8Val(&hdr, BNEP_TYPE_CONTROL);
            CsrBtWriteUint8Val(&hdr, BNEP_CONTROL_TYPE_SETUP_CONNECTION_RESPONSE_MSG);
            len = pkt[BNEP_OFFSETOF_UUID_SIZE];
            /*
            * We need to make sure:
            * - the UUID size is valid
            * - the destination (local) and source (remote) UUID is
            *   NAP or GN or PANU
            * - at least one of the UUIDs is PANU
            *   (unless in experimental mode)
            */
            if (len != 2 && len != 4 && len != 16)
            {
                CsrBtWriteUint16Val(&hdr, BNEP_CONTROL_TYPE_SETUP_RESPONSE_INVALID_UUID_SIZE);
            }
            else if ((luuid = sdp_get_uuid (pkt + BNEP_OFFSETOF_DST_UUID,
                len)) !=
                BNEP_UUID_PANU &&
                luuid != BNEP_UUID_NAP &&
                luuid != BNEP_UUID_GN)
            {
                CsrBtWriteUint16Val(&hdr, BNEP_CONTROL_TYPE_SETUP_RESPONSE_INVALID_DST_UUID);
            }
            else if ((ruuid = sdp_get_uuid (pkt + BNEP_OFFSETOF_DST_UUID + len,
                len)) !=
                BNEP_UUID_GN &&
                ruuid != BNEP_UUID_NAP &&
                ruuid != BNEP_UUID_PANU)
            {
                CsrBtWriteUint16Val(&hdr, BNEP_CONTROL_TYPE_SETUP_RESPONSE_INVALID_SRC_UUID);
            }
            else if (luuid != BNEP_UUID_PANU && ruuid != BNEP_UUID_PANU &&
                !bnep_connection->flags.lax_roles) /* need passive_flags for passives? */
            {
                CsrBtWriteUint16Val(&hdr, BNEP_CONTROL_TYPE_SETUP_RESPONSE_CONNECTION_NOT_ALLOWED);
            }
            /*
            * If this is a new connection request, then we need to make sure
            * it's compatible with the connection's role flags
            * and with the roles of any existing connection.
            * If this is a role change request, then we need to make sure
            * this is the only connection and that
            * it's compatible with the passive connection role flags.
            * HERE this distinction between the active connection role
            * flags and the passive connection role flags may be dubious
            * especially given the test above for lax_roles!
            */
            else
            {
                CsrBool newconn = bnep_connection->state != BNEP_ESTABLISHED;

                if (newconn)
                {
                    if (!BNEP_IS_COMPATIBLE (bnep_connection->flags, luuid, ruuid) ||
                        (bnep_local_role != 0 &&
                        (luuid != bnep_local_role ||
                        ruuid != bnep_remote_role)))
                    {
                        CsrBtWriteUint16Val(&hdr, BNEP_CONTROL_TYPE_SETUP_RESPONSE_CONNECTION_NOT_ALLOWED);
                    }
                    else
                    {
                        CsrBtWriteUint16Val(&hdr, BNEP_CONTROL_TYPE_SETUP_RESPONSE_SUCCESSFUL);
                    }
                }
                else
                {
                    if (bnep_connections->next != NULL ||
                        !BNEP_IS_COMPATIBLE (bnep_passive_connection_flags, luuid, ruuid))
                    {
                        CsrBtWriteUint16Val(&hdr, BNEP_CONTROL_TYPE_SETUP_RESPONSE_CONNECTION_NOT_ALLOWED);
                    }
                    else
                    {
                        CsrBtWriteUint16Val(&hdr, BNEP_CONTROL_TYPE_SETUP_RESPONSE_SUCCESSFUL);
                    }
                }

                /*
                * There's no need to get too excited over
                * role "changes" to the same roles.
                */
                if (CsrBtReadUint16Val(orig_hdr, 2) == BNEP_CONTROL_TYPE_SETUP_RESPONSE_SUCCESSFUL &&
                    (newconn ||
                    bnep_local_role != luuid ||
                    bnep_remote_role != ruuid))
                {
                    bnep_local_role = luuid;
                    bnep_remote_role = ruuid;

                    if (newconn)
                    {
                            CsrSchedTimerCancel (bnep_connection->t_setup, NULL, NULL);

                        bnep_connection->state = BNEP_ESTABLISHED;

                        bnep_send_connect_ind (BNEP_CONNECT_IND_STATUS_SUCCESSFUL_OUT, bnep_connection);
                    }
                    else
                    {
                        BNEP_ROLE_CHANGE_IND_T *abrcit;

                        /*
                        * If we can't send the indication, we might as well panic.
                        */
                            abrcit = CsrPmemAlloc(sizeof (*abrcit));
                        abrcit->type = BNEP_ROLE_CHANGE_IND;
                        abrcit->rem_uuid = ruuid;
                        abrcit->loc_uuid = luuid;
                        PUT_BNEP_MESSAGE (bnep_local_phandle, abrcit);
                    }

                    bnep_send_updown_ind (BNEP_UP_IND, bnep_connection);
                    bnep_enforce_mastery (bnep_connection);
                }
            }
        }
        break;

    case BNEP_CONTROL_TYPE_FILTER_NET_TYPE_SET_MSG:
    case BNEP_CONTROL_TYPE_FILTER_MULTI_ADDR_SET_MSG:
        if (len < 3 ||
            len - 3 < (len_var = pkt[BNEP_OFFSETOF_LIST_LEN] << 8 |
            pkt[BNEP_OFFSETOF_LIST_LEN + 1]))
        {
            IN_ERROR_LOG ("BNEP control packet wrong size",
                IN_ERROR_BADSIZE_BNEP_HANDLE_CONTROL);
        }
        else if (len_read = 3 + len_var,
            bnep_connection->state != BNEP_ESTABLISHED)
        {
            /*
            * Ignore if not established (BNEP1.0d/2.6.3.1).
            */
            IN_ERROR_LOG ("BNEP_FILTER_REQUEST packet received",
                IN_ERROR_UNEXPECTEDREQ_BNEP_HANDLE_CONTROL);
        }
        else
        {
            /*
            * We don't support filters, so reject.
            * BNEP has a timeout mechanism, so if we can't
            * send the control message we can silently discard.
            * Don't do as INspecial as odd length.
            */
            mblk = mblk_xnew_in(&mblk_data, 1 + 1 + 2, FALSE);
            hdr = (CsrUint8 *) mblk_data;
            CsrBtWriteUint8Val(&hdr, BNEP_TYPE_CONTROL);
            CsrBtWriteUint8Val(&hdr, (CsrUint8) (pkt[BNEP_OFFSETOF_CONTROL_TYPE] + 1));
            CsrBtWriteUint16Val(&hdr, BNEP_CONTROL_TYPE_FILTER_RESPONSE_UNSUPPORTED);
        }
        break;

    case BNEP_CONTROL_TYPE_SETUP_CONNECTION_RESPONSE_MSG:
        if (len < 3)
        {
            IN_ERROR_LOG ("BNEP control packet wrong size",
                IN_ERROR_BADSIZE_BNEP_HANDLE_CONTROL);
        }
        else if (len_read = 3,
            bnep_connection->state != BNEP_ESTABLISHING || !bnep_connection->initiator)
        {
            /*
            * We didn't send a request, so we shouldn't receive a response!
            */
            IN_ERROR_LOG ("BNEP_CONTROL_RESPONSE packet received",
                IN_ERROR_UNEXPECTEDRSP_BNEP_HANDLE_CONTROL);
        }
        else
        {
                CsrSchedTimerCancel (bnep_connection->t_setup, NULL, NULL);
            bnep_connection->t_setup = 0;

            /*
            * Make sure our setup connection request was accepted
            */
            if ((pkt[BNEP_OFFSETOF_RESPONSE_MESSAGE] << 8 |
                pkt[BNEP_OFFSETOF_RESPONSE_MESSAGE + 1]) !=
                BNEP_CONTROL_TYPE_SETUP_RESPONSE_SUCCESSFUL)
            {
                bnep_send_connect_ind (BNEP_CONNECT_IND_STATUS_REJ_ROLES, bnep_connection);

                bnep_connection->client_knows = TRUE;
                bnep_send_l2ca_disconnect_req (bnep_connection);
            }
            else
            {
                bnep_connection->state = BNEP_ESTABLISHED;

                bnep_send_connect_ind (BNEP_CONNECT_IND_STATUS_SUCCESSFUL_OUT, bnep_connection);
                bnep_send_updown_ind (BNEP_UP_IND, bnep_connection);
                bnep_enforce_mastery (bnep_connection);
            }
        }
        break;

    case BNEP_CONTROL_TYPE_FILTER_NET_TYPE_RESPONSE_MSG:
    case BNEP_CONTROL_TYPE_FILTER_MULTI_ADDR_RESPONSE_MSG:
        if (len < 3)
        {
            IN_ERROR_LOG ("BNEP control packet wrong size",
                IN_ERROR_BADSIZE_BNEP_HANDLE_CONTROL);
        }
        else
        {
            len_read = 3;
            /*
            * We didn't send a request, so we shouldn't receive a response!
            */
            IN_ERROR_LOG ("BNEP_CONTROL_RESPONSE packet received",
                IN_ERROR_UNEXPECTEDRSP_BNEP_HANDLE_CONTROL);
        }
        break;

    default:
        /*
        * Unknown command, so reject (BNEP1.0d/2.6.3.1).
        * BNEP has a timeout mechanism, so
        * if we can't send the control message we can silently discard.
        */
        mblk = mblk_xnew_in(&mblk_data, 1 + 1 + 1, TRUE);
        hdr = (CsrUint8 *) mblk_data;
        CsrBtWriteUint8Val(&hdr, BNEP_TYPE_CONTROL);
        CsrBtWriteUint8Val(&hdr, BNEP_CONTROL_TYPE_CONTROL_COMMAND_NOT_UNDERSTOOD);
        CsrBtWriteUint8Val(&hdr, pkt[BNEP_OFFSETOF_CONTROL_TYPE]);
        break;
    }

    /*
    * Send a response, if appropriate.
    */
    if (hdr != NULL)
    {
        /*
        * Ask L2CAP to send the BNEP packet;
        * if we can't create the message then silently discard.
        */
        if (!bnep_send_l2ca_data_req (mblk, bnep_connection))
        {
            IN_ERROR_LOG ("No memory for message",
                          IN_ERROR_NOMEMMSG_BNEP_HANDLE_CONTROL);
            CsrMblkDestroy (mblk);
        }
    }
    else
    {
        CsrMblkDestroy (mblk);
    }

    return len_read;
}

/******************************************************************************

    function bnep_data_ind  --  handle incoming BNEP packet

******************************************************************************/

static void bnep_data_ind (CsrMblk *mblk, BNEP_CONNECTION *bnep_connection)
{
    CsrUint8 *pkt, *orig_ptr;
    CsrBool ok, has_extn = FALSE, has_8021q = FALSE;
    ETHER_ADDR src_addr, dst_addr;
    CsrUint16 ether_type = 0, offset_ether_type = 0, hdr_len = 0, unk_extn_hdr_offset = 0;
    CsrMblkSize mblk_length, mblk_offset = 0;

    mblk_length    = CsrMblkGetLength(mblk);
    orig_ptr = pkt = CsrBtMblkConsumeToMemory(&mblk);

    src_addr.w[0] = 0x0000; src_addr.w[1] = 0x0000; src_addr.w[2] = 0x0000;
    dst_addr.w[0] = 0x0000; dst_addr.w[1] = 0x0000; dst_addr.w[2] = 0x0000;
    /*
    * Check we have a connection, and
    * we can see the BNEP header, and
    * that there are enough octets for the appropriate BNEP header.
    * If not, then silently discard.
    * If so, then process the BNEP header.
    */
    ok = TRUE;

    if (bnep_connection == NULL)
    {
        IN_ERROR_LOG ("No connection for BNEP packet",
                      IN_ERROR_NOCONN_BNEP_DATA_IND);
        ok = FALSE;
    }
    else if (pkt == NULL)
    {
        IN_ERROR_LOG ("Couldn't map BNEP packet in",
                      IN_ERROR_NOMAP_BNEP_DATA_IND);
        ok = FALSE;
    }
    else if (mblk_length < BNEP_MIN_HDR_LEN)
    {
        IN_ERROR_LOG ("BNEP packet header too short",
                      IN_ERROR_HDRTOOSHORT_BNEP_DATA_IND);
        ok = FALSE;
    }
    else
    {
        has_extn = pkt[BNEP_OFFSETOF_TYPE] & BNEP_FLG_EXTENSION;

        switch (pkt[BNEP_OFFSETOF_TYPE] & ~BNEP_FLG_EXTENSION)
        {
        case BNEP_TYPE_GENERAL_ETHER:
            if (mblk_length < 1 + 2 * ETHER_ADDR_LEN + 2)
            {
                IN_ERROR_LOG ("BNEP packet header too short",
                    IN_ERROR_HDRTOOSHORT_BNEP_DATA_IND);
                ok = FALSE;
                break;
            }
            bnep_get_addr (&dst_addr, pkt + 1);
            bnep_get_addr (&src_addr, pkt + 1 + ETHER_ADDR_LEN);
            offset_ether_type = 1 + 2 * ETHER_ADDR_LEN;
            break;

        case BNEP_TYPE_COMPRESSED_ETHER:
            if (mblk_length < 1 + 2)
            {
                IN_ERROR_LOG ("BNEP packet header too short",
                    IN_ERROR_HDRTOOSHORT_BNEP_DATA_IND);
                ok = FALSE;
                break;
            }
            dst_addr = bnep_local_addr;
            src_addr = bnep_connection->rem_addr;
            offset_ether_type = 1;
            break;

        case BNEP_TYPE_COMPRESSED_ETHER_SRC_ONLY:
            if (mblk_length < 1 + ETHER_ADDR_LEN + 2)
            {
                IN_ERROR_LOG ("BNEP packet header too short",
                    IN_ERROR_HDRTOOSHORT_BNEP_DATA_IND);
                ok = FALSE;
                break;
            }
            dst_addr = bnep_local_addr;
            bnep_get_addr (&src_addr, pkt + 1);
            offset_ether_type = 1 + ETHER_ADDR_LEN;
            break;

        case BNEP_TYPE_COMPRESSED_ETHER_DST_ONLY:
            if (mblk_length < 1 + ETHER_ADDR_LEN + 2)
            {
                IN_ERROR_LOG ("BNEP packet header too short",
                    IN_ERROR_HDRTOOSHORT_BNEP_DATA_IND);
                ok = FALSE;
                break;
            }
            bnep_get_addr (&dst_addr, pkt + 1);
            src_addr = bnep_connection->rem_addr;
            offset_ether_type = 1 + ETHER_ADDR_LEN;
            break;

        case BNEP_TYPE_CONTROL:
            hdr_len = bnep_handle_control (pkt + 1, (CsrUint16) (mblk_length - 1), bnep_connection);
            if (hdr_len == 0)
            {
                ok = FALSE;
            }
            else
            {
                ++hdr_len; /* For type octet */
            }
            break;

        default:
            /*
            * Ignore unknown packet types (BNEP0.95a/2.4).
            */
            ok = FALSE;
            break;
        }
    }

#ifdef BNEP_SUPPRESS_FROMUS /* HERETODO not discarding; see [AUTOCONF] */
    /*
    * Also make sure the packet doesn't come from us
    * (as it could depending on NAP/GN implementation of mutlicast/broadcast;
    * we don't want to get confused e.g. during duplicate address detection).
    */
    if (ok && ETHER_IS_SAME_ADDR (&src_addr, &bnep_local_addr))
    {
        /* HERETODO maybe shouldn't discard; see [AUTOCONF] */
        IN_ERROR_LOG ("BNEP packet looped back",
                      IN_ERROR_LOOPBACK_BNEP_DATA_IND);
        ok = FALSE;
    }
#endif
    /*
    * Fetch the Ethertype, if present.
    */
    if (offset_ether_type != 0)
    {
        ether_type = pkt[offset_ether_type] << 8 | pkt[offset_ether_type + 1];
        hdr_len = offset_ether_type + 2;
    }
    /*
    * Advance past the header.
    */
    mblk_length -= hdr_len;
    mblk_offset += hdr_len;
    pkt += hdr_len;

    /*
    * Handle any extension headers.
    */
    while (has_extn && ok)
    {
        /*
        * Check that there are enough octets for the BNEP extension header.
        * If not, then silently discard.
        * If so, then process the BNEP extension header.
        */
        if (mblk_length < BNEP_MIN_EXTENSION_HDR_LEN)
        {
            IN_ERROR_LOG ("BNEP extension header too short",
                          IN_ERROR_EXTHDRTOOSHORT_BNEP_DATA_IND);
            ok = FALSE;
        }
        else if (mblk_length < 2 + pkt[BNEP_OFFSETOF_EXTENSION_LEN])
        {
            IN_ERROR_LOG ("BNEP extension header length wrong",
                          IN_ERROR_BADEXTHDRLEN_BNEP_DATA_IND);
            ok = FALSE;
        }
        else
        {
            has_extn = pkt[BNEP_OFFSETOF_EXTENSION_TYPE] & BNEP_FLG_EXTENSION;

            if ((pkt[BNEP_OFFSETOF_EXTENSION_TYPE] & ~BNEP_FLG_EXTENSION) !=
                BNEP_EXTENSION_TYPE_CONTROL)
            {
                /*
                * Ignore unknown extension types (BNEP0.96e/3.1),
                * but remember where they start so they can be forwarded.
                */
                IN_ERROR_LOG ("BNEP packet unknown extension type",
                              IN_ERROR_UNKNOWNEXTTYPE_BNEP_DATA_IND);
                if (unk_extn_hdr_offset == 0)
                {
                    unk_extn_hdr_offset = mblk_offset;
                }
            }
            else
            {
                /*
                * We don't really care if any of the controls are bad,
                * since we can just skip them using the extension length.
                */
                bnep_handle_control (pkt + 2, pkt[BNEP_OFFSETOF_EXTENSION_LEN], bnep_connection);

                /*
                * Extension headers should be ordered in ascending order
                * (BNEP0.96e/3.1).
                */
                if (unk_extn_hdr_offset != 0)
                {
                    /* HERETODO send a HQ_PROT_ERROR */
                }
            }
            hdr_len = 2 + pkt[BNEP_OFFSETOF_EXTENSION_LEN];

            /*
            * Advance past the extension header.
            */
            mblk_length -= hdr_len;
            mblk_offset += hdr_len;
            pkt += hdr_len;
        }
    }

    /*
    * Extra miscellaneous checks.
    */
    if (ok)
    {
        if (offset_ether_type == 0) /* Control packet */
        {
            if (mblk_length != 0)
            {
                IN_ERROR_LOG ("BNEP control packet with trailing crud",
                              IN_ERROR_CONTROLCRUD_BNEP_DATA_IND);
            }
            ok = FALSE; /* So mblk just gets freed below */
        }
        else
        {
            if (bnep_connection->state != BNEP_ESTABLISHED)
            {
                /*
                * Ignore any Ethernet packets if not established (PAN0.96e/2.6.3.1).
                */
                IN_ERROR_LOG ("BNEP premature data",
                              IN_ERROR_PREMATURE_BNEP_DATA_IND);
                ok = FALSE;
            }
            else
            {
                /*
                * Support for 802.1Q (BNEP0.95a/4).
                */
                if (ether_type == ETHER_TYPE_8021Q)
                {
                    if (mblk_length < 4)
                    {
                        IN_ERROR_LOG ("BNEP 802.1Q header too short",
                                      IN_ERROR_HDRTOOSHORT_BNEP_DATA_IND);
                        ok = FALSE;
                    }
                    else
                    {
                        has_8021q = TRUE;
                        ether_type = pkt[2] << 8 | pkt[3];
                        mblk_length -= 4;
                        mblk_offset += 4;
                    }
                }
            }
        }
    }
    if (!ok)
    {
        CsrPmemFree(orig_ptr);
        return;
    }

    /*
    * Don't play any clever tricks if we're not auto-bridging.
    */
    if (!bnep_passive_connection_flags.manual_bridge_cuckoo)
    {
        CsrUint16 unk_extn_len = 0;

        /*
        * Rewind past any 802.1Q/unknown extension headers to forward them.
        */
        if (has_8021q)
        {
            mblk_length -= 4;
            mblk_offset += 4;
        }
        if (unk_extn_hdr_offset != 0)
        {
            unk_extn_len = mblk_offset - unk_extn_hdr_offset;
            mblk_length += unk_extn_len;
            mblk_offset -= unk_extn_len;
        }

        /*
        * Forward multicast packets to all PANU ports
        * (and later to listeners and/or NAP port).
        * HERETODO optimise: don't do this if PANU/only one connection
        */
        if (ETHER_IS_MULTICAST (&dst_addr))
        {
            BNEP_RECORDED *bnep_recorded = CsrPmemAlloc(sizeof (*bnep_recorded));
            bnep_recorded->isARecorded = TRUE;
            bnep_recorded->id_last = BNEP_NO_CID;
            bnep_recorded->id_not = bnep_connection->local_cid;
            bnep_recorded->ether_type = has_8021q ? ETHER_TYPE_8021Q : ether_type;
            bnep_recorded->dst_addr = dst_addr;
            bnep_recorded->src_addr = src_addr;
            bnep_recorded->mblk = CsrBtBnepMblkCreateDup(orig_ptr+mblk_offset, mblk_length);
            bnep_recorded->from_bnep_client = FALSE;
            bnep_recorded->unk_extn_len = unk_extn_len;

            /* HERETODO common code with bnep_data_req () */
            if (bnep_extended_multicast_data_req (bnep_recorded))
            {
                /*
                * Come back later for the listeners and/or NAP port.
                */
                CsrPmemFree(orig_ptr);
                return;
            }
            else
            {
                /*
                 * No PANUs were interested, so just pass on to any listeners.
 */
                CsrMblkDestroy(bnep_recorded->mblk);
                CsrPmemFree(bnep_recorded);
            }
        }

        /*
        * Forward unicast packets to the right PANU, if any,
        * as long as we're not a PANU.
        */
        if (!ETHER_IS_MULTICAST (&dst_addr) && bnep_local_role != BNEP_UUID_PANU)
        {
            BNEP_EXTENDED_DATA_REQ_T bedrtauto, *bedrt = &bedrtauto;
            BNEP_CONNECTION *bnep_connection_remote = bnep_rem_addr_to_connection (&dst_addr);

            /*
            * Avoid loops by making sure the destination isn't
            * the source.
            */
            if (bnep_connection_remote != NULL &&
                bnep_connection_remote != bnep_connection)
            {
                /* bedrt->type = BNEP_EXTENDED_DATA_REQ; */
                bedrt->id = bnep_connection_remote->local_cid;
                bedrt->ether_type = has_8021q ? ETHER_TYPE_8021Q : ether_type;
                bedrt->dst_addr = dst_addr;
                bedrt->src_addr = src_addr;
                bedrt->mblk = CsrBtBnepMblkCreateDup(orig_ptr+mblk_offset, mblk_length);
                bnep_extended_data_req (bedrt, (CsrBool) (unk_extn_len != 0) );
                CsrPmemFree(orig_ptr);
                return;
            }
        }

        /*
        * Now have to undo any 802.1Q/unknown extension header rewinding.
        * HERETODO no: if for EXTENDED_DATA_INDs we should preserve any
        * 802.1Q header.
        */
        if (unk_extn_hdr_offset != 0)
        {
            mblk_length -= unk_extn_len;
            mblk_offset += unk_extn_len;
        }
        if (has_8021q)
        {
            mblk_length -= 4;
            mblk_offset += 4;
        }

        /*
        * Try to find Ethertype/address pair in the listener list,
        * if the stack is not disabled.
        * HERETODO document that any unicast listens must be for
        * the local machine (otherwise they'll get forwarded above
        * and never hit this piece of code).
        */
        if (!bnep_passive_connection_flags.disable_stack_cuckoo)
        {
            if (bnep_send_data_ind (ether_type,
                                    &dst_addr,
                                    CsrBtBnepMblkCreateDup(orig_ptr+mblk_offset, mblk_length)))
            {
                CsrPmemFree(orig_ptr);
                return;
            }
        }
    }

    /*
    * Send an extended data indication, if not disabled.
    */
    if (!bnep_passive_connection_flags.disable_extended_cuckoo)
    {
        BNEP_EXTENDED_DATA_IND_T *abedit;

        /*
        * If we can't create the task message,
        * then silently discard.
        */
        abedit = CsrPmemAlloc(sizeof (*abedit));
        abedit->type = BNEP_EXTENDED_DATA_IND;
        abedit->id = bnep_connection->local_cid;
        abedit->ether_type = ether_type;
        abedit->dst_addr = dst_addr;
        abedit->src_addr = src_addr;
        abedit->mblk = CsrBtBnepMblkCreateDup(orig_ptr+mblk_offset, mblk_length);
        PUT_BNEP_MESSAGE (bnep_local_phandle, abedit);
        CsrPmemFree(orig_ptr);
        return;
    }

    /*
    * Oh, well, too bad.
    */
    IN_ERROR_LOG ("BNEP orphan packet",
                  IN_ERROR_NOLISTENER_BNEP_DATA_IND);
    CsrPmemFree(orig_ptr);
    return;
}


/******************************************************************************

    function bnep_half_config  --  a config half has completed (initiator only)

******************************************************************************/

static void bnep_half_config (BNEP_CONNECTION *bnep_connection)
{
    /*
    * If we're the initiator, then we need to set up the connection,
    * but check for config crossover first (other end sending
    * config_req before it's replied to our config_req).
    */
    if (!bnep_connection->half_config)
    {
        /* Crossover! */
        bnep_connection->half_config = TRUE;
    }
    else
    {
        /*
        * Set up the connection by faking a T_crt timeout.
        */
        bnep_connection->retx_counter = BNEP_T_CRT_N_TX;
        bnep_connection->state = BNEP_ESTABLISHING;
        bnep_timeout_setup (bnep_connection->local_cid, NULL);
    }
}


/******************************************************************************

    function bnep_l2cap_handler  --  handle incoming L2CAP messages

******************************************************************************/

static void bnep_l2cap_handler (L2CA_UPRIM_T *prim)
{
    BNEP_CONNECTION *bnep_connection;

    switch (prim->type)
    {
    case L2CA_REGISTER_CFM:
        /* Oh, good */
        break;

    case L2CA_CONNECT_CFM:
        bnep_connection = bnep_cid_to_connection (BNEP_NO_CID); /* HERETODO this trick means that you can only have one outgoing connection at a time */
        if (prim->l2ca_connect_cfm.result == L2CA_CONNECT_SUCCESS)
        {
            /*
            * Check this isn't a duplicate (I love BlueStack).
            */
            if (bnep_connection != NULL)
            {
                bnep_connection->local_cid = prim->l2ca_connect_cfm.cid;

                /*
                * We're the initiator so we need to do our config now.
                */
                bnep_connection->state = BNEP_CONFIGURING;
                bnep_connection->half_config = FALSE;

                /*
                * If we can't send the request, we might as well panic.
                */
                    L2CA_ConfigReq(prim->l2ca_connect_cfm.cid,
                                   L2CA_SPECIFY_MTU | L2CA_SPECIFY_FLUSH, /* options */
                                   0, /* hints */
                                   0, /* unknownLength */
                                   NULL, /* unknown */
                                   BNEP_MTU,
                                   L2CA_FLUSH_TO_DEFAULT,
                                   NULL, /* qos */
                                   NULL, /* flow */
                                   L2CA_FCS_OFF,
                                   NULL, /* flowspec */
                                   0); /* ext_window */
            }
        }
        else if (prim->l2ca_connect_cfm.result != L2CA_CONNECT_PENDING &&
            prim->l2ca_connect_cfm.result != L2CA_CONNECT_INITIATING &&
            prim->l2ca_connect_cfm.result != L2CA_CONNECT_RETRYING)
        {
            switch(prim->l2ca_connect_cfm.result)
            {
            case L2CA_CONNECT_REJ_PSM:
                bnep_send_connect_ind(BNEP_CONNECT_IND_STATUS_REJ_PSM, bnep_connection);
                break;

            case L2CA_CONNECT_REJ_SECURITY:
            case L2CA_CONNECT_KEY_MISSING:
                bnep_send_connect_ind(BNEP_CONNECT_IND_STATUS_REJ_SECURITY, bnep_connection);
                break;

            case L2CA_CONNECT_REJ_RESOURCES:
                bnep_send_connect_ind(BNEP_CONNECT_IND_STATUS_REJ_RESOURCES, bnep_connection);
                break;

            default:
                bnep_send_connect_ind(BNEP_CONNECT_IND_STATUS_FAILED, bnep_connection);
                break;
            }
            bnep_destroy_connection (bnep_connection);
        }
        break;

    case L2CA_CONNECT_IND:
        {
                l2ca_conn_result_t response;

                /*
                 * Check this isn't a duplicate (I love BlueStack).
                 */
                if (bnep_cid_to_connection (prim->l2ca_connect_ind.cid) != NULL)
                {
                    response = L2CA_CONNECT_SUCCESS;
                }
                /*
                 * Only accept if we're ready for a passive open, and
                 * are not a PANU, and
                 * are not in the process of connecting, and
                 * are in multi-user mode or have no connections.
                 * could do: improve single-user check so that two
                 * rival devices can coexist before one of them
                 * reaches BNEP_ESTABLISHED?
                 * could do: could try to relax this in the future (e.g.
                 * if connecting multiple times as GN, or if passive connection
                 * in progress is guaranteed to be compatible).
                 */
                else if (bnep_passive_connection_flags.accept_in &&
                         bnep_local_role != BNEP_UUID_PANU &&
                         !bnep_connection_in_limbo () &&
                         ((!bnep_passive_connection_flags.single_user &&
                           (bnep_connections == NULL || !bnep_connections->flags.single_user)) ||
                          bnep_connections == NULL) &&
                         (bnep_connection = bnep_create_connection ()) != NULL)
                {
                    bnep_connection->local_cid = prim->l2ca_connect_ind.cid;
                    bnep_connection->flags = bnep_passive_connection_flags;
                    bnep_connection->state = BNEP_CONFIGURING;
                    /* bnep_connection->initiator = FALSE; */
                    /* bnep_connection->mtu_recd = FALSE; */
                    /* bnep_connection->config_bad = FALSE; */
                    bnep_connection->rem_addr = bnep_to_ether_addr (&prim->l2ca_connect_ind.bd_addr);

                    response = L2CA_CONNECT_SUCCESS;

                    /*
                     * We're the responder so wait for initiator to initiate config.
                     */
                }
                else
                {
                    response = L2CA_CONNECT_REJ_RESOURCES;
                }

                L2CA_ConnectRsp(prim->l2ca_connect_ind.cid,
                                prim->l2ca_connect_ind.identifier,
                                response,
                                0); /* context */
                break;
            }

            /*
             * HERETODO reconfiguration
             */
        case L2CA_CONFIG_IND:
            {
                l2ca_conf_result_t response = L2CA_CONFIG_REJECTED;

                bnep_connection = bnep_cid_to_connection (prim->l2ca_config_ind.cid);

                /*
                 * Check this isn't a duplicate (I love BlueStack).
                 */
                if (bnep_connection)
                {
                    if (bnep_connection->state != BNEP_CONFIGURING)
                    {
                        response = L2CA_CONFIG_SUCCESS;
                    }
                    /*
                     * Check the config request:
                     * - if it's malformed, reject it
                     * - if the MTU isn't big enough, say the parameters are unacceptable
                     *   (it's not necessary to say which -- BT1.1 page 292)
                     * - accept any old FT (PAN1.0d/9.3.2)
                     * - if it's got a QoS, reject it (HERETODO?)
                     * - if it's got unknown non-hint options, reject it (to report
                     *   unknown options one has to specify these, which is too big a pain)
                     * - if it's final, and we haven't got an MTU, reject it
     */
                    else if (prim->l2ca_config_ind.config.unknown || prim->l2ca_config_ind.config.qos)
                    {
                        response = L2CA_CONFIG_REJECTED;
                        bnep_connection->config_bad = TRUE;
                    }
                    else if (prim->l2ca_config_ind.config.options & L2CA_SPECIFY_MTU)
                    {
                        /*
                         * Don't bother checking whether we've already got an MTU
                         * (we don't bother checking for duplicate FTs either).
     */
                        bnep_connection->mtu_recd = TRUE;
                        if (prim->l2ca_config_ind.config.mtu < BNEP_MTU)
                        {
                            response = L2CA_CONFIG_UNACCEPTABLE;
                            bnep_connection->config_bad = TRUE;
                        }
                        else
                        {
                            response = L2CA_CONFIG_SUCCESS;
                        }
                    }
                    else if (!bnep_connection->mtu_recd)
                    {
                        response = L2CA_CONFIG_REJECTED;
                        bnep_connection->config_bad = TRUE;
                    }
                    else
                    {
                        response = L2CA_CONFIG_SUCCESS;
                    }
                }

                L2CA_ConfigRsp(prim->l2ca_config_ind.cid,
                               prim->l2ca_config_ind.identifier,
                               response,
                               L2CA_SPECIFY_MTU | L2CA_SPECIFY_FLUSH, /* options */
                               0, /* hints */
                               0, /* unknownLength */
                               NULL,    /* unknown */
                               BNEP_MTU,
                               L2CA_FLUSH_TO_DEFAULT,
                               NULL, /* qos */
                               NULL, /* flow */
                               L2CA_FCS_OFF,
                               NULL, /* flowspec */
                               0); /* ext_window */

                /*
                 * Wait for the peer to stop babbling.
                 * and make sure it's not just a duplicate (I love BlueStack).
 */
                if (bnep_connection && bnep_connection->state == BNEP_CONFIGURING)
                {
                    /*
                     * If we didn't receive an MTU or config was bad, then
                     * configuration has not been successful -- wait for
                     * peer to try again.
 */
                    if (!bnep_connection->mtu_recd || bnep_connection->config_bad)
                    {
                        bnep_connection->mtu_recd = FALSE;
                        bnep_connection->config_bad = FALSE;
                    }
                    /*
                     * If we're the responder, then we need to do our config now.
 */
                    else if (!bnep_connection->initiator)
                    {
                        L2CA_ConfigReq(prim->l2ca_config_ind.cid,
                                       L2CA_SPECIFY_MTU | L2CA_SPECIFY_FLUSH,
                                       0, /* hints */
                                       0, /* unknownLength */
                                       NULL, /* unknown */
                                       BNEP_MTU,
                                       L2CA_FLUSH_TO_DEFAULT,
                                       NULL, /* qos */
                                       NULL, /* flow */
                                       L2CA_FCS_OFF,
                                       NULL, /* flowspec */
                                       0); /* ext_window */
                }
                /*
                * If we're the initiator, then we're now configured!
                * (well, probably: bnep_half_config() checks for config crossover)
                */
                else
                {
                    bnep_half_config (bnep_connection);
                }
            }

            break;
        }

    case L2CA_CONFIG_CFM:
        bnep_connection = bnep_cid_to_connection (prim->l2ca_config_cfm.cid);

        if (bnep_connection == NULL)
        {
            /*
            * This shouldn't happen, so just whistle nonchalantly.
            */
            break;
        }

        if (prim->l2ca_config_cfm.result != L2CA_CONFIG_SUCCESS)
        {
            /*
            * If this was an active open, tell the app about the failure.
            */
            if (bnep_connection->initiator)
            {
                bnep_send_connect_ind (BNEP_CONNECT_IND_STATUS_FAILED, bnep_connection);
            }

            /*
            * Give up.
            */
            bnep_connection->client_knows = TRUE;
            bnep_send_l2ca_disconnect_req (bnep_connection);
            break;
        }

        /*
        * Wait for the peer to stop babbling
        * and make sure it's not just a duplicate (I love BlueStack).
        * HERETODO supposed to send a null config req if contin (see spec)?
        */
        if (bnep_connection->state == BNEP_CONFIGURING)
        {
            /*
            * If we're the responder, then we're now configured!
            */
            if (!bnep_connection->initiator)
            {
                    bnep_connection->t_setup = CsrSchedTimerSet (BNEP_T_SETUP * CSR_SCHED_SECOND,
                                                               bnep_timeout_setup,
                                                               bnep_connection->local_cid, NULL);
                    bnep_connection->retx_counter = 0;
                    bnep_connection->state = BNEP_ESTABLISHING;
                }
                /*
                 * If we're the initiator, then we're not yet configured.
                 * (well, probably: bnep_half_config() checks for config crossover)
 */
                else
                {
                    bnep_half_config (bnep_connection);
                }
            }
            break;

        case L2CA_DATAREAD_IND:
            if (prim->l2ca_dataread_ind.result == L2CA_DATA_SUCCESS)
            {
                bnep_data_ind ((CsrMblk *) prim->l2ca_dataread_ind.data,
                               bnep_cid_to_connection (prim->l2ca_dataread_ind.cid));
            }
            break;

        case L2CA_DATAWRITE_CFM:
            {
                BNEP_CONNECTION *bnep_connection = bnep_cid_to_connection (prim->l2ca_datawrite_cfm.cid);

                if (bnep_connection != NULL)
                {
                    BNEP_RECORDED *bnep_recorded;

                    bnep_recorded = bnep_connection->recordedList[bnep_connection->recordedListRdIdx];
                    /*
                     * Prime for another go.
 */
                    bnep_connection->l2_busyCount--;

                    /*
                     * If there's anything waiting to go, send the first thing off.
 */
                    if (bnep_connection->mblks != NULL)
                    {
                        MBLK_QUEUE *link = bnep_connection->mblks;

                        if (!bnep_send_l2ca_data_req (link->mblk_or_recorded, bnep_connection))
                        {
                            /*
                             * Too bad.
 */
                            if (BNEP_IS_A_RECORDED (link->mblk_or_recorded))
                            {
                                CsrMblkDestroy (((BNEP_RECORDED *) link->mblk_or_recorded)->mblk);
                                CsrPmemFree (link->mblk_or_recorded);
                            }
                            else
                            {
                                CsrMblkDestroy (link->mblk_or_recorded);
                            }
                        }

                        bnep_connection->mblks = link->next;
                        CsrPmemFree (link);
                    }

                    /*
                     * Handle any recorded associated with the completed transmission.
 */
                    if (bnep_recorded != NULL)
                    {
                        bnep_connection->recordedListRdIdx++;
                        bnep_connection->recordedListRdIdx &= BNEP_MAX_RECORDED_COUNT_MASK;
                        /*
                         * Any more takers?
 */
                        ZZZCOUNTER (ldcrecordeds);
                        if (!bnep_extended_multicast_data_req (bnep_recorded))
                        {
                            CsrMblk *mblk = bnep_recorded->mblk;
                            ZZZCOUNTER (ldcrecordedsdone);

                            /*
                             * Fast-forward past any extension headers.
 */
                            if (bnep_recorded->unk_extn_len != 0)
                            {
                                CsrMblkDiscardHead(&mblk, bnep_recorded->unk_extn_len);
                            }
                            /*
                             * If this was an 802.1Q packet,
                             * then pull out the actual Ethertype.
 */
                            if (bnep_recorded->ether_type == ETHER_TYPE_8021Q)
                            {
                                CsrUint8 pkt[4];
                                CsrMblkSize pkt_size;

                                pkt_size = CsrMblkReadHead(&mblk, pkt, sizeof(pkt));

                                if (pkt_size < sizeof(pkt))
                                {
                                    /*
                                     * Impossible!
                                     * HERETODO, actually possible if from host
 */
                                    /* HERETODO send HQ_PROT_ERROR? */
                                    CsrPmemFree (bnep_recorded);
                                    break;
                                }
                                else
                                {
                                    bnep_recorded->ether_type = pkt[2] << 8 | pkt[3];
                                }
                            }
                            /*
                             * Try to find Ethertype/address pair in the listener list,
                             * if the stack is not disabled and the message is not not
                             * to go to it.
 */
                            if (bnep_passive_connection_flags.disable_stack_cuckoo ||
                                bnep_recorded->id_not == BNEP_ID_LOCAL ||
                                !bnep_send_data_ind (bnep_recorded->ether_type, &bnep_recorded->dst_addr, mblk))
                            {
                                ZZZCOUNTER (ldcnottolistener);

                                /*
                                 * No?  Then pass to the client, unless it came from
                                 * there or extendeds are disabled.
 */
                                if (bnep_recorded->from_bnep_client || bnep_passive_connection_flags.disable_extended_cuckoo)
                                {
                                    CsrMblkDestroy (mblk);
                                    if(mblk == bnep_recorded->mblk)
                                    {
                                        bnep_recorded->mblk = NULL;
                                    }
                                }
                                else
                                {
                                    BNEP_EXTENDED_DATA_IND_T *abedit;

                                    ZZZCOUNTER (ldcnotfromclient);
                                    /*
                                     * If we can't create the task message,
                                     * then silently discard.
 */
                                    abedit = CsrPmemAlloc(sizeof (*abedit));
                                    ZZZCOUNTER (ldcxdi);
                                    abedit->type = BNEP_EXTENDED_DATA_IND;
                                    abedit->id = bnep_recorded->id_not;
                                    abedit->ether_type = bnep_recorded->ether_type;
                                    abedit->dst_addr = bnep_recorded->dst_addr;
                                    abedit->src_addr = bnep_recorded->src_addr;
                                    abedit->mblk = mblk;
                                    PUT_BNEP_MESSAGE (bnep_local_phandle, abedit);
                                }
                            }

                            CsrMblkDestroy(bnep_recorded->mblk);
                            CsrPmemFree(bnep_recorded);
                        }
                    }
                }
            }
            break;

        case L2CA_UNKNOWN_SIGNAL_IND:
        case L2CA_MTU_VIOLATION_IND: /* Shouldn't get this, but... */
        case L2CA_TIMEOUT_IND:
            break;

            /*
             * HERETODO need to find out how collisions are handled.
 */
        case L2CA_DISCONNECT_IND:
            {
                BNEP_CONNECTION *bnep_connection = bnep_cid_to_connection (prim->l2ca_disconnect_ind.cid);

                /*
                 * Just in case there's a disconnect collision...
                 */
                if (bnep_connection != NULL)
                {
                    if (bnep_connection->state == BNEP_ESTABLISHED)
                    {
                        bnep_send_updown_ind (BNEP_DOWN_IND, bnep_connection);
                        if (!bnep_connection->client_knows)
                        {
                            bnep_send_disconnect_ind (BNEP_DISCONNECT_IND_REASON_ABEND, bnep_connection);
                        }
                    }
                    else
                    {
                        if (!bnep_connection->client_knows &&
                            bnep_connection->initiator)
                        {
                            bnep_send_connect_ind (BNEP_CONNECT_IND_STATUS_ABEND, bnep_connection);
                        }
                    }

                    bnep_destroy_connection (bnep_connection);
                }

                /*
                 * If we can't send the response, we might as well panic.
                 */
                L2CA_DisconnectRsp(prim->l2ca_disconnect_ind.identifier,
                                   prim->l2ca_disconnect_ind.cid);
                break;
            }

        case L2CA_DISCONNECT_CFM:
            {
                BNEP_CONNECTION *bnep_connection = bnep_cid_to_connection (prim->l2ca_disconnect_cfm.cid);
                /*
                 * We don't care whether the disconnect timed out,
                 * so no need to look at the result.
                 * We don't care about duplicates either,
                 * since the connection was destroyed the first time round.
                 */

                /*
                 * Just in case there's a disconnect collision...
                 * HERETODO mostly common code with above
                 */
                if (bnep_connection != NULL)
                {
                    if (bnep_connection->state == BNEP_ESTABLISHED)
                    {
                        bnep_send_updown_ind (BNEP_DOWN_IND, bnep_connection);
                        if (!bnep_connection->client_knows)
                        {
                            bnep_send_disconnect_ind (bnep_connection->client_requested_disconnection ? BNEP_DISCONNECT_IND_REASON_DONE : BNEP_DISCONNECT_IND_REASON_NOT_MASTER, bnep_connection);
                        }
                    }
                    else
                    {
                        if (!bnep_connection->client_knows &&
                            bnep_connection->initiator)
                        {
                            bnep_send_connect_ind (bnep_connection->switch_failure ? BNEP_CONNECT_IND_STATUS_NOT_MASTER : BNEP_CONNECT_IND_STATUS_ABEND, bnep_connection);
                        }
                    }

                    bnep_destroy_connection (bnep_connection);
                }
                break;
            }

        default:
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
            CsrStateEventException("BNEP", CSR_BT_BNEP_PRIM, prim->type, 0);
#endif
            break;
    }
}


/******************************************************************************

    function bnep_bnep_handler  --  handle incoming BNEP messages

******************************************************************************/

static void bnep_bnep_handler (BNEP_UPRIM_T *prim)
{
    switch (prim->type)
    {
        case BNEP_REGISTER_REQ:
            bnep_register_req ((BNEP_REGISTER_REQ_T *) prim);
            break;
/* bchs change */
        case BNEP_EXTENDED_REGISTER_REQ:
            bnep_extended_register_req ((BNEP_EXTENDED_REGISTER_REQ_T *) prim);
            break;
        case BNEP_CONNECT_REQ:
            bnep_connect_req ((BNEP_CONNECT_REQ_T *) prim);
            break;
        case BNEP_LISTEN_REQ:
            bnep_listen_req ((BNEP_LISTEN_REQ_T *) prim);
            break;
        case BNEP_IGNORE_REQ:
            bnep_ignore_req ((BNEP_IGNORE_REQ_T *) prim);
            break;
        case BNEP_DATA_REQ:
            bnep_data_req ((BNEP_DATA_REQ_T *) prim);
            break;
        case BNEP_EXTENDED_DATA_REQ:
            bnep_extended_data_req ((BNEP_EXTENDED_DATA_REQ_T *) prim, FALSE);
            break;
        case BNEP_EXTENDED_MULTICAST_DATA_REQ:
            {
                BNEP_RECORDED *bnep_recorded = CsrPmemAlloc(sizeof (*bnep_recorded));
                bnep_recorded->isARecorded = TRUE;
                bnep_recorded->id_last = BNEP_NO_CID;
                bnep_recorded->id_not = ((BNEP_EXTENDED_MULTICAST_DATA_REQ_T *) prim)->id_not;
                bnep_recorded->ether_type = ((BNEP_EXTENDED_MULTICAST_DATA_REQ_T *) prim)->ether_type;
                bnep_recorded->dst_addr = ((BNEP_EXTENDED_MULTICAST_DATA_REQ_T *) prim)->dst_addr;
                bnep_recorded->src_addr = ((BNEP_EXTENDED_MULTICAST_DATA_REQ_T *) prim)->src_addr;
                bnep_recorded->mblk = ((BNEP_EXTENDED_MULTICAST_DATA_REQ_T *) prim)->mblk;
                bnep_recorded->from_bnep_client = TRUE;
                bnep_recorded->unk_extn_len = 0;

                if (bnep_extended_multicast_data_req (bnep_recorded))
                {
                    /*
                     * Come back later for the listeners.
 */
                    break;
                }

                /*
                 * No PANUs were interested, so
                 * try to find Ethertype/address pair in the listener list,
                 * if the stack is not disabled and the message is not not
                 * to go to it.
 */
                CsrPmemFree (bnep_recorded);
                if (bnep_passive_connection_flags.disable_stack_cuckoo ||
                    ((BNEP_EXTENDED_MULTICAST_DATA_REQ_T *) prim)->id_not == BNEP_ID_LOCAL ||
                    !bnep_send_data_ind (((BNEP_EXTENDED_MULTICAST_DATA_REQ_T *) prim)->ether_type, &((BNEP_EXTENDED_MULTICAST_DATA_REQ_T *) prim)->dst_addr, ((BNEP_EXTENDED_MULTICAST_DATA_REQ_T *) prim)->mblk))
                {
                    CsrMblkDestroy (((BNEP_EXTENDED_MULTICAST_DATA_REQ_T *) prim)->mblk);
                }

                break;
            }
        case BNEP_DISCONNECT_REQ:
            bnep_disconnect_req ((BNEP_DISCONNECT_REQ_T *) prim);
            break;
        case BNEP_HCI_SWITCH_ROLE_RSP:
            bnep_hci_switch_role_rsp ((BNEP_HCI_SWITCH_ROLE_RSP_T *) prim);
            break;
        case BNEP_DEBUG_REQ:
            break;
        default:
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
            CsrStateEventException("BNEP", CSR_BT_BNEP_PRIM, prim->type, 0);
#endif
            break;
    }
}


/******************************************************************************

    function bnep_init  --  initialise BNEP task

******************************************************************************/

void CsrBtBnepInit(void **gash)
{
    CsrSize i;

    bnep_connections = NULL;
    bnep_local_role = 0;

    /*
     * Initialise the listener list.
 */
    for (i = 0; i < BNEP_NUM_LISTENERS; ++i)
    {
        bnep_listeners[i].addr.w[0] = ETHER_UNKNOWN;
    }
}
/****************************************************************************
    This function is called by the scheduler to perform a graceful shutdown
    of a scheduler task.
    This function must:
    1)    empty the input message queue and free any allocated memory in the
        messages.
    2)    free any instance data that may be allocated.
****************************************************************************/
void CsrBtBnepDeinit(void **gash)
{
    CsrUint16 msg_type=0;
    void *msg_data=NULL;

    /* continue to poll any message of the input queue */
    while (CsrSchedMessageGet(&msg_type, &msg_data))
    {
        switch (msg_type)
        {
            case L2CAP_PRIM:
                {
                    L2CA_FreePrimitive((L2CA_UPRIM_T *) msg_data);
                    msg_data = NULL;
                    break;
                }
            case CSR_BT_BNEP_PRIM:
                {
                    BNEP_UPRIM_T *prim;

                    prim = (BNEP_UPRIM_T *) msg_data;

                    switch (prim->type)
                    {
                        case BNEP_EXTENDED_DATA_REQ :
                            {
                                BNEP_EXTENDED_DATA_REQ_T *prim;

                                prim = (BNEP_EXTENDED_DATA_REQ_T *) msg_data;
                                CsrMblkDestroy(prim->mblk);
                                break;
                            }

                        case BNEP_EXTENDED_MULTICAST_DATA_REQ :
                            {
                                BNEP_EXTENDED_MULTICAST_DATA_REQ_T *prim;

                                prim = (BNEP_EXTENDED_MULTICAST_DATA_REQ_T *) msg_data;
                                CsrMblkDestroy(prim->mblk);
                                break;
                            }
                    }
                    break;
                }

            case CSR_SCHED_PRIM:
                break;

            default:
                {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                    CsrStateEventException("BNEP", msg_type, 0, 0);
#endif
                    break;
                }
        }
        CsrPmemFree (msg_data);
    }

    bnep_destroy_connection (bnep_connections);

    if (*gash)
    {
        /* free the instance data (if any) */
    }
}


/******************************************************************************

    function bnep_task  --  handle incoming messages for the BNEP task

******************************************************************************/

void CsrBtBnepHandler(void **gash)
{
    CsrUint16 msg_type=0;
    void *msg_data=NULL;
    /*
     * Fetch an incoming message and call appropriate handler.
 */
    if (!CsrSchedMessageGet(&msg_type, &msg_data))
    {
        /* Paranoia */
        return;
    }
    switch (msg_type)
    {
        case L2CAP_PRIM:
            bnep_l2cap_handler ((L2CA_UPRIM_T *) msg_data);
            break;
        case CSR_BT_BNEP_PRIM:
            bnep_bnep_handler ((BNEP_UPRIM_T *) msg_data);
            break;

        case CSR_SCHED_PRIM:
            break;

        default:
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
            CsrStateEventException("BNEP", msg_type, 0, 0);
#endif
            break;
    }
    CsrPmemFree (msg_data);
}

#endif

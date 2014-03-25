#ifndef UIF_H__
#define UIF_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#pragma once
#include <guiddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Sdioemb Userspace device interface GUID.
 *
 * {d9b9ce15-461c-4d5c-9a26-b2ebf57e3a77}
 */
DEFINE_GUID(GUID_DEVINTERFACE_SDIOEMB_UIF,
    0xd9b9ce15, 0x461c, 0x4d5c, 0x9a, 0x26, 0xb2, 0xeb, 0xf5, 0x7e, 0x3a, 0x77);

enum sdioemb_uif_cmd_type
{
    SDIOEMB_UIF_CMD52_READ, SDIOEMB_UIF_CMD52_WRITE,
    SDIOEMB_UIF_CMD53_READ, SDIOEMB_UIF_CMD53_WRITE,
};

struct sdioemb_uif_cmd
{
    enum sdioemb_uif_cmd_type type;
    int                       function;
    UINT32                    address;
    UINT8                    *data;
    size_t                    len;
    UINT16                    block_size;
};

struct sdioemb_uif_set_bus_width
{
    UINT8 function;
    UINT8 bus_width;
};

struct sdioemb_uif_get_func_info
{
    UINT8 function;
};

struct sdioemb_uif_set_bus_freq
{
    INT32 bus_freq;
};

struct sdioemb_uif_set_block_size
{
    UINT8  function;
    UINT16 block_size;
};

struct sdioemb_uif_input
{
    union
    {
        struct sdioemb_uif_get_func_info  get_func_info;
        struct sdioemb_uif_cmd            cmd;
        struct sdioemb_uif_set_bus_width  set_bus_width;
        struct sdioemb_uif_set_bus_freq   set_bus_freq;
        struct sdioemb_uif_set_block_size set_block_size;
    };
};

struct sdioemb_uif_card_info
{
    UINT8 num_functions;
};

struct sdioemb_uif_func_info
{
    UINT16 manf_id;
    UINT16 card_id;
    UINT8  std_if;
    UINT16 max_block_size;
    UINT16 block_size;
};

struct sdioemb_uif_output
{
    union
    {
        struct sdioemb_uif_card_info card_info;
        struct sdioemb_uif_func_info func_info;
    };
};

#define SDIOEMB_UIF_IOC_GET_CARD_INFO  CTL_CODE(0x8000, 0x800, METHOD_BUFFERED, FILE_READ_DATA)
#define SDIOEMB_UIF_IOC_GET_FUNC_INFO  CTL_CODE(0x8000, 0x801, METHOD_BUFFERED, FILE_READ_DATA)
#define SDIOEMB_UIF_IOC_CMD            CTL_CODE(0x8000, 0x802, METHOD_BUFFERED, FILE_WRITE_DATA)
#define SDIOEMB_UIF_IOC_WAITFORINT     CTL_CODE(0x8000, 0x803, METHOD_BUFFERED, FILE_WRITE_DATA)
#define SDIOEMB_UIF_IOC_HARDRESET      CTL_CODE(0x8000, 0x804, METHOD_BUFFERED, FILE_WRITE_DATA)
#define SDIOEMB_UIF_IOC_SET_BUS_WIDTH  CTL_CODE(0x8000, 0x805, METHOD_BUFFERED, FILE_WRITE_DATA)
#define SDIOEMB_UIF_IOC_SET_BUS_FREQ   CTL_CODE(0x8000, 0x806, METHOD_BUFFERED, FILE_WRITE_DATA)
#define SDIOEMB_UIF_IOC_SET_BLOCK_SIZE CTL_CODE(0x8000, 0x807, METHOD_BUFFERED, FILE_WRITE_DATA)

#ifdef __cplusplus
}
#endif

#endif /* #ifndef UIF_H__ */
